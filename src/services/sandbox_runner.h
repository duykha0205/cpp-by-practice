#pragma once

#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <atomic>
#include <chrono>
#include <unordered_map>
#include <sys/wait.h>
#include "models/exercise.h"

namespace fs = std::filesystem;

class SandboxRunner {
public:
    struct Config {
        std::string docker_image = "gcc:14";
        int timeout_seconds = 5;
        std::string memory_limit = "128m";
        std::string tmp_dir;
    };

    explicit SandboxRunner(Config config) : config_(std::move(config)) {
        if (config_.tmp_dir.empty()) {
            config_.tmp_dir = (fs::temp_directory_path() / "cpp-sandbox").string();
        }
        fs::create_directories(config_.tmp_dir);
    }

    static bool check_docker(const std::string& image) {
        if (std::system("docker info > /dev/null 2>&1") != 0)
            return false;
        std::string cmd = "docker image inspect " + image + " > /dev/null 2>&1";
        return std::system(cmd.c_str()) == 0;
    }

    bool is_available() {
        return check_docker(config_.docker_image);
    }

    RunResult run(const RunRequest& request) {
        fs::path work_dir;
        try {
            work_dir = create_work_dir();
            write_file(work_dir / "main.cpp", request.source_code);
            write_file(work_dir / "run.sh", build_runner_script(request));

            std::string cmd = build_docker_command(work_dir);
            auto [output, exit_code] = exec_command(cmd);
            RunResult result = parse_output(output, exit_code);

            fs::remove_all(work_dir);
            return result;
        } catch (const std::exception& e) {
            if (!work_dir.empty()) fs::remove_all(work_dir);
            return make_error(std::string("Sandbox error: ") + e.what());
        }
    }

private:
    fs::path create_work_dir() {
        static std::atomic<uint64_t> counter{0};
        auto ts = std::chrono::steady_clock::now().time_since_epoch();
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(ts).count();
        std::string id = "run_" + std::to_string(us) + "_"
                       + std::to_string(counter.fetch_add(1));

        fs::path dir = fs::path(config_.tmp_dir) / id;
        fs::create_directories(dir);
        return dir;
    }

    static void write_file(const fs::path& path, const std::string& content) {
        std::ofstream f(path, std::ios::binary);
        if (!f) throw std::runtime_error("Cannot write " + path.string());
        f << content;
    }

    std::string build_runner_script(const RunRequest& request) const {
        std::string flags = request.compiler_flags.empty()
            ? "-std=c++20 -Wall -Wextra"
            : request.compiler_flags;

        std::ostringstream s;
        s << "#!/bin/bash\n"
          << "cd /tmp\n"
          << "g++ /code/main.cpp -o /tmp/a.out " << flags << " 2>/tmp/ce.txt\n"
          << "COMPILE_RC=$?\n"
          << "if [ $COMPILE_RC -ne 0 ]; then\n"
          << "  echo '===SANDBOX_RESULT==='\n"
          << "  echo \"COMPILE_RC=$COMPILE_RC\"\n"
          << "  echo \"COMPILE_ERR_B64=$(head -c 65536 /tmp/ce.txt | base64 -w0)\"\n"
          << "  echo 'STDOUT_B64='\n"
          << "  echo 'STDERR_B64='\n"
          << "  echo 'RUN_RC=1'\n"
          << "  echo '===END_RESULT==='\n"
          << "  exit 0\n"
          << "fi\n"
          << "timeout " << config_.timeout_seconds
          << " /tmp/a.out >/tmp/out.txt 2>/tmp/err.txt\n"
          << "RUN_RC=$?\n"
          << "echo '===SANDBOX_RESULT==='\n"
          << "echo 'COMPILE_RC=0'\n"
          << "echo 'COMPILE_ERR_B64='\n"
          << "echo \"STDOUT_B64=$(head -c 65536 /tmp/out.txt | base64 -w0)\"\n"
          << "echo \"STDERR_B64=$(head -c 65536 /tmp/err.txt | base64 -w0)\"\n"
          << "echo \"RUN_RC=$RUN_RC\"\n"
          << "echo '===END_RESULT==='\n";
        return s.str();
    }

    std::string build_docker_command(const fs::path& work_dir) const {
        int outer_timeout = config_.timeout_seconds + 15;

        std::ostringstream cmd;
        cmd << "timeout " << outer_timeout
            << " docker run --rm"
            << " --network=none"
            << " --read-only"
            << " --tmpfs /tmp:rw,exec,size=64m"
            << " --cap-drop ALL"
            << " --security-opt no-new-privileges"
            << " --memory=" << config_.memory_limit
            << " --cpus=1"
            << " --pids-limit 64"
            << " --ulimit nofile=64:64"
            << " -v " << work_dir.string() << ":/code:ro"
            << " " << config_.docker_image
            << " bash /code/run.sh"
            << " 2>&1";
        return cmd.str();
    }

    static std::pair<std::string, int> exec_command(const std::string& cmd) {
        std::string output;
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) throw std::runtime_error("Failed to execute sandbox command");

        char buf[4096];
        while (fgets(buf, sizeof(buf), pipe))
            output += buf;

        int status = pclose(pipe);
        int code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        return {output, code};
    }

    static RunResult parse_output(const std::string& output, int docker_exit) {
        const std::string START_MARK = "===SANDBOX_RESULT===";
        const std::string END_MARK   = "===END_RESULT===";

        auto s_pos = output.find(START_MARK);
        auto e_pos = output.find(END_MARK);

        if (s_pos == std::string::npos || e_pos == std::string::npos) {
            if (docker_exit == 124) {
                return make_error("Time limit exceeded");
            }
            RunResult r;
            r.stderr_output = output.empty()
                ? "Container execution failed" : output;
            r.exit_code = docker_exit;
            r.status = "Runtime Error";
            r.time = 0;
            r.memory = 0;
            return r;
        }

        std::string block = output.substr(
            s_pos + START_MARK.size(),
            e_pos - s_pos - START_MARK.size());
        auto vars = parse_vars(block);

        int compile_rc = 0;
        try { compile_rc = std::stoi(vars["COMPILE_RC"]); } catch (...) {}

        RunResult result;
        result.time = 0;
        result.memory = 0;

        if (compile_rc != 0) {
            result.compile_output = base64_decode(vars["COMPILE_ERR_B64"]);
            result.exit_code = compile_rc;
            result.status = "Compilation Error";
            return result;
        }

        int run_rc = 0;
        try { run_rc = std::stoi(vars["RUN_RC"]); } catch (...) {}

        result.stdout_output = base64_decode(vars["STDOUT_B64"]);
        result.stderr_output = base64_decode(vars["STDERR_B64"]);
        result.exit_code = run_rc;
        result.status = (run_rc == 0) ? "Success" : "Runtime Error";
        return result;
    }

    static std::unordered_map<std::string, std::string>
    parse_vars(const std::string& block) {
        std::unordered_map<std::string, std::string> vars;
        std::istringstream stream(block);
        std::string line;
        while (std::getline(stream, line)) {
            if (!line.empty() && line.back() == '\r')
                line.pop_back();
            auto eq = line.find('=');
            if (eq != std::string::npos && eq > 0)
                vars[line.substr(0, eq)] = line.substr(eq + 1);
        }
        return vars;
    }

    static std::string base64_decode(const std::string& input) {
        static constexpr unsigned char T[256] = {
            64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64,64,64,64,62,64,64,64,63,
            52,53,54,55,56,57,58,59,60,61,64,64,64,65,64,64,
            64, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
            15,16,17,18,19,20,21,22,23,24,25,64,64,64,64,64,
            64,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
            41,42,43,44,45,46,47,48,49,50,51,64,64,64,64,64,
            64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
            64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
        };

        std::string out;
        int val = 0, bits = -8;
        for (unsigned char c : input) {
            if (T[c] >= 64) break;
            val = (val << 6) | T[c];
            bits += 6;
            if (bits >= 0) {
                out.push_back(static_cast<char>((val >> bits) & 0xFF));
                bits -= 8;
            }
        }
        return out;
    }

    static RunResult make_error(const std::string& message) {
        RunResult r;
        r.stderr_output = message;
        r.exit_code = -1;
        r.status = "Runtime Error";
        r.time = 0;
        r.memory = 0;
        return r;
    }

    Config config_;
};

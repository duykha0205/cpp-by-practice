#pragma once

#include <crow.h>
#include <nlohmann/json.hpp>
#include "services/sandbox_runner.h"

using json = nlohmann::json;

class ApiRoutes {
public:
    explicit ApiRoutes(SandboxRunner& sandbox)
        : sandbox_(sandbox) {}

    void register_routes(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/api/run").methods("POST"_method)(
            [this](const crow::request& req) {
                return handle_run(req);
            });

        CROW_ROUTE(app, "/api/health")(
            [this]() {
                return handle_health();
            });
    }

private:
    crow::response handle_run(const crow::request& req) {
        try {
            json body = json::parse(req.body);

            RunRequest run_req;
            run_req.source_code = body.value("source_code", "");
            run_req.compiler_flags = body.value("compiler_flags", "");

            if (run_req.source_code.empty()) {
                json err = {{"error", "source_code is required"}};
                return crow::response(400, err.dump());
            }

            auto result = sandbox_.run(run_req);

            json response = {
                {"stdout", result.stdout_output},
                {"stderr", result.stderr_output},
                {"compile_output", result.compile_output},
                {"exit_code", result.exit_code},
                {"status", result.status},
                {"time", result.time},
                {"memory", result.memory}
            };

            crow::response resp(200, response.dump());
            resp.set_header("Content-Type", "application/json");
            return resp;

        } catch (const json::parse_error&) {
            json err = {{"error", "Invalid JSON body"}};
            return crow::response(400, err.dump());
        } catch (const std::exception& e) {
            json err = {{"error", std::string(e.what())}};
            return crow::response(500, err.dump());
        }
    }

    crow::response handle_health() {
        bool ok = sandbox_.is_available();

        json response = {
            {"status", ok ? "ok" : "degraded"},
            {"sandbox", ok ? "available" : "unavailable"}
        };

        int status = ok ? 200 : 503;
        crow::response resp(status, response.dump());
        resp.set_header("Content-Type", "application/json");
        return resp;
    }

    SandboxRunner& sandbox_;
};

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdexcept>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

namespace FileUtils {

inline std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

inline bool file_exists(const std::string& path) {
    return fs::exists(path);
}

inline std::vector<std::string> list_directories(const std::string& path) {
    std::vector<std::string> dirs;
    if (!fs::exists(path)) return dirs;
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_directory()) {
            dirs.push_back(entry.path().filename().string());
        }
    }
    std::sort(dirs.begin(), dirs.end());
    return dirs;
}

} // namespace FileUtils

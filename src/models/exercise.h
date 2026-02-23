#pragma once

#include <string>
#include <vector>

struct CodeBlock {
    int index;
    std::string initial_code;
};

struct Exercise {
    std::string chapter_id;
    std::string slug;
    std::string title;
    std::string content_html;
    std::vector<CodeBlock> code_blocks;
};

struct RunRequest {
    std::string source_code;
    std::string compiler_flags;
};

struct RunResult {
    std::string stdout_output;
    std::string stderr_output;
    std::string compile_output;
    int exit_code;
    std::string status;
    float time;
    int memory;
};

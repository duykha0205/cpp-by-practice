#pragma once

#include <string>
#include <vector>
#include <md4c-html.h>
#include "models/exercise.h"

class MarkdownService {
public:
    struct ParseResult {
        std::string html;
        std::vector<CodeBlock> code_blocks;
    };

    ParseResult render(const std::string& markdown) {
        ParseResult result;

        auto processed = extract_code_blocks(markdown, result.code_blocks);
        result.html = md_to_html(processed);

        return result;
    }

private:
    std::string md_to_html(const std::string& markdown) {
        std::string output;

        md_html(markdown.c_str(),
                static_cast<MD_SIZE>(markdown.size()),
                process_output,
                &output,
                MD_FLAG_TABLES | MD_FLAG_STRIKETHROUGH | MD_FLAG_TASKLISTS,
                0);

        return output;
    }

    static void process_output(const MD_CHAR* text, MD_SIZE size, void* userdata) {
        auto* output = static_cast<std::string*>(userdata);
        output->append(text, size);
    }

    std::string extract_code_blocks(const std::string& markdown,
                                     std::vector<CodeBlock>& blocks) {
        std::string result;
        int block_index = 0;

        size_t pos = 0;
        while (pos < markdown.size()) {
            size_t fence_start = markdown.find("```cpp,editable", pos);
            if (fence_start == std::string::npos) {
                result.append(markdown, pos, markdown.size() - pos);
                break;
            }

            result.append(markdown, pos, fence_start - pos);

            size_t code_start = markdown.find('\n', fence_start);
            if (code_start == std::string::npos) break;
            code_start++;

            size_t fence_end = markdown.find("\n```", code_start);
            if (fence_end == std::string::npos) {
                fence_end = markdown.size();
            }

            std::string code = markdown.substr(code_start, fence_end - code_start);

            while (!code.empty() && code.back() == '\n') code.pop_back();

            blocks.push_back({block_index, code});

            result += "<div class=\"exercise-editor\" data-block-index=\""
                    + std::to_string(block_index) + "\">"
                    + "<div class=\"editor-container\" id=\"editor-"
                    + std::to_string(block_index) + "\"></div>"
                    + "<div class=\"editor-controls\">"
                    + "<button class=\"run-btn\" data-block-index=\""
                    + std::to_string(block_index) + "\">&#9654; Run</button>"
                    + "<button class=\"reset-btn\" data-block-index=\""
                    + std::to_string(block_index) + "\">&#8634; Reset</button>"
                    + "</div>"
                    + "<div class=\"output-panel\" id=\"output-"
                    + std::to_string(block_index) + "\"></div>"
                    + "</div>\n";

            block_index++;

            pos = fence_end + 4; // skip "\n```"
            if (pos < markdown.size() && markdown[pos] == '\n') pos++;
        }

        return result;
    }
};

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "models/chapter.h"
#include "models/exercise.h"
#include "services/markdown_service.h"
#include "utils/file_utils.h"

using json = nlohmann::json;

class ExerciseService {
public:
    explicit ExerciseService(const std::string& content_dir)
        : content_dir_(content_dir), md_service_() {
        load_toc();
    }

    const std::vector<Chapter>& get_chapters() const {
        return chapters_;
    }

    const Chapter* find_chapter(const std::string& chapter_id) const {
        for (const auto& ch : chapters_) {
            if (ch.id == chapter_id) return &ch;
        }
        return nullptr;
    }

    Exercise load_exercise(const std::string& chapter_id,
                           const std::string& exercise_slug) {
        std::string cache_key = chapter_id + "/" + exercise_slug;
        auto it = exercise_cache_.find(cache_key);
        if (it != exercise_cache_.end()) {
            return it->second;
        }

        const Chapter* chapter = find_chapter(chapter_id);
        if (!chapter) {
            throw std::runtime_error("Chapter not found: " + chapter_id);
        }

        std::string file_path;
        std::string title;
        for (const auto& ex : chapter->exercises) {
            if (ex.slug == exercise_slug) {
                file_path = content_dir_ + "/chapters/" + chapter_id + "/" + ex.file;
                title = ex.title;
                break;
            }
        }

        if (file_path.empty()) {
            throw std::runtime_error("Exercise not found: " + exercise_slug);
        }

        std::string markdown = FileUtils::read_file(file_path);
        auto parsed = md_service_.render(markdown);

        Exercise exercise;
        exercise.chapter_id = chapter_id;
        exercise.slug = exercise_slug;
        exercise.title = title;
        exercise.content_html = parsed.html;
        exercise.code_blocks = parsed.code_blocks;

        exercise_cache_[cache_key] = exercise;
        return exercise;
    }

    struct NavInfo {
        std::string prev_chapter;
        std::string prev_slug;
        std::string prev_title;
        std::string next_chapter;
        std::string next_slug;
        std::string next_title;
    };

    NavInfo get_navigation(const std::string& chapter_id,
                           const std::string& exercise_slug) const {
        NavInfo nav;

        struct FlatEntry {
            std::string chapter_id;
            std::string slug;
            std::string title;
        };

        std::vector<FlatEntry> flat;
        for (const auto& ch : chapters_) {
            for (const auto& ex : ch.exercises) {
                flat.push_back({ch.id, ex.slug, ch.title + " - " + ex.title});
            }
        }

        for (size_t i = 0; i < flat.size(); i++) {
            if (flat[i].chapter_id == chapter_id && flat[i].slug == exercise_slug) {
                if (i > 0) {
                    nav.prev_chapter = flat[i - 1].chapter_id;
                    nav.prev_slug = flat[i - 1].slug;
                    nav.prev_title = flat[i - 1].title;
                }
                if (i + 1 < flat.size()) {
                    nav.next_chapter = flat[i + 1].chapter_id;
                    nav.next_slug = flat[i + 1].slug;
                    nav.next_title = flat[i + 1].title;
                }
                break;
            }
        }

        return nav;
    }

private:
    void load_toc() {
        std::string toc_path = content_dir_ + "/toc.json";
        std::string toc_str = FileUtils::read_file(toc_path);
        json toc = json::parse(toc_str);

        int order = 0;
        for (const auto& ch : toc["chapters"]) {
            Chapter chapter;
            chapter.id = ch["id"].get<std::string>();
            chapter.title = ch["title"].get<std::string>();
            chapter.order = order++;

            for (const auto& ex : ch["exercises"]) {
                ExerciseEntry entry;
                entry.slug = ex["slug"].get<std::string>();
                entry.title = ex["title"].get<std::string>();
                entry.file = ex["file"].get<std::string>();
                chapter.exercises.push_back(entry);
            }

            chapters_.push_back(chapter);
        }
    }

    std::string content_dir_;
    MarkdownService md_service_;
    std::vector<Chapter> chapters_;
    std::unordered_map<std::string, Exercise> exercise_cache_;
};

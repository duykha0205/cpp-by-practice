#pragma once

#include <crow.h>
#include "services/exercise_service.h"

class PageRoutes {
public:
    explicit PageRoutes(ExerciseService& exercise_service)
        : exercises_(exercise_service) {}

    void register_routes(crow::SimpleApp& app) {
        CROW_ROUTE(app, "/")(
            [this]() {
                return render_index();
            });

        CROW_ROUTE(app, "/chapter/<string>/<string>")(
            [this](const std::string& chapter_id, const std::string& slug) {
                return render_exercise(chapter_id, slug);
            });
    }

private:
    crow::response render_index() {
        auto& chapters = exercises_.get_chapters();

        crow::mustache::context ctx;
        crow::json::wvalue::list chapter_list;

        for (const auto& ch : chapters) {
            crow::json::wvalue ch_obj;
            ch_obj["id"] = ch.id;
            ch_obj["title"] = ch.title;

            crow::json::wvalue::list ex_list;
            for (const auto& ex : ch.exercises) {
                crow::json::wvalue ex_obj;
                ex_obj["slug"] = ex.slug;
                ex_obj["title"] = ex.title;
                ex_obj["url"] = "/chapter/" + ch.id + "/" + ex.slug;
                ex_list.push_back(std::move(ex_obj));
            }
            ch_obj["exercises"] = std::move(ex_list);
            chapter_list.push_back(std::move(ch_obj));
        }

        ctx["chapters"] = std::move(chapter_list);
        ctx["page_title"] = "C++ By Practice";

        auto page = crow::mustache::load("index.html");
        return crow::response(page.render(ctx));
    }

    crow::response render_exercise(const std::string& chapter_id,
                                    const std::string& slug) {
        try {
            auto exercise = exercises_.load_exercise(chapter_id, slug);
            auto nav = exercises_.get_navigation(chapter_id, slug);
            auto& chapters = exercises_.get_chapters();

            crow::mustache::context ctx;
            ctx["page_title"] = exercise.title + " - C++ By Practice";
            ctx["exercise_title"] = exercise.title;
            ctx["chapter_id"] = chapter_id;
            ctx["content"] = exercise.content_html;

            // Sidebar chapters
            crow::json::wvalue::list chapter_list;
            for (const auto& ch : chapters) {
                crow::json::wvalue ch_obj;
                ch_obj["id"] = ch.id;
                ch_obj["title"] = ch.title;
                ch_obj["is_active"] = (ch.id == chapter_id);

                crow::json::wvalue::list ex_list;
                for (const auto& ex : ch.exercises) {
                    crow::json::wvalue ex_obj;
                    ex_obj["slug"] = ex.slug;
                    ex_obj["title"] = ex.title;
                    ex_obj["url"] = "/chapter/" + ch.id + "/" + ex.slug;
                    ex_obj["is_current"] = (ch.id == chapter_id && ex.slug == slug);
                    ex_list.push_back(std::move(ex_obj));
                }
                ch_obj["exercises"] = std::move(ex_list);
                chapter_list.push_back(std::move(ch_obj));
            }
            ctx["chapters"] = std::move(chapter_list);

            // Code blocks as JSON for JavaScript
            crow::json::wvalue::list blocks;
            for (const auto& block : exercise.code_blocks) {
                crow::json::wvalue b;
                b["index"] = block.index;
                b["code"] = block.initial_code;
                blocks.push_back(std::move(b));
            }
            ctx["code_blocks_json"] = crow::json::wvalue(std::move(blocks)).dump();

            // Navigation
            ctx["has_prev"] = !nav.prev_slug.empty();
            ctx["has_next"] = !nav.next_slug.empty();
            if (!nav.prev_slug.empty()) {
                ctx["prev_url"] = "/chapter/" + nav.prev_chapter + "/" + nav.prev_slug;
                ctx["prev_title"] = nav.prev_title;
            }
            if (!nav.next_slug.empty()) {
                ctx["next_url"] = "/chapter/" + nav.next_chapter + "/" + nav.next_slug;
                ctx["next_title"] = nav.next_title;
            }

            auto page = crow::mustache::load("exercise.html");
            return crow::response(page.render(ctx));

        } catch (const std::exception& e) {
            crow::mustache::context ctx;
            ctx["page_title"] = "Not Found - C++ By Practice";
            ctx["error_message"] = std::string(e.what());
            auto page = crow::mustache::load("error.html");
            return crow::response(404, page.render(ctx));
        }
    }

    ExerciseService& exercises_;
};

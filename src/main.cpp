#include <crow.h>
#include <iostream>
#include <cstdlib>

#include "services/exercise_service.h"
#include "services/sandbox_runner.h"
#include "routes/page_routes.h"
#include "routes/api_routes.h"

int main() {
    std::string content_dir = "content";
    int port = 8080;

    if (const char* env = std::getenv("CONTENT_DIR"))
        content_dir = env;
    if (const char* env = std::getenv("PORT"))
        port = std::atoi(env);

    SandboxRunner::Config sandbox_config;
    if (const char* env = std::getenv("SANDBOX_IMAGE"))
        sandbox_config.docker_image = env;
    if (const char* env = std::getenv("SANDBOX_TIMEOUT"))
        sandbox_config.timeout_seconds = std::atoi(env);
    if (const char* env = std::getenv("SANDBOX_MEMORY"))
        sandbox_config.memory_limit = env;
    if (const char* env = std::getenv("SANDBOX_TMP_DIR"))
        sandbox_config.tmp_dir = env;

    std::cout << "=== C++ By Practice ===" << std::endl;
    std::cout << "Checking Docker availability..." << std::endl;

    if (!SandboxRunner::check_docker(sandbox_config.docker_image)) {
        std::cerr << "ERROR: Docker is not available or image '"
                  << sandbox_config.docker_image << "' not found." << std::endl;
        std::cerr << "Make sure Docker is running and run:" << std::endl;
        std::cerr << "  docker pull " << sandbox_config.docker_image << std::endl;
        return 1;
    }

    std::cout << "Docker OK (image: " << sandbox_config.docker_image << ")"
              << std::endl;

    crow::SimpleApp app;

    crow::mustache::set_global_base("templates");

    ExerciseService exercise_service(content_dir);
    SandboxRunner sandbox(sandbox_config);

    PageRoutes page_routes(exercise_service);
    ApiRoutes api_routes(sandbox);

    page_routes.register_routes(app);
    api_routes.register_routes(app);

    std::cout << "Content dir: " << content_dir << std::endl;
    std::cout << "Starting on port " << port << "..." << std::endl;

    app.port(port).multithreaded().run();

    return 0;
}

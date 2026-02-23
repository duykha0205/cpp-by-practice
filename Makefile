VCPKG_ROOT := $(CURDIR)/vcpkg
CMAKE_TOOLCHAIN := $(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake
BUILD_DIR := build
BINARY := $(BUILD_DIR)/cpp-by-practice

.PHONY: all setup deps build run clean rebuild docker docker-run help

all: build

# ---------- Setup ----------

setup: ## Bootstrap vcpkg + install deps + pull gcc:14
	@if [ ! -f $(VCPKG_ROOT)/vcpkg ]; then \
		echo ">>> Bootstrapping vcpkg..."; \
		$(VCPKG_ROOT)/bootstrap-vcpkg.sh; \
	fi
	$(VCPKG_ROOT)/vcpkg install
	docker pull gcc:14

deps: ## Install vcpkg dependencies only
	$(VCPKG_ROOT)/vcpkg install

# ---------- Build ----------

build: ## Configure + build the project
	cmake -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(CMAKE_TOOLCHAIN)
	cmake --build $(BUILD_DIR) --parallel

rebuild: clean build ## Clean then build from scratch

# ---------- Run ----------

run: build ## Build then run the server
	cd $(BUILD_DIR) && ./cpp-by-practice

# ---------- Docker ----------

docker: ## Build Docker image
	docker compose build

docker-run: ## Run with Docker Compose
	docker compose up --build

# ---------- Clean ----------

clean: ## Remove build directory
	rm -rf $(BUILD_DIR)

# ---------- Help ----------

help: ## Show available commands
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
		awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-14s\033[0m %s\n", $$1, $$2}'

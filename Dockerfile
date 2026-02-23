FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg \
    && /opt/vcpkg/bootstrap-vcpkg.sh

ENV VCPKG_ROOT=/opt/vcpkg
ENV PATH="${VCPKG_ROOT}:${PATH}"

WORKDIR /build
COPY vcpkg.json CMakeLists.txt ./
COPY src/ src/
COPY templates/ templates/
COPY static/ static/
COPY content/ content/

RUN cmake -B build \
    -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel $(nproc)

# --- Runtime stage ---
FROM ubuntu:24.04

COPY --from=docker:27-cli /usr/local/bin/docker /usr/local/bin/docker

WORKDIR /app

COPY --from=builder /build/build/cpp-by-practice .
COPY templates/ templates/
COPY static/ static/
COPY content/ content/

ENV PORT=8080
ENV CONTENT_DIR=content
ENV SANDBOX_IMAGE=gcc:14
ENV SANDBOX_TIMEOUT=5
ENV SANDBOX_MEMORY=128m

EXPOSE 8080

CMD ["./cpp-by-practice"]

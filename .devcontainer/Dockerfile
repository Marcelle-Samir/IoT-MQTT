FROM mcr.microsoft.com/devcontainers/cpp:1-debian-11
ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

# Install system dependencies
RUN apt-get update && \
    apt-get install -y \
    libboost-all-dev \
    libcurl4-openssl-dev \
    uuid-dev \
    python3 \
    python3-pip \
    build-essential \
    cmake \
    git \
    mosquitto \
    mosquitto-clients \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/protocolbuffers/protobuf.git /protobuf && \
    cd /protobuf && \
    git checkout v5.27.0 && \
    git submodule update --init --recursive && \
    mkdir build && cd build && \
    cmake .. -Dprotobuf_BUILD_TESTS=OFF && \
    make -j$(nproc) && \
    make install && \
    ldconfig

RUN pip3 install protobuf==5.27.0

# Install Node.js and npm (for running json-server)
RUN curl -sL https://deb.nodesource.com/setup_18.x | bash - \
    && apt-get install -y nodejs \
    && npm install -g json-server \
    && rm -rf /var/lib/apt/lists/*

# Install Conan using pip
RUN pip3 install conan

# Manually create a default Conan profile with correct format
RUN mkdir -p /home/vscode/.conan2/profiles && \
    echo '[settings]\n' \
    'os=Linux\n' \
    'compiler=gcc\n' \
    'compiler.version=11\n' \
    'compiler.libcxx=libstdc++11\n' \
    'build_type=Release\n' \
    'arch=x86_64\n' > /home/vscode/.conan2/profiles/default

# Ensure correct permissions
RUN chown -R vscode:vscode /home/vscode/.conan2
RUN apt-get update && apt-get install -y iputils-ping

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh
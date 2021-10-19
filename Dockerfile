FROM ubuntu:18.04
LABEL org.truebit.image.authors="Evin Grano"

SHELL ["/bin/bash", "-c"]

# Get packages list and user utilities
RUN apt-get update && apt-get install -y jq nano tmux vim

# Set up Emscripten
RUN apt-get install -y cmake g++ git python \
 && git clone https://github.com/emscripten-core/emsdk.git emsdk \
 && cd emsdk \
 && ./emsdk install sdk-fastcomp-1.37.36-64bit \
 && ./emsdk install binaryen-tag-1.37.36-64bit \
 && ./emsdk activate sdk-fastcomp-1.37.36-64bit \
 && ./emsdk activate binaryen-tag-1.37.36-64bit

# Install LLVM components
RUN apt-get install -y ninja-build \
 && git clone https://github.com/llvm-mirror/llvm \
 && cd llvm/tools \
 && git clone https://github.com/llvm-mirror/clang \
 && git clone https://github.com/llvm-mirror/lld \
 && cd /llvm \
 && git checkout release_60 \
 && cd tools/clang \
 && git checkout release_60 \
 && cd ../lld \
 && git checkout release_60 \
 && mkdir /build \
 && cd /build \
 && cmake -G Ninja -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=WebAssembly -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=/usr/ /llvm \
 && ninja \
 && ninja install \
 && cd / \
 && rm -rf build llvm

# Add support for Rust tasks
RUN apt-get install curl \
 && curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y \
 && source $HOME/.cargo/env \
 && rustup install 1.40.0 \
 && rustup default 1.40.0 \
 && rustup target add wasm32-unknown-emscripten \
 && cd emsdk \
 && ./emsdk install 1.38.33 \
 && ./emsdk install 1.39.8 \
 && rm -r zips

# Install Node package manager
RUN apt-get install wget \
 && wget -qO- https://raw.githubusercontent.com/nvm-sh/nvm/v0.35.3/install.sh | bash \
 && source ~/.nvm/nvm.sh \
 && nvm install 14.10.0

# Install Solidity
RUN cd bin \
 && wget https://github.com/ethereum/solidity/releases/download/v0.5.17/solc-static-linux \
 && mv solc-static-linux solc \
 && chmod 744 solc

# Install Geth
RUN wget https://gethstore.blob.core.windows.net/builds/geth-alltools-linux-amd64-1.10.8-26675454.tar.gz \
 && tar xf geth*tar.gz \
 && rm geth*tar.gz \
 && cd geth* \
 && cp geth /bin \
 && cp clef /bin \
 && rm -rf /geth*

# Install IPFS
RUN wget https://dist.ipfs.io/go-ipfs/v0.7.0/go-ipfs_v0.7.0_linux-amd64.tar.gz \
 && tar xf go-ipfs_v0.7.0_linux-amd64.tar.gz \
 && cd go-ipfs \
 && ./install.sh \
 && cd / \
 && rm -rf go-ipfs*

# Download Truebit
COPY . truebit-eth/
RUN cd /truebit-eth \
 && wget -O truebit-os https://truebit.io/downloads/truebit-linux \
 && chmod 755 truebit-os \
 && rm -r .git

# Install ocaml-offchain interpreter
RUN apt-get update \
 && apt-get install -y libffi-dev libzarith-ocaml-dev m4 opam pkg-config zlib1g-dev \
 && opam init -y \
 && eval `opam config env` \
 && opam update \
 && opam install cryptokit ctypes ctypes-foreign yojson -y \
 && cd /truebit-eth/ocaml-offchain/interpreter \
 && make \
 && rm -rf ~/.opam

# Install Emscripten module wrapper and dependencies for deploying sample tasks
RUN source ~/.nvm/nvm.sh \
 && ln -s /truebit-eth/emscripten-module-wrapper /root/emscripten-module-wrapper \
 && ln -s /truebit-eth/ocaml-offchain \
 && cd truebit-eth \
 && npm i

# Install Toolchain libraries
RUN apt-get install -y autoconf bison flex libtool lzip \
 && source /emsdk/emsdk_env.sh \
 && sed -i "s|LLVM_ROOT = emsdk_path + '/fastcomp-clang/e1.37.36_64bit'|LLVM_ROOT = '/usr/bin'|" /emsdk/.emscripten \
 && sed -i "s|EMSCRIPTEN_NATIVE_OPTIMIZER = emsdk_path + '/fastcomp-clang/e1.37.36_64bit/optimizer'|EMSCRIPTEN_NATIVE_OPTIMIZER = ''|" /emsdk/.emscripten \
 && cd /truebit-eth/wasm-ports \
 && sh gmp.sh \
 && sh openssl.sh \
 && sh secp256k1.sh \
 && sh libff.sh \
 && sh boost.sh \
 && sh libpbc.sh

# Install Tasks Directory
RUN cd /truebit-eth/wasm-ports \
 && git clone https://github.com/TruebitProtocol/truebit-sample-tasks.git sample-tasks

# Compile  C/C++ sample tasks
RUN ipfs init \
 && ( ipfs daemon & ) \
 && source /emsdk/emsdk_env.sh \
 && sed -i "s|LLVM_ROOT = emsdk_path + '/fastcomp-clang/e1.37.36_64bit'|LLVM_ROOT = '/usr/bin'|" /emsdk/.emscripten \
 && sed -i "s|EMSCRIPTEN_NATIVE_OPTIMIZER = emsdk_path + '/fastcomp-clang/e1.37.36_64bit/optimizer'|EMSCRIPTEN_NATIVE_OPTIMIZER = ''|" /emsdk/.emscripten \
 && cd /truebit-eth/wasm-ports/sample-tasks/tasks/map_reduce \
 && sh compile.sh \
 && rm -r /root/.ipfs

# Move initialization scripts for compiling, network, and authentication.  Re-configure for C/C++ samples.  Clean up root directory.
RUN mv /truebit-eth/goerli.sh / \
 && mv /truebit-eth/mainnet.sh / \
 && cd emsdk \
 && ./emsdk activate sdk-fastcomp-1.37.36-64bit \
 && ./emsdk activate binaryen-tag-1.37.36-64bit \
 && cd / \
 && rm -r boot home media mnt opt srv tmp/* \
 && echo -e '\n# Set up Emscripten\nsource /emsdk/emsdk_env.sh &>/dev/null\n\n# Create Geth keystore directories\nmkdir -p ~/.ethereum/keystore\nmkdir -p ~/.ethereum/goerli/keystore' >> ~/.bashrc

# Open IPFS and blockchain ports
EXPOSE 4001 8080 8545 8546 30303

# Container incantations
# BUILD: docker build . -t truebit:latest
# START CONTAINER: docker run --rm -it truebit:latest /bin/bash
# OPEN NEW TERMINAL WINDOW: docker exec -it _yourContainerName_ /bin/bash
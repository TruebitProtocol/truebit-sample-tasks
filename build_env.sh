#1/bin/bash
# Set up build environment
/emsdk/emsdk activate sdk-fastcomp-1.37.36-64bit
/emsdk/emsdk activate binaryen-tag-1.37.36-64bit
source /emsdk/emsdk_env.sh

# tweak emsdk setup
sed -i "s|EMSCRIPTEN_NATIVE_OPTIMIZER = emsdk_path + '/fastcomp-clang/e1.37.36_64bit/optimizer'|EMSCRIPTEN_NATIVE_OPTIMIZER = ''|" /emsdk/.emscripten
sed -i "s|LLVM_ROOT = emsdk_path + '/fastcomp-clang/e1.37.36_64bit'|LLVM_ROOT = '/usr/bin'|" /emsdk/.emscripten

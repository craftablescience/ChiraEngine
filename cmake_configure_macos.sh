#!/bin/sh

#  cmake_configure_macos.sh
#  
#
#  Created by Ash on 8/12/22.
#  

cmake -G "Unix Makefiles" -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER="/usr/local/opt/llvm/bin/clang" -DCMAKE_CXX_COMPILER="/usr/local/opt/llvm/bin/clang++"

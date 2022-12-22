#!/bin/sh

#  cmake_configure_macos.sh
#  
#
#  Created by Ash on 8/12/22.
#  

config_noxcode()
{
    echo "Configuring Cmake for Unix Makefiles"
    cmake -G "Unix Makefiles" -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER="/usr/local/opt/llvm/bin/clang" -DCMAKE_CXX_COMPILER="/usr/local/opt/llvm/bin/clang++"
}

config_xcode()
{
    echo "Configuring Cmake for Xcode Projects"
    cmake -G "Xcode" -B build -DCMAKE_BUILD_TYPE=Debug
}

check_param()
{
    TYPE="$1"
    
    if [ "$TYPE" ];
    then
        if [ "$TYPE" == "Xcode" ];
        then
            config_xcode
        elif [ "$TYPE" == "Makefiles" ];
        then
            config_noxcode
        else
            echo "No valid parameter given"
            echo "Supported types: Xcode, Makefiles"
        fi
    else
        echo "No valid parameter given"
        echo "Supported types: Xcode, Makefiles"
    fi
}

check_param "$1"

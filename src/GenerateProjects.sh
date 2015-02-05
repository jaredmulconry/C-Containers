#!/bin/bash

argCount=$#

if [ $argCount -lt 2  ]; then 
	echo "Too few arguments to script."
	echo "arg1: directory to store generated project files."
	echo "arg2: build target in cmake format (Debug, RelWithDebInfo, etc.)"
	exit 1
fi
if [ ! -e "./CMakeLists.txt" ]; then
	echo "Current directory does not container CMakeLists."
	exit 1
fi

#set up paths and other settings
args=("$@")
src_dir=$(pwd)
dest_dir=${args[0]}
build_mode=${args[1]}
args=("$@")
gxx_dest_suffix=gcc
clangxx_dest_suffix=clang
gxx_dest_dir="${dest_dir}/${gxx_dest_suffix}"
clangxx_dest_dir="${dest_dir}/${clangxx_dest_suffix}"

#Make directories for each compiler target
mkdir -p $gxx_dest_dir
mkdir -p $clangxx_dest_dir

#Move to gxx project directory
cd $gxx_dest_dir
#Generate gxx make files
CC=gcc CXX=g++ cmake -D CMAKE_BUILD_TYPE=$build_mode -G "Unix Makefiles" $src_dir
#Move to clangxx project directory
cd $clangxx_dest_dir
#Generate clangxx make files
CC=clang CXX=clang++ cmake -D CMAKE_BUILD_TYPE=$build_mode -G "Unix Makefiles" $src_dir

#Return to source directory
cd $src_dir


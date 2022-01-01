#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
. $SCRIPT_DIR/common.sh

mkdir $VENDOR

# Donwload STB
cd $VENDOR
STB_IMAGE_LIB="https://raw.githubusercontent.com/nothings/stb/master/stb_image.h"
mkdir -p $VENDOR/stb_image
cd $VENDOR/stb_image/

echo '#define STB_IMAGE_IMPLEMENTATION' > stb_image.cpp
echo '#include "stb_image.h"' >> stb_image.cpp

wget $STB_IMAGE_LIB
cd $SCRIPT_DIR

# GLUT
sudo apt-get install freeglut3-dev

# Donwload GLFW and build
cd $VENDOR
GLFW_TGZ="https://webwerks.dl.sourceforge.net/project/glew/glew/2.1.0/glew-2.1.0.tgz"
mkdir -p $VENDOR
wget $GLFW_TGZ
tar -xvf glew-2.1.0.tgz
cd glew-2.1.0
make
cd $SCRIPT_DIR

# GLM
sudo apt install libglm-dev

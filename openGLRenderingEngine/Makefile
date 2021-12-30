CXX ?= g++

CXXFLAGS = -std=c++17 -I. \
			-I./vendor/glew-2.1.0/include \
			-I./vendor \
			-Wl,-no-as-needed \
			-Wno-error

LDFLAGS =	./vendor/glew-2.1.0/lib/libGLEW.a \
			-lGL    \
			-lGLU   \
			-lglfw

cppFiles = $(shell find . | grep -v vendor | grep -E ".cpp$$")
hppFiles = $(shell find . | grep -v vendor | grep -E ".hpp$$")
hFiles = $(shell find . | grep -v vendor | grep -E ".h$$")
vendorFilesToBuild = ./vendor/stb_image/stb_image.cpp

TARGET = window

all: $(TARGET)
	echo Done Building

$(TARGET): $(cppFiles) $(hppFiles) $(hFiles)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(cppFiles) $(vendorFilesToBuild) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(TARGET)
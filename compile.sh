#!/bin/sh
g++ -o teeworlds3d -O3 -g -pg -DLINUX -Isrc -Iother/sdl/include -Iother/glew/include -Iother/glm --std=c++1y src/shared/*.cpp src/shared/world/*.cpp src/client/*.cpp src/client/components/*.cpp src/client/components/graphics/*.cpp src/client/components/graphics/models/*.cpp src/client/components/graphics/shaders/*.cpp src/client/components/graphics/geometry/*.cpp -lGL -lGLEW -lSDL2 -lSDL2_image -lSDL2_ttf -lGLU 

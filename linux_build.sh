#!/bin/sh

# Enviornment Settings
mkdir -p tmp

# MiniEngine Library
echo "Building MiniEngine..."
g++ -Wall -fexceptions -O2 -std=c++14 -c MiniEngine.cpp -o tmp/MiniEngine.o
g++ -Wall -fexceptions -O2 -std=c++14 -c MiniEngine_Event.cpp -o tmp/MiniEngine_Event.o
g++ -Wall -fexceptions -O2 -std=c++14 -c MiniEngine_Widget.cpp -o tmp/MiniEngine_Widget.o
gcc -Wall -O2 -c sqlite/sqlite3.c -o tmp/sqlite.o
g++ -Wall -fexceptions -O2 -std=c++14 -c MiniEngine_SQLite.cpp -o tmp/MiniEngine_SQLite.o

# Add Your File Here
echo "Build Your Files..."
# g++ -Wall -fexceptions -O2 -std=c++14 -c main.cpp -o tmp/main.o

# Link
echo "Linking..."
g++ -o prog tmp/MiniEngine.o tmp/MiniEngine_Event.o tmp/MiniEngine_Widget.o tmp/sqlite.o tmp/MiniEngine_SQLite.o tmp/main.o -lstdc++ -lpthread -ldl -lm -lz -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_mixer -lSDL2_ttf -s

# Clean Up
echo "Cleaning Up..."
rm -rf tmp


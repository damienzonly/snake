rm -rf build
mkdir build
cd build
CC=gcc cmake ..
clear
make
./snake
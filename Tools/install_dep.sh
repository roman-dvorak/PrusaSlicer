sudo apt-get install  -y git build-essential autoconf cmake libglu1-mesa-dev libgtk-3-dev libdbus-1-dev

cd deps
mkdir build
cd build
cmake .. -DDEP_WX_GTK3=ON
make
cd ../..

mkdir build
cd build
cmake .. -DSLIC3R_STATIC=1 -DSLIC3R_GTK=3 -DSLIC3R_PCH=OFF -DCMAKE_PREFIX_PATH=$(pwd)/../deps/build/destdir/usr/local
make -j4

cd src
./prusa-slicer --help

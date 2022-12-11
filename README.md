# plotter

# Requirements:

- gnuplot: http://tmacchant33.starfree.jp/gnuplot_files/gp550-20221210-win64-mingw.zip
  - Need to add `bin` directory to PATH manually
- CMake: https://cmake.org/download/
- git: https://git-scm.com/downloads

Make sure that PATH knows about all required tools above.

# How to start (Windows):
Open a console in some directory and then:
```
git clone https://github.com/zankokun/plotter.git
cd plotter
mkdir build
cd build
cmake ..
cmake --build .
./plotter.sln
```
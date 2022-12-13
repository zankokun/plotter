# plotter

# Requirements & Links to download (Windows):

- git: https://git-scm.com/downloads
- CMake: https://cmake.org/download/
- gnuplot: http://tmacchant33.starfree.jp/gnuplot_files/gp550-20221210-win64-mingw.zip
  - Need to unpack somewhere and then add `bin` directory to PATH manually
- ffmpeg: https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-master-latest-win64-gpl.zip
  - Need to unpack somewhere and then add `bin` directory to PATH manually

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

# Possible issues:
- Garbadge in the output
  - Enable the new UTF-8 option in `Windows settings`. Go to the `language settings`, click `Administrative language settings`, then `Change system locale…` and tick the Beta: `Use Unicode UTF-8 for worldwide language support option`.
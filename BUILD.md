# Build Instructions

## Contents

- [Getting the source](#getting-the-source)
- [Linux and FreeBSD](#linux-and-freebsd)
- [Windows](#windows)
- [macOS](#macos)
- [OpenBSD](#openbsd)

# Getting the source

Clone the source code using git, and grab all the submodules:

```
git clone -b stable https://github.com/fn2006/PolyMC.git
cd PolyMC
git submodule init
git submodule update
```

**The rest of the documentation assumes you have already cloned the repository.**

# Linux and FreeBSD

Getting the project to build and run on Linux is easy if you use any modern and up-to-date linux distribution. If you're using FreeBSD you should use 13.0-RELEASE or newer.

## Build dependencies
- A C++ compiler capable of building C++11 code.
- Qt Development tools 5.6 or newer (`qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools libqt5core5a libqt5network5 libqt5gui5` on Debian-based system)
- cmake 3.1 or newer (`cmake` on Debian-based system)
- zlib (`zlib1g-dev` on Debian-based system)
- Java JDK (`openjdk-17-jdk`on Debian-based system)
- GL headers (`libgl1-mesa-dev` on Debian-based system)
- games/lwjgl port if using FreeBSD

You can use IDEs, like KDevelop or QtCreator to open the CMake project, if you want to work on the code.

### Building a portable binary

```bash
mkdir install
# configure the project
cmake -S . -B build \
   -DCMAKE_INSTALL_PREFIX=./install
# build
cd build
make -j$(nproc) install
```

### Building & installing to the system

This is the preferred method of installation, and is suitable for packages.

```bash
# configure everything
cmake -S . -B build \
   -DCMAKE_BUILD_TYPE=Release \
   -DCMAKE_INSTALL_PREFIX="/usr" \ # Use "/usr" when building Linux packages. If building on FreeBSD or not for package, use "/usr/local"
   -DLauncher_PORTABLE=OFF
cd build
make -j$(nproc) install # Optionally specify DESTDIR for packages (i.e. DESTDIR=${pkgdir})
```

### Building a .deb

Requirements: [makedeb](https://docs.makedeb.org/) installed on your system.

```
git clone https://mpr.makedeb.org/polymc.git
cd polymc
makedeb -s
```

The .deb will be located in the directory the repo was cloned in.

### Building an .rpm

Build dependencies are automatically installed using `DNF`, however, you will also need the `rpmdevtools` package (on Fedora),
in order to fetch sources and setup your tree.  
You don't need to clone the repo for this; the spec file handles that.

```
cd ~
# setup your ~/rpmbuild directory, required for rpmbuild to work.
rpmdev-setuptree
# get the rpm spec file from the polymc-misc repo
wget https://copr-dist-git.fedorainfracloud.org/cgit/sentry/polymc/polymc.git/plain/polymc.spec
# install build dependencies
sudo dnf builddep polymc.spec
# download build sources
spectool -g -R polymc.spec
# now build!
rpmbuild -bb polymc.spec
```

The path to the .rpm packages will be printed once the build is complete.

### Building a Slackware package

To build a Slackware package, first install [qt5 SlackBuild](http://slackbuilds.org/repository/14.2/libraries/qt5/) (on 15.0 and newer installed by defualt), then set up a [JDK](https://codeberg.org/glowiak/SlackBuilds/raw/branch/master/tgz/adoptium-jdk8.tar.gz).

If you're using Slackware 14.2, update CMake with these commands:

```
mkdir -p /tmp/SBo
cd /tmp/SBo
wget -c https://github.com/Kitware/CMake/releases/download/v3.22.2/cmake-3.22.2.tar.gz
tar xzvf cmake-3.22.2.tar.gz
cd cmake-3.22.2
./configure --prefix=/usr
make
sudo make install
```

Next, download the [SlackBuild](https://codeberg.org/glowiak/SlackBuilds/raw/branch/master/tgz/polymc.tar.gz), unpack it, and then type the following in the extracted directory:

```
sudo ./polymc.SlackBuild # script will do everything, just sit up and wait
sudo /sbin/installpkg /tmp/polymc-version-arch-1_SBo.tgz # install the created package
```

### Building a Flatpak

You don't need to clone the entire PolyMC repo for this; the Flatpak file handles that.  
Both `flatpak` and `flatpak-builder` must be installed on your system to proceed.

```bash
git clone https://github.com/flathub/org.polymc.PolyMC
cd org.polymc.PolyMC
# remove --user --install if you want to build without installing
flatpak-builder --user --install flatbuild org.polymc.PolyMC.yml
```

### Installing Qt using the installer (optional)

1. Run the Qt installer.
2. Choose a place to install Qt.
3. Choose the components that you wish install.
   - You need Qt 5.6.x 64-bit ticked.
   - You need Tools/Qt Creator ticked.
   - Other components are selected by default, you can un-tick them if you don't need them.
4. Accept the license agreements.
5. Double-check the install details and then click "Install".
   - Installation can take a very long time, go grab a cup of tea or something and let it work.

### Loading the project in Qt Creator (optional)

1. Open Qt Creator.
2. Choose `File->Open File or Project`.
3. Navigate to the Launcher source folder you cloned and choose CMakeLists.txt.
4. Read the instructions that just popped up about a build location and choose one.
5. You should see "Run CMake" in the window.
   - Make sure that Generator is set to "Unix Generator (Desktop Qt 5.6.x GCC 64bit)".
   - Hit the "Run CMake" button.
   - You'll see warnings and it might not be clear that it succeeded until you scroll to the bottom of the window.
   - Hit "Finish" if CMake ran successfully.
6. Cross your fingers, and press the "Run" button (bottom left of Qt Creator).
   - If the project builds successfully it will run and the Launcher window will pop up.

**If this doesn't work for you, please let us know on our Discord sever, or Matrix Space.**

### Loading the project in CLion (optional)
1. Open CLion
2. Choose `File->Open`
3. Navigate to the source folder
4. Go to settings `Ctrl+Alt+S`
5. Navigate to `Toolchains` in `Build, Execution, Deployment`
    - Set the correct build tools ([see here](https://i.imgur.com/daFAdVe.png))
    - CMake: `cmake` (optional)
    - Make: `make` (optional)
    - C Compiler: `gcc`
    - C++ Compiler: `g++`
    - Debugger: `gdb` (optional)
6. Navigate to `CMake` in `Build, Execution, Deployment`
    - Set `Build directory` to `build`
7. Navigate to `Edit Configurations`  ([see here](https://i.imgur.com/fu53nc3.png))
    - Create a new configuration
    - Name: `All`
    - Target: `All targets`
    - Choose the newly added configuration as default

Now you should be able to build and test PolyMC with the `Build` and `Run` buttons.

# Windows

We recommend using a build workflow based on MSYS2, as it's the easiest way to get all of the build dependencies.

## Dependencies

- [MSYS2](https://www.msys2.org/) - Software Distribution and Building Platform for Windows
  - Make sure to follow all instructions on the webpage.
- [Java JDK 8 or later](https://adoptium.net/)
  - Make sure that "Set JAVA_HOME variable" is enabled in the Adoptium installer.

## Getting set up

### Preparing MSYS2

1. Open the *MSYS2 MinGW x64* shortcut from the start menu
  - NOTE: There are multiple different MSYS2 related shortcuts. Make sure you actually opened the right **MinGW** version.
  - If you are on a 32-bit Windows machine, you probably need to start *MSYS2 MinGW x86* instead.
2. Install helpers: Run `pacman -Syu pactoys git` in the MSYS2 shell.
3. Install all build dependencies using `pacboy`: Run `pacboy -S toolchain:p cmake:p ninja:p qt5:p`.
  - This might take a while, as it will install Qt and all the build tools required.

### Compile from command line on Windows

1. Open the right **MSYS2 MinGW** shell and clone PolyMC by doing `git clone --recursive https://github.com/PolyMC/PolyMC.git`, and change directory to the folder you cloned to.
2. Now we can prepare the build itself: Run `cmake -Bbuild -DCMAKE_INSTALL_PREFIX=install`. These options will copy the final build to `C:\msys64\home\<your username>\PolyMC\install` after the build.
3. If you want PolyMC to store its data in `%APPDATA%`, append `-DLauncher_PORTABLE=OFF` to the previous command.
4. Now you need to run the build itself: Run `cmake --build build -jX`, where *X* is the number of cores your CPU has.
5. Now, wait for it to compile. This could take some time, so hopefully it compiles properly.
6. Run the command `cmake --install build`, and it should install PolyMC to whatever the `-DCMAKE_INSTALL_PREFIX` was.
7. In most cases, whenever compiling, the OpenSSL DLLs aren't put into the directory to where PolyMC installs, meaning that you cannot log in. The best way to fix this, is just to do `cp /mingw64/bin/libcrypto-1_1-x64.dll /mingw64/bin/libssl-1_1-x64.dll install`. This should copy the required OpenSSL DLLs to log in.

# macOS

## Install prerequisites:

- Install XCode Command Line tools.
- Install the official build of CMake (https://cmake.org/download/).
- Install JDK 8 (https://adoptium.net/releases.html?variant=openjdk8&jvmVariant=hotspot).
- Get Qt 5.6, and install it (https://download.qt.io/new_archive/qt/5.6/5.6.3/) or higher (tested) (https://www.qt.io/download-qt-installer?utm_referrer=https%3A%2F%2Fwww.qt.io%2Fdownload-open-source).

You can use `homebrew` to simplify the installation of build dependencies.

### XCode Command Line tools

If you don't have XCode Command Line tools installed, you can install them with this command:

```bash
xcode-select --install
```

### Build

Choose an installation path. 

This is where the final `PolyMC.app` will be constructed when you run `make install`. Supply it as the `CMAKE_INSTALL_PREFIX` argument during CMake configuration. By default, it's in the dist folder, under PolyMC.

```
mkdir build
cd build
cmake \
 -DCMAKE_C_COMPILER=/usr/bin/clang \
 -DCMAKE_CXX_COMPILER=/usr/bin/clang++ \
 -DCMAKE_BUILD_TYPE=Release \
 -DCMAKE_INSTALL_PREFIX:PATH="$(dirname $PWD)/dist/" \
 -DCMAKE_PREFIX_PATH="/path/to/Qt/" \
 -DQt5_DIR="/path/to/Qt/" \
 -DCMAKE_OSX_DEPLOYMENT_TARGET=10.7 \
 ..
make install
```

Remember to replace `/path/to/Qt/` with the actual path. For newer Qt installations, it is often in your home directory.

**Note:** The final app bundle may not run due to code signing issues, which
need to be fixed with `codesign -fs -`.

# OpenBSD

Tested on OpenBSD 7.0-alpha i386. It should also work on older versions.

## Build dependencies
- A C++ compiler capable of building C++11 code (included in base system).
- Qt Development tools 5.6 or newer ([meta/qt5](https://openports.se/meta/qt5)).
- cmake 3.1 or newer ([devel/cmake](https://openports.se/devel/cmake)).
- zlib (included in base system).
- Java JDK ([devel/jdk-1.8](https://openports.se/devel/jdk/1.8)).
- GL headers (included in base system).
- lwjgl ([games/lwjgl](https://openports.se/games/lwjgl) and [games/lwjgl3](https://openports.se/games/lwjgl3)).

You can use IDEs, like KDevelop or QtCreator, to open the CMake project if you want to work on the code.

### Building a portable binary

```bash
mkdir install
# configure the project
cmake -S . -B build \
   -DCMAKE_INSTALL_PREFIX=./install -DCMAKE_PREFIX_PATH=/usr/local/lib/qt5/cmake
# build
cd build
make -j$(nproc) install
```

### Building & installing to the system

This is the preferred method of installation, and is suitable for packages.

```bash
# configure everything
cmake -S . -B build \
   -DCMAKE_BUILD_TYPE=Release \
   -DCMAKE_INSTALL_PREFIX="/usr/local" \ # /usr/local is default in OpenBSD and FreeBSD
   -DLauncher_PORTABLE=OFF -DCMAKE_PREFIX_PATH=/usr/local/lib/qt5/cmake # use linux layout and point to qt5 libs
cd build
make -j$(nproc) install # Optionally specify DESTDIR for packages (i.e. DESTDIR=${pkgdir})
```

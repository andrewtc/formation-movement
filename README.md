# Formation Movement Test Bed App
This app was developed as part of my [_Formation Movement for Real-time Strategy Games_](https://andrewtc.dev/thesis/) thesis project at [The Guildhall at SMU](https://www.smu.edu/guildhall). The code is provided as-is and with the express permission of the faculty at Southern Methodist University.

## 📦 Downloading binaries
See the [Releases](https://github.com/andrewtc/formation-movement/releases) page for pre-built executable files. These are currently **only** available for Windows.

## 🏗️ Building from source code
Please note that this app has **only** been tested on Windows so far. As such, only Windows build instructions are available.

### Windows
The test bed app can be built using the [meson](https://mesonbuild.com/) build system.

#### Prerequisites
 - Install Visual Studio, including the MSVC toolchain.
 - [Install meson](https://mesonbuild.com/Getting-meson.html).

#### Building

1. Clone the repo, e.g. to `C:\dev\formation-movement`.
2. Open a command prompt in the root project folder.
3. To create and configure a new build directory named `builddir`, run:
   ```ps1
   meson setup builddir -Dglfw:install=true
   ```
4. To compile and install the app, run:
   ```ps1
   meson install -C builddir --tags=runtime --destdir "C:\formation-movement"
   ```
   Note that `"C:\formation-movement"` can be the name of any directory. This is where the installed files will end up.
5. Navigate to the `bin` subfolder of the install directory, e.g. `cd C:\formation-movement\bin`.
6. To launch the app, run `.\FormationMovement.exe`.

If successful, you should see something like the following:

![A preview of the test bed app running.](/screenshots/test-bed-app.png)
# Building from source
Please note that this app has **only** been tested on Windows so far. As such, only Windows build instructions are available.

## Windows
The test bed app can be built using the [meson](https://mesonbuild.com/) build system.

### Prerequisites
 - Install Visual Studio, including the MSVC toolchain.
 - [Install meson](https://mesonbuild.com/Getting-meson.html).

### Building

1. Clone the repo, e.g. to `C:\dev\formation-movement`.
2. Open a command prompt in the root project folder.
3. To create and configure a new build directory named `builddir`, run:
   ```ps1
   meson builddir -Dglfw:install=true
   ```
4. To compile and install the app, run:
   ```ps1
   meson install -C builddir --destdir "C:\formation-movement"
   ```
   Note that `"C:\formation-movement"` can be the name of any directory. This is where the installed files will end up.
5. Navigate to the `bin` subfolder of the install directory, e.g. `cd C:\formation-movement\bin`.
6. To launch the app, run `.\FormationMovement.exe`.

If successful, you should see something like the following:

![A preview of the test bed app running.](/screenshots/test-bed-app.png)
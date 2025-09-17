# FirstProject2 — C++ + SFML 3 with CMake, vcpkg & VS Code

Cross‑platform starter for a small 2D game using **SFML 3** (no engine). The repo is set up so macOS and Windows teammates can build and debug with the same commands and the same VS Code buttons.

---

## Quick Start (both macOS & Windows)

> Do this from the repository root.

```bash
# 1) Make sure a local vcpkg/ folder exists at the repo root (not committed)
#    If it's missing:
# macOS/Linux:   git clone https://github.com/microsoft/vcpkg.git
# Windows:       git clone https://github.com/microsoft/vcpkg.git

# 2) Bootstrap vcpkg (one time per machine)
# macOS/Linux:
./vcpkg/bootstrap-vcpkg.sh
# Windows (PowerShell):
.\vcpkg\bootstrap-vcpkg.bat

# 3) Configure (first run will auto-install SFML via vcpkg)
cmake --preset default

# 4) Build
cmake --build --preset build

# 5) Run
# macOS:
./build/your_game
# Windows:
.\build\your_game.exe
```

**VS Code**: Press `F5` to run/debug. The single `launch.json` uses the **active CMake target** so it works on both OSes.

---

## Prerequisites

### macOS

* Xcode **Command Line Tools** (`xcode-select --install`)
* VS Code + extensions: `CMake Tools` and `C/C++`
* Optional: `brew install ninja`

### Windows

* **Visual Studio 2022** with *Desktop development with C++* (MSVC, Windows SDK, CMake)
* VS Code + extensions: `CMake Tools` and `C/C++`
* Optional: `winget install Ninja-build.Ninja` (or remove the Ninja generator from presets)

---

## Repository Layout

```
.vscode/              # Shared editor/debug config (works on Mac + Windows)
build/                # Generated build tree (ignored)
src/                  # Your C++ sources (e.g., main.cpp)
vcpkg/                # Local copy of vcpkg (not committed)
.gitignore            # Keeps build junk out of the repo
CMakeLists.txt        # Build recipe for this project
CMakePresets.json     # Shared configure/build presets + vcpkg toolchain hook
README.md             # This file
vcpkg.json            # Dependencies manifest (e.g., "sfml")
```

### What those files do (quick)

* **CMakeLists.txt** — defines the executable and links SFML 3 targets.
* **CMakePresets.json** — tells CMake to use the **vcpkg toolchain** at `vcpkg/scripts/buildsystems/vcpkg.cmake`, sets build directories, generator, etc.
* **vcpkg.json** — declares dependencies. vcpkg installs them on first configure.
* **.vscode/** — one cross‑platform launch config using `${command:cmake.launchTargetPath}`.

---

## Building in VS Code (everyone)

1. Open the repo folder in VS Code.
2. Command Palette → **CMake: Select Configure Preset** → choose `default`.
3. Build (status‑bar hammer icon or Command Palette **CMake: Build**).
4. Press **F5** to run/debug. On macOS it uses **LLDB**; on Windows it uses **MSVC** automatically.

> The launch config runs the **selected CMake target** (usually `your_game`) so you don’t need OS‑specific paths.

---

## How the build works (compile/link mental model)

```
CMake configure            Build (compile)                 Link
-----------------         -------------------             -----------------
read CMakeLists.txt  -->  .cpp  --clang/cl-->  .o/.obj --> + SFML libs --> your_game(.exe)
read CMakePresets.json
use vcpkg toolchain
install SFML if missing
```

* **Configure (cmake --preset default)**

  * CMake reads `CMakeLists.txt` and uses the **vcpkg toolchain** from the preset.
  * vcpkg sees `vcpkg.json` and installs **SFML 3** (plus transitive deps like freetype, zlib…) into `build/vcpkg_installed/<triplet>/`.
  * `find_package(SFML 3 ...)` locates SFML’s `SFMLConfig.cmake` there and exposes imported targets: `SFML::Graphics`, `SFML::Window`, etc.

* **Compile (cmake --build --preset build)**

  * Each `src/*.cpp` is compiled into an object file using include paths provided by those SFML targets.

* **Link**

  * The linker combines your objects **and** the SFML libraries (and their deps) into one executable. CMake wires this up via:

```cmake
find_package(SFML 3 COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
target_link_libraries(your_game PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)
```

### Where do the libraries come from?

* You list them in **`vcpkg.json`**.
* During **configure**, the vcpkg toolchain installs them into **`build/vcpkg_installed/<triplet>/`** (e.g., `arm64-osx`, `x64-windows`).
* `find_package` then resolves the package and gives you proper include dirs, link libs, and any transitive dependencies automatically.

> Windows typically links against `.lib` files and runs with `.dll`s present. macOS uses `.a` / `.dylib`. vcpkg chooses sensible defaults per platform.

---

## Coding notes

* SFML 3 prefers **vectors** for sizes/positions and uses **scancodes** for keys.
* Minimal example (`src/main.cpp`) is included and shows a window, a shape, events, and a delta‑time update.
* Put images/fonts/sounds under `assets/`. During dev, run from `build/` and reference `../assets/...` or add a small CMake copy step later.

---

## Daily Workflow

```bash
git pull
cmake --build --preset build
# Run via VS Code F5 or:
./build/your_game            # macOS
.\build\your_game.exe        # Windows
```

---

## Troubleshooting

* **Ninja not found**: install Ninja (`brew install ninja` or `winget install Ninja-build.Ninja`), or remove the `"generator": "Ninja"` line from `CMakePresets.json` and re‑configure.
* **C++ compiler not found**: macOS → `xcode-select --install`; Windows → install *Desktop development with C++* in Visual Studio 2022.
* **SFML version mismatch**: ensure `find_package(SFML 3 ...)` and capitalized targets `SFML::...` are used. (We’re on SFML 3 via vcpkg.)
* **VS Code says “debug type is not recognized”**: install/enable the Microsoft **C/C++** extension and reload VS Code.
* **Missing vcpkg/**: clone it into the repo root and bootstrap again (Step 1 in Quick Start).

---

## Notes on committing

* **Do not commit**: `build*/`, `vcpkg_installed/`, any compiler cache files.
* The `vcpkg/` folder is local tooling; keep it **uncommitted** so each dev can update independently.
* Consider pinning versions once stable: run `vcpkg x-update-baseline` and commit the modified `vcpkg.json`.

---

## License

Add your project license here.

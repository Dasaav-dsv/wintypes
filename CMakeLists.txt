cmake_minimum_required(VERSION 3.13.0)
project(wintypes LANGUAGES C CXX)

# TODO: dynamic library support
add_library("wintypes" STATIC)
target_compile_features("wintypes" PRIVATE cxx_std_20)

target_link_libraries("wintypes" PUBLIC "mimalloc")

target_include_directories("wintypes" PRIVATE
    "include"
    "dependencies"
    "dependencies/thirdparty"
)

target_sources("wintypes" PRIVATE
    "source/LightMutex.cpp"
)

# mimalloc options
option(MI_SECURE            "Use full security mitigations (like guard pages, allocation randomization, double-free mitigation, and free-list corruption detection)" OFF)
option(MI_DEBUG_FULL        "Use full internal heap invariant checking in DEBUG mode (expensive)" OFF)
option(MI_PADDING           "Enable padding to detect heap block overflow (always on in DEBUG or SECURE mode, or with Valgrind/ASAN)" OFF)
option(MI_OVERRIDE          "Override the standard malloc interface (e.g. define entry points for malloc() etc)" ON)
option(MI_XMALLOC           "Enable abort() call on memory allocation failure by default" OFF)
option(MI_SHOW_ERRORS       "Show error and warning messages by default (only enabled by default in DEBUG mode)" OFF)
option(MI_TRACK_VALGRIND    "Compile with Valgrind support (adds a small overhead)" OFF)
option(MI_TRACK_ASAN        "Compile with address sanitizer support (adds a small overhead)" OFF)
option(MI_TRACK_ETW         "Compile with Windows event tracing (ETW) support (adds a small overhead)" OFF)
option(MI_USE_CXX           "Use the C++ compiler to compile the library (instead of the C compiler)" OFF)
option(MI_SEE_ASM           "Generate assembly files" OFF)
option(MI_OSX_INTERPOSE     "Use interpose to override standard malloc on macOS" ON)
option(MI_OSX_ZONE          "Use malloc zone to override standard malloc on macOS" ON)
option(MI_WIN_REDIRECT      "Use redirection module ('mimalloc-redirect') on Windows if compiling mimalloc as a DLL" ON)
option(MI_LOCAL_DYNAMIC_TLS "Use slightly slower, dlopen-compatible TLS mechanism (Unix)" OFF)
option(MI_BUILD_SHARED      "Build shared library" OFF)
option(MI_BUILD_STATIC      "Build static library" ON)
option(MI_BUILD_OBJECT      "Build object library" OFF)
option(MI_BUILD_TESTS       "Build test executables" OFF)
option(MI_DEBUG_TSAN        "Build with thread sanitizer (needs clang)" OFF)
option(MI_DEBUG_UBSAN       "Build with undefined-behavior sanitizer (needs clang++)" OFF)
option(MI_SKIP_COLLECT_ON_EXIT "Skip collecting memory on program exit" OFF)
option(MI_NO_PADDING        "Force no use of padding even in DEBUG mode etc." OFF)

add_subdirectory("dependencies/thirdparty/mimalloc" "mimalloc" EXCLUDE_FROM_ALL)

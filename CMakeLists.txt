cmake_minimum_required(VERSION 3.13.0)
project(wintypes LANGUAGES C CXX)

add_library("wintypes" STATIC)
target_compile_features("wintypes" PRIVATE cxx_std_20)

target_include_directories("wintypes" PUBLIC
    "include"
)

target_sources("wintypes" PUBLIC
    "source/LightMutex.cpp"
    "source/NamedMutex.cpp"
    "source/NamedResource.cpp"
    "source/ProtectMemory.cpp"
    "source/SharedMutex.cpp"
    "source/ibo32.cpp"
)

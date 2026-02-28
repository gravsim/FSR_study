cmake_minimum_required(VERSION 3.16)
project(FSR_homeworks)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)  # GNU99
set(CMAKE_C_FLAGS "-Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign")
file(GLOB_RECURSE SRC_FILES
        "${CMAKE_SOURCE_DIR}/*.c"
)
add_executable(FSR_homeworks ${SRC_FILES}
        semester2/yandex/2026-02-16/A.c
        semester1/yandex/2025-12-15/B.c
        "semester2/informatics/2026-03-12 shortest_paths/A.c"
)

cmake_minimum_required(VERSION 3.6)
project(totp)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)

set(TOTP_SOURCES
    ${PROJECT_SOURCE_DIR}/main.c
    ${PROJECT_SOURCE_DIR}/base32.c
    ${PROJECT_SOURCE_DIR}/totp.c
)

add_executable(totp ${TOTP_SOURCES})
target_include_directories(totp PUBLIC ${PROJECT_SOURCE_DIR})

pkg_check_modules(LIBCRYPTO REQUIRED libcrypto)
target_include_directories(totp PUBLIC ${LIBCRYPTO_INCLUDE_DIRS})
target_link_libraries(totp ${LIBCRYPTO_LIBRARIES})
target_compile_options(totp PUBLIC ${LIBCRYPTO_CFLAGS_OTHER})

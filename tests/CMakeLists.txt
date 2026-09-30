set (CMAKE_CXX_STANDARD 20)

set(SOURCES
        memory/memory_operations.cpp
        memory/memory_requests.cpp
        memory/memory_strategies.cpp
        memory/memory_types.cpp
        processes/processes_helpers.cpp
        processes/processes_operations.cpp
        processes/processes_requests.cpp
        processes/processes_types.cpp
        main.cpp
        )

add_executable(tests ${SOURCES})

target_include_directories(tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty")

# Catch2 2.12.2 assumes MINSIGSTKSZ is a compile-time constant, which stopped
# being true in glibc 2.34+. Its POSIX signal handler is not needed here.
target_compile_definitions(tests PRIVATE CATCH_CONFIG_NO_POSIX_SIGNALS)

target_link_libraries(tests schedulers)

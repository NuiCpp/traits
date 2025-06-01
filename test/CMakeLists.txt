include(GoogleTest)

add_executable(traits-tests main.cpp)

find_package(GTest REQUIRED)

target_link_libraries(traits-tests PRIVATE
    traits-library
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
)

target_compile_features(traits-tests PRIVATE cxx_std_20)

set_target_properties(traits-tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)



gtest_discover_tests(traits-tests)

if (DEFINED ENV{MSYSTEM})
    add_custom_command(TARGET traits-tests POST_BUILD
        COMMAND bash -c "ldd $<TARGET_FILE:traits-tests>" | "grep" "clang" | awk "NF == 4 { system(\"${CMAKE_COMMAND} -E copy \" \$3 \" $<TARGET_FILE_DIR:traits-tests>\") }"
        VERBATIM
    )
endif()
cmake_minimum_required (VERSION 3.16)

project (traits-library)

add_library(traits-library INTERFACE)
target_include_directories(traits-library INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS traits-library
    EXPORT traits-library-targets
    INCLUDES DESTINATION include
)

# Unit testing
if (TRAITS_LIBRARY_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
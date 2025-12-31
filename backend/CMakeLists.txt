cmake_minimum_required(VERSION 3.16)

project(mantisbased LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mantisbased src/main.cpp)

add_subdirectory(./3rdParty/mantisbase)
target_include_directories(mantisbased PRIVATE 3rdParty/mantisbase/include)
target_link_libraries(mantisbased PRIVATE mantisbase_lib)

include(GNUInstallDirs)
install(TARGETS mantisbased
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

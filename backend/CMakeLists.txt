cmake_minimum_required(VERSION 3.16)

project(salama-pos-backend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(salama-pos-backend 
    src/main.cpp
    src/api_endpoints.cpp
)

add_subdirectory(./3rdParty/mantisbase)
target_include_directories(salama-pos-backend PRIVATE 
    3rdParty/mantisbase/include
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_link_libraries(salama-pos-backend PRIVATE mantisbase_lib)

include(GNUInstallDirs)
install(TARGETS salama-pos-backend
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

cmake_minimum_required(VERSION 3.5)

project(neos-test-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(neos-test-project main.cxx lexis.cxx syntax.cxx evaluation.cxx)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -fsanitize=undefined -fsanitize-address-use-after-scope -fsanitize-undefined-trap-on-error")

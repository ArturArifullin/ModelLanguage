cmake_minimum_required(VERSION 3.20)
project(Grammatics)

set(CMAKE_CXX_STANDARD 14)

add_executable(Grammatics main.cpp lexical_analisys.h lexical_analisys.cpp syntax_analysys.h semantic_analysys.cpp systax_analysys_state.cpp interpretation.h interpratation.cpp)

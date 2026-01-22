cmake_minimum_required(VERSION 3.10)
project(peroxide C)

target_include_directories(peroxide PRIVATE include)
target_compile_options(peroxide PRIVATE -Ofast)

add_executable(peroxide
  src/decider.c
  src/parser.c
  src/forwardsim.c
  src/tm.c
  main.c
)
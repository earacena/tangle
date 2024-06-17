add_executable(tangle tangle.cpp)
target_compile_features(tangle PRIVATE cxx_std_20)

# Link libraries
target_link_libraries(tangle PRIVATE journal)
target_link_libraries(tangle PRIVATE argparse)

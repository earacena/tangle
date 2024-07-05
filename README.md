
# Tangle

## Description

An encrypted command-line journaling tool written in C++20.

### Features

* Command-line operations
	* Creating new entries
	* Listing existing entries in stored journal file
* File I/O
	* Storing and reading data in binary file
	* Storing and reading data in JSON file (WIP)
* Encryption
	* Encryption and decryption of journal data as needed using libsodium (WIP)

### Supported Platforms

 Linux 64-bit distributions

### Dependencies

* Clang++
* CMake

The rest of the dependencies are loaded automatically using CMake or included in the ```external/``` folder.

## Usage
### Build

To build:

```bash
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=clang++ ..
cmake --build .
```

### Run

#### Testing

The following tests are available:
```txt
disk_manager_test
argument_parser_test
command_test
journal_entry_test
util_test
config_parser_test
```

After building the project with no issues, any of the tests above can be run by going to ```build/``` and running it directly. For example, this is the full build and test sequence:

```bash
cd build
cmake --build .

[...CMake build output...]

./disk_manager_test

[...Catch2 test output...]
```

### Clean up

To remove all CMake build files and the compiled executables, simply delete the build folder and follow the build instructions to build the project again.
## Install Dependencies

We need to install CMake, Boost, and the Linux headers for the given environment.

### Ubuntu/Debian

```bash
$ # Install CMake, boost, and kernel headers
$ sudo apt-get install build-essential cmake libboost-dev linux-headers-$(uname -r)
```

### Red Hat (Fedora/CentOS)

```bash
$ # Install CMake, boost, and kernel headers
$ sudo dnf install cmake boost-devel kernel-devel
```

## Build and Run Tests

```bash
$ # Compile the library, executables, and tests
$ cmake .
$ make
$ # Run tests
$ ctest
$ # Clean the binaries
$ make clean
```
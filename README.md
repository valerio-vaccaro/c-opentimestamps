# c-opentimestamps
C/C++ implementation of OpenTimestamps

#### Compile Lib
Build `libc_opentimestamps` shared library and `main` sample binary.
```bash
mkdir build
cd build
cmake ../
make
```

#### Import and Run
The sample `main.cpp` imports and uses shared library.
To compile successfully follow:
```bash
g++ main.cpp -o main -L./build/ -lc_opentimestamps
./main
```

#### Problems Compiling
If `cmake ../` result's is somethings similar to:
```bash
{…omissis…}
CMake Error at /usr/local/{…omissis…}/cmake/Modules/FindPackageHandleStandardArgs.cmake:137 (message):
  Could NOT find OpenSSL, try to set the path to OpenSSL root folder in the
  system variable OPENSSL_ROOT_DIR (missing: OPENSSL_INCLUDE_DIR)
{…omissis…}
```

you can specify the right path to OpenSSL in `OPENSSL_ROOT_DIR`; for example:
```
cmake -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl ../
```

to debug:
```
cmake -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl -DCMAKE_BUILD_TYPE=Debug ../
```


#### Connect to Python
Install `swig` to generate the wrapper code `ots.py`
```bash
cd swig
./build.sh
```

Import and Run on python2.7
```python
Python 2.7.14 (v2.7.14:84471935ed, Sep 16 2017, 12:01:12)
[GCC 4.2.1 (Apple Inc. build 5666) (dot 3)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> import ots
>>> dir(ots)
```

#### Dependencies
libopenssl library

# c-opentimestamps
C/C++ implementation of OpenTimestamps

#### Compile
```bash
mkdir build
cd build
cmake ../
make
```

#### Run
```bash
./c_opentimestamps
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
#### Dependencies
libopenssl library

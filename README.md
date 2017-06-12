# Babelfish library

This is a PoC of a future Babelfish lib.
It includes a C++ [gRPC](http://www.grpc.io/) client,
a minimal C API wrapper over this C++ code
and a Python example on top of this C API.

Have a look at the [DESIGN](DESIGN.md) document
for more information about why C++ was chosen.

It may be interesting to use [SWIG](http://www.swig.org/)
to generate bindings instead of doing it by hand.
Alternatively python bindings could be constructed
with [CFFI](https://cffi.readthedocs.io/en/latest/) too.

## Build

You need gRPC for C++ installed.
You may want to look at its [documentation](http://www.grpc.io/docs/quickstart/cpp.html).

If you have problems with [GCC7](https://github.com/grpc/grpc/issues/11376),
use an older version or clang instead.

If you have problems with [OpenSSL 1.1](https://github.com/grpc/grpc/issues/10589),
switch to OpenSSL 1.0:

```
PKG_CONFIG_PATH=/usr/lib/openssl-1.0/pkgconfig make
```

Note that if gRPC is built from sources,
you may need to setup the environment properly, i.e.:

```
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
export LD_LIBRARY_PATH=/usr/local/lib
```

With gRPC properly installed, `make` should work:

```
> PKG_CONFIG_PATH=/usr/local/lib/pkgconfig make
protoc --cpp_out src github.com/gogo/protobuf/gogoproto/gogo.proto
g++ -Isrc -fPIC -std=c++11 -I/usr/local/include -pthread  -c -o gogo.pb.o src/github.com/gogo/protobuf/gogoproto/gogo.pb.cc
protoc --cpp_out src github.com/bblfsh/sdk/uast/generated.proto
g++ -Isrc -fPIC -std=c++11 -I/usr/local/include -pthread  -c -o uast.pb.o src/github.com/bblfsh/sdk/uast/generated.pb.cc
protoc --cpp_out src github.com/bblfsh/sdk/protocol/generated.proto
g++ -Isrc -fPIC -std=c++11 -I/usr/local/include -pthread  -c -o protocol.pb.o src/github.com/bblfsh/sdk/protocol/generated.pb.cc
protoc --grpc_out src --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` github.com/bblfsh/sdk/protocol/generated.proto
g++ -Isrc -fPIC -std=c++11 -I/usr/local/include -pthread  -c -o protocol.grpc.pb.o src/github.com/bblfsh/sdk/protocol/generated.grpc.pb.cc
g++ -Isrc -fPIC -shared src/bblfsh.cc -o libbblfsh.so `pkg-config --cflags --libs grpc++ libprotobuf-c` protocol.grpc.pb.o protocol.pb.o uast.pb.o gogo.pb.o
```

## Run

Make sure [bblfsh server](https://github.com/bblfsh/server/) is running.

You can look at the [documentation](https://doc.bblf.sh/user/getting-started.html)
for instructions on how to build and run `bblfsh server`.

Once the server is running,
just call the python sample client:

```
python2 main.py filename
```

where filename is the path to the file to parse.
For example:

```
> python2 main.py main.py
gRPC call worked!
Module
Import
Import.names
...
```

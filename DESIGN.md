# Babelfish lib PoC

We want to create a library
with some common functionality
to make it easier for developers
to build their analysis tools on top of it.
Its current scope includes:

- Communication with Babelfish server
  through [GRPC](http://www.grpc.io/)
  (this may get out of the scope in the future).
- Navigation through the UAST,
  as returned by [driver protocol](https://doc.bblf.sh/driver/protocol.html)
- Useful generic algorithms
  built on top of this navigation
  (i.e. XPath  querying).


This library should serve as a basis
for all the client libraries
in different programming languages.
Alternatively users could use directly the GRPC protocol
to build their own tools without this library.

As the library should be easily callable from different languages,
it makes sense to consider a C ABI for this library.
This limits the choice of language for the library implementation
to a few languages:
C, C++, D, Nim and Rust have been considered.

In the following sections each of this languages is evaluated

## C

**Pros**
- The obvious choice for a library with a C ABI
- There's a [protobuf](https://github.com/protobuf-c/protobuf-c) implementation
- There's a [grpc](https://github.com/protobuf-c/protobuf-c-rpc) implementation

**Cons**
- Most low-level language
- It'd probably require an external library for basic data structures/algorithms
- No package manader
- The protobuf implentation is not officially supported
- There's no official GRPC implementation (through the core is implemented in C)
- The (independent) grpc implementation doesn't seem mature enough

Since the independendent GRPC library is not mature enough,
the C++ version of the library should probably be used.
The C++ version of GRPC works with the C++ version of protobuf.
This means that we end up with all our internal data structures being classes,
which are not valid C types,
so at this point it makes more sense to use C++ internally
and just expose a C API.

Another approach would be to use de low-level core GRPC implementation,
but that would probably make us build
a C GRPC high-level implementation on top of it.
This is not much better than using the independent GRPC library.

## C++

**Pros**
- There's an official
  [protobuf](https://developers.google.com/protocol-buffers/docs/cpptutorial)
  implementation
- There's an official [grpc](http://www.grpc.io/docs/quickstart/cpp.html)
  implementation

**Cons**
- There's no package manager,
  the recommended way to install it is compiling from sources.
  
Building gRPC from sources has some issues:
- compilation seems to fail with last version of gcc (gcc7)
- compilation seems to fail with OpenSSL 1.1.x (requires OpenSSL 1.0.x)

Apart from these (solvable) issues,
it seems to be the best supported approach
(unless not using gRPC is considered).

## D

**Pros**
- A package manager
- A mature
  [protobuf implementation](https://github.com/msoucy/dproto)

**Cons**
- No gRPC implementation

Using D would require to build
a wrapper on top of the C++ implementation of gRPC,
and to expose a C API on top of that.

## Nim

The language has no support for protobuf or gRPC, it's discarded as not mature
enough.

## Rust

**Pros**
- A package manager
- A mature
  [protobuf implementation](https://github.com/stepancheg/rust-protobuf)

**Cons**
- An immature [gRPC implementation](https://github.com/stepancheg/grpc-rust)

Using Rust would either require to use it's native gRPC implementation
or to build a wrapper on top of the C++ one.
The native implementaition is actively maintained and may work,
but it's rather immature.
all: proto lib

lib: src/bblfsh.cc
	$(CXX) -Isrc -fPIC -shared src/bblfsh.cc -o libbblfsh.so `pkg-config --cflags --libs grpc++ libprotobuf-c` protocol.grpc.pb.o protocol.pb.o uast.pb.o gogo.pb.o

proto: github.com/bblfsh/sdk/protocol/generated.proto \
	github.com/bblfsh/sdk/uast/generated.proto \
	github.com/gogo/protobuf/gogoproto/gogo.proto

	protoc --cpp_out src github.com/gogo/protobuf/gogoproto/gogo.proto
	$(CXX) -Isrc -fPIC -std=c++11 -I/usr/local/include -pthread  -c -o gogo.pb.o src/github.com/gogo/protobuf/gogoproto/gogo.pb.cc

	protoc --cpp_out src github.com/bblfsh/sdk/uast/generated.proto
	$(CXX) -Isrc -fPIC -std=c++11 -I/usr/local/include -pthread  -c -o uast.pb.o src/github.com/bblfsh/sdk/uast/generated.pb.cc

	protoc --cpp_out src github.com/bblfsh/sdk/protocol/generated.proto
	$(CXX) -Isrc -fPIC -std=c++11 -I/usr/local/include -pthread  -c -o protocol.pb.o src/github.com/bblfsh/sdk/protocol/generated.pb.cc

	protoc --grpc_out src --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` github.com/bblfsh/sdk/protocol/generated.proto
	$(CXX) -Isrc -fPIC -std=c++11 -I/usr/local/include -pthread  -c -o protocol.grpc.pb.o src/github.com/bblfsh/sdk/protocol/generated.grpc.pb.cc

clean:
	rm -rf src/github.com
	rm -rf *.o *.so *.pyc __pycache__

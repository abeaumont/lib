#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include "github.com/bblfsh/sdk/protocol/generated.pb.h"
#include "github.com/bblfsh/sdk/uast/generated.pb.h"
#include "github.com/bblfsh/sdk/protocol/generated.grpc.pb.h"

#include "bblfsh.h"

using grpc::Channel;
using grpc::ClientContext;

using github::com::bblfsh::sdk::protocol::ProtocolService;
using github::com::bblfsh::sdk::protocol::ParseUASTRequest;
using github::com::bblfsh::sdk::protocol::ParseUASTResponse;
using github::com::bblfsh::sdk::protocol::Status;

using namespace github::com::bblfsh::sdk;

extern "C"
{
	const UAST* create_uast(const char *filename, const char* language, const char* content) {
		auto channel = grpc::CreateChannel("localhost:9432",  grpc::InsecureChannelCredentials());
		auto service = ProtocolService::NewStub(channel);
		ClientContext context;
		ParseUASTRequest request;
		request.set_filename(filename);
		request.set_language(language);
		request.set_content(content);
		ParseUASTResponse *response = new ParseUASTResponse;
		grpc::Status status = service->ParseUAST(&context, request, response);
		if (status.ok()) {
			std::cout << "gRPC call worked!\n";
		} else {
			std::cout << "gRPC call failed, is the server running?\n";
		}
		return reinterpret_cast<const UAST*>(response);
	}

	int uast_status(const UAST* uast) {
		reinterpret_cast<const ParseUASTResponse*>(uast)->status();
	}

	void delete_uast(const UAST* uast) {
		delete reinterpret_cast<const ParseUASTResponse*>(uast);
	}

	const Node* uast_root(const UAST* uast) {
		const ParseUASTResponse *response = reinterpret_cast<const ParseUASTResponse*>(uast);
		const uast::Node &n = response->uast();
		return reinterpret_cast<const Node*>(&response->uast());
	}

	const char* node_internal_type(const Node* node) {
		return reinterpret_cast<const uast::Node*>(node)->internal_type().c_str();
	}

	int node_children_size(const Node* node) {
		return reinterpret_cast<const uast::Node*>(node)->children_size();
	}
	
	const Node* node_children(const Node* node, int index) {
		const uast::Node &n = reinterpret_cast<const uast::Node*>(node)->children(index);
		return reinterpret_cast<const Node*>(&n);
	}

}

#ifndef __BBLFSH_H__
#define __BBLFSH_H__

extern "C" {
	struct UAST;
	struct Node;
	struct Position;
	typedef struct Uast Uast;
	typedef struct Node Node;
	typedef struct Position Position;

	const UAST* create_uast(const char* filename, const char* language, const char* content);
	int uast_status(const UAST* uast);
	void delete_uast(const UAST* uast);

	const Node* uast_root(const UAST *uast);
	const Node* uast_find_node(const UAST* uast, const char* name);

	const char* node_internal_type(const Node* node);
	int node_children_size(const Node* node);
	const Node* node_children(const Node* node, int index);
	const char* node_token(const Node* node);
	Position* node_start_position(Node* node);
	Position* node_end_position(Node* node);
	// ...

	uint32_t position_offset(const Position* node);
	uint32_t position_line(const Position* node);
	uint32_t position_col(const Position* node);
}

#endif

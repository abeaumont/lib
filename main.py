import argparse
from bblfsh import *

def iter(node, f):
    f(node)
    for child in node.children():
        iter(child, f)

def print_node(node):
    print(node.internal_type())

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Python client example.")
    parser.add_argument('filename', type=str, help='a file to parse')
    parser.add_argument('--language', default='', help="language (default: ''")
    args = parser.parse_args()

    with open(args.filename) as f:
        contents = f.read()

    with uast(args.filename, args.language, contents) as uast:
        if uast.status() == status.OK:
            iter(uast.uast_root(), print_node)
        else:
            print('Something went wrong')
    

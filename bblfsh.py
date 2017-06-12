from contextlib import contextmanager
from ctypes import CDLL, c_char_p, c_int, c_void_p

class status(object):
    OK = 0
    ERROR = 1
    FATAL = 2

class Node(object):
    def __init__(self, bblfsh, node):
        self.bblfsh = bblfsh
        self.bblfsh.node_internal_type.argtypes = [c_void_p]
        self.bblfsh.node_internal_type.restype = c_char_p
        self.bblfsh.node_children_size.argtypes = [c_void_p]
        self.bblfsh.node_children_size.restype = c_int
        self.bblfsh.node_children.argtypes = [c_void_p, c_int]
        self.bblfsh.node_children.restype = c_void_p
        
        self.node = node

    def internal_type(self):
        return self.bblfsh.node_internal_type(self.node)

    def children(self):
        for i in range(self.bblfsh.node_children_size(self.node)):
            yield Node(self.bblfsh, self.bblfsh.node_children(self.node, i))

@contextmanager
def uast(filename, language, content):
        
    class UAST(object):

        def __init__(self, filename, language, content):
            self.bblfsh = CDLL("./libbblfsh.so")
            self.bblfsh.create_uast.argtypes = [c_char_p, c_char_p, c_char_p]
            self.bblfsh.create_uast.restype = c_void_p
            self.bblfsh.uast_status.argtypes = [c_void_p]
            self.bblfsh.uast_status.restype = c_int
            self.bblfsh.uast_root.argtypes = [c_void_p]
            self.bblfsh.uast_root.restype = c_void_p
            self.bblfsh.delete_uast.argtypes = [c_void_p]

            self.uast = self.bblfsh.create_uast(filename, language, content)

        def status(self):
            return self.bblfsh.uast_status(self.uast)

        def uast_root(self):
            return Node(self.bblfsh, self.bblfsh.uast_root(self.uast))
            
        def cleanup(self):
            self.bblfsh.delete_uast(self.uast)

    uast = UAST(filename, language, content)
    yield uast
    uast.cleanup()

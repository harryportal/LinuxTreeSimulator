#ifndef filesystem_h
#define filesystem_h

typedef enum {DIR, _FILE} nodeType;

typedef struct _FileNode {
    char name[64];
    nodeType type;
    struct _FileNode *childPtr, 
                     *siblingPtr, 
                     *parentPtr;
} Node;

// filesystem structure
typedef struct _FileSystem {
    Node *root, *cwd;
} FileSystem;

// helper functions

// create a new node for a directory or file
Node* newNode(const char *basename, const nodeType type); 

// extract directory and base from a pathname
void dbname(const char *pathname, char *dname, char *bname);

// search for a node given a directory path, output error with specified command name<cmd>
// the node is specified here as an output parameter
Node* searchDir(FileSystem *fs, const char *pathname, const char *cmd);


#endif
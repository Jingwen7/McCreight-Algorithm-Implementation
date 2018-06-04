 /* C++ implementation of reading a tree from a txt file
 Author: Jingwen Ren,  USC ID: 7025118377
 */
 
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <cstdio>
#include <string>
#define MAX_CHAR 5   //MAX_CHAR is alphabet size, which includes "$"


using namespace std;

 
// declare a node of tree
struct treenode {
   int start;
   int end;
   treenode *children[MAX_CHAR];  
   treenode () { //constructor
         for (int s = 0; s < MAX_CHAR; ++s)  {
             children[s] = NULL;
         }
         start = -1;
         end = -1;
     }
    ~treenode(){} //destructor
};

// This function constructs tree from a file pointed by 'fp'
// Read next item from file. If the next item is marker which means there are no more items, then return 1 
// otherwise, read the next two items, put them as starting and ending indices of a child node of the previous node and return 0
int deserialize(treenode *node, FILE *fp) {
    int node_start;
    int node_end;
    char val;
    if (fscanf(fp, "%i %i ", &node_start, &node_end) == 2)  { // successfully read two numbers instead of char "#"
        node->start = node_start;
        node->end = node_end;
        
        for (int i = 0; i < MAX_CHAR; ++i) { 
            treenode *children = new treenode();
            node->children[i] = children;
            if (deserialize(node->children[i], fp)) { //deserialize(node->children[i], fp) == 1 means no children[i] below node->children[i]
               delete children;  
               node->children[i] = NULL; 
               break;
            }
        }
        return 0;      
    }
    else {
        fscanf(fp, "%c ", &val);
        return 1;
    }
}


int 
main (int argc, const char * const argv[]) {
    
    if (argc != 2) {
       cerr << "NEEDS INPUTS: " << argv[0] << " PATH OF THE TREE FILE! " << endl;
       return EXIT_FAILURE;
    }
    
    string filepath = argv[1];
    
    //deserialize the stored tree from a file pointed by 'fp'
    treenode *root = new treenode();
    FILE *fp = fopen(filepath.c_str(), "r");
    deserialize(root, fp);
    return 0;
}

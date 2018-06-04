/* C++ implementation of McCreight's algorithm construction of suffix tree 
 Author: Jingwen Ren,  USC ID: 7025118377
 */
 
#include <iostream>
#include <new> 
#include <string>
#include <cstddef> 
#include <algorithm>  
#include <cstdio>
#include<time.h>
#include <fstream>
#define MAX_CHAR 5   //MAX_CHAR is alphabet size, which includes "$"
#define MARKER '#'  //when serializing a tree, put "#" behind node all children of which have already been stored in txt file


using namespace std;

// function to read fastq file
static void
read_fasta_file_single_sequence(const string &filename, string &T) {

  ifstream in(filename.c_str());
  string line;
  in >> line;
  while (in >> line)
    T += line;
}


struct treenode 
{
     struct treenode *children[MAX_CHAR];
     int start;
     int end;
     struct treenode *parent;
     struct treenode *suffixlink;
     treenode () { //constructor
         for (int s = 0; s < MAX_CHAR; ++s)  {
             children[s] = NULL;
         }
         parent = NULL;
         suffixlink = NULL;
         start = -1; 
         end = -1;
     }
     ~treenode(){} //destructor
};

struct node_point
{
     int pstart;
     int pend;
     node_point (struct treenode *node) {
         pstart = node->start;
         pend = node->end;
     }
     ~node_point() {}
};


// Function to jumpdown, return the node where jumpdown ends at
treenode *jumpdown (treenode *node, node_point beta, const string &T) {
     treenode *current = node;
     int i = 0;
     while (beta.pstart <= beta.pend) {  
         i = 0;
         while (T[current->children[i]->start] != T[beta.pstart]) {  // choosing the right direction to jumpdown
             ++i;
         } 
         current = current->children[i];
         beta.pstart = beta.pstart + (current->end - current->start) + 1; // update beta.pstart such that beta.pstart is the position where next 
                                                                        // turn of choosing right direction begins at
     }
     if (beta.pstart == beta.pend + 1) { // jumpdown stops at node "current"
         return current;          
     }
     else { // jumpdown stops at an edge, need split the edge and insert new node "w"
         treenode *w = new treenode();
         w->start = current->start;
         w->end =  current->end  - (beta.pstart - beta.pend) + 1;
         w->parent = current->parent;
         current->parent->children[i] = w;
         current->parent = w;
         current->start = w->end + 1;
         w->children[0] = current;
         return w;     
     }
}

// Function to walkdown, return the node where walkdown ends at
treenode *walkdown (treenode *node, node_point &gamma, int &p, const string &T) {  // p records that the leafnode which is going to be inserted is the p-th 
                                                                                   // children of the node which walkdown ends at
     treenode *current = node;   
     treenode *return_node;                                 
     while (gamma.pstart <= gamma.pend) {
          int i = 0;
          while (current->children[i] != NULL && T[current->children[i]->start] != T[gamma.pstart]) { // choosing the right direction to walkdown
              ++i;       
          } 
          if (!current->children[i]) { // current->children[i] == NULL, indicates walkdown stops at node current
              cout << "NODE" << "\t\t";
              p = i;
              return_node = current;
              break;   
          }
          else {
          
              int intermediate = gamma.pstart + (current->children[i]->end - current->children[i]->start);
              int dist = 0;
              while (T[current->children[i]->start + dist] == T[gamma.pstart] && gamma.pstart <= min(intermediate, gamma.pend)) {
                  ++dist;
                  ++(gamma.pstart);
              }
              if (gamma.pstart > min(intermediate, gamma.pend)) { // this only happens when matching till node current->children[i], so continue walkdown
                  current = current->children[i];                      
              }
              else { // this only happens when mismatch happens in the middle of an edge, split this edge and add a new node "new_node_v"
                  cout << "EDGE" << "\t\t";
                  treenode * new_node_v = new treenode();  
                  new_node_v->start = current->children[i]->start;
                  new_node_v->end = current->children[i]->start + dist - 1;
                  current->children[i]->start = current->children[i]->start + dist;
                  current->children[i]->parent = new_node_v;
                  new_node_v->children[0] = current->children[i];
                  new_node_v->parent = current;
                  p = 1;
                  current->children[i] = new_node_v;  
                  return_node = new_node_v;   
                  break;                        
              }           
          }               
     }
     return return_node;
}


// Function to build the suffix tree
treenode *build_suffix_tree (const string &T, int len) {
     
     cout << "j" << "\t" << "jd?" << "\t" << "jb stops?" << "\t" << "jbstart" << "\t" <<
    "jbend" << "\t" << "wd?" << "\t"<< "wd stops?" << "\t" << "wdstart" << "\t" << 
    "wdend" << "\t" <<"leafstart" << "\t" << "leafend" << endl;
     
     // initialize root and leafnode0 for the suffix tree
     treenode *root = new treenode();
     root->parent = root;
     root->suffixlink = root;     
     treenode *leafnode0 = new treenode();
     root->children[0] = leafnode0;
     leafnode0->parent = root;
     leafnode0->start = 0;
     leafnode0->end = len;
     
     //initialize node_v_j in the McCreight lecture
     treenode *node_v_j;
     node_v_j = root;
     treenode *w;
     
     node_point beta = node_point(root);
     node_point gamma = node_point(leafnode0); 
     
     for (int j = 1; j<=len; ++j) { // now we're going to insert suffix j 
     
         cout << j << "\t";
         treenode *current_leafnode = new treenode(); // "current_leafnode" is the node we are going to insert in this turn
         current_leafnode->start = j;
         current_leafnode->end = len;
         
         int I = 1; // indicator of whether need jumpdown 
         if (node_v_j == root) {// no need to jumpdown
            ++gamma.pstart;
            I = 0; 
         }
         else if (node_v_j->parent == root) {
            if (beta.pstart == beta.pend) {// no need to jumpdown
               I = 0;
            }
            else {// need to jumpdown
               ++beta.pstart;
            }
         }

         
         if (I == 0) {
            // No jumpdown, only need walkdown from the root;
            cout << "NO" << "\t" << "NA" << "\t\t" <<"NA" << "\t" << "NA" << "\t" << "YES" << "\t";
            int p = 0;
            treenode * new_node_v;
            new_node_v = walkdown (root, gamma, p, T);
            new_node_v->children[p] = current_leafnode;
            current_leafnode->parent = new_node_v;
            current_leafnode->start = gamma.pstart;
            if (beta.pstart == beta.pend && beta.pstart != -1) {node_v_j->suffixlink = root;}
            cout << new_node_v->start << "\t" << new_node_v->end << "\t" << current_leafnode->start << "\t\t" << current_leafnode->end << endl;//////
            
         } 
         else { // need jumpdown;
            cout << "YES" << "\t";
            w = jumpdown(node_v_j->parent->suffixlink, beta, T);
            node_v_j->suffixlink = w;

            if (!w->children[1]) { // w->children[1] = NULL;  w is the split node when jumpdown is finished, so no need for walkdown, inserting leafnode directly
               w->children[1] = current_leafnode;
               current_leafnode->start = gamma.pstart; 
               current_leafnode->parent = w;
               cout<< "EDGE" << "\t\t" << w->start << "\t" << w->end << "\t" << 
               "NO" << "\t"<< "NA" << "\t\t" << "NA" << "\t" << "NA" << "\t" << current_leafnode->start << "\t\t"
               << current_leafnode->end << endl;
            }
            else {  // jumpdown stops at an existed node w, need walkdown from node "w"
               cout << "NODE" << "\t\t" << w->start << "\t" << w->end << "\t" << "YES" << "\t";
               int p = 0;
               treenode * new_node_v;
               new_node_v = walkdown (w, gamma, p, T);
               new_node_v->children[p] = current_leafnode;
               current_leafnode->parent = new_node_v;
               current_leafnode->start = gamma.pstart;  
               cout << new_node_v->start << "\t" << new_node_v->end << "\t" << current_leafnode->start << "\t\t" << current_leafnode->end << endl;        
            }
         }
         
        //update gamma, beta and node_v_j
        gamma = node_point(current_leafnode);
        beta = node_point(current_leafnode->parent);
        node_v_j = current_leafnode->parent;        
     }
     return root;
}


//store the suffix_tree into a file pointed by fp
void serialize(treenode *root, FILE *fp)
{
    if (root == NULL) return;
 
    // Else, store current node and recurse for its children
    fprintf(fp, "%i %i ", root->start, root->end);
    for (int i = 0; i < MAX_CHAR && root->children[i] != NULL; i++) {serialize(root->children[i], fp);}
 
    // Store marker at the end of children
    fprintf(fp, "%c ", MARKER);
}



int 
main (int argc, const char * const argv[]) {
    
    if (argc != 3) {
       cerr << "NEEDS INPUTS: " << argv[0] << " <FASTA-FILE> WITHOUT '$'!" << " AND PATH TO STORE THE TREE.TXT FILE!" << endl;
       return EXIT_FAILURE;
    }
    
    const string filename(argv[1]);
    string T;
    string filepath(argv[2]); 
    read_fasta_file_single_sequence(filename, T);

    clock_t start, end;
    double duration;
    start = clock(); // get the start time
    
    int len = T.length();
    T.push_back('$');
    treenode *root = build_suffix_tree(T, len);
    
    end = clock(); // get the end time
    duration = ((double) end - start) / CLOCKS_PER_SEC;
    
     // output running time
    cout << "Running time for building suffix tree: " << duration << " seconds" << endl; 
    
    // open a file and store the tree into the file
    filepath += "tree.txt";
    FILE *fp = fopen(filepath.c_str(), "w");
    serialize(root, fp);
    fclose(fp);
    
    return EXIT_SUCCESS;  
}    

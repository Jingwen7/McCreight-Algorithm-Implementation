1. The input format.
   It requires two inputs. A fasta file with a single sequence is required. And a path to store the tree.txt file. The default alphabet size is 5. If you want to change it, change the `MAX_CHAR` in the header of `McCreight_suffix_tree.cpp`. Notice that `MAX_CHAR` should take `"$"` into account.

2. The output format.
   There will be three outputs.        
      
     * A table which records the process of building the suffix tree based on McCreight's algorithm will be output to screen directly. It contains 11 columns, which are `j, jb?, jd stops?, jbstart, jbend, wd?, wd stops?, wdstart, wdend, leafstart, leafend`.
    
        + `j`: the suffix beginning at position j in the input sequence is now being inserted.
        + `jb?`: shows whether it requires jumpdown or not to insert suffix j. 
        + `jb stops?`: shows whether jumpdown stops at a node or in the middle of an edge, if it requires jumpdown when inserting suffix j. Otherwise it will show "NA" in this column.
        + `jbstart`: shows the starting index of the node at which the jumpdown process ends, no matter whether this node exists or is just newly inserted. If it doesn't require jumpdown to insert suffix j, it will show "NA" in this column.
        + `jbend`: shows the ending index of the node at which the jumpdown process ends, no matter whether this node exists or is just newly inserted. If it doesn't require jumpdown to insert suffix j, it will show "NA" in this column.
        + `wd?`: shows whether it requires walkdown or not to insert suffix j.
        + `wd stops`: shows whether walkdown stops at a node or in the middle of an edge, if it requires walkdown when inserting suffix j. Otherwise, it will show "NA" in this column.
        + `wdstart`: shows the starting index of the node at which the walkdown process ends, no matter whether this node exists or is just newly inserted. If it doesn't require walkdown to insert suffix j, it will show "NA" in this column.
        + `wdend`: shows the ending index of the node at which the walkdown process ends, no matter whether this node exists or is just newly inserted. If it doesn't require walkdown to insert suffix j, it will show "NA" in this column.
        + `leafstart`: shows the starting index of the leaf node which corresponds to the suffix beginning at position j in input sequence.
        + `leafend`: shows the ending index of the leaf node which corresponds to the suffix beginning at position j in input sequence.    
   * A tree.txt file which stores the tree nodes' starting and ending indices in the preorder traversal of the suffix tree will be output. In the tree.txt file, `"#"` will be put behind if all the children of a node have already been stored. `read_tree.cpp` is provided to read tree from the txt file. `read_tree.cpp` only requires the path of tree.txt file as input. After reading the tree from tree.txt, every tree node will have starting and ending indices and pointers to children nodes. 
   * Running time for building suffix tree will be output to screen. The running time excludes the time used for reading sequence from the input fasta file and the time used to store the tree into the tree.txt file. 

  
3. Speed performance.   
   The running time of `McCreight_suffix_tree.cpp` on randomly generated sequences of length from 1000000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000, 4500000, 5000000, 6000000, 7000000, 8000000,9000000,10000000,15000000, 20000000 are shown in the following figure. These scatter dots can be fitted well by a linear line $y = $2e-5$x + 1.8063$, which indicates that the running time (sec) is approximately $2e-5$ times the length of input sequence. This is in accordance with the fact that McCreight's algorithm is a linear algorithm.    

4. Memory requirement.   
   The sizes of outputs tree.txt on randomly generated sequences of length from 1000000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000, 4500000, 5000000, 6000000, 7000000, 8000000, 9000000, 10000000, 15000000, 20000000 are shown in the following figure. These scatter dots can be fitted well by a linear line $y = $3e-5$x-7.2302$, which indicates that the size of output tree.txt file (Mega) is approximately $3e-5$ times the length of input sequence. 

5. Four main functions in `McCreight_suffix_tree.cpp`.
    * Function `build_suffix_tree` returns the root and it involves two main parts: suffix tree initialization and inserting the rest suffixes. The first part is to initialize root and the first leaf node, while the second part is to insert suffixes beginning from position 1 to position $n$ ($n$ is the length of input sequence). There are three cases when inserting suffixes.
          
        + No need for jumpdown, only need to walkdown from the root.
        + Need to jumpdown, and the jumpdown process stops at a newly inserted node, therefore there is no need to walkdown.
        + Need to jumpdown, and the jumpdown process stops at an existing node, therefore walkdown is needed.   
    * Function `jumpdown` returns node at which jumpdown ends, no matter whether it's a newly inserted or existing node. It contains two cases: jumpdown ends at an existing node or in the middle of an edge. If it's the latter case, this edge needs to be split and a new node will be created.
    * Function `walkdown` returns node at which walkdown ends, no matter whether it's a newly inserted or existing node. It contains two cases: walkdown ends at an existing node or in the middle of an edge. If it's the latter case, this edge needs to be split and a new node will be created.
    * Function `serialize` is to write the tree nodes' starting and ending indices into a tree.txt file in the preorder traversal of the suffix tree. When all the children of a node have already been stored in tree.txt, `"#"` will be put behind as an indicator of no more children of this node later.




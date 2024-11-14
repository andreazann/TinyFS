# TinyFS - Hierarchical filesystem in C
## Brief description

Simple hierarchical filesystem with storage only in main memory.  
Hierarchical filesystem that organizes resources in a tree structure, and identifies each resource uniquely by the path that connects it to the root.  
Files can only be represented as leaves of the tree while folders can be either leaves or intermediate nodes of the tree.  
First root node is the root directory of the filesystem.  
For the sake of this project only directories can be associated with metadata. These metadata contain the name of the directory's parents until the root is encountered.  
Only libc is allowed to be used.  
An input file also called "diary" contains a list of actions to be performed in the filesystem, after each action perfored an output need to be printed in stdout and saved in an output.txt file.  

## Input formatting

Paths in the filesystem are represented with UNIX syntax, separating resources names with a /

<img width="285" alt="tinyfs-img1" src="https://github.com/user-attachments/assets/bdc07a57-e0d9-4495-89da-b4b7a2ada31a">

The path identifying file0 in the picture is /file0 while for file3 it is /dir2/file3 

The following restrictions apply:
* resources names are alphanumeric and can be at most 255 characters long
* maximum tree height for the filesystem is 255
* maximum children for a node is 1024

The program receives one of the following commands while reading each line of the diary where <path> identifies a generic path and <name> an alphanumeric string of length 255:
* **create <path>**: the command has the effect of creating an empty file, i.e. without associated data, within the filesystem. It prints the result "ok" if the file was created correctly, "no" if the file creation was not successful (for example, if you try to create a file in a non-existent directory, or if you exceed the limits of the filesystem).
* **create_dir <path>**: creates an empty directory in the filesystem. Prints the result "ok" if the directory was created successfully, "no" if the creation failed.
* **read <path>**: Reads the entire contents of a file, printing "contents" followed by a space character and the file contents to output if the file exists, or prints "no" if the file does not exist.
* **write <path><content>**: Writes the entire contents of a file, which must already exist (if the file already had content, it is overwritten); prints "ok" followed by the number of characters written if the writing was successful, "no" otherwise. The parameter <content> takes the form of a sequence of alphanumeric characters and spaces delimited by double quotes. Example: write /poems/jabberwocky "It was brillig and the slithy toves"
* **delete <path>**: Deletes a resource, prints as output "ok" - "no". A resource is eresable only when it has no children.
* **delete_r <path>**: Deletes a resource and all its children if present. Print as output "ok" - "no"
* **find <name>**: Finds the location of the resource <name> within the filesystem . Prints "ok" followed by the resource path for each correctly named resource found. Resources must be printed in lexicographic order wiith respect to the path printed in output for each resource found. Path separator characters must be considered in the sorting, meaning the sorting comparison must be made between entire strings representing paths. If no resource is found, the command prints "no".
* **exit**: Ends execution of the filesystem. Nothing printed in output.

## Time complexity requirements

Given l as the length of a path, d the number of resources in the entire filesystem, d-path the number of resources greater than the one specified by the path, and f the number of resources found by a search, the expected time complexities of the commands specified in the previous section are as follows:

<img width="418" alt="tinyfs-img2" src="https://github.com/user-attachments/assets/113c4d8c-5afa-458f-9b31-0cb357cb01f8">

## Instructions for use

Compile main.c in /src and after running it, type the command you wish to execute in the filesystem 

Full specifications, already listed above, also in the following file (in italian).

[TinyFS_Specifications_IT.pdf](https://github.com/user-attachments/files/17741308/TinyFS_Specifications_IT.pdf)

## Final time complexity for delete_r and find

**Delete_r**  

The delete_r function is a recursive function that traverses a tree to delete a node and all its children. Here's the complexity analysis:

* Worst-case complexity: The function visits each node once and performs constant time operations (removal and deallocation) on each. If n represents the total number of nodes in the subtree rooted at the given node, the time complexity is O(n) because it needs to traverse and delete every node in the subtree.
* Reasoning: The function performs a removal operation on each node and recursively visits the children. Each visit takes constant time O(1), but since all nodes in the subtree are visited, the total time grows linearly with the number of nodes.

**Find**

The find_r function recursively searches for a file or directory with a specific name in the tree.

* Worst-case complexity: The function may need to explore the entire tree to find all occurrences of the specified file or directory. The time complexity is O(n), where n is the total number of nodes in the tree.
* Reasoning: find_r visits each node and performs a comparison (strcmp), which is an O(m) operation where m is the length of the filename. However, the overall complexity remains O(n) because each node is visited once, and the comparison operation does not significantly affect the linear complexity.

# Conclusion

## Data structure used for the filesystem

The tree structure used in the provided code is implemented using a custom type called **inode_t**, which represents nodes in a tree-like structure. Each node can be a file or directory, and it has references to its children and parent. The children are stored in a hashtable (or similar structure) linked to each **inode_t**, allowing efficient lookup and operations within each node's set of children.

* Each node (inode_t) has fields for the filename, content (if it is a file), type (file or directory), a reference to its parent, and a hashtable for its children.

## Why This Data Structure is Used:

* **Flexibility in Representing Hierarchical Data**: Using a node structure with references to children allows for a flexible representation of hierarchical data like file systems. This approach makes it easy to model parent-child relationships and traverse directories recursively.

* **Efficient Child Node Management**: The use of a hashtable for storing children provides efficient operations such as insertion, deletion, and lookup, typically with an average time complexity of O(1). This is particularly useful for operations that need to manage a potentially large number of children nodes within a directory.

* **Scalability**: This structure scales well for directory nodes that contain many entries. The hashtable reduces the time needed to search for specific children compared to simpler structures like linked lists, which would require O(n) time for lookup.

* **Hierarchical Traversal Support**: By linking each node to its parent and maintaining children as a collection within the node, recursive operations (such as deletion and searching) are easily implemented. The parent reference allows traversal both up and down the tree.

## Algorithm used to traverse the tree

**DFS** (Depth-first search) is preferred against **BFS** (Breadth-first search) for operations like **delete_r** and **find_r** because it is simpler to implement, uses less space in many scenarios, and is well-suited for operations that require processing a complete subtree before moving to other branches. BFS is more useful for finding the shortest paths or visiting nodes level by level in scenarios where tree height is limited, but it requires more complex management and more memory than BFS.

## Algorithm used for hashing

The hashing technique uses the djb2 algorithm as the base hash function, with enhancements like scaling with the silver ratio and double hashing for collision resolution.



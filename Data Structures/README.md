# Data Structures
Data structures built as templates using recursive worker/wrapper functions.

## AVL.h
A dictionary class utilizing a self balancing binary tree built on top of BST.h

## BST.h
A non-balancing dictionary class utilizing a binary tree as the internal data structure.

## Dictionary.h
Dictionary utilizing a singly linked list as the internal data structure

### Basic usage:
The data structures are used the exact same way, they differ in internal data structures and algorithms
#### Initialization:
```
AVL<std::string, std::string> avlDict;
```
#### Insertion:
```
avlDict.insert("key", "data");
```
#### Lookup
```
avlDict.lookup("key");
```
#### Removal
```
avlDict.remove("key");
```
#### Printing the Tree (not implemented for Dictionary.h)
```
avlDict.printTree();
```
#### Printing entries
```
avlDict.printEntries();
```
or
```
std::cout << avlDict << "\n";
```

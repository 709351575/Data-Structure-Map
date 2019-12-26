# Data-Structure-Map
Implement STL::Map by Red-black Tree in C++

## Code Directory Structure
- `root`
    - `map.hpp`: the `map` class
    - `main.cpp`: the main program for test
    - `map.cbp`: the Codeblocks project file
    - `exceptions.hpp`: handles exceptions.
    - `class-bint.hpp`, `class-integer.hpp`, `class-matrix.hpp`, `deque.hpp` and `utility.hpp`: different classes of instances
    - `test/`
        - `map-basic.cc`: the basic test file
        - `map-advance-1.cc`, `map-advance-2.cc`, `map-advance-3.cc`, `map-advance-4.cc` and `map-advance-5.cc`: advanced test files
        - `testans-map-basic`: test results from `map-basic.cc`
        - `testans-map-advance-1`, `testans-map-advance-2`, `testans-map-advance-3`, `testans-map-advance-4` and `testans-map-advance-5`: corresponding test results from advanced test files
        
## Installation and Test
It takes the following steps to test the `map` class:

1. Install Codeblocks and open the project file `map.cbp`.

2. Copy the code from any test file, paste it to `main.cpp` and then run it.

Note: Please make sure that the compiler follows the C++11 language standard.

## The Map Class

### Template
The Map Class adopts the following template:

    template<class Key, class T, class Compare = std::less<Key> >
    
Here, **Key** is the key for data elements, **T** stands for the data value and class ** Compare** means comparison between values. For convenience, I define `value_type` as the pair of key and value.

    typedef pair<const Key, T> value_type;
    
### Struct mapNode:
The `mapNode` struct contains the following data members:

        value_type* data: the key and value
        mapNode* parent: points to the parent node
        mapNode* left: points to the left child node
        mapNode* right: points to the right child node
        int colour: indicates whether the node is black or red.

The maintain balance in the Red-black Tree, I have implemented the following functions:
    
        void LL(mapNode *&x)
        void LR(mapNode *&t,mapNode *&p)
        void RL(mapNode *&t,mapNode *&p)
        void RR(mapNode *&y)
        void insertRebalance(mapNode *&t): maintains balance upon insertion
        void removeReblance(mapNode *&node,mapNode *&father): maintains balance when an element is removed
    
### Class iterator and Class const_iterator

The iterator classes enable some operations in map like `map.begin()`. if there is anything wrong, it will throw `invalid_iterator` exception. Here is an example:

        it = map.begin(); 
        --it;

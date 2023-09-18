#ifndef DSTREE_H
#define DSTREE_H
// AVL Tree Implementation referenced from Michael Hahsler - (https://github.com/mhahsler/CS2341/blob/main/Chapter4_Trees/AVLTree/AvlTree.h)

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// templated with the key and ValueType value
template <typename KeyType, typename ValueType>

class DSTree
{
private:
    struct DSMapNode
    {
        // DSMapNode has a key and an element
        KeyType key;
        ValueType element;

        int height; // holds height
        // tree has two DSNodes, left and right
        DSMapNode *left;
        DSMapNode *right;

        // constructor to initialize the key, element derived from key, and height. Initialize DSMapNodes as nullptr
        DSMapNode(const KeyType &k, const ValueType &c = ValueType{}, DSMapNode *lefty = nullptr, DSMapNode *righty = nullptr, int h = 0)
            : key{k}, element{c},  height{h} , left{lefty},  right{righty} {}
    };
    // DSTreeIterator FOR RANGE BASED FOR LOOP
    class DSTreeIterator
    {
    private:
        //https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp - Logic and functions
        //https://medium.com/@alejandrodanielnoel/making-a-custom-iterator-class-in-c-3424f74a7138 - Use a stack-like implementation 

        vector<DSMapNode *> pointers;

    public:
        DSTreeIterator() = default;

        DSTreeIterator(DSMapNode *p)
        {
            while (p != nullptr)
            {
                pointers.push_back(p);
                p = p->left;
            }
        }

        bool operator==(const DSTreeIterator &it) const
        {
            return pointers == it.pointers;
        }

        bool operator!=(const DSTreeIterator &it) const
        {
            return !(*this == it);
        }

        //Pointer to last value of pointers stack (lkeeps track of where we are)
        DSMapNode &operator*() const
        {
            return *pointers.back();
        }

        DSMapNode *operator->() const
        {
            return &(operator*());
        }

        DSTreeIterator &operator++()
        {
            DSMapNode *p = pointers.back()->right;
            pointers.pop_back();
            while (p != nullptr)
            {
                pointers.push_back(p);
                p = p->left;
            }
            return *this;
        }

        DSTreeIterator operator++(int)
        {
            DSTreeIterator tmp = *this;
            ++(*this);
            return tmp;
        }
    };

    // AVL Trees should have a root
    DSMapNode *root = nullptr;

    // outer functionality
public:
    // default constructor
    DSTree()
    {
        root = nullptr;
    }
    // copy constructor - uses clone function
    DSTree(const DSTree &rhs)
    {
        root = nullptr;
        root = clone(rhs.root);
    }

    // destructor - uses makeEmpty function
    ~DSTree()
    {
        makeEmpty();
    }

    // copy assignment operator
    DSTree &operator=(const DSTree &rhs)
    {
        makeEmpty();            // clear
        root = clone(rhs.root); // copy
        return *this;           // returns pointer to original object
    }

    // searches if element is in the tree. Returns true if element is found in tree
    bool contains(const KeyType &k) const
    {
        return contains(k, root);
    }
    // returns true if the tree is empty
    bool isEmpty() const
    {
        return root == nullptr; // if root is nullptr, empty
    }

    // prints tree contents in sorted order (MHashler Github)
    void printTreeSort() const
    {
        ostream &out = cout;
        printTreeSort(root, out);
    }

    // make tree empty
    void makeEmpty()
    {
        makeEmpty(root);
    }
    // insert x into the tree
    void insert(const KeyType &x)
    {
        insert(x, root);
    }
    // remove x from the tree
    void remove(const KeyType &x)
    {
        remove(x, root);
    }

    // returns Pointer to memory of ValueType element (vector of documents)
    ValueType* find(const KeyType& x)
    {
        // gets root node
        DSMapNode *current = root;
        // iterates through
        while (current != nullptr)
        {
            if (x == current->key)
            {
                auto temp = &current->element;
                return temp;
            }
            else if (x < current->key)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
        return nullptr; // Return an empty vector if the key is not found
    }
    //Begin and end necessary for range based for loop
    DSTreeIterator begin() const
    {
        return DSTreeIterator(root);
    }

    DSTreeIterator end() const
    {
        return nullptr;
    }

    // inner functionality
private:
    // inserts key into the tree
    void insert(const KeyType &k, DSMapNode *&t)
    {
        // if the first node is empty
        if (t == nullptr)
        {

            t = new DSMapNode{k}; // place key
            return;               // single node, balanced
        }
        else if (k < t->key)
        {
            insert(k, t->left); // wHEN KEY IS LESS THAN ELEMENT AT NODE T, INSERT TO LEFT
        }
        else if (k > t->key)
        {
            insert(k, t->right); // wHEN KEY IS MORE THAN ELEMENT AT NODE T, INSERT TO RIGHT
        }
        else
        {
            // duplicate key, do nothing
        }
        // this will call balance on the way back up the tree. It will only balance once at node where the tree is
        // imbalance and updates height
        balance(t);
    }

    // remove from a subtree with the keyvalue k
    // code referenced from https://github.com/Ekan5h/AVLtree/blob/master/AVL.cpp
    void remove(const KeyType &k, DSMapNode *&t)
    {
        // tree empty
        if (t == nullptr)
        {
            return;
        }
        // if key k is less than current node key, recurse to left subtree
        if (k < t->key)
        {
            remove(k, t->left);
        }
        // if key is greater than current node key, recurse to right subtree
        else if (k > t->key)
        {
            remove(k, t->right);
        }
        else
        {
            // current node key matches the ke
            DSMapNode *righty = t->right;
            // checks if left or right child is nullptr
            // if right is nullptr set current node to left child and delete original. vice-versa
            if (righty == nullptr)
            {
                DSMapNode *lefty = t->left;
                delete (t);
                t = lefty;
            }
            else if (t->left == nullptr)
            {
                delete t;
                t = righty;
            }
            else
            {
                // neither hcild node is nullptr. find leftmost node in right subtree of current node and swaps its element to remove
                while (righty->left != nullptr)
                {
                    righty = righty->left;
                }
                t->key = righty->key;
                remove(righty->key, t->right);
                return;
            }
        }
        balance(t); // keeps avl tree balanced
    }

    // checks if key exists in tree
    bool contains(const KeyType &k, DSMapNode *t) const
    {
        if (t == nullptr)
        {
            return false;
        }
        else if (k < t->key)
        {
            return contains(k, t->left);
        }
        else if (k > t->key)
        {
            return contains(k, t->right);
        }
        else
        {
            return true;
        }
    }

    // clears subtree
    void makeEmpty(DSMapNode *&t)
    {
        if (t == nullptr)
            return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = nullptr;
    }

    // cloning subtree
    DSMapNode *clone(DSMapNode *t) const
    {
        if (t == nullptr)
            return nullptr;
        return new DSMapNode{t->key, t->element, clone(t->left), clone(t->right), t->height};
    }

    // Prints subtree rooted at t in sorted order (Inorder Traversal)
    void printTreeSort(DSMapNode *t, ostream &out) const
    {
        if (t != nullptr)
        {
            printTreeSort(t->left, out);
            out << t->element << " ";
            printTreeSort(t->right, out);
        }
        // recursion
    }

    // returns height of tree
    int height(DSMapNode *t) const
    {
        return t == nullptr ? -1 : t->height;
    }

    static const int ALLOWED_IMBALANCE = 1; // 1 is the default; more will make balancing cheaper

    // Assume t is balanced or within one of being balanced since we check this after each manipulation
    // t represents alpha in the textbook
    void balance(DSMapNode *&t)
    {
        if (t == nullptr)
            return;

        if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) // unbalancing insertion was left
        {
            if (height(t->left->left) >= height(t->left->right))
                rotateWithLeftChild(t); // case 1 (outside)
            else
                doubleWithLeftChild(t); // case 2 (inside)
        }
        else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) // unbalancing insertion was right
        {
            if (height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t); // case 4 (outside)
            else
                doubleWithRightChild(t); // case 3 (inside)
        }

        // update height
        t->height = max(height(t->left), height(t->right)) + 1;
    }

    // returns max
    int max(int lhs, int rhs) const
    {
        return lhs > rhs ? lhs : rhs;
    }
    // case one, rotating with left child
    void rotateWithLeftChild(DSMapNode *&k2)
    {
        DSMapNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;
        k2 = k1;
    }

    // case two, rotating with right child
    void rotateWithRightChild(DSMapNode *&k1)
    {
        DSMapNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->right), k1->height) + 1;
        k1 = k2;
    }

    // double rotate binary tree node
    void doubleWithLeftChild(DSMapNode *&k3)
    {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    void doubleWithRightChild(DSMapNode *&k1)
    {
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }
};
#endif

// word points to a set of values, singly linked list, with document names
// can use a vector to store key and values
// needs to have an important property, entry for the keys use a binary search tree that makes it easier to sort for files
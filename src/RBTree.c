/**
 * @file RBTree.c
 * @author  Moshe Tannenbaum <moshe.tannenbaum@mail.huji.ac.il>
 * @version 1.0
 * @date 12 Dec 2019
 *
 * @brief Implements a generic Red and black tree,
 *
 */
#include "RBTree.h"
#include <stdlib.h>
#include <stdio.h>


void traverseInOrder(Node *node, forEachFunc func, void *args);

void freeTreeHelper(Node *node, RBTree *tree);

void caseOneInsert(Node *node, RBTree *tree);

void caseThreeInsert(Node *node, RBTree *tree);

void caseFourInsert(Node *node, RBTree *tree);

void rightRotate(Node *node, RBTree *tree);

Node *getSibling(Node *node);

/**
 * Constructs a new RBTree with the given CompareFunc and freeFunc
 * @param compFunc - function that compares between to two elements in the tree
 * @param freeFunc - function that Free an element in the tree
 * @return
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree *rbTree = malloc(sizeof(RBTree));
    rbTree->root = NULL;
    rbTree->compFunc = compFunc;
    rbTree->freeFunc = freeFunc;
    rbTree->size = 0;
    return rbTree;
}

/**
 * Reconstructs the given tree to the Red and Black tree convention
 * @param newNode - new node that was added that broke the convention
 * @param tree - rb tree with at most one violation
 * @return 1 on success
 */
int fixingUpTheTree(Node *newNode, RBTree *tree)
{
    // Case 1: if newNode is root
    if ((newNode)->parent == NULL)
    {
        caseOneInsert(newNode, tree);
        return 1;
    }
    // Case 2: if newNode parent is black no need to change
    if ((newNode)->parent->color == BLACK)
    {
        return 1;
    }
    // Case 3: parent and uncle of newNode are RED
    Node *uncle = getSibling((newNode)->parent);
    if (uncle != NULL && uncle->color == RED)
    {
        caseThreeInsert(newNode, tree);
        return 1;
    }
    // Case 4: the parent is red and the uncle is black
    caseFourInsert(newNode, tree);
    return 1;
}

/**
 * Performs a left rotate on the given tree
 * @param node - new node
 * @param tree - tree
 */
void leftRotate(Node *node, RBTree *tree)
{
    Node *newNode = node->right;
    if (newNode != NULL)
    {
        node->right = newNode->left;
    }

    if (node->right != NULL)
    {
        node->right->parent = node;
    }
    if (node->parent == NULL)
    {
        tree->root = newNode;
        newNode->parent = NULL;
    }
    else
    {
        newNode->parent = node->parent;

        if (node->parent->left == node)
        {
            newNode->parent->left = newNode;
        }
        else
        {
            newNode->parent->right = newNode;
        }
    }
    newNode->left = node;
    node->parent = newNode;
}

/**
 * Performs a right rotate on the given tree
 * @param node - new node
 * @param tree - rb tree
 */
void rightRotate(Node *node, RBTree *tree)
{
    Node *temp = node->left;
    node->left = temp->right;

    if (node->left != NULL)
    {
        node->left->parent = node;
    }
    if (node->parent == NULL)
    {
        tree->root = temp;
        temp->parent = NULL;
    }
    else
    {
        temp->parent = node->parent;
        if (node->parent->right == node)
        {
            temp->parent->right = temp;
        }
        else
        {
            temp->parent->left = temp;
        }
    }
    temp->right = node;
    node->parent = temp;
}


/**
 * Case 1: the new node is the root of the tree
 * @param node: the new Node
 * @param tree: the tree to insert the node to
 */
void caseOneInsert(Node *node, RBTree *tree)
{
    if (node->parent == NULL)
    {
        node->color = BLACK;
        tree->root->color = BLACK; //TODO: change this its reduntt
    }
}

/**
 * Case 4: The parent is red and the uncle is black
 * @param node: the new Node
 * @param tree: tree to insert the node to
 */
void caseFourInsert(Node *node, RBTree *tree)
{
    Node *p = node->parent;
    Node *g = p->parent;

    if ((node == p->right) && (p == g->left))
    {
        leftRotate(p, tree);
        node = node->left;

    }
    else if (node == p->left && p == g->right)
    {
        rightRotate(p, tree);
        node = node->right;
    }

    p = node->parent;
    g = p->parent;
    if (node == p->left)
    {
        rightRotate(g, tree);
    }
    else
    {
        leftRotate(g, tree);
    }
    p->color = BLACK;
    g->color = RED;
}

/**
 * Case 3: parent and uncle of newNode are RED
 * @param node: the new Node
 * @param tree: tree to insert the node to
 */
void caseThreeInsert(Node *node, RBTree *tree)
{
    Node *g = node->parent->parent;
    node->parent->color = BLACK;
    getSibling(node->parent)->color = BLACK;
    g->color = RED;
    fixingUpTheTree(g, tree);
}

/**
 * Gets the givens nodes siblings if it has any
 * @param node
 * @return The given nodes siblings, if the node doesn't have any then returns NULL
 */
Node *getSibling(Node *node)
{
    Node *p = node->parent;

    if (p == NULL)
    {
        return NULL;
    }
    if (node == p->left)
    {
        return p->right;
    }
    return p->left;

}

/**
 * Add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int addToRBTree(RBTree *tree, void *data)
{

    if (data == NULL)
    {
        return 0;
    }
    if (tree == NULL)
    {
        return 0;
    }
    // Creating new Node
    Node *newNode = malloc(sizeof(Node));
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->data = data;
    newNode->color = RED;


    Node *curNode = tree->root;
    // Empty tree, so set the root to the new Node
    if (curNode == NULL)
    {
        tree->root = newNode;
        newNode->color = BLACK;
        tree->size++;
        return 1;
    }

    // Traversing through tree till find place to add the node
    while (curNode != NULL)
    {
        int relation = tree->compFunc(data, curNode->data);
        if (relation == 0)
        {
            free(newNode);
            return 0; // Item already in tree
        }
        if (relation > 0)
        {
            if (curNode->right == NULL)
            {
                curNode->right = newNode;
                newNode->parent = curNode;
                break;
            }
            curNode = curNode->right;
        }
        if (relation < 0)
        {
            if (curNode->left == NULL)
            {
                curNode->left = newNode;
                newNode->parent = curNode;
                break;
            }
            curNode = curNode->left;
        }
    }

    fixingUpTheTree(newNode, tree);
    tree->size++;
    return 1;
}

/**
 * Checks if tree contains theh given data
 * @param tree -
 * @param data -
 * @return 1 if tree contains data, else 0, -1 if fail
 */
int containsRBTree(RBTree *tree, void *data)
{
    if (tree == NULL)
    {
        return -1;
    }
    Node *curNode = tree->root;
    // Empty tree, so set the root to the new Node
    while (curNode != NULL)
    {
        int relation = tree->compFunc(data, curNode->data);
        if (relation == 0)
        {
            return 1; // Item in tree
        }
        if (relation > 0)
        {
            curNode = curNode->right;
        }
        if (relation < 0)
        {
            curNode = curNode->left;
        }
    }
    return 0;
}

/**
 *
 * @param tree
 * @param func
 * @param args
 * @return
 */
int forEachRBTree(RBTree *tree, forEachFunc func, void *args)
{
    if (tree == NULL)
    {
        return 0;
    }
    traverseInOrder(tree->root, func, args);
    return 1;
}

/**
 * travese sthrough the tree
 * @param node
 * @param func
 * @param args
 */
void traverseInOrder(Node *node, forEachFunc func, void *args)
{
    if (node == NULL)
    {
        return;
    }
    traverseInOrder(node->left, func, args);
    func(node->data, args);
    traverseInOrder(node->right, func, args);
}

/**
 * frees tree
 * @param tree
 */
void freeRBTree(RBTree *tree)
{
    freeTreeHelper(tree->root, tree);
    free(tree);
}

/**
 * helps free the tree recursively
 * @param node
 * @param tree
 */
void freeTreeHelper(Node *node, RBTree *tree)
{
    if (node == NULL)
    {
        return;
    }
    freeTreeHelper(node->left, tree);
    freeTreeHelper(node->right, tree);

    if (tree != NULL && tree->freeFunc != NULL)
    {
        tree->freeFunc(node->data);
        node->data = NULL;
    }
    free(node);
}


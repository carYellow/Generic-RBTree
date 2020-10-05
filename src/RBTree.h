/**
 * @file RBTree.c
 * @author  Moshe Tannenbaum <moshe.tannenbaum@mail.huji.ac.il>
 * @version 1.0
 * @date 12 Dec 2019
 *
 * @brief header of generic Red and black tree,
 *
 */

#ifndef RBTREE_RBTREE_H
#define RBTREE_RBTREE_H

/**
 * The color of node on RBTree
 */
typedef enum Color
{
	RED, BLACK
} Color;

/**
 * Compares between two given items.
 * @param a - pointer to first item
 * @param a - pointer to second item
 * @return: equal to 0 iff a == b. Lower than 0 if a < b. Greater than 0 iff b < a.
 */
typedef int (*CompareFunc)(const void *a, const void *b);

/**
 * A function to apply on the tree items.
 * @object: a pointer to an item of the tree.
 * @args: pointer to other arguments for the function.
 * @return: 0 on failure, other on success.
 */
typedef int (*forEachFunc)(const void *object, void *args);

/**
 * a function to free a structure item
 * @object: a pointer to an item of the tree.
 */
typedef void (*FreeFunc)(void *data);

/*
 * Node of the tree.
 */
typedef struct Node
{
	struct Node *parent, *left, *right;
	Color color;
	void *data;

} Node;

/**
 * Represents the tree
 */
typedef struct RBTree
{
	Node *root;
	CompareFunc compFunc;
	FreeFunc freeFunc;
	int size;
} RBTree;

/**
 * constructs a new RBTree with the given CompareFunc and freeFunc
 * @param compFunc - function that compares between to two elements in the tree
 * @param freeFunc - function that Free an element in the tree
 * @return
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc);

/**
 * Adds an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int addToRBTree(RBTree *tree, void *data);

/**
 * check whether the tree contains this item.
 * @param tree: the tree to check
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int containsRBTree(RBTree *tree, void *data);


/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(RBTree *tree, forEachFunc func, void *args);

/**
 * free all memory of the data structure.
 * @param tree: the tree to free.
 */
void freeRBTree(RBTree *tree);


#endif //RBTREE_RBTREE_H
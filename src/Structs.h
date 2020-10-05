/**
 * @file Structs.c
 * @author  Moshe Tannenbaum <moshe.tannenbaum@mail.huji.ac.il>
 * @version 1.0
 * @date 12 Dec 2019
 *
 * @brief Implements a vector struct and a string struct to be used in a generic RBTree
 *

 */
#include "RBTree.h"

#ifndef STRUCTS_H
#define STRUCTS_H

/**
 * Represents a vector. The double* is dynamically allocated
 */
typedef struct Vector
{
	int len;
	double *vector;
} Vector;


/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b);

/**
 * ForEach function that concatenates the given word to pConcatenated.
 * Note: pConcatenated must already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated); // implement it in Structs.c

/**
 * Free the given strings memory
 * @param s - pointer of string to free
 */
void freeString(void *s);

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If the vectors are of different lengths, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b);

/**
 * Free the given vectors memory
 * @param pVector - pointer of vector to free
 */
void freeVector(void *pVector);

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector);

/**
 * @param tree - a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree);


#endif //STRUCTS_H

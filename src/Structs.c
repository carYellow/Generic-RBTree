/**
 * @file Structs.c
 * @author  Moshe Tannenbaum <moshe.tannenbaum@mail.huji.ac.il>
 * @version 1.0
 * @date 12 Dec 2019
 *
 * @brief Implements a vector struct and a string struct to be used in a generic RBTree
 *


 */
#include "Structs.h"
#include "RBTree.h"
#include "string.h"
#include <stdlib.h>

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    if (a == NULL || b == NULL)
    {
        return -1;
    }
    const char *aString = (char *) a;
    const char *bString = (char *) b;

    return strcmp(aString, bString);
}

/**
 * ForEach function that concatenates the given word to pConcatenated. pConcatenated is already allocated with
 * enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    if (word == NULL || pConcatenated == NULL)
    {
        return 0;
    }
    const char *wordSting = (char *) word;
    char *pConcatenatedString = (char *) pConcatenated;
    strcat(pConcatenatedString, wordSting);
    strcat(pConcatenatedString, "\n");
    return 1;
}

/**
 * Free the given strings memory
 * @param s - pointer of string to free
 */
void freeString(void *s)
{
    char *string = (char *) s;
    free(string);
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If the vectors are of different lengths, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    if (a == NULL || b == NULL)
    {
        return -1;
    }
    Vector *v1 = (Vector *) a;
    Vector *v2 = (Vector *) b;
    if (v1->vector == NULL || v2->vector == NULL || v1->len < 0 || v1->len < 0)
    {
        return -1;
    }
    int minLength = v1->len > v2->len ? v2->len : v1->len;

    for (int i = 0; i < minLength; ++i)
    {
        if (v1->vector[i] > v2->vector[i])
        {
            return 1;
        }
        if (v1->vector[i] < v2->vector[i])
        {
            return -1;
        }
    }
    if (v1->len > v2->len)
    {
        return 1;
    }
    if (v1->len < v2->len)
    {
        return -1;
    }
    return 0;
}

/**
 * Free the given vectors memory
 * @param pVector - pointer of vector to free
 */
void freeVector(void *pVector)
{
    Vector *v1 = (Vector *) pVector;
    if (v1 != NULL)
    {
        free(v1->vector);
    }

    free(v1);
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if (pVector == NULL || pMaxVector == NULL)
    {
        return 0;
    }
    Vector *v1 = (Vector *) pVector;
    Vector *maxVector = (Vector *) pMaxVector;

    int lengthOfVec = v1->len;
    if (lengthOfVec < 0)
    {
        return 0;
    }
    if (maxVector->vector == NULL)
    {
        maxVector->vector = (double *) malloc(sizeof(double) * lengthOfVec);
        memcpy(maxVector->vector, v1->vector, sizeof(double) * lengthOfVec);
        maxVector->len = v1->len;
        return 1;
    }
    // Calculating the norms of the vector
    double normOfV1 = 0;
    for (int i = 0; i < v1->len; ++i)
    {
        normOfV1 += ((v1->vector[i]) * (v1->vector[i]));
    }
    double normOfMaxV = 0;
    for (int i = 0; i < maxVector->len; ++i)
    {
        normOfMaxV += ((maxVector->vector[i]) * (maxVector->vector[i]));
    }

    if (normOfV1 > normOfMaxV)
    {
        maxVector->vector = (double *) realloc(maxVector->vector, sizeof(double) * lengthOfVec);
        memcpy(maxVector->vector, v1->vector, sizeof(double) * lengthOfVec);
        maxVector->len = v1->len;
    }

    return 1;
}

/**
 * @param tree -a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    if (tree == NULL)
    {
        return NULL;
    }
    Vector *maxVector = (Vector *) malloc(sizeof(Vector));
    maxVector->len = 0;
    maxVector->vector = NULL;
    forEachRBTree(tree, copyIfNormIsLarger, maxVector);
    return maxVector;
}

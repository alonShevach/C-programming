#include "Structs.h"
#include <stdlib.h>
#include <string.h>

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    char* strA = (char*)a;
    char* strB = (char*)b;
    return strcmp(strA, strB);
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
    if (pConcatenated != NULL && word != NULL)
    {
        char *strWord = (char *) word;
        char *strPCon = (char *) pConcatenated;
        strcat(strPCon, strWord);
        strcat(strPCon, "\n");
        return 1;
    }
    return 0;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    char* str = (char*)s;
    if( s != NULL)
    {
        free(str);
    }
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector *vecA = (Vector *) a;
    Vector *vecB = (Vector *) b;
    int smallerVec;
    if (vecA->len > vecB->len)
    {
        smallerVec = vecB->len;
    }
    else
    {
        smallerVec = vecA->len;
    }
    for (int i = 0; i < smallerVec; ++i)
    {
        if (vecA->vector[i] > vecB->vector[i])
        {
            return 1;
        }
        if (vecA->vector[i] < vecB->vector[i])
        {
            return -1;
        }
    }
    if (vecA->len > vecB->len)
    {
        return 1;
    }
    if (vecA->len < vecB->len)
    {
        return -1;
    }
    return 0;
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    if(pVector == NULL)
    {
        return;
    }
    Vector *newVec = (Vector*)pVector;
    if(newVec->vector != NULL)
    {
        free(newVec->vector);
        newVec->vector = NULL;
    }
    free(newVec);
}
/**
 * a function that calculated the norm of the given vector
 * @param vec a vector to calculate the norm
 * @return the vector's norm.
 */
double calculateNorm(Vector *vec)
{
    double norm = 0;
    for(int i = 0; i < vec->len; ++i)
    {
        norm += (vec->vector[i] * vec->vector[i]);
    }
    return norm;
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
	Vector *pVec = (Vector*)pVector;
	Vector *pMaxVec = (Vector*)pMaxVector;
    if (pVec != NULL && pMaxVec != NULL)
    {
        if (pMaxVec->vector == NULL)
        {
            pMaxVec->vector = (double *) malloc(pVec->len * sizeof(double));
            pMaxVec->len = pVec->len;
            for (int i = 0; i < pVec->len; i++)
            {
                pMaxVec->vector[i] = pVec->vector[i];
            }
            return 1;
        }
        if (pVec->vector == NULL)
        {
            return 0;
        }
        double pNorm = calculateNorm(pVec);
        double maxNorm = calculateNorm(pMaxVec);
        if (pNorm > maxNorm)
        {
            pMaxVec->vector = (double *) realloc(pMaxVec->vector, sizeof(double) * pVec->len);
            pMaxVec->len = pVec->len;
            for (int i = 0; i < pVec->len; i++)
            {
                (pMaxVec)->vector[i] = pVec->vector[i];
            }
            return 1;
        }
    }
    return 0;
}

/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
	if (tree == NULL)
	{
		return NULL;
	}
	Vector *biggestVec = (Vector*)malloc(sizeof(Vector));
    biggestVec->vector = NULL;
    biggestVec->len = 0;
	forEachRBTree(tree, copyIfNormIsLarger, biggestVec);
	return biggestVec;
}

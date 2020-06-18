#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define NUM_ARGS 3
#define WRONG_NUM_OF_ARGS_MSG "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n"
#define INVALID_INPUT_MSG "Invalid input\n"
#define MEMORY_ALLOCATION_FAILED "Memory allocation failed\n"
#define MAX_LINE_LEN 1024
#define BASE 10
#define INPUT_DELIMS " \t\r\n"
#define DEFAULT_PARENT -1
#define DEFAULT_DIAMETER 0

enum validityType
        {
    INVALID_INPUT,
    NOT_A_TREE,
    BAD_MEMORY_ALLOCATION,
    VALID_INPUT
        };

/**
 * structure that represents a Node in the graph. Each node has a different key,
 * each nodes has children if it has ones, and each has a parent and parent key, unless he is a root.
 */
typedef struct Node
{
    int key;
    int numOfChildren;
    int parentKey;
    int distance;
    struct Node *previus;
    struct Node **children;
    struct Node *parent;
} Node;

/**
 * A function that frees the given node.
 * @param node the Node to be freed.
 */
void freeNode(Node *node)
{
    if (node != NULL)
    {
        if (node->children != NULL)
        {
            free(node->children);
        }
    }
}

/**
 * structure that represents a tree graph. Each Tree has a root, the nodes in the tree,
 * the number of nodes in that tree and the depth of the tree.
 */
typedef struct Tree
{
    int numOfEdges;
    int numOfNodes;
    int root;
    int maxBranch;
    int minBranch;
    int diameter;
    int pathLen;
    int *path;
    Node *nodes;
} Tree;

/**
 * A function that frees the given node.
 * @param node the tree to be freed.
 */
void freeTree(Tree *tree)
{
    if (tree != NULL)
    {
        if (tree->nodes != NULL)
        {
            for (int i = 0; i < tree->numOfNodes; i++)
            {
                freeNode(tree->nodes + i);
            }
            free(tree->nodes);
        }
        free(tree->path);
    }
}

/**
 * a function that initializes the given tree with its default values.
 * @param tree the tree to initiate.
 */
void initiateTree(Tree *tree)
{
    tree->minBranch = 0;
    tree->numOfNodes = 0;
    tree->pathLen = 0;
    tree->numOfEdges = 0;
    tree->diameter = 0;
    tree->maxBranch = 0;
    tree->root = 0;
    tree->path = NULL;
    tree->nodes = NULL;
}

/**
 * a function that initializes the given node with its default values.
 * @param newNode the node to initiate.
 * @return 0 if it was successful and 1 if there was noe memory allocation
 */
int newNode(Node *newNode)
{
    if (newNode != NULL)
    {
        newNode->parentKey = DEFAULT_PARENT;
        newNode->distance = 0;
        newNode->numOfChildren = 0;
        newNode->key = 0;
        newNode->previus = NULL;
        newNode->children = NULL;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

/**
 * the known BFS function, used to find the longest path in the tree, also to varify if it
 * is a tree.
 * @param tree the graph in which the search is done.
 * @param s the node from which the search begin.
 * @return 0 if the graph is a tree and 1 if it's not.
 */
int BFS(Tree *tree, Node *s)
{
    int inf = tree->numOfNodes + 1;
    Node *curNode;
    int  curNodeKey;
    // init the distace of the nodes.
    for (int i = 0; i < tree->numOfNodes; ++i)
    {
        tree->nodes[i].distance = inf;
    }
    s->previus = NULL;
    s->distance = 0;
    Queue * Q = allocQueue();
    enqueue(Q, (unsigned int)s->key);
    while (!queueIsEmpty(Q))
    {
        curNodeKey = (int)dequeue(Q);
        curNode = &tree->nodes[curNodeKey];
        for (int j = 0; j < (curNode->numOfChildren); ++j)
        {
            if (curNode->previus == NULL || curNode->previus != curNode->children[j])
            {
                if (curNode->children[j]->distance == inf)
                {
                    enqueue(Q, (unsigned int)curNode->children[j]->key);
                    curNode->children[j]->previus = curNode;
                    curNode->children[j]->distance = curNode->distance + 1;
                }
                else
                {
                    return EXIT_FAILURE;  //since the node was visited already
                }
            }
        }
        if (curNode->parentKey != DEFAULT_PARENT &&
            (curNode->previus == NULL || curNode->previus->key != curNode->parentKey))
        {
            enqueue(Q, (unsigned int)curNode->parentKey);
            curNode->parent->previus = curNode;
            curNode->parent->distance = curNode->distance + 1;
        }
    }
    freeQueue(&Q);
    return EXIT_SUCCESS;
}

/**
 * a function creating a tree, while creating its nodes according to the input, and set
 * his node's keys.
 * @param treeNumOfNodes the number of the nodes in the tree.
 * @return the created tree from the function.
 */
enum validityType newTree(int treeNumOfNodes, Tree* myTree)
{
    if (myTree != NULL)
    {
        myTree->numOfNodes = treeNumOfNodes;
        myTree->numOfEdges = 0;
        myTree->diameter = DEFAULT_DIAMETER;
        myTree->path = (int*)malloc(sizeof(int) * treeNumOfNodes);
        myTree->nodes = (Node *) malloc(treeNumOfNodes * sizeof(Node));
        if (myTree->nodes != NULL)
        {
            for (int i = 0; i < treeNumOfNodes; i++)
            {
                if (newNode(&myTree->nodes[i]) == EXIT_FAILURE)
                {
                    return BAD_MEMORY_ALLOCATION;
                }
                if (&(myTree->nodes[i]) == NULL)
                {
                    freeTree(myTree);
                    return BAD_MEMORY_ALLOCATION;
                }
                myTree->nodes[i].key = i;
            }
        }
        else
        {
            // creating tree without spaces.
            return BAD_MEMORY_ALLOCATION;
        }
    }
    else
    {
        return BAD_MEMORY_ALLOCATION;
    }
    return VALID_INPUT;
}

/**
 * a function to calculate and update the tree according to the longest branches.
 * @param tree the tree to calculate the branches in.
 * @param minBranch the minimum branch in the tree
 * @param maxBranch the maximum branch in the tree
 */
void minMaxBranch(Tree *tree, int *minBranch, int *maxBranch)
{
    for (int i = 0; i< tree->numOfNodes; ++i)
    {
        if (tree->nodes[i].distance > *maxBranch)
        {
            *maxBranch = tree->nodes[i].distance;
        }
        if (tree->nodes[i].numOfChildren == 0)
        {
            if (tree->nodes[i].distance < *minBranch)
            {
                *minBranch = tree->nodes[i].distance;
            }
        }
    }
}


/**
 * a function checking if the given graph is a tree or not ( checking if it is a connected graph
 * without any circles)
 * @param tree the graph
 * @return VALID_INPUT enum if the graph is a tree and NOT_A_TREE enum if it is not
 */
enum validityType isTheGraphTree(Tree *tree)
{
    int minBranch = (tree->numOfNodes + 1);
    int maxBranch = 0;
    if (tree->numOfEdges != (tree->numOfNodes - 1))
    {
        return NOT_A_TREE;
    }
    if (BFS(tree, &tree->nodes[tree->root]) == EXIT_FAILURE)
    {
        return NOT_A_TREE;
    }
    for (int i = 0; i < tree->numOfNodes; ++i)
    {
        if (tree->nodes[i].distance >= tree->numOfNodes)
        {
            return NOT_A_TREE;
        }
    }
    minMaxBranch(tree, &minBranch, &maxBranch);
    tree->maxBranch = maxBranch;
    tree->minBranch = minBranch;
    return VALID_INPUT;
}

/**
 * Calculates the diameter of the tree and creates the path between u and v in the tree
 * @param tree the tree in which the path and the diameter and the path are calculated
 * @param uKey the first vertex
 * @param vKey the second vertex
 */
void diameterAndPath(Tree *tree, Node *uKey, Node *vKey)
{
    int maxBranch = 0;
    int minBranch = 0;
    for (int i = 0; i < tree->numOfNodes; ++i)
    {
        // finding the maximum branch using BFS.
        BFS(tree, &tree->nodes[i]);
        minMaxBranch(tree, &minBranch, &maxBranch);
        if (tree->nodes[i].key == uKey->key)
        {
            tree->pathLen = 0;
            int curNodeKey = vKey->key;
            while (curNodeKey != uKey->key)
            {
                tree->path[tree->pathLen] = curNodeKey;
                curNodeKey = tree->nodes[curNodeKey].previus->key;
                (tree->pathLen)++;
            }
            tree->path[tree->pathLen] = uKey->key;
            (tree->pathLen)++;
        }
    }
    tree->diameter = maxBranch;
}

/**
 * Process the number as a char and turns it to an integer
 * @param value the representation of the number as a string
 * @param length the length of the number
 * @param nPointer holds the number
 * @return VALID_INPUT enum if the number was processed and INVALID_INPUT enum if the given value is not an integer.
 */
enum validityType processN(const char *value, int length, int *nPointer)
{
    if (length > 1 && value[0] == '0')
    {
        return INVALID_INPUT;
    }
    for (int i = 0; i < length; i++)
    {
        if ((value[i] < '0') || (value[i] > '9'))
        {
            return INVALID_INPUT;
        }
    }
    * nPointer = (int)strtol(value, NULL , BASE);
    return VALID_INPUT;
}

/**
 * Count the number of children of each node
 * @param line the line in which the children are written
 * @param numOfChildren holds the number of children
 * @param length the length of the line
 */
void childrenCounter(const char *line, int *numOfChildren, int length)
{
    for (int i = 0; i < length; ++i)
    {
        if (line != NULL && line[i] != ' ' && line[i] != '\n')
        {
            (*numOfChildren)++;
            while(line[i] != ' ' && line[i] != '\n' && i < length)
            {
                i++;
            }
        }
    }
}

/**
 * Exits the program - prints the relevant message and  frees the tree
 * @param problem the current problem
 * @param tree the tree
 * @return 1 if there is a problem and 0 if not
 */
int exitPro(enum validityType problem, Tree *tree)
{
    if (problem != VALID_INPUT)
    {
        if ((problem == INVALID_INPUT) || (problem == NOT_A_TREE))
        {
            fprintf(stderr, INVALID_INPUT_MSG);
        }
        else if (problem == BAD_MEMORY_ALLOCATION)
        {
            fprintf(stderr, MEMORY_ALLOCATION_FAILED);
        }
        freeTree(tree);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


/**
 * Process each line in the file - counts the number of the children of each node and initializes the tree accordingly
 * @param curNode the node that the given line describes it's children
 * @param line the line that is processed, describes the children of the given node
 * @param numOfNodes the number of nodes in the given tree
 * @param treeP pointer to the tree
 * @return INVALID_INPUT enum if the line is not valid, NOT_A_TREE enum if the line
 * is valid but does not describe a tree and VALID_INPUT enum if it is ok
 */
enum validityType processLine(Node *curNode, char *line, const int *numOfNodes, Tree *treeP)
{
    char *value = NULL;
    int length = (int)strlen(line);
    if (line != NULL && line[0] == '-')
    {
        if ((length == 0) || ((length > 1 && line[1] != '\n') && (length > 1 && line[1] != '\r')))
        {
            return INVALID_INPUT;
        }
        else
        {
            curNode->children = NULL;
            curNode->numOfChildren = 0;
            return VALID_INPUT;
        }
    }
    int numOfChildren = 0;
    enum validityType isAtree = VALID_INPUT;
    childrenCounter(line, &numOfChildren, length);
    if (numOfChildren == 0)
    {
        return INVALID_INPUT;
    }
    treeP->numOfEdges += numOfChildren;
    if (numOfChildren > *numOfNodes)
    {
        isAtree = NOT_A_TREE;
    }
    curNode->numOfChildren = numOfChildren;
    curNode->children = (Node**)malloc(numOfChildren * sizeof(Node));
    if (curNode->children != NULL)
    {
        value = strtok(line, INPUT_DELIMS);
        int i = 0;
        while (value != NULL)
        {
            int sonKey;
            int precessResult = processN(value, (int)strlen(value), &sonKey);
            if (precessResult == INVALID_INPUT || sonKey >= *numOfNodes)
            {
                return INVALID_INPUT;
            }
            curNode->children[i] = &treeP->nodes[sonKey];
            // if the node is already visited
            if (curNode->children[i]->parentKey != DEFAULT_PARENT)
            {
                isAtree = NOT_A_TREE;
            }
            curNode->children[i]->parentKey = curNode->key;
            curNode->children[i]->parent = curNode;
            i++;
            value = strtok(NULL, INPUT_DELIMS);
        }
        if (isAtree == NOT_A_TREE)
        {
            return NOT_A_TREE;
        }
    }
    else
    {
        return BAD_MEMORY_ALLOCATION;
    }
    return VALID_INPUT;
}

/**
 * Checks if the given input is a valid input, checking each line is valid, and checks if the number of vertices
 * is valid comparing the given input.
 * @param argv the arguments that are given to the program
 * @param nPointer A pointer to the number of vertices in the graph - the first line in the file
 * @param uPointer A pointer to the second argument that is given as an input - the first of the vertices
 * @param vPointer A pointer to the third argument that is given as an input - the second of the vertices
 * @param treeP A pointer to the tree
 * @param fileP the file that is given as the first argument
 * @return INVALID INPUT enum if the file is not valid or one of the lines in the file is not valid,
 * NOT_A_TREE enum if the given file does not describe a tree, BAD_MEMORY_ALLOCATION enum
 * if there is no memory allocation and VALID_INPUT enum if the file was processed successfully
 */
enum validityType inputValidityCheck(char **argv, int *nPointer, int *uPointer, int *vPointer, Tree *treeP, FILE *fileP)
{
    enum validityType isAtree = VALID_INPUT;
    char *value = NULL;
    if (fileP == NULL)
    {
        return INVALID_INPUT;
    }
    int count_lines = 0;
    char currentLine[MAX_LINE_LEN];
    if (fgets(currentLine, MAX_LINE_LEN, fileP) == NULL)  //can't open the file
    {
        return INVALID_INPUT;
    }
    count_lines++;
    value = strtok(currentLine, INPUT_DELIMS);
    if (value == NULL || strtok(NULL, INPUT_DELIMS) != NULL) //no numbers in the first line or more than one number
    {
        return INVALID_INPUT;
    }
    int length = (int)strlen(value);
    if (length == 0 || processN(currentLine, length, nPointer) == INVALID_INPUT ||
        processN(argv[2], (int)strlen(argv[2]), uPointer) == INVALID_INPUT ||
        processN(argv[3], (int)strlen(argv[3]), vPointer) == INVALID_INPUT ||
        *uPointer >= *nPointer || *vPointer >= *nPointer)
    {
        return INVALID_INPUT;
    }
    if (newTree(*nPointer, treeP) == BAD_MEMORY_ALLOCATION)
    {
        return BAD_MEMORY_ALLOCATION;
    }
    while (fgets(currentLine, MAX_LINE_LEN, fileP))
    {
        count_lines++;
        if (count_lines - 1 > *nPointer)
        {
            return INVALID_INPUT;
        }
        enum validityType processResult = processLine(&treeP->nodes[count_lines - 2], currentLine, nPointer, treeP);
        if (processResult == BAD_MEMORY_ALLOCATION)
        {
            return BAD_MEMORY_ALLOCATION;
        }
        else if (processResult == INVALID_INPUT)
        {
            return INVALID_INPUT;
        }
        else if (processResult == NOT_A_TREE)
        {
            isAtree = NOT_A_TREE;
        }
    }
    if (count_lines - 1 != *nPointer) // more vertex than declared in the first line
    {
        return INVALID_INPUT;
    }
    if (isAtree == NOT_A_TREE)
    {
        return NOT_A_TREE;
    }
    fclose(fileP);
    return VALID_INPUT;
}

/**
 * Finds the root of the tree
 * @param tree the tree to find the root in
 * @return NOT_A_TREE enum if the given graph is not a tree and VALID_INPUT enum otherwise
 */
enum validityType findRoot(Tree *tree)
{
    int noParent = 0;
    int noParentIndex = -1;
    for (int i = 0; i< tree->numOfNodes; ++i)
    {
        if (tree->nodes[i].parentKey == DEFAULT_PARENT)
        {
            noParent++;
            noParentIndex = i;
        }

    }
    if (noParent == 1)
    {
        tree->root = noParentIndex;
        return VALID_INPUT;
    }
    else
    {
        return NOT_A_TREE;
    }
}


/**
 * Prints the information about the tree
 * @param tree the tree that needs to be printed
 * @param uKey the first of the vertices given as input
 * @param vKey the second of the vertices given as input
 */
void treePrinter(Tree *tree, int uKey, int vKey)
{
    printf("Root Vertex: %d\n", tree->root);
    printf("Vertices Count: %d\n", tree->numOfNodes);
    printf("Edges Count: %d\n", tree->numOfEdges);
    printf("Length of Minimal Branch: %d\n", tree->minBranch);
    printf("Length of Maximal Branch: %d\n", tree->maxBranch);
    printf("Diameter Length: %d\n", tree->diameter);
    printf("Shortest Path Between %d and %d:", uKey, vKey);
    for (int i = tree->pathLen - 1; i >= 0; --i)
    {
        printf(" %d", tree->path[i]);
    }
    printf("\n");
}

/**
 * The main function that runs that program. Closes the program if the number of the given arguments given as input
 * is incorrect and if there is a problem with the input or the given graph is not a tree.
 * Prints the information about the tree that was created in the program.
 * @param argc number of given arguments
 * @param argv the given arguments as an input
 * @return 1 if the program failed and 0 otherwise
 */
int main(int argc, char* argv[])
{
    if (argc-1 != NUM_ARGS) // entered wrong amount of arguments
    {
        fprintf(stderr, WRONG_NUM_OF_ARGS_MSG);
        return EXIT_FAILURE;
    }
    FILE* fp;
    fp = fopen(argv[1], "r");
    int u, v, n;
    Tree myTree;
    initiateTree(&myTree);
    enum validityType processResult = inputValidityCheck(argv, &n, &u, &v, &myTree, fp);
    if (exitPro(processResult, &myTree) == EXIT_FAILURE)
    {
        fclose(fp);
        return EXIT_FAILURE;
    }
    enum validityType findRootResult = findRoot(&myTree);
    if (isTheGraphTree(&myTree) == NOT_A_TREE)
    {
        exitPro(NOT_A_TREE, &myTree);
        return EXIT_FAILURE;
    }
    if (exitPro(findRootResult, &myTree) == EXIT_FAILURE)
    {
        fclose(fp);
        return EXIT_FAILURE;
    }
    diameterAndPath(&myTree, &myTree.nodes[u], &myTree.nodes[v]);
    treePrinter(&myTree, u, v);
    freeTree(&myTree);
    return EXIT_SUCCESS;
}

#include <stdlib.h>
#include <stdio.h>
#include "RBTree.h"
void case3(Node *newNode, RBTree *tree);
void case4(Node *newNode, RBTree* tree);

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
	RBTree *newTree = (RBTree*)malloc(sizeof(RBTree));
    newTree->root = NULL;
    newTree->compFunc = compFunc;
    newTree->freeFunc = freeFunc;
    newTree->size = 0;
	return newTree;
}

/**
 * a function that returns the given node parent.
 * @param node: the node.
 * @return the node's parent
 */
Node* findNodeParent(Node *node)
{
    if (node->parent == NULL)
    {
        return NULL;
    }
    else
    {
        return node->parent;
    }
}

/**
 * a function that returns the given node's grandParent.
 * @param node: the node.
 * @return the node's grandpa
 */
Node* findGrandParent(Node *node)
{
    return findNodeParent(node->parent);
}

/**
 * a function that returns the given node's uncle.
 * @param node: the node.
 * @return the node's uncle
 */
Node* findNodeUncle(Node *node)
{
    int isRightNode = 0;
    if (findNodeParent(node) != NULL && findGrandParent(node) != NULL)
    {
        if (node->parent == findGrandParent(node)->left)
        {
            isRightNode = 1;
        }
        else
        {
            isRightNode = 0;
        }
    }
    else
    {
        return NULL;
    }
    if(isRightNode)
    {
        return findGrandParent(node)->right;
    }
    return findGrandParent(node)->left;
}

/**
 * a function that initiate a left rotate on a given tree, from the given node.
 * @param node the node to do the rotation from.
 * @param tree the tree to make the rotation on.
 */
void rotateLeft(Node *node, RBTree* tree)
{
    Node *temp = node->right;
    if(temp == NULL)
    {
        return;
    }
    Node *p = findNodeParent(node);
    node->right = temp->left;
    temp->left = node;
    temp->parent = node->parent;
    node->parent = temp;

    if(node->right != NULL)
    {
        node->right->parent = node;
    }
    if(p != NULL)
    {
        if(node == p->left)
        {
            p->left = temp;
        }
        else if(node == p->right)
        {
            p->right = temp;
        }
    }
    else
    {
        tree->root = temp;
    }
    temp->parent = p;
}

/**
 * a function that initiate a right rotate on a given tree, from the given node.
 * @param node the node to do the rotation from.
 * @param tree the tree to make the rotation on.
 */
void rotateRight(Node *node)
{
    Node *temp = node->left;
    Node *p = findNodeParent(node);
    if(temp == NULL)
    {
        return;
    }

    node->left = temp->right;
    temp->right = node;
    node->parent = temp;
    if(node->left != NULL)
    {
        node->left->parent = node;
    }

    if(p != NULL)
    {
        if(node == p->left)
        {
            p->left = temp;
        }
        else if(node == p->right)
        {
            p->right = temp;
        }
    }
    temp->parent = p;
}

/**
 * a recursive function that finds if the given data is already in the given tree.
 * @param tree the tree to search in.
 * @param node a node to start from.
 * @param data the data to compare.
 * @return 0 if the item is not in the tree, other if it is.
 */
static int findDataInNode(RBTree *tree , Node *node, void *data)
{
	if(node == NULL)
	{
		return 0;
	}
	if(tree->compFunc(node->data, data) == 0)
	{
		return 1;
	}
	if(tree->compFunc(node->data, data) < 0)
	{
		return findDataInNode(tree, node->right, data);
	}
	else
	{
		return findDataInNode(tree, node->left, data);
	}
}

/**
 * check whether the tree contains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int containsRBTree(RBTree *tree, void *data)
{
	if(tree->root == NULL)
	{
		return 0;
	}
	if(tree->compFunc(tree->root->data, data) == 0)
	{
		return 1;
	}
	return findDataInNode(tree, tree->root, data);
}

/**
 * a function that free's all the given tree's roots.
 * @param node a pointer to pointer for the tree node.
 * @param tree the tree to free the nodes in.
 * @return
 */
void freeNodes(Node **node, RBTree *tree)
{
    if(*node == NULL)
    {
        return;
    }
    if ((*node)->left != NULL)
    {
        freeNodes(&(*node)->left, tree);
    }
    if ((*node)->right != NULL)
    {
        freeNodes(&(*node)->right, tree);
    }

    tree->freeFunc((*node)->data);
    free(*node);
    *node = NULL;
}

/**
 * free all memory of the data structure.
 * @param tree: the tree to free.
 */
void freeRBTree(RBTree *tree)
{
	freeNodes(&tree->root, tree);
	tree->root = NULL;
	free(tree);
}

/**
 * the function that is incharge of fixing the tree, according to the instructions.
 * @param newNode the newNode we wish to fix in the tree.
 * @param tree the tree that we fixes.
 */
void fixTree (Node *newNode, RBTree *tree)
{
    if (newNode->parent == NULL)
    {
        newNode->color = BLACK;
        return;
    }
    else if (newNode->parent->color == BLACK)
    {
        return;
    }
    else if (findNodeUncle(newNode) != NULL && findNodeUncle(newNode)->color == RED)
    {
        case3(newNode, tree);
    }
    else if((findNodeUncle(newNode) == NULL || findNodeUncle(newNode)->color == BLACK) &&
            findGrandParent(newNode) != NULL)
    {
        case4(newNode, tree);
    }
}

/**
 * the third case according to the exercise instructions.
 * @param newNode the newNode we wish to fix in the tree.
 * @param tree the tree that we fixes.
 */
void case3(Node *newNode, RBTree *tree)
{
    newNode->parent->color = BLACK;
    findNodeUncle(newNode)->color = BLACK;
    newNode->parent->parent->color = RED;
    fixTree(findGrandParent(newNode), tree);
}

/**
 * the fourth case according to the exercise instructions.
 * @param newNode the newNode we wish to fix in the tree.
 * @param tree the tree that we fixes.
 */
void case4(Node *newNode, RBTree *tree)
{
    if(newNode == findNodeParent(newNode)->left && newNode->parent == findGrandParent(newNode)->right)
    {
        rotateRight(findNodeParent(newNode));
        newNode = newNode->right;
    }
    else if(newNode == findNodeParent(newNode)->right && newNode->parent == findGrandParent(newNode)->left)
    {
        rotateLeft(findNodeParent(newNode), tree);
        newNode = newNode ->left;
    }
    Node *parent = findNodeParent(newNode);
    Node *grandparent = findNodeParent(parent);
    if(newNode == parent->left)
    {
        rotateRight(grandparent);
    }
    else
    {
        rotateLeft(grandparent, tree);
    }
    parent->color = BLACK;
    grandparent->color = RED;
}

/**
 * a function that updates the given tree, from the given node.
 * @param newNode the newNode we wish to fix in the tree.
 * @param tree the tree that we fixes.
 */
void updateRoot(Node *newNode, RBTree *tree)
{
    while(newNode->parent != NULL)
    {
        newNode = newNode->parent;
    }
    tree->root = newNode;
}

void initNode(void *data, Node *newNode)
{
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->color = RED;
    newNode->data = data;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
void addToTree(RBTree *tree, Node *root , Node *node)
{
    if (root == NULL || tree->compFunc(node->data, root->data) >= 0)
    {
        if (root != NULL)
        {
            if (root->right != NULL)
            {
                addToTree(tree, root->right, node);
                return;
            }
            else
            {
                root->right = node;
            }
        }
    }
    else
    {
        if (root->left != NULL)
        {
            addToTree(tree, root->left, node);
            return;
        }
        else
        {
            root->left = node;
        }
    }
    node->parent = root;
}
/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int addToRBTree(RBTree *tree, void *data)
{
    if (data != NULL && tree != NULL)
    {
        if (containsRBTree(tree, data))
        {
            return 0;
        }
        Node *newNode = (Node *) malloc(sizeof(Node));
        if (newNode == NULL)
        {
            return 0;
        }
        initNode(data, newNode);
        addToTree(tree, tree->root, newNode);
        fixTree(newNode, tree);
        updateRoot(newNode, tree);
        tree->size++;
        return 1;
    }
    return 0;
}

/**
 * a simple function that finds the leaf in the tree.
 * @param node the node to start the leaf search from.
 * @return the leaf.
 */
Node* findLeaf(Node *node)
{
	while(node->left != NULL)
	{
		node = node->left;
	}
	return node;
}

/**
 * a function that returns the tree nodes in the right order.
 * @param node a node.
 * @return the next node in the order.
 */
Node *returnNodesInOrder(Node *node)
{
    if (node->right == NULL)
    {
        Node *p = node->parent;
        while (p != NULL && node == p->right)
        {
            node = p;
            p = p->parent;
        }
        return p;
    }
    else
    {
        return findLeaf(node->right);
    }
}


/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(RBTree *tree, forEachFunc func, void *args)
{
    if(tree->size == 0)
    {
        return 0;
    }
	Node *curr = findLeaf(tree->root);
	for(int i = 0; i < tree->size; i++)
	{
		func(curr->data, args);
        curr = returnNodesInOrder(curr);
	}
	return 1;
}

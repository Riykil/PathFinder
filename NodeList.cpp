
#include "NodeList.h"
#include <iostream>

/*
 * Setting all the nodes to a null pointer and starting the length variable to keep track of the current size of the NodeList;
 */
NodeList::NodeList()
{
    maxLength = NODE_LIST_ARRAY_MAX_SIZE;
    nodes = new Node *[maxLength];
    for (int i = 0; i < maxLength; i++)
    {
        nodes[i] = nullptr;
    }
    length = 0;
}
NodeList::NodeList(int max)
{
    maxLength = max;
    nodes = new Node *[maxLength];
    for (int i = 0; i < maxLength; i++)
    {
        nodes[i] = nullptr;
    }
    length = 0;
}

/*
 *  Creates deep copy of the NodeList
 */
NodeList::NodeList(NodeList &other)
{

    maxLength = other.maxLength;
    nodes = new Node *[maxLength];
    for (int i = 0; i < maxLength; i++)
    {
        nodes[i] = nullptr;
    }
    length = other.length;
    for (int i = 0; i < length; i++)
    {
        nodes[i] = new Node(*other.nodes[i]);
    }
}

/*
 * Deletes the list and sets all the nodes back to nullptr;
 */
NodeList::~NodeList()
{
    clear();
    delete[] nodes;
    // for (int i = 0; i < maxLength; i++)
    // {
    //     delete nodes[i];
    //     nodes[i] = nullptr;
    // }
    // delete[] nodes;
}

int NodeList::getLength()
{
    return length;
}

/*
 * Creates temp resultNodePtr and sets to nullptr then checks if the given index is valid or not.
 * If invalid returns the nullptr otherwise returns the node at given index.
 */
NodePtr NodeList::get(int i)
{
    NodePtr resultNodePtr = nullptr;
    if (i >= 0 && i < length)
    {
        resultNodePtr = nodes[i];
    }
    return resultNodePtr;
}

void NodeList::addBack(NodePtr newNode)
{
    NodePtr copyNewNode = new Node(*newNode);
    if (length < maxLength - 1)
    {
        nodes[length] = copyNewNode;
        length++;
    }
    /* If the maxLength is not enough to store more nodes create a new list and copy contents of old and then clear old list */
    else
    {
        Node **newNodes = new Node *[maxLength + 10];
        for (int i = 0; i < maxLength + 10; i++)
        {
            newNodes[i] = nullptr;
        }

        for (int i = 0; i < length; i++)
        {
            newNodes[i] = nodes[i];
        }
        clear();

        nodes = newNodes;

        nodes[length] = copyNewNode;
        length++;
    }
}

/*
 * Loops through the list and checks if a node within the list is the same as the given node.
 */
bool NodeList::containsNode(NodePtr node)
{
    for (int i = 0; i < length; i++)
    {
        if (nodes[i]->compareNodes(*node))
        {
            return true;
        }
    }
    return false;
}
/* Checks for the node and returns the node if present otherwise return nullptr */
NodePtr NodeList::containsGet(NodePtr node)
{
    for (int i = 0; i < length; i++)
    {
        if (nodes[i]->compareNodes(*node))
        {
            return nodes[i];
        }
    }
    return nullptr;
}

/*
 * Clears list by setting all pointers to null and deleting the nodes.
 */
void NodeList::clear()
{
    for (int i = 0; i < maxLength; i++)
    {
        delete nodes[i];
        nodes[i] = nullptr;
    }
    length = 0;
}

/*
 * Comparing the two lists. If the lengths are the same, compare the nodes.
 * Starting with assumaption that list will be the same so issame starts as true.
 * Checks lengths: if same then check nodes otherwise change to issame to false.
 */
bool NodeList::compareLists(NodeList &other)
{
    bool issame = true;
    if (length == other.getLength())
    {
        for (int i = 0; i < length; i++)
        {
            if (!(nodes[i]->compareNodes(*other.get(i))))
            {
                issame = false;
            }
        }
    }
    else
    {
        issame = false;
    }
    return issame;
}

/* Getters to help check current length to the maxsize  */
int NodeList::getMaxSize()
{
    return maxLength;
}

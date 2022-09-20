
#include "PathPlanner.h"
#include "milestone4.h"
#include <iostream>

/*
 * Creating a copy of the env that this class can use since the other methods ain't allowed to read in env
 * Setting up nodesExplored.
 */
PathPlanner::PathPlanner(Env env, int row, int col)
{
    rows = row;
    cols = col;
    maze = make_env(rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            maze[i][j] = env[i][j];
        }
    }
    int maxSize = rows * cols;
    nodeExplored = new NodeList(maxSize);
    openList = new NodeList(maxSize);
}

PathPlanner::~PathPlanner()
{
    if (nodeExplored != nullptr)
    {
        delete nodeExplored;
        nodeExplored = nullptr;
    }

    if (openList != nullptr)
    {
        delete openList;
        openList = nullptr;
    }

    if (rows >= 0 && cols >= 0)
    {
        for (int i = 0; i != rows; ++i)
        {
            delete[] maze[i];
        }
        delete[] maze;
    }
}

/*
 * Adding the starter node to the nodes list.
 */
void PathPlanner::initialPosition(int row, int col)
{
    NodePtr startNode = new Node(row, col, 0);
    openList->addBack(startNode);
    delete startNode;
}

NodeList *PathPlanner::getReachableNodes()
{
    /* Clearing the closed list to make sure its compeletly fresh if it's not nullptr. */

    nodeExplored->clear();
    openList->clear();

    /* Looping through to find the starting position. */
    for (int row = 0; row < rows; row++)
    {

        for (int col = 0; col < cols; col++)
        {
            if (maze[row][col] == SYMBOL_START)
            {
                // Setting the starting position.
                initialPosition(row, col);
            }
        }
    }
    if (openList->getLength() == 0)
    {
        throw std::runtime_error("Start not found.");
    }

    /* Setting up two arrays to help look around the nodes/maze in order of up,right,down,left. */
    int nextRows[4] = {-1, 0, +1, 0};
    int nextCols[4] = {0, +1, 0, -1};

    /* Setting up currNode and pointing to Start position. */
    NodePtr currNode = nullptr;
    currNode = openList->get(0);
    /* Loop untill openList and nodeExplored(ClosedList) are the same. there is no p in O but not in C as per algorithm provided. */
    do
    {
        for (int i = 0; i < 4; i++)
        {
            /* Setting up the temp node that lets us look up,right,down,left. */
            int tempRow = currNode->getRow() + nextRows[i];
            int tempCol = currNode->getCol() + nextCols[i];
            NodePtr tempNode = new Node(tempRow, tempCol, currNode->getDistanceToS() + 1);
            /* Checking if the open node has the temp/next node and if it is a valid reachable location */
            if (!openList->containsNode(tempNode))
            {
                if (maze[tempRow][tempCol] == SYMBOL_GOAL || maze[tempRow][tempCol] == SYMBOL_EMPTY)
                {
                    openList->addBack(tempNode);
                }
            }
            /* Deleting the tempNode so we don't have tempNodes floating around */
            delete tempNode;
        }
        nodeExplored->addBack(currNode);
        currNode = getNextNode(openList, nodeExplored);

    } while (!openList->compareLists(*nodeExplored));

    NodeList *reachablePositions = new NodeList(*nodeExplored);
    return reachablePositions;
}

NodeList *PathPlanner::getPath()
{
    /* Creating the Backwards Solution  nodeList as the path is worked out backwards */
    NodeList *BWsolution = new NodeList(nodeExplored->getMaxSize());

    /* Setting up two arrays to help look around the nodes/maze in order of up,right,down,left since we will be using a simliar alg as getReachablePositions */
    int nextRows[4] = {-1, 0, +1, 0};
    int nextCols[4] = {0, +1, 0, -1};

    /* Creating the pointers to the start and goal nodes. */
    NodePtr startNode = nullptr;
    NodePtr goalNode = nullptr;

    /* Setting up the above nodes */
    setGoalStartNodes(&startNode, &goalNode);

    NodePtr currNode = goalNode;

    bool nodeFound = false;

    while (!currNode->compareNodes(*startNode))
    {
        nodeFound = false;
        BWsolution->addBack(currNode);
        for (int i = 0; i < 4; i++)
        {
            if (!nodeFound)
            {
                int tempRow = currNode->getRow() + nextRows[i];
                int tempCol = currNode->getCol() + nextCols[i];
                NodePtr tempNode = new Node(tempRow, tempCol, 0);
                NodePtr tempPtr = nodeExplored->containsGet(tempNode);
                if (tempPtr != nullptr)
                {
                    if (tempPtr->getDistanceToS() == currNode->getDistanceToS() - 1)
                    {
                        currNode = tempPtr;
                        nodeFound = true;
                    }
                }
                delete tempNode;
                tempNode = nullptr;
                tempPtr = nullptr;
            }
        }
    }
    BWsolution->addBack(startNode);

    NodeList *forwardSolution = new NodeList(BWsolution->getMaxSize());

    for (int i = BWsolution->getLength() - 1; i >= 0; i--)
    {
        forwardSolution->addBack(BWsolution->get(i));
    }

    NodeList *path = new NodeList(*forwardSolution);

    delete forwardSolution;
    delete BWsolution;
    delete startNode;
    delete goalNode;

    return path;
}

/*
 * Gets the next node.
 * Checks if node is not in Closed list.
 */
NodePtr PathPlanner::getNextNode(NodeList *openList, NodeList *nodeExplored)
{
    NodePtr retnode = nullptr;
    bool nodeFound = false;
    for (int i = 0; i < openList->getLength(); i++)
    {
        NodePtr currNode = openList->get(i);
        /* Checks if the node is in the closed list and if the loop has already found a node that fits */
        if (!nodeExplored->containsNode(currNode) && !nodeFound)
        {
            retnode = currNode;
            nodeFound = true;
        }
    }
    return retnode;
}

/*
 * Function to help find the starting and goal nodes within the nodeExplored nodeList.
 * Will set the provided nodes to point to the both nodes within nodeExplored.
 */
void PathPlanner::setGoalStartNodes(NodePtr *startNode, NodePtr *goalNode)
{
    /* Start node will always be the start of the closed list so we set it here */
    *startNode = new Node(nodeExplored->get(0)->getRow(), nodeExplored->get(0)->getCol(), nodeExplored->get(0)->getDistanceToS());

    /* Creating temp node that will be used in the loop */
    NodePtr tmpNode = nullptr;

    /* Need to loop through the closed list to check for the goal node as it can be found before the last node with the current verison of getReachablePositions */

    for (int i = 0; i < nodeExplored->getLength(); i++)
    {
        tmpNode = nodeExplored->get(i);
        int tmpRow = tmpNode->getRow();
        int tmpCol = tmpNode->getCol();
        if (maze[tmpRow][tmpCol] == SYMBOL_GOAL)
        {
            *goalNode = new Node(tmpRow, tmpCol, nodeExplored->get(i)->getDistanceToS());
        }
    }

    if (*goalNode == nullptr)
    {
        throw std::runtime_error("Goal not reachable.");
    }
}

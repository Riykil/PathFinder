#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "Types.h"
#include "Node.h"
#include "NodeList.h"
#include "PathPlanner.h"
#include "milestone4.h"
/*
 * For this program I tried to be as systematic as possible, breaking the program into smaller parts and making helper functions to help do the require logic checks and the like.
 * I also tried to make sure all the new helper functions were annotated as much as possible so that the logic behind them was as clear as I think they needed to be.
 * As I progressed with the project, the first issue to pop up were Segmentation faults that were caused by mishandling pointers and incorrectly using them, I needed to reference the C++ pointers documention to fix these issues.
 * The second major issue that appeared was memory leaks. I had misplaced my deletion calls in some of the methods placing them in areas that would not be called at the proper time.
 *
 */

// Helper test functions
void testNode();
void testNodeList();

// Read a environment from standard input.
void readEnvStdin(Env env);
void readEnvStdin(Env &env, int &rows, int &cols);

// Print out a Environment to standard output with path.
// To be implemented for Milestone 3
void printPath(Env env, NodeList *solution);
void printPath(Env env, NodeList *solution, const int &rows, const int &cols);

/*
 * Used to test if the nodes selected by getReachablePositions are correct. Changes all positions in list to 'R'
 *
 */
void printReachablePositions(Env env, NodeList *reachablePositions);
void printReachablePositions(Env env, NodeList *reachablePositions, const int rows, const int cols);

int main(int argc, char **argv)
{
    // THESE ARE SOME EXAMPLE FUNCTIONS TO HELP TEST YOUR CODE
    // AS YOU WORK ON MILESTONE 2. YOU CAN UPDATE THEM YOURSELF
    // AS YOU GO ALONG.
    // COMMENT THESE OUT BEFORE YOU SUBMIT!!!
    // std::cout << "TESTING - COMMENT THE OUT TESTING BEFORE YOU SUBMIT!!!" << std::endl;
    // testNode();
    // testNodeList();
    // std::cout << "DONE TESTING" << std::endl
    //           << std::endl;

    // Load Environment
    int rows = 1;
    int cols = 0;
    int &rowsRef = rows;
    int &colsRef = cols;
    Env env;
    readEnvStdin(env, rows, cols);

    // Solve using forwardSearch
    // THIS WILL ONLY WORK IF YOU'VE FINISHED MILESTONE 2
    /* Used for testing read in */

    PathPlanner *pathplanner = new PathPlanner(env, rows, cols);

    NodeList *reachablePositions = nullptr;
    reachablePositions = pathplanner->getReachableNodes();

    // printReachablePositions(env, reachablePositions, rows, cols);

    // // Get the path
    // // THIS WILL ONLY WORK IF YOU'VE FINISHED MILESTONE 3
    NodeList *solution = pathplanner->getPath();

    // // print the path
    printPath(env, solution, rows, cols);

    delete pathplanner;
    delete reachablePositions;
    delete solution;
    delete_env(env, rows, cols);
}

/*
 * Reads in a variable amount of inputs depending on when the EOF symbol is parsed in.
 */
void readEnvStdin(Env &env, int &rows, int &cols)
{
    char chRead;
    /* Temp string to hold the env and bool endline that will be triggered when the end line is found */
    std::string tmpString;

    bool colsEnd = false;
    while (!std::cin.eof())
    {
        if (std::cin.good())
        {

            std::cin.get(chRead);

            if (!(chRead == SYMBOL_EMPTY || chRead == SYMBOL_GOAL || chRead == SYMBOL_START || chRead == SYMBOL_WALL || isspace(chRead) || chRead == EOF))
            {
                throw std::runtime_error("Invalid Maze - Invalid Symbols");
            }
            /* If the character read in is not a newline keep adding to tmpString otherwise increase row count and don't add to tmpString */
            else
            {
                if (chRead != '\n' & chRead != EOF)
                {
                    if (!(colsEnd))
                    {
                        cols++;
                    }
                    tmpString += chRead;
                }
                else
                {
                    rows++;
                    colsEnd = true;
                }
            }
        }
    }

    /* Checks if the Maze is of proper size */
    if ((cols < 3) || ((rows < 3)))
    {
        throw std::runtime_error("Invalid Maze - Too Small");
    }
    env = make_env(rows, cols);
    int index = 0;
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; ++col)
        {
            if (index < tmpString.size())
            {

                env[row][col] = tmpString[index];
                index++;
            }
        }
    }
}
/*
 * Reads in ENV_DIM*ENV_DIM inputs, checks if they are one of the four symbols that are valid for the maze,
 * if invalid, throw error.
 */
void readEnvStdin(Env env)
{
    char chRead;
    // bool invalidSymbols = false;
    env = make_env(ENV_DIM, ENV_DIM);
    for (int row = 0; row < ENV_DIM; row++)
    {
        for (int col = 0; col < ENV_DIM; col++)
        {
            if (std::cin.good())
            {
                std::cin >> chRead;
                /* Checking if the Maze is valid. */
                if (!(chRead == SYMBOL_EMPTY || chRead == SYMBOL_GOAL || chRead == SYMBOL_START || chRead == SYMBOL_WALL))
                {
                    // invalidSymbols = true;
                    throw std::runtime_error("Invalid Maze");
                }
                env[row][col] = chRead;
            }
        }
    }
}

/*
 * Changes the locations within the solution list to appropriate symbols.
 * Then prints out the changed env with the solution direcitons.
 */
void printPath(Env env, NodeList *solution)
{
    for (int i = 1; i < solution->getLength() - 1; i++)
    {
        if (solution->get(i)->getCol() < solution->get(i + 1)->getCol())
        {
            env[solution->get(i)->getRow()][solution->get(i)->getCol()] = '>';
        }
        else if (solution->get(i)->getCol() > solution->get(i + 1)->getCol())
        {
            env[solution->get(i)->getRow()][solution->get(i)->getCol()] = '<';
        }
        else if (solution->get(i)->getRow() > solution->get(i + 1)->getRow())
        {
            env[solution->get(i)->getRow()][solution->get(i)->getCol()] = '^';
        }
        else if (solution->get(i)->getRow() < solution->get(i + 1)->getRow())
        {
            env[solution->get(i)->getRow()][solution->get(i)->getCol()] = 'v';
        }
    }
    for (int row = 0; row < ENV_DIM; row++)
    {
        for (int col = 0; col < ENV_DIM; col++)
        {
            std::cout << env[row][col];
        }
        /* Check to see if newline is required */
        if (!(row == ENV_DIM - 1))
        {
            std::cout << std::endl;
        }
    }
}
void printPath(Env env, NodeList *solution, const int &rows, const int &cols)
{
    for (int i = 1; i < solution->getLength() - 1; i++)
    {
        if (solution->get(i)->getCol() < solution->get(i + 1)->getCol())
        {
            env[solution->get(i)->getRow()][solution->get(i)->getCol()] = '>';
        }
        else if (solution->get(i)->getCol() > solution->get(i + 1)->getCol())
        {
            env[solution->get(i)->getRow()][solution->get(i)->getCol()] = '<';
        }
        else if (solution->get(i)->getRow() > solution->get(i + 1)->getRow())
        {
            env[solution->get(i)->getRow()][solution->get(i)->getCol()] = '^';
        }
        else if (solution->get(i)->getRow() < solution->get(i + 1)->getRow())
        {
            env[solution->get(i)->getRow()][solution->get(i)->getCol()] = 'v';
        }
    }
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            std::cout << env[row][col];
        }
        /* Check to see if newline is required */
        if (row != rows - 1)
        {
            std::cout << std::endl;
        }
    }
}

/*
 * Gets the reachablePositions from the getReachablePoisition function and changes all the reachable positions to R. Including S and G.
 */
void printReachablePositions(Env env, NodeList *reachablePositions)
{
    /* Creating an reachableEnv so that the starting env does not get changed */
    Env reachableEnv = make_env(ENV_DIM, ENV_DIM);
    for (int i = 0; i < ENV_DIM; i++)
    {
        for (int j = 0; j < ENV_DIM; j++)
        {
            reachableEnv[i][j] = env[i][j];
        }
    }
    for (int i = 0; i < reachablePositions->getLength(); i++)
    {

        reachableEnv[reachablePositions->get(i)->getRow()][reachablePositions->get(i)->getCol()] = 'R';
    }
    std::cout << std::endl;
    for (int row = 0; row < ENV_DIM; row++)
    {
        for (int col = 0; col < ENV_DIM; col++)
        {
            std::cout << reachableEnv[row][col];
        }
        if (row == ENV_DIM - 1)
        {
        }
        else
        {
            std::cout << std::endl;
        }
    }
    delete_env(reachableEnv, ENV_DIM, ENV_DIM);
}
void printReachablePositions(Env env, NodeList *reachablePositions, const int rows, const int cols)
{
    /* Creating an reachableEnv so that the starting env does not get changed */
    Env reachableEnv = make_env(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            reachableEnv[i][j] = env[i][j];
        }
    }
    for (int i = 0; i < reachablePositions->getLength(); i++)
    {

        reachableEnv[reachablePositions->get(i)->getRow()][reachablePositions->get(i)->getCol()] = 'R';
    }
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            std::cout << reachableEnv[row][col];
        }
        if (row == rows - 1)
        {
        }
        else
        {
            std::cout << std::endl;
        }
    }
    delete_env(reachableEnv, rows, cols);
}

void testNode()
{
    std::cout << "TESTING Node" << std::endl;

    // Make a Node and print out the contents
    Node *node = new Node(1, 1, 2);
    std::cout << node->getRow() << ",";
    std::cout << node->getCol() << ",";
    std::cout << node->getDistanceToS() << std::endl;
    delete node;

    // Change Node and print again
    node = new Node(4, 2, 3);
    std::cout << node->getRow() << ",";
    std::cout << node->getCol() << ",";
    std::cout << node->getDistanceToS() << std::endl;
    delete node;
}

void testNodeList()
{
    std::cout << "TESTING NodeList" << std::endl;

    // Make a simple NodeList, should be empty size
    NodeList *nodeList = new NodeList();
    std::cout << "NodeList size: " << nodeList->getLength() << std::endl;

    // Add a Node to the NodeList, print size
    Node *b1 = new Node(1, 1, 1);
    nodeList->addBack(b1);
    std::cout << "NodeList size: " << nodeList->getLength() << std::endl;

    // Add second Nodetest
    Node *b2 = new Node(0, 0, 1);
    nodeList->addBack(b2);
    std::cout << "NodeList size: " << nodeList->getLength() << std::endl;

    // Test Get-ith - should be 0,0,1
    Node *getB = nodeList->get(1);
    std::cout << getB->getRow() << ",";
    std::cout << getB->getCol() << ",";
    std::cout << getB->getDistanceToS() << std::endl;

    // Print out the NodeList
    std::cout << "Printing out out NodeList" << std::endl;
    // Testing Clear function
    nodeList->clear();
    // Adding more nodes to see print out better
    for (int i = 0; i < 10; i++)
    {
        Node *tmp = new Node(i, i, i);
        nodeList->addBack(tmp);
    }

    for (int i = 0; i < nodeList->getLength(); i++)
    {
        std::cout << nodeList->get(i)->getRow() << " " << nodeList->get(i)->getCol() << " " << nodeList->get(i)->getDistanceToS() << " " << std::endl;
    }
    delete nodeList;
}

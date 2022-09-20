
#include "Node.h"
#include <iostream>
Node::Node(int row, int col, int distanceToS)
{

   this->row = row;
   this->col = col;
   this->distanceToS = distanceToS;
}

Node::~Node()
{
}

int Node::getRow()
{

   return row;
}

int Node::getCol()
{

   return col;
}

int Node::getDistanceToS()
{

   return distanceToS;
}

void Node::setDistanceToS(int distanceToS)
{
   this->distanceToS = distanceToS;
}

/*
 * Compares the two nodes.
 * Returns true if the nodes are the same other wise returns false
 */
bool Node::compareNodes(Node &other)
{
   return (this->row == other.getRow()) && (this->col == other.getCol());
}

void Node::printNode()
{
   std::cout << "Node: " << this->getRow() << " " << this->getCol() << " " << this->getDistanceToS() << std::endl;
}
Node::Node(Node &other) : row(other.row), col(other.col),
                          distanceToS(other.distanceToS)
{
}

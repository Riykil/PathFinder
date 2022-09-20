
#ifndef COSC_ASS_ONE_PATH_PLANNING
#define COSC_ASS_ONE_PATH_PLANNING

#include "Node.h"
#include "NodeList.h"
#include "Types.h"

class PathPlanner
{
public:
   /*                                           */
   /* DO NOT MOFIFY ANY CODE IN THIS SECTION    */
   /*                                           */

   // Initialise with a given Env of size (rows,cols)
   PathPlanner(Env env, int rows, int cols);

   // Clean-up
   ~PathPlanner();

   // Set The initial position
   void initialPosition(int row, int col);

   // Method for Milestone 2
   // Return a DEEP COPY of the NodeList of all node's
   //    that the robot can reach with distances
   NodeList *getReachableNodes();

   // Method for Milestone 3
   // Get the path from the starting position to the given goal co-ordinate
   //    The path should be a DEEP COPY
   NodeList *getPath();

   /*                                           */
   /* YOU MAY ADD YOUR MODIFICATIONS HERE       */
   /*                                           */
   NodePtr getNextNode(NodeList *openList, NodeList *nodeExplored);
   void setGoalStartNodes(NodePtr *startNode, NodePtr *goalNode);

private:
   Env maze;

   /* 'Closed list', named nodeExplored due to doc requirements. */
   NodeList *nodeExplored;
   /* Openlist*/
   NodeList *openList;
   int rows;
   int cols;
};

#endif // COSC_ASS_ONE_PATH_PLANNING
#include <iostream>

#include "Environment.h"
#include "GraphGenerator.h"
#include "PathPlanner.h"

int main() {

    // Note: Use const string &s constructor for CLion project
    Environment env("./src/example_input.txt");
    // Note: Use default constructor for CLion project
//    Environment env;
//    env.printConfig();
    GraphGenerator graph{env};
    graph.generateGraph(50, 1.0);
    PathPlanner plan(graph);
    Node* startNode = plan.getStartNode();
    Node* goalNode = plan.getGoalNode();
    deque<pair<double,double>> waypoints = plan.findPathWaypoints(startNode, goalNode);
    plan.printWaypoints(waypoints);
    return 0;
}

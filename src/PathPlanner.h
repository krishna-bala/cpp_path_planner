//
// Created by krish on 5/9/21.
//

#ifndef APL_PATH_PLANNING_PATHPLANNER_H
#define APL_PATH_PLANNING_PATHPLANNER_H
#include "Environment.h"
#include "GraphGenerator.h"

#include <list>
#include <cmath>
#include <unordered_map>
#include <map>
#include <queue>
#include <algorithm>

class PathPlanner {
public:
    PathPlanner(GraphGenerator graph);
    deque<pair<double,double>> findPathWaypoints(Node* start, Node* end);
    void printWaypoints(deque<pair<double,double>> path);
    Node* getStartNode();
    Node* getGoalNode();

private:
    GraphGenerator graph;

};


#endif //APL_PATH_PLANNING_PATHPLANNER_H

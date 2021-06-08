//
// Created by krish on 5/9/21.
//

#include "PathPlanner.h"

PathPlanner::PathPlanner(GraphGenerator graph) : graph(graph) {
}


deque<pair<double, double>> PathPlanner::findPathWaypoints(Node* start, Node* end) {

    deque<pair<double,double>> path;
    set<int> closed;
    vector<Node*> frontier;

    start->cost = 0;
    frontier.push_back(start);
    make_heap(frontier.begin(), frontier.end(), greater1());

    // While there are still nodes to explore
    while(!frontier.empty()){

        // Get front node and remove from frontier
        Node* currNode = frontier.front();
        Node* head = frontier.front();

        pop_heap(frontier.begin(), frontier.end(), greater1());
        frontier.pop_back();

        // Save solution if we reach goal
        if (currNode->id == end->id){
            pair<double,double> state{currNode->x, currNode->y};
            path.push_front(state);
            currNode = currNode->parent;
            while (currNode != nullptr){
                state.first = currNode->x;
                state.second = currNode->y;
                path.push_front(state);
                currNode = currNode->parent;
            }
            break;
        }

        // Skip if we've already explored this node
        if (closed.find(currNode->id) != closed.end()) continue;

        // Mark node as explored
        closed.insert(currNode->id);

        // Iterate through all adjacent nodes to currNode
        while (currNode->next != nullptr){

            // Initialize a neighbor node ptr
            Node* neighbor = currNode->next;

            if (closed.find(neighbor->id) != closed.end()){
                // If neighbor is already in closed, advance currNode down list
                currNode = currNode->next;
                continue;
            }

            double dx, dy;
            dx = neighbor->x - currNode->x;
            dy = neighbor->y - currNode->y;

            // Cost to neighbor node
            double ncost = currNode->cost + graph.getDist(dx, dy);

            // Check if neighbor is in frontier already and if frontier neighbor has better cost
            bool better_neighbor_in_frontier = false;
            for (Node* node : frontier){
                if (node->id == neighbor->id && node->cost <= ncost){
                    better_neighbor_in_frontier = true;
                    break;
                }
            }
            // If no better neighbor in frontier
            if (!better_neighbor_in_frontier){
                // Search through adjacency list to update the linked list of nodes to the "new" node
                Node* addToFrontier = new Node{};
                for (const Node* node : graph.adj_list){
                    if (node->id == neighbor->id){
                        // Change neighbor to the frontier neighbor (so it has the correct adj nodes)
                        addToFrontier->x = node->x;
                        addToFrontier->y = node->y;
                        addToFrontier->id = node->id;
                        addToFrontier->h = node->h;
                        addToFrontier->cost = ncost;
                        addToFrontier->next = node->next;
                        addToFrontier->parent = head;
                        break;
                    }
                }
                frontier.push_back(addToFrontier);
                push_heap(frontier.begin(), frontier.end(), greater1());
            }
            currNode = currNode->next;
        }

    }
    if (path.empty()){
        throw invalid_argument("Goal Node is not reachable!");
    }

    // To visualize that all nodes in path are only connect_radius away from prev node
//    pair<double,double> curr = graph.env.getStartPos();
//    for (pair<double,double> &point : path){
//        double dx = point.first - curr.first;
//        double dy = point.second - curr.second;
//        double dist = graph.getDist(dx, dy);
//        cout << dist << endl;
//        curr = point;
//    }

    return path;
}

void PathPlanner::printWaypoints(deque<pair<double, double>> path) {
    while (!path.empty()){
        pair<double,double> node = path.front();
        cout << "(" << node.first << ", " << node.second << ")" << endl;
        path.pop_front();
    }

}

Node* PathPlanner::getStartNode() {
    pair<double,double> start = graph.env.getStartPos();
    for(Node *nodePtr : graph.adj_list){
        if(nodePtr->x == start.first && nodePtr->y == start.second){
            return nodePtr;
        }
    }
    throw invalid_argument("Start Node is not in adjacency list!");
}

Node* PathPlanner::getGoalNode() {
    pair<double,double> goal = graph.env.getEndPos();
    for(Node *nodePtr : graph.adj_list){
        if(nodePtr->x == goal.first && nodePtr->y == goal.second){
            return nodePtr;
        }
    }
    throw invalid_argument("Start Node is not in adjacency list!");
}





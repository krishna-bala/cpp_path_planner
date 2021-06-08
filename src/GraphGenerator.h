//
// Created by krish on 5/10/21.
//

#ifndef APL_PATH_PLANNING_GRAPHGENERATOR_H
#define APL_PATH_PLANNING_GRAPHGENERATOR_H

#include "Environment.h"
#include <complex>

struct Node {
    Node();
    Node(const Node &node);

    double x, y, cost, h;
    int id;
    Node* next;
    Node* parent;

};

struct greater1{
    // Compare operator for make_heap, push_heap, pop_heap
    bool operator()(const Node* a, const Node* b) const{
        return a->cost + a->h > b->cost + b->h;
    }
};

class GraphGenerator {
public:
    GraphGenerator(Environment env);
    void generateGraph(int num_nodes, double connect_radius);
    double getDist(double dx, double dy);
    // Note: Change adj_list implementation to map<int, vector<int>>.
    //       Keys represent the id for each node, values are a vector of all adjacent node id's
    vector<Node*> adj_list;
    Environment env;

private:
    static double haltonSampler(int j, int b, pair<double,double> limit);
    vector<Node> sampleNodes(int samples);
    void setLimits();

    void addNode(Node &node1, double connect_radius);
    Node initStartNode(int num_nodes);
    Node initEndNode(int num_nodes);
    bool evaluateEdge(Node node1, Node node2, double step = 0.1);
    vector<bool> evaluateStates(const vector<Node>& proposed_nodes);
    double getCostToGo(Node node);

    vector<Node> valid_nodes;
    int nodes_in_adj_list;
    vector<pair<double,double>> limits;
};


#endif //APL_PATH_PLANNING_GRAPHGENERATOR_H

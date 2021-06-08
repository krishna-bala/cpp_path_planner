//
// Created by krish on 5/10/21.
//

#include "GraphGenerator.h"

GraphGenerator::GraphGenerator(Environment env) {
    env = env;
    nodes_in_adj_list = 0;
}

void GraphGenerator::generateGraph(int num_nodes, double connect_radius) {

    int sampled = 0;
    int to_sample = 0;
    int valid = 0;
    vector<Node> proposed_nodes;
    vector<Node> good_nodes;
    vector<bool> proposed_node_validity;

    while (valid < num_nodes){
        // Num nodes sampling; continuously increases until enough valid nodes
        to_sample = sampled + (num_nodes - valid);
        proposed_nodes = sampleNodes(to_sample);
        Node startNode = initStartNode(proposed_nodes.size());
        proposed_nodes.push_back(startNode);
        Node endNode = initEndNode(proposed_nodes.size());
        proposed_nodes.push_back(endNode);
        proposed_node_validity = evaluateStates(proposed_nodes);

        // Zero the valid nodes and good nodes
        valid = 0;
        good_nodes.resize(0);
        for (size_t i = 0; i < proposed_node_validity.size(); ++i){
            if (proposed_node_validity[i]) {
                valid++;
                good_nodes.push_back(proposed_nodes[i]);
            }
        }
        sampled = to_sample;
    }

    valid_nodes = good_nodes;
    for (Node &node : good_nodes){
        addNode(node, connect_radius);
    }

}

double GraphGenerator::haltonSampler(int j, int b, pair<double, double> limit) {
    // See Halton Sequence (Wikipedia)
    double size = limit.second - limit.first;
    double r = 0;
    int i = j;
    while (i > 0){
        size = size / b;
        r += size * (i % b);
        i = i / b;
    }
    return r + limit.first;
}

vector<Node> GraphGenerator::sampleNodes(int samples) {
    // Resize number of nodes
    setLimits();

    vector<Node> proposed_nodes;
    Node* node = new Node{};
    for (int j = 0; j < samples; ++j){
        node->x = haltonSampler(j, 2, limits[0]);
        node->y = haltonSampler(j, 3, limits[1]);
        node->id = j;
        proposed_nodes.push_back(*node);
    }
    return proposed_nodes;
}

void GraphGenerator::setLimits() {

    pair<double,double> start = env.getStartPos();
    pair<double,double> end = env.getEndPos();

    pair<double,double> center;
    // First -> xc, Second -> yc
    center.first = (end.first - start.first)/2 + start.first;
    center.second = (end.second - start.second)/2 + start.second;

    double dx = end.first - start.first;
    double dy = end.second - start.second;
    pair<double,double> lim;

    // Lower and Upper Bounds for xlim
    lim.first = center.first - dx;
    lim.second = center.first + dx;
    limits.push_back(lim);

    // Lower and Upper Bounds for ylim
    lim.first = center.second - dy;
    lim.second = center.second + dy;
    limits.push_back(lim);

}

void GraphGenerator::addNode(Node &node1, double connect_radius) {

    node1.h = getCostToGo(node1);
    double dx, dy;
    double dist;

    Node* head = new Node{node1};
    Node* currNode = head;
    for (Node &node : valid_nodes){
        dx = node.x - node1.x;
        dy = node.y - node1.y;
        dist = getDist(dx, dy);
        if (dist < connect_radius && node1.id != node.id){
            if (evaluateEdge(node1, node)){
                node.h = getCostToGo(node);
                currNode->next = new Node{node};
                currNode = currNode->next;
                currNode->parent = new Node{*head};
            }
        }
    }
    adj_list.push_back(head);
    nodes_in_adj_list++;

}

Node GraphGenerator::initStartNode(int num_nodes) {
    Node startNode{};
    pair<double,double> start_pos = env.getStartPos();
    startNode.x = start_pos.first;
    startNode.y = start_pos.second;
    startNode.id = num_nodes;
    return startNode;
}

Node GraphGenerator::initEndNode(int num_nodes) {
    Node endNode{};
    pair<double,double> end_pos = env.getEndPos();
    endNode.x = end_pos.first;
    endNode.y = end_pos.second;
    endNode.id = num_nodes;
    return endNode;

}

bool GraphGenerator::evaluateEdge(Node node1, Node node2, double step) {

    double dx = node2.x - node1.x;
    double dy = node2.y - node1.y;


    double dist = getDist(dx, dy);
    int num_steps = int(dist / step);
    double theta = atan2(dy, dx);

    double step_dx = step * cos(theta);
    double step_dy = step * sin(theta);
    double x = node1.x;
    double y = node1.y;

    pair<double,double> point;
    // Starting node can't be in collision due to evaluateStates
    for (int i = 0; i < num_steps; ++i){
        x += step_dx;
        y += step_dy;
        point.first = x;
        point.second = y;
        if (env.inCollision(point)){
            return false;
        }
    }
    return true;
}

vector<bool> GraphGenerator::evaluateStates(const vector<Node>& proposed_nodes) {

    vector<bool> valid_nodes;

    for (Node node : proposed_nodes){
        pair<double,double> temp{node.x, node.y};
        if (env.inCollision(temp)){
            valid_nodes.push_back(false);
        } else{
            valid_nodes.push_back(true);
        }
    }
    return valid_nodes;
}

double GraphGenerator::getCostToGo(Node node) {
    pair<double,double> goal = env.getEndPos();
    double dx, dy;
    dx = goal.first - node.x;
    dy = goal.second - node.y;
    double dist_to_goal = getDist(dx, dy);
    return dist_to_goal;
}

double GraphGenerator::getDist(double dx, double dy) {
    complex<double> coords{dx,dy};
    double dist = sqrt(norm(coords));
    return dist;
}

Node::Node() {
    x = 0;
    y = 0;
    cost = 0;
    h = 0;
    id = 0;
    next = nullptr;
    parent = nullptr;
}

Node::Node(const Node &node) {
    x = node.x;
    y = node.y;
    cost = node.cost;
    h = node.h;
    id = node.id;
    next = nullptr;
    parent = node.parent;
}

# C++ Path Planner Toy Problem

## Setup
From base dir (cpp-path-planner/), run
`g++ -std=c++11 -o main.out src/main.cpp src/Environment.cpp src/GraphGenerator.cpp src/PathPlanner.cpp`

To execute, run `./main.out`.

## Problem Description
We are given an input file that contains: 
- a list of circular objects parameterized by (x, y, rad); 
- a list rectangular objects parameterized by (x, y, dx, dy); 
- the start position of a navigating agent;
- the goal position of the navigating agent.

We assume the objects are static. A sample input file is given in `example_input.txt`.

The goal is to find a list of waypoints (x,y) that describe a path that does from start to goal position that does not collide with any of the given objects.

## Assumptions:
### Obstacles
- All obstacles are static obstacles. 
- Their location on the map is implemented with an occupancy map (x,y) that rounds the position to the nearest integer. 
	- For example, a rectangle parameterized by (1.4, 2.7, 0.5, 0.2) will only appear on the occupancy map as (1,2).
	- Similarly, a rectangle parameterized by (1, 2, 1.2, 1.7) will give hits on the occupancy map at (1,2), (1,3), (2,2), and (2,3).

## Approach
### General
The approach intended to use A\* graph search to find a path.
The graph is made by sampling nodes (x,y) throughout the workspace.
After the graph is generated, we use A\* and a heuristic function to build a list of waypoints (nodes) to the goal location.

### Graph Generation
#### Workspace
The workspace is defined as the midpoint (xc, yc) between the start and goal positions +/- the distance from start to goal position (dx,dy).
The workspace can be represented as four limits:
- (xc - dx, yc - dy)
- (xc + dx, yc - dy)
- (xc - dx, yc + dy)
- (xc + dx, yc + dy)

#### Graph Nodes
To find candidate nodes in the workspace, a halton sequence sampler is implemented to provide quality coverage of the workspace.
The halton sequence sampler generates a set of proposed nodes (x,y).
After generating a set of proposed nodes, we check if the proposed nodes collide with any of the objects using the occupancy map.

#### Graph Adjacency List
After generating a set of K proposed nodes that do not collide with objects in the occupancy map,
we evaluate the edges for each node.
For each node (host), a valid edge is defined as a line between the host and a different node.
- Valid edges must be within `connection_radius` of the host node and not in collision with any objects in the occupancy map.

After edges are evaluated, an adjacency list that describes the graph is generated.
The adjacenecy list is a vector of `Node*`s. 
A `Node` is a data struct that contains:
- `double x,y` that describes the Node's location.
- `double cost,h` that describes the cost to Node (from start Node) and cost-to-go (to end Node).
- `Node* next` to next node (used in adjacency list to determine adjacent nodes).
- `Node* parent` to parent node (used for backtracking for path generation).

The adjacent nodes are maintained with a linked list using the `next` pointer to point to the next adjacent node.

### Graph Search
#### A\*
After generating the graph, A\* graph search will be used to find a list of waypoints (nodes) describe collision free navigation from start to goal (not yet implemented).

## Next Steps

### Lazy A*
Implement lazy search for A*.
- Rather than evaluate edges (collision detection) prior to implementing A* algorithm, evaluate edges as they are explored. 

### RRT/RRT*

Implement RRT instead of A*. 
- This will require updating how nodes generated and added to the graph, as well as the process for building the adjacency list.

### Occupancy Map 

- Use collision checking function instead of occupancy map.

### Adjacency List

- Update the implementation of the adjacency list.

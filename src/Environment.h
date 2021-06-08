//
// Created by krish on 5/9/21.
//

#ifndef APL_PATH_PLANNING_ENVIRONMENT_H
#define APL_PATH_PLANNING_ENVIRONMENT_H

#include <iostream>
#include <iterator>
#include <fstream>
#include <cassert>
#include <sstream>
#include <vector>
#include <set>

using namespace std;

class Environment {
public:
    Environment();
    Environment(const string& file_name);
    pair<double, double> getStartPos();
    pair<double, double> getEndPos();
    bool inCollision(pair<double,double> node);
    void printConfig();

private:
    void loadConfig();
    void loadCircleObjs(string circle_data);
    void loadSquareObjs(string square_data);
    void loadStartPos(string start_data);
    void loadEndPos(string end_data);
    void generateMapData();

    string config_file;
    // TODO: Create base_object class
    // TODO: Create circle object class
    vector<vector<double> > circle_objs;
    // TODO: Create square object class
    vector<vector<double> > square_objs;

    vector<double> start_pos;
    vector<double> end_pos;

    /* TODO: Rather than create an occupancy map, check edges are in collision with any objects
     *       using a distance function.
     *       Example: Check collision with circle object with position (xc, yc) and radius (r)
     *          if ((x - xc)^2 + (y - yc)^2 < r ) -> collision
     */
    set<pair<int,int>> map_data;

};


#endif //APL_PATH_PLANNING_ENVIRONMENT_H

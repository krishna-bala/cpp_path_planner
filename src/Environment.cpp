//
// Created by krish on 5/9/21.
//

#include "Environment.h"

Environment::Environment() : config_file("../src/example_input.txt"){
    loadConfig();

}

Environment::Environment(const string &file_name) : config_file(file_name){
    loadConfig();
}

void Environment::loadConfig() {
	
    // Create stream to read config
    ifstream configSource;
    configSource.open(config_file.c_str());
    assert(configSource.is_open());

    // Get data for circle objects
    string circle_data;
    getline(configSource, circle_data);
    loadCircleObjs(circle_data);

    // Get data for Square objects
    string square_data;
    getline(configSource, square_data);
    loadSquareObjs(square_data);

    string dummy;
    getline(configSource, dummy);

    // Get data for Start Pos
    string start_pos;
    getline(configSource, start_pos);
    loadStartPos(start_pos);

    // Get data for End Pos
    string end_pos;
    getline(configSource, end_pos);
    loadEndPos(end_pos);

    generateMapData();

}

void Environment::loadCircleObjs(string circle_data) {
    stringstream tokenizer(circle_data);
    string data;
    string temp;
    size_t pos;
    vector<double> circle;

    while(tokenizer >> data){
        // Hacky way to separate out when there is no space separation...
        if (data.find(',') != string::npos){
            pos = data.find(',');
            temp = data.substr(0, pos);
            circle.push_back(stod(temp));
            circle_objs.push_back(circle);
            circle.clear();
            data = data.substr(pos+1);
        }

        if(data != "," && data != "#"){
            circle.push_back(stod(data));
            assert(circle.size() < 4);
        } else{
            circle_objs.push_back(circle);
            circle.clear();
            if (data == "#") break;
        }
    }

}

void Environment::loadSquareObjs(string square_data) {

    stringstream tokenizer(square_data);
    string data;
    string temp;
    size_t pos;
    vector<double> square;

    while(tokenizer >> data){
        // Hacky way to separate out when there is no space separation...
        if (data.find(',') != string::npos){
            pos = data.find(',');
            temp = data.substr(0, pos);
            square.push_back(stod(temp));
            square_objs.push_back(square);
            square.clear();
            data = data.substr(pos+1);
        }

        if(data != "," && data != "#"){
            square.push_back(stod(data));
            assert(square.size() < 5);
        } else{
            square_objs.push_back(square);
            square.clear();
            if (data == "#") break;
        }
    }


}

void Environment::loadStartPos(string start_data) {

    stringstream tokenizer(start_data);
    string data;
    string temp;
    size_t pos;

    while(tokenizer >> data){
        // Hacky way to separate out when there is no space separation...
        if (data.find(',') != string::npos){
            pos = data.find(',');
            temp = data.substr(0, pos);
            start_pos.push_back(stod(temp));
            data = data.substr(pos+1);
        }

        if(data != "," && data != "#"){
            start_pos.push_back(stod(data));
            assert(start_pos.size() < 3);
        } else{
            if (data == "#") break;
        }
    }


}

void Environment::loadEndPos(string end_data) {

    stringstream tokenizer(end_data);
    string data;
    string temp;
    size_t pos;

    while(tokenizer >> data){
        // Hacky way to separate out when there is no space separation...
        if (data.find(',') != string::npos){
            pos = data.find(',');
            temp = data.substr(0, pos);
            end_pos.push_back(stod(temp));
            data = data.substr(pos+1);
        }

        if(data != "," && data != "#"){
            end_pos.push_back(stod(data));
            assert(end_pos.size() < 3);
        } else{
            if (data == "#") break;
        }
    }

}

void Environment::printConfig() {
    int obj_num = 0;
    for (vector<double> obj : square_objs){
        cout << "Square #" << obj_num << endl;
        for (double data : obj){
            cout << data << " ";
        }
        ++obj_num;
        cout << endl;
    }

    obj_num = 0;
    for (vector<double> obj : circle_objs){
        cout << "Circle #" << obj_num << endl;
        for (double data : obj){
            cout << data << " ";
        }
        ++obj_num;
        cout << endl;
    }

    cout << "Start Pos: " << endl;
    for (double pos : start_pos){
        cout << pos << " ";
    }
    cout << endl;

    cout << "End Pos: " << endl;
    for (double pos : end_pos){
        cout << pos << " ";
    }
    cout << endl;

}

pair<double, double> Environment::getStartPos() {
    pair<double,double> pairStart(start_pos[0],start_pos[1]);
    return pairStart;
}

pair<double, double> Environment::getEndPos() {
    pair<double,double> pairEnd(end_pos[0], end_pos[1]);
    return pairEnd;
}

void Environment::generateMapData() {

    int xc, yc, rad;
    int xmax, xmin, ymax, ymin;
    pair<int,int> occupied;
    for (vector<double> obj : circle_objs){
        xc = obj[0];
        yc = obj[1];
        rad = obj[2];
        xmax = xc + rad;
        xmin = xc - rad;
        ymax = yc + rad;
        ymin = yc - rad;
        for (int x = xmin; x <= xmax; ++x){
            for (int y = ymin; y <= ymax; ++y){
                occupied.first = x;
                occupied.second = y;
                map_data.insert(occupied);
            }
        }
    }

    int dx, dy;
    for (vector<double> obj : square_objs){
        xc = obj[0];
        yc = obj[1];
        dx = obj[2];
        dy = obj[3];
        xmax = xc + dx;
        xmin = xc - dx;
        ymax = yc + dy;
        ymin = yc - dy;
        for (int x = xmin; x <= xmax; ++x){
            for (int y = ymin; y <= ymax; ++y){
                occupied.first = x;
                occupied.second = y;
                map_data.insert(occupied);
            }
        }
    }

}

bool Environment::inCollision(pair<double, double> node) {
    // Since map_data is an occupancy map over ints, need to convert node to int
    int x = int(node.first);
    int y = int(node.second);
    pair<int,int> temp_node(x,y);

    if (map_data.find(temp_node) == map_data.end()) {
        return false;
    } else {
        return true;
    }

}

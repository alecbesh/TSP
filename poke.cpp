

#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <math.h>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <getopt.h>
#include "poke.h"

using namespace std;

void Poke::get_options(int argc, char **argv) {
    int option_index = 0, option = 0;
    opterr = false;
    string mode;


    // use getopt to find command line options
    struct option longOpts[] = {{ "mode", 1, nullptr, 'm' },
                                { "help", 0, nullptr, 'h' },
                                { nullptr, 0, nullptr, '\0' }};
    
    while ((option = getopt_long(argc, argv, "m:h", longOpts, &option_index)) != -1) {
        switch (option) {
            case 'm':
                mode = optarg;
                break;

            case 'h':
                cout << "This program does pokemon stuff\n"
                          <<  "Usage: \'./hunt\n\t[--help | -h]\n"
                          <<                   "\t[--mode | -m]\n";
                exit(0);
        }
    }
    read_options();

    if (mode == "MST") MST();
    else if (mode == "FASTTSP") {
        FASTTSP(true);
    }
    else if (mode == "OPTTSP") {
        OPTTSP();
    }
    else {
        cerr << "Invalid mode\n";
        exit(1);
    }
}

void Poke::read_options() {
    Vertex v;
    uint32_t n;
    cin >> n;
    graph.resize(n);
    TSP.reserve(n+1);
    for (uint32_t i = 0; i < n; ++i) {
        cin >> v.x;
        cin >> v.y;
        if (v.x < 0 && v.y < 0) v.terrain = 's'; // assign sea terrain
        else if ((v.x == 0 && v.y < 0) || (v.x < 0 && v.y == 0) || (v.x == 0 && v.y == 0)) v.terrain = 'c'; // assign coast terrain
        else v.terrain = 'l'; // assign land terrain
        v.number = i;
        graph[i] = v;
    }
}



double Poke::MST_distance(Vertex &v1, Vertex &v2) {
    if (v1.terrain == 'l' && v2.terrain == 's') return std::numeric_limits<double>::infinity();
    else if (v1.terrain == 's' && v2.terrain == 'l') return std::numeric_limits<double>::infinity();
    else {
        return sqrt((double)((double)(v1.x - v2.x) * (double)(v1.x - v2.x)) + ((double)(v1.y - v2.y) * (double)(v1.y - v2.y)));
    }
}



void Poke::MST() {
    // Using linear search:
    double distance = std::numeric_limits<double>::infinity();
    uint32_t minIndex = 0;
    graph[0].d = 0;
    for (uint32_t i = 0; i < graph.size(); ++i) {
        distance = std::numeric_limits<double>::infinity();
        for (uint32_t v = 0; v < graph.size(); ++v) {
            if (graph[v].k == false && graph[v].d < distance) {
                minIndex = v;
                distance = graph[v].d;
            }
        }
        graph[minIndex].k = true;
        for (uint32_t j = 0; j < graph.size(); ++j) {
                if (!graph[j].k) {
                    distance = MST_distance(graph[minIndex], graph[j]);
                if (distance < graph[j].d) {
                    graph[j].d = distance;
                    graph[j].p = minIndex;
                }
            }
        }
    }


    for (uint32_t i = 1; i < graph.size(); ++i) {
        if (graph[i].d == std::numeric_limits<double>::infinity()) {
            cerr << "Cannot construct MST" << endl;
            exit(1);
        }
        total += graph[i].d;
    }
    cout << total << "\n";
    for (uint32_t i = 1; i < graph.size(); ++i) {
        if (i < graph[i].p) {
            cout << i << " " << graph[i].p << "\n";
        }
        else if (i > graph[i].p) {
            cout << graph[i].p << " " << i << "\n";
        }
    }
}



double Poke::TSP_distance(Vertex &v1, Vertex &v2) {
    return sqrt((double)((double)(v1.x - v2.x) * (double)(v1.x - v2.x)) + ((double)(v1.y - v2.y) * (double)(v1.y - v2.y)));
}


void Poke::FASTTSP(bool print) {
    // first run greedy nearest neighbor
    Vertex v;
    graph[0].d = 0;
    TSP.push_back(graph[0]);
    graph[0].k = true;

    double distance;
    uint32_t n = 0;
    while (TSP.size() != graph.size()) {
            for (uint32_t j = 0; j < graph.size(); ++j) {
                if (graph[j].k == false) {
                    distance = TSP_distance(graph[n], graph[j]);
                    if (distance < v.d) {
                        v.d = distance;
                        v.number = j;
                        v.x = graph[j].x;
                        v.y = graph[j].y;
                    }
                }
            }
        n = v.number;
        graph[n].k = true;
        TSP.push_back(v);
        v.d = std::numeric_limits<double>::infinity();
    }

    
    // Run 2-OPT
    graph[0].d = TSP_distance(graph[0], graph[graph.size() - 1]);
    TSP.push_back(graph[0]);
    for (uint32_t i = 0; i < TSP.size() - 1; ++i) {
        for (uint32_t j = i + 2; j < TSP.size() - 1; ++j) {
                if ((TSP_distance(TSP[i], TSP[i + 1]) + TSP_distance(TSP[j], TSP[j + 1])) > (TSP_distance(TSP[i], TSP[j]) + TSP_distance(TSP[i+1], TSP[j+1]))) {
                        std::reverse(TSP.begin() + (i+1), TSP.begin() + (j+1));
                }
        }
    }
    
    for (uint32_t i = 0; i < TSP.size() - 1; ++i) {
        total += TSP_distance(TSP[i], TSP[i+1]);
    }
    if (print) {
        cout << total << "\n";
        for (uint32_t i = 0; i < TSP.size() - 1; ++i) {
            cout << TSP[i].number << " ";
        }
        cout << "\n";
    }
}















bool C::promising(size_t permLength) {
    // Compute MST
    double distance = std::numeric_limits<double>::infinity();
    uint32_t minIndex = 0;
    for (uint32_t k = (uint32_t)permLength; k < path.size(); ++k) {
        path[k].k = false;
        path[k].d = std::numeric_limits<double>::infinity();
    }
    path[permLength].d = 0;
    for (uint32_t i = (uint32_t)permLength; i < path.size(); ++i) {
        distance = std::numeric_limits<double>::infinity();
        for (uint32_t v = (uint32_t)permLength; v < path.size(); ++v) {
            if (path[v].k == false && path[v].d < distance) {
                minIndex = v;
                distance = path[v].d;
            }
        }
        path[minIndex].k = true;
        for (uint32_t j = (uint32_t)permLength; j < path.size(); ++j) {
                if (!path[j].k) {
                    distance = C_distance(path[minIndex], path[j]);
                if (distance < path[j].d) {
                    path[j].d = distance;
                    path[j].p = minIndex;
                }
            }
        }
    }
    distance = 0;
    for (uint32_t i = (uint32_t)permLength + 1; i < path.size(); ++i) {
        distance += path[i].d;
    }



    // Compute arms
    double arm1 = std::numeric_limits<double>::infinity();
    double arm2 = std::numeric_limits<double>::infinity();
    for (uint32_t i = (uint32_t)permLength; i < path.size(); ++i) {
        if (C_distance(path[0], path[i]) < arm1) {
            arm1 = C_distance(path[0], path[i]);
        }
    }
    for (uint32_t i = (uint32_t)permLength; i < path.size(); ++i) {
        if (C_distance(path[permLength - 1], path[i]) < arm2) {
            arm2 = C_distance(path[permLength - 1], path[i]);
        }
    }



    distance += curr_distance;
    distance += arm1;
    distance += arm2;
    if (distance <= best_distance) return true;
    else return false;
}


void C::genPerms(size_t permLength) {
        if (permLength == path.size()) {
            double closing = C_distance(path[0], path[permLength - 1]);
            curr_distance += closing;
            if (curr_distance < best_distance) {
                best_distance = curr_distance;
                for (uint32_t i = 0; i < path.size(); ++i) {
                    best_path[i] = path[i];
                }
            }
            curr_distance -= closing;
            return;
        }
        if (!promising(permLength)) return;
        for (size_t i = permLength; i < path.size(); ++i) {
            swap(path[permLength], path[i]);
            double edge = C_distance(path[permLength - 1], path[permLength]);
            curr_distance += edge; 
            genPerms(permLength + 1);
            curr_distance -= edge;
            swap(path[permLength], path[i]);
        }
}





void Poke::OPTTSP() {
    C c;
    FASTTSP(false);
    c.best_distance = total;
    c.best_path.resize(TSP.size() - 1);
    c.path.resize(TSP.size() - 1);
    for (uint32_t i = 0; i < TSP.size() - 1; ++i) {
        c.best_path[i] = TSP[i];
        c.path[i] = c.best_path[i];
    }
    c.curr_distance = 0;
    c.genPerms(1);
    cout << c.best_distance << "\n";
    cout << c.best_path[0].number << " ";
    for (uint32_t i = (uint32_t)(c.best_path.size() - 1); i > 0; --i) {
        cout << c.best_path[i].number << " ";
    }
    cout << "\n";
}
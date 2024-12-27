

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

using namespace std;

struct Vertex {
        double d = std::numeric_limits<double>::infinity(); // distance
        int x;
        int y;
        uint32_t number;
        uint32_t p; // shortest path
        char terrain;
        bool k = false; // visited?
};
class COMP {
        public:
        bool operator()(const Vertex &v1, const Vertex &v2) {
            return (v1.d > v2.d);
        }
};

class Poke {
    public:

    void get_options(int argc, char **argv);
    void read_options();
    void MST();
    double MST_distance(Vertex &v1, Vertex &v2);
    double TSP_distance(Vertex &v1, Vertex &v2);
    void FASTTSP(bool print);
    void OPTTSP();


    private:
    vector<Vertex> graph;
    vector<Vertex> TSP;
    double total = 0;
};



class C {
    public:


    void genPerms(size_t permLength);
    bool promising(size_t permLength);
    double C_distance(Vertex &v1, Vertex &v2) {
        return sqrt((double)((double)(v1.x - v2.x) * (double)(v1.x - v2.x)) + ((double)(v1.y - v2.y) * (double)(v1.y - v2.y)));
    }

    vector<Vertex> best_path;
    vector<Vertex> path; // current
    double curr_distance = 0;
    double best_distance = 0;
};
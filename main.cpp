

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
#include <iomanip>

using namespace std;


int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);
    cout << std::setprecision(2);
    cout << std::fixed;
    
    Poke p;
    p.get_options(argc, argv);
    
    return 0;
}
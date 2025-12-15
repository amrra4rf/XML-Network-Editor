#include <bits/stdc++.h>
#include "Graph.hpp"


class Reader {
private:
    std::string filename;
    Graph network;

    std::string extractValue(const std::string& line);

public:
    Reader(const std::string& file);
    void read();
    Graph getnet();
};
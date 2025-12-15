#ifndef Reader_HPP
#define Reader_HPP

#include <bits/stdc++.h>
#include "../Graph/Graphs.hpp"
#include "../Classes/Posts.hpp"
#include "../Classes/User.hpp"

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

#endif


#ifndef Readers_HPP
#define Readers_HPP
#include <bits/stdc++.h>
#include "Graphs.hpp"
#include "Users.hpp"
#include "Posts.hpp"
class Reader{
private:
    std::string filename;
    Graph network;

    std::string extractValue(const std::string& line);
    string strip(string&s);

public:
    Reader(const std::string& file);
    void read();
    Graph getnet();
    int stringToInt(const std::string& s);
};



#endif
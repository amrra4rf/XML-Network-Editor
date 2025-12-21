#ifndef Reader_HPP
#define Reader_HPP

#include <bits/stdc++.h>
#include "../Graph/Graphs.hpp"
#include "../Classes/Posts.hpp"
#include "../Classes/User.hpp"

class Reader {
private:
    string filename;
    Graph network;
    vector<int> users_ids;

    string strip(string& s);

public:
    Reader(const string& file);
    void read(PostsBuilder& builder);
    Graph& getnet();
    vector<int> getusers();
};

#endif

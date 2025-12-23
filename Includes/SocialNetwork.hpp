#ifndef SOCIAL_NETWORK_GRAPH_HPP
#define SOCIAL_NETWORK_GRAPH_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <climits>
#include "Reader.hpp"
#include "Posts.hpp"

using namespace std;

class SocialNetworkGraph
{
public:
    Graph network;

    SocialNetworkGraph(const string &filePath, PostsBuilder &builder);
    SocialNetworkGraph(const Graph &n);

    Graph &getGraph();
    Users getInfluencer();
    Users getMostActiveUser();
    vector<Posts> getPostsByWord(const string &word);
    vector<Posts> getPostsByTopicFast(const string &topic);
    unordered_map<int, vector<Users>> getSuggestions();
    vector<Users> getMutualFollowers(const vector<int> &userIds);
};

#endif
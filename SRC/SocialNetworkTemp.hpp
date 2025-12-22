#ifndef SOCIAL_NETWORK_GRAPH_HPP
#define SOCIAL_NETWORK_GRAPH_HPP
 
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <climits>
#include "../Parse/Reader.hpp"
 
using namespace std;
 
// Global hash for pair<int,int>
 
class SocialNetworkGraph
{
public:
    Graph network;
    // ---------- Constructors ----------
    SocialNetworkGraph(const string &filePath, PostsBuilder &builder)
    {
        Reader r(filePath);
        r.read(builder); // pass builder to Reader
        network = r.getnet();
    }
 
    SocialNetworkGraph(const Graph &n) : network(n) {}
 
    Graph &getGraph() { return network; }
 
    // ---------- Influencer ----------
    Users getInfluencer()
    {
        int maxFollowers = -1;
        int maxId = -1;
 
        for (const auto &u : network.getUsers())
        {
            int count = network.GetFollowers(u).size();
            if (count > maxFollowers)
            {
                maxFollowers = count;
                maxId = u.getId();
            }
        }
        return (maxId != -1) ? network.getUserFromID(maxId) : Users();
    }
 
    // ---------- Most Active ----------
    Users getMostActiveUser()
    {
        int maxFollowing = -1;
        int maxId = -1;
 
        for (const auto &u : network.getUsers())
        {
            int count = network.GetFollowing(u).size();
            if (count > maxFollowing)
            {
                maxFollowing = count;
                maxId = u.getId();
            }
        }
 
        return (maxId != -1) ? network.getUserFromID(maxId) : Users();
    }
 
    // ---------- Post Search ----------
    vector<Posts> getPostsByWord(const string &word)
    {
        vector<Posts> res;
 
        for (const auto &u : network.getUsers())
        {
            for (const auto &p : u.getPosts())
            {
                if (p.getcontent().find(word) != string::npos)
                    res.push_back(p);
            }
        }
 
        return res;
    }

};
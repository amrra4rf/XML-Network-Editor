    
#include <bits/stdc++.h>
#include "Posts.hpp"
#include "Users.hpp"
#include "Graphs.hpp"
using namespace std;
class SocialNetworkGraph{
    private:
        Graph Network;
    public:
    SocialNetworkGraph(Graph n);
    Users getInfluencer();
    Users getMostActiveUser();
    vector<Posts> getPostsByTopic(string topic);
    vector<Posts> getPostsByTopicFastAlgorithm(string topic);
    vector<Posts> getPostsByWord(string word );
    void printSuggestions(int id,unordered_set<int> &suggestions);
    void getSuggestions();
    
    vector<Users> getMutualFollowers(vector<int> goal);//this takes id's of mutlipel users and return a vector of users who follow simutenously follow the given user id's
};
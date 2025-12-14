    
#include <bits/stdc++.h>
#include "Src/Posts.cpp"
#include "Src/User.cpp"
#include "Src/Graph.cpp"
using namespace std;
class SocialNetworkGraph{
    private:
        Graph Network;
    public:
    User getInfluencer();
    User getMostActiveUser();
    vector<Post> getPostsByTopic(string topic);
    vector<Post> getPostsByTopicFastAlgorithm(string topic);
    vector<Post> getPostsByWord(string word );
    void printSuggestions(int id,unordered_set<int> &suggestions);
    void getSuggestions();
    
    vector<User> getMutualFollowers(vector<int> goal);//this takes id's of mutlipel users and return a vector of users who follow simutenously follow the given user id's
};
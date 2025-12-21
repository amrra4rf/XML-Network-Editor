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

    vector<Posts> getPostsByTopicFast(const string &topic){
        vector<Posts> res;
        string t = topic;

        // trim whitespace
        t.erase(t.begin(), find_if(t.begin(), t.end(), [](unsigned char ch)
                                   { return !isspace(ch); }));
        t.erase(find_if(t.rbegin(), t.rend(), [](unsigned char ch)
                        { return !isspace(ch); })
                    .base(),
                t.end());

        // lowercase
        transform(t.begin(), t.end(), t.begin(), ::tolower);

        auto &topicMap = network.getTopicIndex();
        auto it = topicMap.find(t);
        if (it == topicMap.end())
            return res;
        for (const Posts *pptr : it->second)
        {
            if (pptr)
                res.push_back(*pptr);
        }

        return res;
    }

    // ---------- Suggestions ----------
    unordered_map<int, vector<Users>> getSuggestions()
    {
        unordered_map<int, vector<Users>> allSuggestions;

        for (const auto &u : network.getUsers())
        {
            unordered_set<int> suggestionIds;

            for (int f1 : network.GetFollowers(u))
            {
                for (int f2 : network.GetFollowers(f1))
                {
                    if (f2 != u.getId() && !network.isFollowing(u.getId(), f2))
                    {
                        suggestionIds.insert(f2);
                    }
                }
            }

            for (int id : suggestionIds)
            {
                if (network.getIDtoUser().count(id))
                {
                    allSuggestions[u.getId()].push_back(network.getUserFromID(id));
                }
            }
        }

        return allSuggestions;
    }
    
    vector<Users> getMutualFollowers(const vector<int> &userIds)
    {
        if (userIds.empty())
            return {};

        // 1. Get the follower set of the first user to start the intersection
        unordered_set<int> mutualIds;
        try
        {
            vector<int> firstFollowers = network.GetFollowers(userIds[0]);
            mutualIds.insert(firstFollowers.begin(), firstFollowers.end());
        }
        catch (...)
        {
            return {}; // User 1 doesn't exist
        }

        // 2. Intersect with the followers of the remaining users
        for (size_t i = 1; i < userIds.size(); ++i)
        {
            unordered_set<int> nextSet;
            try
            {
                vector<int> currentFollowers = network.GetFollowers(userIds[i]);
                for (int id : currentFollowers)
                {
                    if (mutualIds.count(id))
                    {
                        nextSet.insert(id);
                    }
                }
                mutualIds = nextSet; // Update mutual set to the intersection
            }
            catch (...)
            {
                return {}; // One of the users doesn't exist, so intersection is empty
            }

            if (mutualIds.empty())
                break; // Optimization: stop if no mutual followers left
        }

        // 3. Convert IDs back to User objects
        vector<Users> result;
        for (int id : mutualIds)
        {
            result.push_back(network.getUserFromID(id));
        }
        return result;
    }
};

#endif // SOCIAL_NETWORK_GRAPH_HPP

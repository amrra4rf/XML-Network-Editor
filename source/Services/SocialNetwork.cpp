#include "../../Includes\SocialNetwork.hpp"
#include <algorithm>

SocialNetworkGraph::SocialNetworkGraph(const string &filePath, PostsBuilder &builder)
{
    Reader r(filePath);
    r.read(builder);
    network = r.getnet();
}

SocialNetworkGraph::SocialNetworkGraph(const Graph &n) : network(n) {}

Graph &SocialNetworkGraph::getGraph()
{
    return network;
}

Users SocialNetworkGraph::getInfluencer()
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

Users SocialNetworkGraph::getMostActiveUser()
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

vector<Posts> SocialNetworkGraph::getPostsByWord(const string &word)
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



vector<Posts> SocialNetworkGraph::getPostsByTopicFast(const string & topic)
{
    vector<Posts> answer;
    for(int i=0;i<network.getUsers().size();i++)
    {
         const vector<Posts>& tempPosts = network.getUsers()[i].getPosts();
        for(int j=0;j<tempPosts.size();j++)
        {
           const unordered_set<string>& tempTopics=tempPosts[j].gettopics();
           if(tempTopics.find(topic)!=tempTopics.end())
                answer.push_back(tempPosts[j]);
          
        }
    }
    return answer;
}

unordered_map<int, vector<Users>> SocialNetworkGraph::getSuggestions()
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

vector<Users> SocialNetworkGraph::getMutualFollowers(const vector<int> &userIds)
{
    if (userIds.empty())
        return {};

    unordered_set<int> mutualIds;
    try
    {
        vector<int> firstFollowers = network.GetFollowers(userIds[0]);
        mutualIds.insert(firstFollowers.begin(), firstFollowers.end());
    }
    catch (...)
    {
        return {};
    }

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
            mutualIds = nextSet;
        }
        catch (...)
        {
            return {};
        }

        if (mutualIds.empty())
            break;
    }

    vector<Users> result;
    for (int id : mutualIds)
    {
        result.push_back(network.getUserFromID(id));
    }
    return result;
}
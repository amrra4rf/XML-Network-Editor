/**
 * 
 * 
 * Purpose:
 * - Defines the Graph class used to represent a social media network as a graph.
 * - Each user is a node, and follow/follower relationships are directed edges.
 * - Supports:
 *      - Adding User
 *      - Adding follower / following edges
 *      - Adding posts to User
 *      - Querying followers/following
 * 
 * Internal Structure:
 * - vector<User> User:
 *        Stores all user objects by index. Each user's position is its graph node index.
 * 
 * - unordered_map<int, User> IDtoUser:
 *        Maps user ID → full User object.
 * 
 * - unordered_map<int, int> indexmapper:
 *        Maps user ID → node index inside `User` vector.
 * 
 * - vector<vector<int>> Followers_id:
 *        For each node index, stores IDs of followers.
 * 
 * - vector<vector<int>> Following_id:
 *        For each node index, stores IDs the user follows.
 * 
 * - UserBuilder ubuild:
 *        Creates new User if XML references a user that wasn't created yet.
 * 
 * Notes:
 * - getNodeIndex(): safely converts user ID to node index.
 * - AddUser() handles:
 *        → new user insertion
 *        → update existing user name if duplicate
 * - AddFollower / AddFollow both support auto-creating missing User.
 */



#include <bits/stdc++.h>
#include "Src/Post.cpp"
#include "User.hpp"
#include "User.cpp"
using namespace std;
class Graph {
private:
    std::vector<User> users;
    std::unordered_map<int, User> IDtoUser;
    std::unordered_map<int, int> indexmapper;

    //by (amr )
    unordered_map<string,vector<Post*>> topicIndex;//give  a topic as a key and it returns a vector of pointers to the posts taht includes this keys

    //if i said followers_id[0] does it return a vector of all the users who follow the first user?
    std::vector<std::vector<int>> Followers_id;
    std::vector<std::vector<int>> Following_id;

    UserBuilder ubuild;

    int getNodeIndex(int uid) const;

public:
    Graph() = default;
    Graph(UserBuilder& ubuild2);

    
    bool AddUser(User& user);
    bool AddUser(std::string& name, int id);

    void CheckExistance(const User& user) const;

    bool AddFollower(User& tofollow, int followerid);
    bool AddFollow(User& follower, int tofollowid);

    void AddPost(Post& p, User& writer);

    std::vector<int> GetFollowers(User& user);
    std::vector<int> GetFollowers(int userid);

    std::vector<int> Getfollowing(User& user);
    std::vector<int> getfollowing(int userid);

    //added by amr:

   bool Graph::isFollowing(int followerId, int followeeId);
    friend class SocialNetworkGraph;
       

};



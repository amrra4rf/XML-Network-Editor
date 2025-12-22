/**
 * Graph.hpp
 * 
 * Purpose:
 * - Defines the Graph class used to represent a social media network as a graph.
 * - Each user is a node, and follow/follower relationships are directed edges.
 * - Supports:
 *      - Adding users
 *      - Adding follower / following edges
 *      - Adding posts to users
 *      - Querying followers/following
 * 
 * Internal Structure:
 * - vector<Users> users:
 *        Stores all user objects by index. Each user's position is its graph node index.
 * 
 * - unordered_map<int, Users> IDtoUser:
 *        Maps user ID → full Users object.
 * 
 * - unordered_map<int, int> indexmapper:
 *        Maps user ID → node index inside `users` vector.
 * 
 * - vector<vector<int>> Followers_id:
 *        For each node index, stores IDs of followers.
 * 
 * - vector<vector<int>> Following_id:
 *        For each node index, stores IDs the user follows.
 * 
 * - UsersBuilder ubuild:
 *        Creates new Users if XML references a user that wasn't created yet.
 * 
 * Notes:
 * - getNodeIndex(): safely converts user ID to node index.
 * - AddUser() handles:
 *        → new user insertion
 *        → update existing user name if duplicate
 * - AddFollower / AddFollow both support auto-creating missing users.
 */

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <bits/stdc++.h>
#include "./Posts.hpp"
#include "./User.hpp"

class Graph {
private:
    std::vector<Users> users;
    std::unordered_map<int, Users> IDtoUser;
    std::unordered_map<int, int> indexmapper;
    std::vector<std::vector<int>> Followers_id;
    std::vector<std::vector<int>> Following_id;

    // Topic index for fast post search
    std::unordered_map<std::string, std::vector<Posts*>> topicIndex;

    UsersBuilder ubuild;

    int getNodeIndex(int uid) const;

public:
    Graph() = default;
    Graph(UsersBuilder& ubuild2) : ubuild(ubuild2) {ubuild.reset();}

    // ---------- User Management ----------
    bool AddUser(Users& user);
    bool AddUser(const std::string& name, int id);

    void CheckExistance(const Users& user) const;

    bool AddFollower(Users& tofollow, int followerid);
    bool AddFollow(Users& follower, int tofollowid);

    void AddPost(Posts& p, Users& writer);

    // Return reference to allow modifying posts
    Users& getUserFromID(int id);

    // ---------- Query Followers / Following ----------
    std::vector<int> GetFollowers(const Users& user);
    std::vector<int> GetFollowers(int userid);

    std::vector<int> GetFollowing(const Users& user);
    std::vector<int> GetFollowing(int userid);

    // ---------- Getters for external access ----------
    std::vector<Users>& getUsers() { return users; }
    const std::vector<Users>& getUsers() const { return users; }

    std::unordered_map<int, Users>& getIDtoUser() { return IDtoUser; }
    const std::unordered_map<int, Users>& getIDtoUser() const { return IDtoUser; }

    std::unordered_map<std::string, std::vector<Posts*>>& getTopicIndex() { return topicIndex; }
    const std::unordered_map<std::string, std::vector<Posts*>>& getTopicIndex() const { return topicIndex; }

    // ---------- Utility ----------
    bool isFollowing(int userId, int targetId) const;
};

#endif
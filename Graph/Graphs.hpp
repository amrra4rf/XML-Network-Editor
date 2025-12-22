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
#include "../Classes/Posts.hpp"
#include "../Classes/User.hpp"

class Graph {
private:
    std::vector<Users> users;
    std::unordered_map<int, Users> IDtoUser;
    std::unordered_map<int, int> indexmapper;

    std::vector<std::vector<int>> Followers_id;
    std::vector<std::vector<int>> Following_id;

    UsersBuilder ubuild;

    int getNodeIndex(int uid) const;

public:
    Graph() = default;
    Graph(UsersBuilder& ubuild2);

    bool AddUser(Users& user);
    bool AddUser(std::string& name, int id);

    void CheckExistance(const Users& user) const;

    bool AddFollower(Users& tofollow, int followerid);
    bool AddFollow(Users& follower, int tofollowid);

    void AddPost(Posts& p, Users& writer);

    Users getuserfromID(int id);

    std::vector<int> GetFollowers(Users& user);
    std::vector<int> GetFollowers(int& userid);

    std::vector<int> Getfollowing(Users& user);
    std::vector<int> FetDollowing(int& userid);
};

#endif

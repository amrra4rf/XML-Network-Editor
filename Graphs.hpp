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


using namespace std;

Graph::Graph(UsersBuilder& ubuild2) : ubuild(ubuild2) {}

int Graph::getNodeIndex(int uid) const {
    auto it = indexmapper.find(uid);
    if (it == indexmapper.end()) {
        throw runtime_error("User id not found in indexmapper: " + to_string(uid));
    }
    return it->second;
}

bool Graph::AddUser(Users& user) {
    int uid = user.getId();

    auto it = IDtoUser.find(uid);
    if (it != IDtoUser.end()) {
        it->second = user;  // overwrite existing user entirely
        int idx = indexmapper[uid];
        users[idx] = user;
        return false;
    }

    int idx = users.size();
    indexmapper[uid] = idx;
    users.push_back(user);
    IDtoUser[uid] = user;
    Followers_id.push_back({});
    Following_id.push_back({});
    return true;
}

bool Graph::AddUser(string& name, int id) {
    Users u = ubuild.CreateUser(name, id);
    return this->AddUser(u);
}

void Graph::CheckExistance(const Users& user) const {
    if (IDtoUser.count(user.getId()) == 0) {
        throw runtime_error("Add User first");
    }
}

bool Graph::AddFollower(Users& tofollow, int followerid) {
    CheckExistance(tofollow);
    int idx = getNodeIndex(tofollow.getId());
    Followers_id[idx].push_back(followerid);

    if (indexmapper.find(followerid) != indexmapper.end()) {
        int fidx = indexmapper.at(followerid);
        Following_id[fidx].push_back(tofollow.getId());
        return true;
    }

    Users u = ubuild.CreateUser("Add Name to user", followerid);
    int newIdx = users.size();
    indexmapper[followerid] = newIdx;
    users.push_back(u);
    IDtoUser[followerid] = u;

    Followers_id.push_back({});
    Following_id.push_back({});

    Following_id[newIdx].push_back(tofollow.getId());
    return false;
}

bool Graph::AddFollow(Users& follower, int tofollowid) {
    CheckExistance(follower);
    int followeridx = getNodeIndex(follower.getId());
    Following_id[followeridx].push_back(tofollowid);

    if (indexmapper.find(tofollowid) != indexmapper.end()) {
        int tidx = indexmapper.at(tofollowid);
        Followers_id[tidx].push_back(follower.getId());
        return true;
    }

    Users u = ubuild.CreateUser("Add Name to user", tofollowid);
    int newIdx = users.size();
    indexmapper[tofollowid] = newIdx;
    users.push_back(u);
    IDtoUser[tofollowid] = u;

    Followers_id.push_back({});
    Following_id.push_back({});
    Followers_id[newIdx].push_back(follower.getId());
    return false;
}

void Graph::AddPost(Posts& p, Users& writer) {
    CheckExistance(writer);
    int idx = indexmapper[writer.getId()];
    users[idx].addPost(p);
}

vector<int> Graph::GetFollowers(Users& user) {
    CheckExistance(user);
    int idx = indexmapper[user.getId()];
    return Followers_id.at(idx);
}

vector<int> Graph::GetFollowers(int& userid) {
    return this->GetFollowers(IDtoUser[userid]);
}

vector<int> Graph::Getfollowing(Users& user) {
    CheckExistance(user);
    int idx = indexmapper[user.getId()];
    return Following_id.at(idx);
}

vector<int> Graph::FetDollowing(int& userid) {
    return this->Getfollowing(IDtoUser[userid]);
}

Users Graph::getuserfromID(int id){
    return IDtoUser[id];
}


#endif

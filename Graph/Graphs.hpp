#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <bits/stdc++.h>
#include "Classes/Posts.hpp"
#include "Classes/User.hpp"

using namespace std;

/**
 * CLASS: Graph
 * ---------------------------------------------------------------
 * This class represents the main social-network graph structure.
 *
 * Nodes:
 *   - Each node is a user (stored in the `users` vector).
 *
 * Edges:
 *   - Followers_id[i]  = list of user IDs that follow user at index i
 *   - Following_id[i]  = list of user IDs that user at index i follows
 *
 * Internal Representation:
 *   - Users are stored in `vector<Users> users`, indexed 0..N-1.
 *   - A user’s real ID (from XML) maps to an internal index using:
 *         indexmapper[userID] = index
 *
 * Access Maps:
 *   - IDtoUser: maps userID → Users object (quick existence check).
 *   - indexmapper: maps userID → position inside `users` vector.
 *
 * Graph Builder:
 *   - UsersBuilder ubuild is used to create new Users when adding by (name, id).
 *
 * Core Operations:
 *   - AddUser(): insert new user and allocate adjacency lists.
 *   - AddFollower(): "user A follows user B" relation.
 *   - AddFollow(): same relation but opposite direction call.
 *   - AddPost(): attaches a Post to the internal user object.
 *   - GetFollowers() / GetFollowing(): return adjacency lists.
 *
 * Notes:
 *   - Followers and following lists store **user IDs**, not indices.
 *   - For graph analysis, users are accessed quickly using index mapping.
 *   - Class supports insertion before or after follower exists.
 *
 * Purpose:
 *   - Designed for parsing XML into a graph.
 *   - Used for social-network analysis (influencers, suggestions, etc.).
 * ---------------------------------------------------------------
 */

class Graph {
private:
    vector<Users> users;                            // All users in index form
    unordered_map<int, Users> IDtoUser;             // ID → full User object
    unordered_map<int, int> indexmapper;            // ID → index in users[]

    vector<vector<int>> Followers_id;               // followers adjacency
    vector<vector<int>> Following_id;               // following adjacency

    UsersBuilder ubuild;                            // tool to construct Users

public:
    Graph() = default;
    Graph(UsersBuilder &ubuild2);

    void AddUser(Users &user);
    void AddUser(string &name, int id);

    void CheckExistance(const Users &user) const;

    bool AddFollower(Users &tofollow, int followerid);
    bool AddFollow(Users &follower, int tofollowid);

    void AddPost(Posts &p, Users &writer);

    vector<int> GetFollowers(Users &user);
    vector<int> GetFollowers(int &userid);

    vector<int> Getfollowing(Users &user);
    vector<int> FetDollowing(int &userid);
};

#endif

#ifndef USERS_HPP
#define USERS_HPP

#include <bits/stdc++.h>
#include "Posts.hpp"
using namespace std;

/*
Users.hpp

Purpose:
- Defines the Users class representing a social media user.
- Each user has a name, a unique ID, followers, and posts.
- Uses a builder pattern: only UsersBuilder can create Users.
- Ensures user IDs are unique.
*/

class Users {
private:
    string name;
    int id;
    vector<int> followersid;
    vector<Posts> posts;

    // Private constructor
    Users(const string& s, int x);

public:
    Users() = default;
    Users(const Users&) = default;
    Users& operator=(const Users&) = default;
    ~Users() = default;

    friend class UsersBuilder;

    // Getters
    string getName() const;
    int getId() const;
    const vector<int>& getFollowers() const;
    const vector<Posts>& getPosts() const;

    // Setters / modifiers
    void setname(const string& s);
    void setid(int idx);
    void addFollower(int followerId);
    void addPost(const Posts& p);
};

class UsersBuilder {
private:
    static unordered_set<int> uniqueIds;  // declaration only

public:
    Users CreateUser(const string& s, int x);
};

#endif // USERS_HPP

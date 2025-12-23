#ifndef USERS_HPP
#define USERS_HPP

#include <bits/stdc++.h>
#include "Posts.hpp"
using namespace std;

class Users {
private:
    string name;
    int id;
    vector<int> followersid;
    vector<Posts> posts;

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
    vector<Posts>& getPosts();

    // Setters
    void setname(const string& s);
    void setid(int idx);
    void addFollower(int followerId);
    void addPost(const Posts& p);
};

class UsersBuilder {
private:
    // CHANGE 1: Removed 'static'
    unordered_set<int> uniqueIds; 

public:
    void reset() { 
        uniqueIds.clear(); 
    }

    Users CreateUser(const string& s, int x) {
        if (!uniqueIds.count(x)) {
            uniqueIds.insert(x);
            return Users(s, x);
        }
        throw runtime_error("Ids must be unique");
    }
};

#endif
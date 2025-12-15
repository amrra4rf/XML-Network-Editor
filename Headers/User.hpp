#ifndef USER_HPP
#define USER_HPP

#include <bits/stdc++.h>
#include "Post.hpp"

using namespace std;

/*
Users.hpp

Purpose:
- Defines the Users class representing a social media user.
- Each user has a name, a unique ID, followers, and posts.
- Uses a builder pattern: only UsersBuilder can create Users.
- Ensures user IDs are unique.
*/

class User {
private:
    string name;
    int id;
    vector<int> followersid;
    vector<Post> posts;

    // Private constructor
    User(const string& s, int x);

public:
    User() = default;
    User(const User&) = default;
    User& operator=(const User&) = default;
    ~User() = default;

    friend class UserBuilder;

    // Getters
    string getName() const;
    int getId() const;
    const vector<int>& getFollowers() const;
    const vector<Post>& getPosts() const;

    // Setters / modifiers
    void setname(const string& s);
    void setid(int idx);
    void addFollower(int followerId);
    void addPost(const Post& p);
};

class UserBuilder {
private:
    static unordered_set<int> uniqueIds;  // declaration only

public:
    User CreateUser(const string& s, int x);
};

#endif 
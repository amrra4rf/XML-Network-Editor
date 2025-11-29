/* 
Users.hpp

Purpose:
- Defines the Users class representing a social media user.
- Each user has a name, a unique ID, a list of followers, and a list of posts.
- Uses a builder pattern: the constructor is private, and only UsersBuilder can create Users.
- Ensures user IDs are unique across all Users objects.
- Can store Posts objects inside Users to represent user posts.

Key Features:
- name: string storing the user's name
- id: integer, unique identifier for each user
- followersid: vector<int> storing the IDs of followers
- posts: vector<Posts> storing the posts made by the user
- UsersBuilder: handles creation of Users and ensures unique IDs
- Getter methods for name, ID, followers, and posts
- addFollower(int) and addPost(const Posts&) methods for modifying user data

Usage:
- Include this file in any C++ file that needs to create or manage Users.
- Create users using UsersBuilder, e.g.:
    UsersBuilder builder;
    Users u = builder.CreateUser("Alice", 1);
- Add followers or posts to a user using addFollower and addPost.

Notes:
- Uses include guards to prevent multiple inclusions.
- Includes "Posts.hpp" because Users store Posts.
*/

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


    Users(const string& s, int x) : name(s), id(x) {}

public:
    friend class UsersBuilder;  // Only UsersBuilder can create Users

    string getName() const { return name; }
    int getId() const { return id; }
    const vector<int>& getFollowers() const { return followersid; }
    const vector<Posts>& getPosts() const { return posts; }


    void addFollower(int followerId) { followersid.push_back(followerId); }
    void addPost(const Posts& p) { posts.push_back(p); }
};

class UsersBuilder {
private:
    static unordered_set<int> uniqueIds;  

public:
    Users CreateUser(const string& s, int x) {
        if (!uniqueIds.count(x)) {
            uniqueIds.insert(x);
            return Users(s, x);
        }
        throw runtime_error("Ids must be unique");
    }
};

unordered_set<int> UsersBuilder::uniqueIds;

#endif 

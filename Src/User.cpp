/* 
User.hpp

Purpose:
- Defines the User class representing a social media user.
- Each user has a name, a unique ID, a list of followers, and a list of posts.
- Uses a builder pattern: the constructor is private, and only UserBuilder can create User.
- Ensures user IDs are unique across all User objects.
- Can store Posts objects inside User to represent user posts.

Key Features:
- name: string storing the user's name
- id: integer, unique identifier for each user
- followersid: vector<int> storing the IDs of followers
- posts: vector<Posts> storing the posts made by the user
- UserBuilder: handles creation of User and ensures unique IDs
- Getter methods for name, ID, followers, and posts
- addFollower(int) and addPost(const Posts&) methods for modifying user data

Usage:
- Include this file in any C++ file that needs to create or manage User.
- Create User using UserBuilder, e.g.:
    UserBuilder builder;
    User u = builder.CreateUser("Alice", 1);
- Add followers or posts to a user using addFollower and addPost.

Notes:
- Uses include guards to prevent multiple inclusions.
- Includes "Posts.hpp" because User store Posts.
*/



#include <bits/stdc++.h>
#include "Posts.cpp"
using namespace std;
//***amr reanamed the class to user ***
class User {
private:

    string name;
    int id;
    vector<Post> posts;


    User(const string& s, int x) : name(s), id(x) {}

public:
    User() = default;
    User(const User&) = default;
    User& operator=(const User&) = default;
    ~User() = default;
    friend class UserBuilder;  // Only UserBuilder can create User

    string getName() const { return name; }
    int getId() const { return id; }
    void setname( string s) { this->name=s; }
  
    const vector<Post>& getPosts() const { return posts; }



    void addPost(const Post& p) { posts.push_back(p); }
};

class UserBuilder {
private:
    static unordered_set<int> uniqueIds;  

public:
    User CreateUser(const string& s, int x) {
        if (!uniqueIds.count(x)) {
            uniqueIds.insert(x);
            return User(s, x);
        }
        throw runtime_error("Ids must be unique");
    }
};
//what is this??
unordered_set<int> UserBuilder::uniqueIds;



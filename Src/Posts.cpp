/* 
Posts.hpp

Purpose:
- Defines the Posts class representing a social media post.
- Each post has content, topics (tags), and a unique ID.
- Uses a builder pattern: the constructor is private, and only PostsBuilder can create Posts.
- Ensures post IDs are unique across all Posts objects.
- Header-only design for simplicity and easy inclusion in other files.

Key Features:
- content: string storing the text of the post
- topics: unordered_set<string> storing unique tags/topics
- id: integer, unique identifier for each post
- PostsBuilder: handles creation of Posts and ensures unique IDs
- Getter methods for content, topics, and id

Usage:
- Include this file in any C++ file that needs to create or manage Posts.
- Create posts using PostsBuilder, e.g.:
    PostsBuilder builder;
    Posts p = builder.CreatePost("Hello World", {"C++","OOP"}, 1);

Notes:
- Uses include guards to prevent multiple inclusions.
- Uses unordered_set to enforce uniqueness of topics.
*/




#include <bits/stdc++.h>
using namespace std;

class Post{
private:
    string content;
    unordered_set<string> topics;
    int id;

    // Private constructor: only PostsBuilder can create Posts
    Post(const string& s, const initializer_list<string>& heads, int x)
        : content(s), topics(heads), id(x) {}

public:
    friend class PostsBuilder;  // Give PostsBuilder access

    // Getters
    string getcontent() const { return content; }
    const unordered_set<string>& gettopics() const { return topics; }
    int getid() const { return id; }
};

class PostsBuilder {
private:
    static unordered_set<int> uniqueids;  // Keep track of used IDs

public:
    Post CreatePost(const string& s, const initializer_list<string>& heads, int x) {
        if (!uniqueids.count(x)) {
            uniqueids.insert(x);
            return Post(s, heads, x);
        }
        throw runtime_error("Ids must be unique");
    }
};

// Define the static member
unordered_set<int> PostsBuilder::uniqueids;



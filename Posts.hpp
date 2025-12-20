#ifndef POSTS_HPP
#define POSTS_HPP

#include <bits/stdc++.h>
using namespace std;

/*
Posts.hpp

Purpose:
- Defines the Posts class representing a social media post.
- Each post has content, topics (tags), and a unique ID.
- Uses a builder pattern: only PostsBuilder can create Posts.
- Ensures post IDs are unique across all Posts objects.

Key Features:
- content: string storing the text of the post
- topics: unordered_set<string> storing unique tags/topics
- id: integer, unique identifier for each post
*/

class Posts {
private:
    string content;
    unordered_set<string> topics;
    int id;

    // Private constructor: only PostsBuilder can create Posts
    Posts(const string& s, const initializer_list<string>& heads, int x);

public:
    friend class PostsBuilder;

    Posts() = default;

    // Getters
    string getcontent() const;
    const unordered_set<string>& gettopics() const;
    int getid() const;

    // Modifiers
    void setBody(const string& s);
    bool addTopic(const string& s);
};

class PostsBuilder {
private:
    static unordered_set<int> uniqueids;  // declaration only

public:
    Posts CreatePost(const string& s,
                     const initializer_list<string>& heads,
                     int x);
};



/* ===================== Posts ===================== */

Posts::Posts(const string& s,
             const initializer_list<string>& heads,
             int x)
    : content(s), topics(heads), id(x) {}

string Posts::getcontent() const {
    return content;
}

const unordered_set<string>& Posts::gettopics() const {
    return topics;
}

int Posts::getid() const {
    return id;
}

void Posts::setBody(const string& s) {
    content = s;
}

bool Posts::addTopic(const string& s) {
    return topics.insert(s).second;
}

/* ===================== PostsBuilder ===================== */

// SINGLE definition of the static member
unordered_set<int> PostsBuilder::uniqueids;

Posts PostsBuilder::CreatePost(const string& s,
                               const initializer_list<string>& heads,
                               int x) {
    if (!uniqueids.count(x)) {
        uniqueids.insert(x);
        return Posts(s, heads, x);
    }
    throw runtime_error("Ids must be unique");
}

#endif // POSTS_HPP

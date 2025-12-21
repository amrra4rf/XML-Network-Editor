#ifndef POSTS_HPP
#define POSTS_HPP

#include <bits/stdc++.h>
using namespace std;

/*
Posts.hpp

Purpose:
- Represents a social media post.
- Each post has content, topics (tags), and a unique ID.
- Only PostsBuilder can create Posts.
*/

class Posts {
private:
    string content;
    unordered_set<string> topics;
    int id;

    // Private constructor: only PostsBuilder can call
    Posts(int id, const string& content, const vector<string>& topicsVec)
        : content(content), id(id) {
        for (const auto& t : topicsVec)
            topics.insert(t);
    }

public:
    friend class PostsBuilder;

    Posts() = default;

    // Getters
    string getcontent() const { return content; }
    const unordered_set<string>& gettopics() const { return topics; }
    int getid() const { return id; }

    // Modifiers
    void setBody(const string& s) { content = s; }
    bool addTopic(const string& s) { return topics.insert(s).second; }
};

class PostsBuilder {
private:
    unordered_set<int> usedIds;  // IDs used by this builder
    int nextId = 1;

public:
    void reset() {
        usedIds.clear();
        nextId = 1;
    }

    // Create a post with automatic ID assignment
    Posts CreatePost(const string& content, const vector<string>& topics) {
        while (usedIds.count(nextId)) nextId++;
        int id = nextId;
        usedIds.insert(id);
        return Posts(id, content, topics);
    }
};

#endif // POSTS_HPP

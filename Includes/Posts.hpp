#ifndef Posts_HPP
#define Posts_HPP
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

class PostsBuilder;

class Posts {
private:
    string content;
    unordered_set<string> topics;
    int id;

    Posts(int id, const string& content, const vector<string>& topicsVec);

public:
    friend class PostsBuilder;

    Posts() = default;

    string getcontent() const;
    const unordered_set<string>& gettopics() const;
    int getid() const;

    void setBody(const string& s);
    bool addTopic(const string& s);
};

class PostsBuilder {
private:
    static unordered_set<int> usedIds;
    static int nextId;

public:
    static void reset();
    static Posts CreatePost(const string& content, const vector<string>& topics);
};

#endif
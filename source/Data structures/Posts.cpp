#include "../../Includes\Posts.hpp"

Posts::Posts(int id, const string& content, const vector<string>& topicsVec)
    : content(content), id(id) {
    for (const auto& t : topicsVec)
        topics.insert(t);
}

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

unordered_set<int> PostsBuilder::usedIds;
int PostsBuilder::nextId = 1;

void PostsBuilder::reset() {
    usedIds.clear();
    nextId = 1;
}

Posts PostsBuilder::CreatePost(const string& content, const vector<string>& topics) {
    while (usedIds.count(nextId)) nextId++;
    int id = nextId;
    usedIds.insert(id);
    return Posts(id, content, topics);
}
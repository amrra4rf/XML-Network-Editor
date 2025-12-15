#include "Post.hpp"

/* ===================== Posts ===================== */

Post::Post(const string& s,
             const initializer_list<string>& heads,
             int x)
    : content(s), topics(heads), id(x) {}

string Post::getcontent() const {
    return content;
}

const unordered_set<string>& Post::gettopics() const {
    return topics;
}

int Post::getid() const {
    return id;
}

void Post::setBody(const string& s) {
    content = s;
}

bool Post::addTopic(const string& s) {
    return topics.insert(s).second;
}

/* ===================== PostsBuilder ===================== */

// SINGLE definition of the static member
unordered_set<int> PostsBuilder::uniqueids;

Post PostsBuilder::CreatePost(const string& s,
                               const initializer_list<string>& heads,
                               int x) {
    if (!uniqueids.count(x)) {
        uniqueids.insert(x);
        return Post(s, heads, x);
    }
    throw runtime_error("Ids must be unique");
}
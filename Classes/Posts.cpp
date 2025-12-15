#include "Posts.hpp"

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

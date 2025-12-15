#include "User.hpp"

/* ===================== Users ===================== */

User::User(const string& s, int x)
    : name(s), id(x) {}

string User::getName() const {
    return name;
}

int User::getId() const {
    return id;
}

const vector<int>& User::getFollowers() const {
    return followersid;
}

const vector<Post>& User::getPosts() const {
    return posts;
}

void User::setname(const string& s) {
    name = s;
}

void User::setid(int idx) {
    id = idx;
}

void User::addFollower(int followerId) {
    followersid.push_back(followerId);
}

void User::addPost(const Post& p) {
    posts.push_back(p);
}

/* ===================== UsersBuilder ===================== */

// SINGLE definition
unordered_set<int> UserBuilder::uniqueIds;

User UserBuilder::CreateUser(const string& s, int x) {
    if (!uniqueIds.count(x)) {
        uniqueIds.insert(x);
        return User(s, x);
    }
    throw runtime_error("Ids must be unique");
}
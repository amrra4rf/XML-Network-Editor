#include "User.hpp"

/* ===================== Users Implementation ===================== */

Users::Users(const string& s, int x) : name(s), id(x) {}

string Users::getName() const { return name; }
int Users::getId() const { return id; }
const vector<int>& Users::getFollowers() const { return followersid; }
const vector<Posts>& Users::getPosts() const { return posts; }
vector<Posts>& Users::getPosts() { return posts; }

void Users::setname(const string& s) { name = s; }
void Users::setid(int idx) { id = idx; }
void Users::addFollower(int followerId) { followersid.push_back(followerId); }
void Users::addPost(const Posts& p) { posts.push_back(p); }

/* ===================== UsersBuilder Implementation ===================== */

// CHANGE 2: Removed the static definition that was here:
#include "Graphs.hpp"

Graph::Graph(UsersBuilder &ubuild2)
    : ubuild(ubuild2) {}


// ===========================
//       ADD USER
// ===========================
void Graph::AddUser(Users &user) {
    indexmapper[user.getId()] = users.size();
    users.push_back(user);
    IDtoUser[user.getId()] = user;

    Followers_id.push_back({});
    Following_id.push_back({});
}

void Graph::AddUser(string &name, int id) {
    Users u = ubuild.CreateUser(name, id);
    this->AddUser(u);
}


// ===========================
//   CHECK USER EXISTENCE
// ===========================
void Graph::CheckExistance(const Users &user) const {
    if (IDtoUser.count(user.getId()) == 0) {
        throw runtime_error("Add User first");
    }
}


// ===========================
//        ADD FOLLOWER
// ===========================
bool Graph::AddFollower(Users &tofollow, int followerid) {
    CheckExistance(tofollow);
    int idx = indexmapper[tofollow.getId()];

    Followers_id[idx].push_back(followerid);

    if (indexmapper.count(followerid)) {
        int fidx = indexmapper[followerid];
        Following_id[fidx].push_back(tofollow.getId());
        return true;
    }
    return false;
}


// ===========================
//       ADD FOLLOW (reverse)
// ===========================
bool Graph::AddFollow(Users &follower, int tofollowid) {
    CheckExistance(follower);
    int followeridx = indexmapper[follower.getId()];

    Following_id[followeridx].push_back(tofollowid);

    if (indexmapper.count(tofollowid)) {
        int tIdx = indexmapper[tofollowid];
        Followers_id[tIdx].push_back(follower.getId());
        return true;
    }
    return false;
}


// ===========================
//          ADD POST
// ===========================
void Graph::AddPost(Posts &p, Users &writer) {
    CheckExistance(writer);
    int idx = indexmapper[writer.getId()];
    users[idx].addPost(p);
}


// ===========================
//        GET FOLLOWERS
// ===========================
vector<int> Graph::GetFollowers(Users &user) {
    CheckExistance(user);
    int idx = indexmapper[user.getId()];
    return Followers_id.at(idx);
}

vector<int> Graph::GetFollowers(int &userid) {
    return this->GetFollowers(IDtoUser[userid]);
}


// ===========================
//        GET FOLLOWING
// ===========================
vector<int> Graph::Getfollowing(Users &user) {
    CheckExistance(user);
    int idx = indexmapper[user.getId()];
    return Following_id.at(idx);
}

vector<int> Graph::FetDollowing(int &userid) {
    return this->Getfollowing(IDtoUser[userid]);
}
int Graph::getnodeid(Users&u){
    CheckExistance(u);
    return indexmapper[u.getId()];
}
int Graph::getnodeid(int&uid){
    CheckExistance(IDtoUser[uid]);
    return indexmapper[uid];
}

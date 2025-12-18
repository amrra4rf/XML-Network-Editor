#include "Graph.hpp"

using namespace std;

Graph::Graph(UserBuilder& ubuild2) : ubuild(ubuild2) {}

int Graph::getNodeIndex(int uid) const {
    auto it = indexmapper.find(uid);
    if (it == indexmapper.end()) {
        throw runtime_error("User id not found in indexmapper: " + to_string(uid));
    }
    return it->second;
}

bool Graph::AddUser(User& newUser) {
    int uid = newUser.getId();

    if (IDtoUser.find(uid) != IDtoUser.end()) {
        User& mapRef = IDtoUser[uid];
        mapRef.setname(newUser.getName());

        if (indexmapper.find(uid) == indexmapper.end())
            throw runtime_error("Inconsistent state: ID exists in map but not in indexmapper");

        int idx = indexmapper.at(uid);
        if (idx < 0 || idx >= (int)users.size())
            throw runtime_error("Index out of range for existing user");

        users[idx].setname(newUser.getName());
        return false;
    }

    int idx = users.size();
    indexmapper[uid] = idx;
    users.push_back(newUser);
    IDtoUser[uid] = newUser;

    Followers_id.push_back({});
    Following_id.push_back({});

    return true;
}

bool Graph::AddUser(string& name, int id) {
    User u = ubuild.CreateUser(name, id);
    return this->AddUser(u);
}

void Graph::CheckExistance(const User& user) const {
    if (IDtoUser.count(user.getId()) == 0) {
        throw runtime_error("Add User first");
    }
}

bool Graph::AddFollower(User& tofollow, int followerid) {
    CheckExistance(tofollow);
    int idx = getNodeIndex(tofollow.getId());
    Followers_id[idx].push_back(followerid);

    if (indexmapper.find(followerid) != indexmapper.end()) {
        int fidx = indexmapper.at(followerid);
        Following_id[fidx].push_back(tofollow.getId());
        return true;
    }

    User u = ubuild.CreateUser("user not nammed yet", followerid);
    int newIdx = users.size();
    indexmapper[followerid] = newIdx;
    users.push_back(u);
    IDtoUser[followerid] = u;

    Followers_id.push_back({});
    Following_id.push_back({});

    Following_id[newIdx].push_back(tofollow.getId());
    return false;
}

bool Graph::AddFollow(User& follower, int tofollowid) {
    CheckExistance(follower);
    int followeridx = getNodeIndex(follower.getId());
    Following_id[followeridx].push_back(tofollowid);

    if (indexmapper.find(tofollowid) != indexmapper.end()) {
        int tidx = indexmapper.at(tofollowid);
        Followers_id[tidx].push_back(follower.getId());
        return true;
    }

    User u = ubuild.CreateUser("Add Name to user", tofollowid);
    int newIdx = users.size();
    indexmapper[tofollowid] = newIdx;
    users.push_back(u);
    IDtoUser[tofollowid] = u;

    Followers_id.push_back({});
    Following_id.push_back({});
    Followers_id[newIdx].push_back(follower.getId());
    return false;
}

void Graph::AddPost(Post& p, User& writer) {
    CheckExistance(writer);
    int idx = indexmapper[writer.getId()];
    users[idx].addPost(p);

    //added by (amr)
    //this maps all the posts to it's topics 
    unordered_set<string> temp=p.gettopics();
    for(string topic :temp)
    {
        topicIndex[topic].push_back(&p);
    }
    
}

vector<int> Graph::GetFollowers(User& user) {
    CheckExistance(user);
    int idx = indexmapper[user.getId()];
    return Followers_id.at(idx);

 
}
//made it int userid instead of int& userid by amr 
vector<int> Graph::GetFollowers(int userid) {
    return this->GetFollowers(IDtoUser[userid]);
}

vector<int> Graph::Getfollowing(User& user) {
    CheckExistance(user);
    int idx = indexmapper[user.getId()];
    return Following_id.at(idx);
}


//made it int userid instead of int& userid by amr 
vector<int> Graph::getfollowing(int userid) {
    return this->Getfollowing(IDtoUser[userid]);
}



bool Graph::isFollowing(int followerId, int followeeId)  {
    // check if both users exist or not
    if (indexmapper.find(followerId) == indexmapper.end()) return false;
    if (indexmapper.find(followeeId) == indexmapper.end()) return false;

    // get the list of users that followerId is following now
    int followerIdx = indexmapper[followerId];
    const vector<int>& following = Following_id[followerIdx];

    
    if(find(following.begin(), following.end(), followeeId) != following.end())
    return true;
    return false;
}





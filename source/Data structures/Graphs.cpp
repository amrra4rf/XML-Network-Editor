#include "../../Includes/Graphs.hpp"

using namespace std;

int Graph::getNodeIndex(int uid) const {
    auto it = indexmapper.find(uid);
    if (it == indexmapper.end())
        throw runtime_error("User id not found: " + to_string(uid));
    return it->second;
}

bool Graph::AddUser(Users& user) {
    int uid = user.getId();
    if (IDtoUser.count(uid)) {
        int idx = indexmapper[uid];
        users[idx] = user;
        IDtoUser[uid] = user;
        return false;
    }

    int idx = users.size();
    users.push_back(user);
    IDtoUser[uid] = user;
    indexmapper[uid] = idx;

    Followers_id.push_back({});
    Following_id.push_back({});
    return true;
}

bool Graph::AddUser(const std::string& name, int id) {
    Users u = ubuild.CreateUser(name, id);
    return AddUser(u);
}

void Graph::CheckExistance(const Users& user) const {
    if (!IDtoUser.count(user.getId()))
        throw runtime_error("User does not exist");
}

bool Graph::AddFollower(Users& tofollow, int followerid) {
    CheckExistance(tofollow);
    int idx = getNodeIndex(tofollow.getId());
    Followers_id[idx].push_back(followerid);

    if (indexmapper.count(followerid)) {
        int fidx = indexmapper.at(followerid);
        Following_id[fidx].push_back(tofollow.getId());
        return true;
    }

    Users u = ubuild.CreateUser("Unknown", followerid);
    int newIdx = users.size();
    users.push_back(u);
    IDtoUser[followerid] = u;
    indexmapper[followerid] = newIdx;

    Followers_id.push_back({});
    Following_id.push_back({});
    Following_id[newIdx].push_back(tofollow.getId());
    return false;
}

bool Graph::AddFollow(Users& follower, int tofollowid) {
    CheckExistance(follower);
    int followerIdx = getNodeIndex(follower.getId());
    Following_id[followerIdx].push_back(tofollowid);

    if (indexmapper.count(tofollowid)) {
        int tidx = indexmapper.at(tofollowid);
        Followers_id[tidx].push_back(follower.getId());
        return true;
    }

    Users u = ubuild.CreateUser("Unknown", tofollowid);
    int newIdx = users.size();
    users.push_back(u);
    IDtoUser[tofollowid] = u;
    indexmapper[tofollowid] = newIdx;

    Followers_id.push_back({});
    Following_id.push_back({});
    Followers_id[newIdx].push_back(follower.getId());
    return false;
}

void Graph::AddPost(Posts& p, Users& writer) {
    CheckExistance(writer);
    int idx = getNodeIndex(writer.getId());
    users[idx].addPost(p);

    // Add to topic index
    for (const string& t : p.gettopics()) {
        string lowerTopic = t;
        // Trim whitespace
        lowerTopic.erase(lowerTopic.begin(), find_if(lowerTopic.begin(), lowerTopic.end(), [](unsigned char ch){ return !isspace(ch); }));
        lowerTopic.erase(find_if(lowerTopic.rbegin(), lowerTopic.rend(), [](unsigned char ch){ return !isspace(ch); }).base(), lowerTopic.end());
        // Lowercase
        transform(lowerTopic.begin(), lowerTopic.end(), lowerTopic.begin(), ::tolower);
        // Push post pointer
        topicIndex[lowerTopic].push_back(&users[idx].getPosts().back());
    }
}

vector<int> Graph::GetFollowers(const Users& user) {
    CheckExistance(user);
    int idx = getNodeIndex(user.getId());
    return Followers_id[idx];
}

vector<int> Graph::GetFollowers(int userid) {
    return GetFollowers(IDtoUser.at(userid));
}

vector<int> Graph::GetFollowing(const Users& user) {
    CheckExistance(user);
    int idx = getNodeIndex(user.getId());
    return Following_id[idx];
}

vector<int> Graph::GetFollowing(int userid) {
    return GetFollowing(IDtoUser.at(userid));
}

// Return reference instead of copy
Users& Graph::getUserFromID(int id) {
    return IDtoUser.at(id);
}

bool Graph::isFollowing(int userId, int targetId) const {
    if (!indexmapper.count(userId)) return false;
    int idx = indexmapper.at(userId);
    const auto& following = Following_id[idx];
    return std::find(following.begin(), following.end(), targetId) != following.end();
}

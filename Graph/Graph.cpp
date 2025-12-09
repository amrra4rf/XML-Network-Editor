#include <bits/stdc++.h>
#include "Classes/Posts.hpp"
#include "Classes/User.hpp"

using namespace std;

class Graph{
    private:
        vector<Users>users;
        unordered_map<int,Users>IDtoUser;
        unordered_map<int,int>indexmapper;
        vector<vector<int>>Followers_id;
        vector<vector<int>>Following_id;
        UsersBuilder ubuild;
   
        int getNodeIndex(int uid) const {
            auto it = indexmapper.find(uid);
            if (it == indexmapper.end()) {
                throw runtime_error("User id not found in indexmapper: " + to_string(uid));
            }
            return it->second;
        }

    public:
        Graph()=default;
        Graph(UsersBuilder&ubuild2):ubuild(ubuild2){};
       bool AddUser(Users &user) {
            int uid = user.getId();
            if (IDtoUser.find(uid) != IDtoUser.end()) {
                Users &mapRef = IDtoUser[uid];      
                mapRef.setname(user.getName());

                if (indexmapper.find(uid) == indexmapper.end())
                    throw runtime_error("Inconsistent state: ID exists in map but not in indexmapper");

                int idx = indexmapper.at(uid);
                if (idx < 0 || idx >= (int)users.size())
                    throw runtime_error("Index out of range for existing user");

                users[idx].setname(user.getName());
                return false;
    }

                int idx = users.size();
                indexmapper[uid] = idx;
                users.push_back(user);
                IDtoUser[uid] = user;        
                Followers_id.push_back({});
                Following_id.push_back({});
                return true;
            }



        bool AddUser(string&name,int id){
            Users u=ubuild.CreateUser(name,id);
            return this->AddUser(u);
        }
        void CheckExistance(const Users&user)const{
            if(IDtoUser.count(user.getId())==0){
                throw runtime_error("Add User first");
            }

        }
      bool AddFollower(Users &tofollow, int followerid) {
            CheckExistance(tofollow);
            int idx = getNodeIndex(tofollow.getId());
            Followers_id[idx].push_back(followerid); 

            if (indexmapper.find(followerid) != indexmapper.end()) {
                int fidx = indexmapper.at(followerid);
                Following_id[fidx].push_back(tofollow.getId());
                return true;
            }

            Users u = ubuild.CreateUser("Add Name to user", followerid);
            int newIdx = users.size();
            indexmapper[followerid] = newIdx;
            users.push_back(u);
            IDtoUser[followerid] = u;
            Followers_id.push_back({});
            Following_id.push_back({});

            // now add edge using new index
            Following_id[newIdx].push_back(tofollow.getId());
            return false;
}


       bool AddFollow(Users &follower, int tofollowid) {
            CheckExistance(follower);
            int followeridx = getNodeIndex(follower.getId());
            Following_id[followeridx].push_back(tofollowid);

            if (indexmapper.find(tofollowid) != indexmapper.end()) {
                int tidx = indexmapper.at(tofollowid);
                Followers_id[tidx].push_back(follower.getId());
                return true;
            }

            Users u = ubuild.CreateUser("Add Name to user", tofollowid);
            int newIdx = users.size();
            indexmapper[tofollowid] = newIdx;
            users.push_back(u);
            IDtoUser[tofollowid] = u;
            Followers_id.push_back({});
            Following_id.push_back({});
            Followers_id[newIdx].push_back(follower.getId());
            return false;
}

        void AddPost(Posts&p ,Users&writer ){
            CheckExistance(writer);
            int idx = indexmapper[writer.getId()];
            users[idx].addPost(p);      // modify inside graph
        }
        vector<int>GetFollowers(Users&user){
            CheckExistance(user);
            int idx = indexmapper[user.getId()];
            return Followers_id.at(idx);
        }
        vector<int>GetFollowers(int&userid){
            return this->GetFollowers(IDtoUser[userid]);
        }
        vector<int>Getfollowing(Users&user){
            CheckExistance(user);
            int idx = indexmapper[user.getId()];
            return Following_id.at(idx);
        }
        vector<int>FetDollowing(int&userid){
            return this->Getfollowing(IDtoUser[userid]);
        }
        

};
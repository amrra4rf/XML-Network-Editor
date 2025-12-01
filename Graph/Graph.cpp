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
    public:
        Graph()=default;
        Graph(UsersBuilder&ubuild2):ubuild(ubuild2){};
        void AddUser(Users&user){
            indexmapper[user.getId()]=users.size();
            users.push_back(user);
            IDtoUser[user.getId()]=user;
            Followers_id.push_back({});
            Following_id.push_back({});
        }
        void AddUser(string&name,int id){
            Users u=ubuild.CreateUser(name,id);
            this->AddUser(u);
        }
        void CheckExistance(const Users&user)const{
            if(IDtoUser.count(user.getId())==0){
                throw runtime_error("Add User first");
            }

        }
        bool AddFollower(Users&tofollow,int followerid){
            CheckExistance(tofollow);
            int idx = indexmapper[tofollow.getId()];
            Followers_id[idx].push_back(followerid);
            if(indexmapper.count(followerid)){
                int fidx=indexmapper[followerid];
                Following_id[fidx].push_back(tofollow.getId());
                return true;
            }
            return false;
            
        }
        bool AddFollow(Users&follower,int tofollowid){
            CheckExistance(follower);
            int followeridx = indexmapper[follower.getId()];
            Following_id[followeridx].push_back(tofollowid);
            if(indexmapper.count(tofollowid)){
                Followers_id[tofollowid].push_back(follower.getId());
                return true;
            }
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
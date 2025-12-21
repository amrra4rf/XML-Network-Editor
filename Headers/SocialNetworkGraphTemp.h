#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <climits>
#include "Graphs.hpp" 
#include "Readers.hpp" 

using namespace std;

class SocialNetworkGraph {
private:
    Graph Network;


public:
    SocialNetworkGraph(const string& filePath) {
        Reader *r=new Reader (filePath) ;
        r->read();
        Network=r->getnet();
    }

    // Constructor
    SocialNetworkGraph(Graph n) {
        Network = n;
    }

    // --- Influencer & Activity Logic ---


    Users getInfluencer() {
        int maxFollowers = INT_MIN;
        int maxFollowersId = INT_MIN;
        for (int i = 0; i < Network.users.size(); i++) {
            int userID = Network.users[i].getId();
            // followers is a vector so iam trying to get the size of the vector to get the number of followers
            int userFollowers = Network.GetFollowers(userID).size();
            if (userFollowers > maxFollowers) {
                maxFollowers = userFollowers;
                maxFollowersId = userID;
            }
        }
        if (maxFollowersId != -1)
            return Network.IDtoUser[maxFollowersId]; // return the user object of the most followed person
        else
            return Users(); // returns and empty user object 
    }

    Users getMostActiveUser() {
        int maxFollowing = INT_MIN;
        int maxFollowingId = INT_MIN;
        for (int i = 0; i < Network.users.size(); i++) {
            int userID = Network.users[i].getId();
            // using the naming from your snippet: Getfollowing
            int userFollowings = Network.Getfollowing(userID).size(); 
            if (userFollowings > maxFollowing) {
                maxFollowing = userFollowings;
                maxFollowingId = userID;
            }
        }
        if (maxFollowingId != INT_MIN)
            return Network.IDtoUser[maxFollowingId]; // return the user object of the most active person
        else
            return Users(); // returns and empty user object 
    }

    // --- Mutual Followers ---

    vector<Users> getMutualFollowers(vector<int> goal) {
        if (goal.size() <= 0) return {};
        vector<Users> mutualFollowers;
        
        // first make sure that the goal id's are actually existing id's
        for (int i = 0; i < goal.size(); i++) {
            if (!Network.IDtoUser.count(goal[i])) {
                cout << "Invalid user id: " << goal[i] << " Plz enter a valid id" << endl;
                return {};
            }
        }

        map<int, int> freq; // the key is the id and the value is how many times this id has appeared 
        vector<vector<int>> allGivenFollowers;
        for (int i = 0; i < goal.size(); i++) {
            // this puts all the followers of the given id's into a 2d vector 
            allGivenFollowers.push_back(Network.GetFollowers(Network.IDtoUser[goal[i]]));
        }

        for (auto row : allGivenFollowers) {
            unordered_set<int> seen;
            for (int element : row) {
                seen.insert(element);
            }
            for (int elementInSet : seen) {
                freq[elementInSet]++;
            }
        }

        for (auto id : freq) {
            if (freq[id.first] == goal.size()) {
                mutualFollowers.push_back(Network.IDtoUser[id.first]);
            }
        }
        return mutualFollowers;
    }

    // --- Post Searching ---

    vector<Posts> getPostsByWord(string word) {
        vector<Posts> answer;
        for (int i = 0; i < Network.users.size(); i++) {
            const vector<Posts>& tempPosts = Network.users[i].getPosts();
            for (int j = 0; j < tempPosts.size(); j++) {
                if (tempPosts[j].getcontent().find(word) != string::npos)
                    answer.push_back(tempPosts[j]);
            }
        }
        return answer;
    }

    vector<Posts> getPostsByTopic(string topic) {
        vector<Posts> answer;
        for (int i = 0; i < Network.users.size(); i++) {
            const vector<Posts>& tempPosts = Network.users[i].getPosts();
            for (int j = 0; j < tempPosts.size(); j++) {
                const unordered_set<string>& tempTopics = tempPosts[j].gettopics();
                if (tempTopics.find(topic) != tempTopics.end())
                    answer.push_back(tempPosts[j]);
            }
        }
        return answer;
    }

    vector<Posts> getPostsByTopicFastAlgorithm(string topic) {
        vector<Posts> answer;
        if (!Network.topicIndex.count(topic)) return {};
        for (auto post : Network.topicIndex[topic]) {
            answer.push_back(*post);
        }
        return answer;
    }

    // --- Suggestions ---

    void printSuggestions(int id, unordered_set<int>& suggestions) {
        cout << "Suggestion for '" << Network.IDtoUser[id].getName() << "' :" << endl;
        for (int suggestId : suggestions) {
            cout << Network.IDtoUser[suggestId].getName() << endl;
        }
    }

    unordered_map<int, vector<Users>> getSuggestions() 
    {
        unordered_map<int, vector<Users>> allSuggestions;
        for (auto &u : Network.users) {
            vector<Users> tempUsers;
            auto followersLevel1 = Network.GetFollowers(u.getId());
            for (int followerId : followersLevel1) {
                auto followerOfFolowers = Network.GetFollowers(followerId);
                
                for (int idOfFollowersOfFollowers : followerOfFolowers) {
                    if (idOfFollowersOfFollowers != u.getId() && !Network.isFollowing(u.getId(), idOfFollowersOfFollowers)) {
                       
                        tempUsers.push_back(Network.getuserfromID(idOfFollowersOfFollowers));
                    }
                }
            }
            
            allSuggestions[u.getId()]=tempUsers;
     
        }
        return allSuggestions;
    }
};
#include "SocialNetworkGraph.hpp"

User SocialNetworkGraph::getInfluencer()
{
    int maxFollowers=INT_MIN;
    int maxFollowersId=INT_MIN;
    for(int i=0;i<Network.users.size();i++)
    {
        int userID=Network.users[i].getId();
        int userFollowers=Network.GetFollowers(userID).size();//followers is a vector so iam trying to get the size of the vector to get the number of followers
        if(userFollowers>maxFollowers)
        {
            maxFollowers=userFollowers;
            maxFollowersId=userID;
        }
    }
    if(maxFollowersId!=-1)
     return Network.IDtoUser[maxFollowersId];//return the user object of the most followed person
    else 
        return User();//rturns and empty user object 
}



User SocialNetworkGraph::getMostActiveUser()
{
    int maxFollowing=INT_MIN;
    int maxFollowingId=INT_MIN;
    for(int i=0;i<Network.users.size();i++)
    {
        int userID=Network.users[i].getId();

        // is this ok or should it be int userFollowings=Following_id[i].size();
        int userFollowings=Network.getfollowing(userID).size();//following is a vector so iam trying to get the size of the vector to get the number of followers            
        if(userFollowings>maxFollowing)
        {
            maxFollowing=userFollowings;
            maxFollowingId=userID;
        }
    }
    
    if(maxFollowingId!=INT_MIN)
     return Network.IDtoUser[maxFollowingId];//return the user object of the most active person
    else 
        return User();//rturns and empty user object 
}

vector<User>SocialNetworkGraph:: getMutualFollowers(vector<int> goal)
{
    if(goal.size()<=0 )return {};
    vector<User>mutualFollowers;
    //first make sure that the goal id's are actually existing id's
    for(int i=0;i<goal.size();i++)
    {
        if(!Network.IDtoUser.count(goal[i]))
        {
            //there is an invalid user id so return empty vector
            cout<<"Invalid user id: "<<goal[i]<<"Plz enter a valid id"<<endl;
            return {};
        }
    }

    //now we r sure the id's do exist
    map<int,int> freq;//the key is the id and the value is how many times this id has appeared 
    vector<vector<int>>allGivenFollowers;
    for(int i=0;i<goal.size();i++)
    {
        allGivenFollowers.push_back(Network.GetFollowers(Network.IDtoUser[goal[i]]));//this puts all the followers of the given id's into a 2d vector 
    }
   
    for(auto row :allGivenFollowers)
    {
        unordered_set<int> seen;
        for(int element:row)
        {
            seen.insert(element);
        }
        for(int elementInSet: seen)
        {
            //is it validated that the id's of the follower are for valid users

            freq[elementInSet]++;
        }
    }
    //store all the users that follow all the give id's in a vector to be returned
    for(auto id :freq)
    {
        if(freq[id.first]==goal.size())
        {
            mutualFollowers.push_back(Network.IDtoUser[id.first]);
        }
    }
    return mutualFollowers;
    
}

vector<Post> SocialNetworkGraph::getPostsByWord(string word)
{
    vector<Post> answer;
    
    for(int i=0;i<Network.users.size();i++)
    {
        const vector<Post>& tempPosts = Network.users[i].getPosts();
        for(int j=0;j<tempPosts.size();j++)
        {
            //post found:
            if(tempPosts[j].getcontent().find(word)!=string::npos)
            answer.push_back(tempPosts[j]);
        }
    }
    return answer;
}

//slow algorthim
vector<Post> SocialNetworkGraph::getPostsByTopic(string topic)
{
    vector<Post> answer;
    for(int i=0;i<Network.users.size();i++)
    {
         const vector<Post>& tempPosts = Network.users[i].getPosts();
        for(int j=0;j<tempPosts.size();j++)
        {
           const unordered_set<string>& tempTopics=tempPosts[j].gettopics();
           if(tempTopics.find(topic)!=tempTopics.end())
                answer.push_back(tempPosts[j]);
          
        }
    }
    return answer;
}


//fast algorithm using maps 

vector<Post> SocialNetworkGraph::getPostsByTopicFastAlgorithm(string topic)
{
    vector<Post> answer;
    //no post with this topic so return empty vector 
    if(!Network.topicIndex.count(topic))return {};
    for(auto post :Network.topicIndex[topic])
    {
        answer.push_back(*post);
    }
    return answer;
}


void SocialNetworkGraph::printSuggestions(int id,unordered_set<int>& suggestions)
{
    cout<<"Suggestion for '"<<Network.IDtoUser[id].getName()<<"' :"<<endl;
    for(int suggestId :suggestions)
    {
        cout<<Network.IDtoUser[suggestId].getName()<<endl;
    }
}

void SocialNetworkGraph::getSuggestions()
{
  for(auto &u:Network.users)
  {
    unordered_set<int> suggestions;
        //level 1 followers
        auto followersLevel1= Network.GetFollowers(u.getId());
        for(int followerId: followersLevel1)
        {
            //level 2 followers
            auto followerOfFolowers=Network.GetFollowers(followerId);
                for (int idOfFollowersOfFollowers : followerOfFolowers) 
                {
                    if (idOfFollowersOfFollowers != u.getId() && !Network.isFollowing(u.getId(), idOfFollowersOfFollowers)) {
                        suggestions.insert(idOfFollowersOfFollowers);
                    }
                }
        }
      printSuggestions(u.getId(),suggestions);  
   }
}

#ifndef Reader_HPP
#define Reader_HPP

#include <bits/stdc++.h>
#include "./Graphs.hpp"
#include "./Posts.hpp"
#include "./User.hpp"

class Reader {
private:
    std::string filename;
    Graph network;
    vector<int> users_ids;

    std::string extractValue(const std::string& line);
    string strip(string&s);

public:
    Reader(const std::string& file);
    void read();
    Graph getnet();
    vector<int> getusers();
};


using namespace std;

Reader::Reader(const string& file)
    : filename(file) {}

string Reader::extractValue(const string& line) {
    size_t start = line.find('>') + 1;
    size_t end = line.rfind('<');
    return line.substr(start, end - start);
}

string Reader::strip(string& s) {
    while (!s.empty() && s.back() == ' ')
        s.pop_back();


    while (!s.empty() && s.front() == ' ')
        s.erase(s.begin());

    return s;
}

void Reader::read() {
    ifstream xml(filename);
    if (!xml.is_open())
        throw runtime_error("Cannot open XML file");

    string line;

    int currentUserId = -1;
    string currentUserName;

    bool inUser = false;
    bool inFollowers = false;
    bool inPost = false;

    Posts currentPost;
    Users u;

    while (getline(xml, line)) {

        // -------- USER START --------
        if (line.find("<user>") != string::npos) {
            inUser = true;
            currentUserId = -1;
            currentUserName.clear();
            u = Users();
        }

        // -------- USER ID --------
        else if (line.find("<id>") != string::npos && inUser && !inFollowers) {
            getline(xml,line);
            currentUserId = stoi(strip(line));
            u.setid(currentUserId);
            users_ids.push_back(currentUserId);
        }

        // -------- USER NAME --------
        else if (line.find("<name>") != string::npos) {
            getline(xml,line);
            currentUserName = strip(line);
            u.setname(currentUserName);
        }

        // -------- USER END --------
        else if (line.find("</user>") != string::npos) {
            network.AddUser(u);
            inUser = false;
        }

        // -------- POSTS --------
        else if (line.find("<post>") != string::npos) {
            inPost = true;
            currentPost = Posts();
        }

        else if (line.find("<body>") != string::npos && inPost) {
            getline(xml,line);
            currentPost.setBody(strip(line));
        }

        else if (line.find("<topic>") != string::npos && inPost) {
            getline(xml,line);
            currentPost.addTopic(strip(line));
        }

        else if (line.find("</post>") != string::npos) {
            u.addPost(currentPost);
            inPost = false;
        }

        // -------- FOLLOWERS --------
        else if (line.find("<followers>") != string::npos) {
            inFollowers = true;
        }

        else if (line.find("</followers>") != string::npos) {
            inFollowers = false;
        }

        else if (line.find("<follower>") != string::npos && inFollowers) {
            getline(xml , line);
            getline(xml ,line);
             // <id>X</id>
            int followerId = stoi(strip(line));
            u.addFollower(followerId);
        }
    }
}

Graph Reader::getnet(){return this->network;}

vector<int> Reader::getusers(){return this->users_ids;}

#endif

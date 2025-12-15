#include "Reader.hpp"

using namespace std;

Reader::Reader(const string& file)
    : filename(file) {}

string Reader::extractValue(const string& line) {
    size_t start = line.find('>') + 1;
    size_t end = line.rfind('<');
    return line.substr(start, end - start);
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
            u=Users();
        }

        // -------- USER ID --------
        else if (line.find("<id>") != string::npos && inUser && !inFollowers) {
            currentUserId = stoi(extractValue(line));
            u.setid(currentUserId);
        }

        // -------- USER NAME --------
        else if (line.find("<name>") != string::npos) {
            currentUserName = extractValue(line);
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
            currentPost.setBody(extractValue(line));
        }

        else if (line.find("<topic>") != string::npos && inPost) {
            currentPost.addTopic(extractValue(line));
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
            getline(xml, line); // <id>X</id>
            int followerId = stoi(extractValue(line));
             u.addFollower(followerId);
        }
    }
}
Graph Reader::getnet(){return this->network;}


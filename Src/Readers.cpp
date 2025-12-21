#include "Readers.hpp"

using namespace std;

 
#include "Readers.hpp"
#include <stdexcept>

int Reader::stringToInt(const std::string& s) {
    if (s.empty())
        throw std::invalid_argument("Cannot convert empty string to int");

    int num = 0;
    int sign = 1;
    size_t i = 0;

    // Handle negative numbers
    if (s[0] == '-') {
        sign = -1;
        i = 1;
    }

    for (; i < s.size(); i++) {
        if (s[i] < '0' || s[i] > '9')
            throw std::invalid_argument("Invalid character in integer string");
        num = num * 10 + (s[i] - '0');
    }

    return num * sign;
}

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
            u=Users();
        }

        // -------- USER ID --------
        else if (line.find("<id>") != string::npos && inUser && !inFollowers) {
            getline(xml,line);
            currentUserId = stoi(strip(line));
            u.setid(currentUserId);
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
            getline(xml, line);
            getline(xml,line);
             // <id>X</id>
            int followerId = stoi(strip(line));
             u.addFollower(followerId);
        }
    }
}
Graph Reader::getnet(){return this->network;}


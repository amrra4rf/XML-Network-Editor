#include "Reader.hpp"
using namespace std;

Reader::Reader(const string& file)
    : filename(file) {}

string Reader::strip(string& s) {
    while (!s.empty() && isspace(s.back()))
        s.pop_back();
    while (!s.empty() && isspace(s.front()))
        s.erase(s.begin());
    return s;
}

void Reader::read(PostsBuilder& builder) {
    ifstream xml(filename);
    if (!xml.is_open())
        throw runtime_error("Cannot open XML file");

    users_ids.clear();

    string line;
    bool inUser = false;
    bool inFollowers = false;
    bool inPost = false;

    Users u;
    Posts currentPost;

    while (getline(xml, line)) {

        if (line.find("<user>") != string::npos) {
            inUser = true;
            u = Users();
        }
        else if (line.find("<id>") != string::npos && inUser && !inFollowers) {
            getline(xml, line);
            int uid = stoi(strip(line));
            u.setid(uid);
            users_ids.push_back(uid);
        }
        else if (line.find("<name>") != string::npos && inUser) {
            getline(xml, line);
            u.setname(strip(line));
            network.AddUser(u);
        }
        else if (line.find("<post>") != string::npos) {
            inPost = true;
            currentPost = Posts(); // reset current post
        }
        else if (line.find("<body>") != string::npos && inPost) {
            getline(xml, line);
            currentPost.setBody(strip(line));
        }
        else if (line.find("<topic>") != string::npos && inPost) {
            getline(xml, line);
            string topic = strip(line);
            transform(topic.begin(), topic.end(), topic.begin(), ::tolower);
            currentPost.addTopic(topic);
        }
        else if (line.find("</post>") != string::npos && inPost) {
            // create post via builder
           currentPost = builder.CreatePost(currentPost.getcontent(), 
                                 vector<string>(currentPost.gettopics().begin(), currentPost.gettopics().end()));
            network.AddPost(currentPost, u);
            inPost = false;
        }
        else if (line.find("<followers>") != string::npos) {
            inFollowers = true;
        }
        else if (line.find("</followers>") != string::npos) {
            inFollowers = false;
        }
        else if (line.find("<follower>") != string::npos && inFollowers) {
            getline(xml, line); // skip <id>
            getline(xml, line); // actual id
            int followerId = stoi(strip(line));
            network.AddFollower(u, followerId);
        }
        else if (line.find("</user>") != string::npos) {
            inUser = false;
        }
    }
}

Graph& Reader::getnet() { return network; }

vector<int> Reader::getusers() { return users_ids; }
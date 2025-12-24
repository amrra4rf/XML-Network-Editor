#include "../../Includes/CLI_h.hpp"
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include "../../Includes\visualize.hpp"

using namespace std;

void visualize(const string& inputPath, const string& outputPath, PostsBuilder& builder)
{
    Graph g;
    Reader reader(inputPath);

    builder.reset();
    reader.read(builder);

    g = reader.getnet();
    vector<int> userIds = reader.getusers();

    ofstream dot(outputPath + "/network.dot");
    if (!dot.is_open()) return;

    dot << "digraph SocialNetwork {\n";
    dot << "    rankdir=LR;\n";
    dot << "    node [fontname=\"Arial\"];\n";
    dot << "    edge [fontname=\"Arial\"];\n\n";

    // =========================
    // 1. USERS
    // =========================
    dot << "    // Users\n";
    dot << "    node [shape=ellipse, style=filled, fillcolor=lightblue];\n";

    for (int uid : userIds) {
        Users& u = g.getUserFromID(uid);

        dot << "    user_" << u.getId()
            << " [label=\""
            << u.getName()
            << "\\n(ID: " << u.getId() << ")\"];\n";
    }

    // =========================
    // 2. FOLLOWERS (Directed)
    // =========================
    dot << "\n    // Followers (directed)\n";

    for (int uid : userIds) {
        vector<int> followers = g.GetFollowers(uid);
        for (int fid : followers) {
            dot << "    user_" << fid
                << " -> user_" << uid
                << " [label=\"follows\"];\n";
        }
    }

    // =========================
    // 3. POSTS & TOPICS
    // =========================
    dot << "\n    // Posts\n";
    dot << "    node [shape=box, style=filled, fillcolor=lightyellow];\n";

    int postCounter = 0;

    // IMPORTANT: iterate over Graph's internal users
    for (auto& u : g.getUsers()) {
        const vector<Posts>& posts = u.getPosts();

        for (const auto& p : posts) {
            string postNode = "post_" + to_string(postCounter++);

            // Post node
            dot << "    " << postNode
                << " [label=\"Post\\n"
                << p.getcontent()
                << "\"];\n";

            dot << "    user_" << u.getId()
                << " -> " << postNode
                << " [label=\"writes\"];\n";

            // Topics
            for (string t : p.gettopics()) {
                // Trim leading/trailing spaces
                t.erase(t.begin(),
                        find_if(t.begin(), t.end(),
                                [](unsigned char c){ return !isspace(c); }));
                t.erase(find_if(t.rbegin(), t.rend(),
                                [](unsigned char c){ return !isspace(c); }).base(),
                        t.end());

                string topicNode = postNode + "_topic_" + t;

                // Graphviz-safe name
                for (char& c : topicNode)
                    if (isspace(c)) c = '_';

                dot << "    " << topicNode
                    << " [label=\"" << t
                    << "\", shape=note, style=filled, fillcolor=lightgreen];\n";

                dot << "    " << postNode
                    << " -> " << topicNode
                    << " [label=\"has\"];\n";
            }
        }
    }

    dot << "}\n";
    dot.close();

    // Generate PNG
    string cmd = "dot -Tpng \"" + outputPath +
                 "/network.dot\" -o \"" +
                 outputPath + "/network.jpg\"";
    system(cmd.c_str());
}

void visualize(const string& inputPath, const string& outputPath)
{
    PostsBuilder builder;
    visualize(inputPath, outputPath, builder);
}

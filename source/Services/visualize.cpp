#include "../../Includes/CLI_h.hpp"
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include "../../Includes/visualize.hpp"

using namespace std;

void visualize(const string& inputPath, const string& outputPath, PostsBuilder& builder)
{
    Graph g;
    Reader reader(inputPath);

    builder.reset();
    reader.read(builder);

    g = reader.getnet();
    vector<int> userIds = reader.getusers();

    // Create the DOT file
    string dotPath = outputPath + "/network.dot";
    ofstream dot(dotPath);
    if (!dot.is_open()) {
        cerr << "Error: Could not create DOT file at " << dotPath << endl;
        return;
    }

    dot << "digraph SocialNetwork {\n";
    dot << "    rankdir=LR;\n";
    dot << "    node [fontname=\"Arial\"];\n";
    dot << "    edge [fontname=\"Arial\"];\n\n";

    // =========================
    // 1. USERS ONLY
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
    // 2. FOLLOWER RELATIONSHIPS
    // =========================
    dot << "\n    // Followers (directed)\n";

    for (int uid : userIds) {
        vector<int> followers = g.GetFollowers(uid);
        for (int fid : followers) {
            // "fid" follows "uid" -> Arrow points from Follower to User
            dot << "    user_" << fid
                << " -> user_" << uid
                << " [label=\"follows\"];\n";
        }
    }

    // (Posts and Topics section has been removed)

    dot << "}\n";
    dot.close();

    // Generate PNG
    // Note: Changed extension to .png to match the flag -Tpng (or keep .jpg if you prefer)
    string cmd = "dot -Tpng \"" + dotPath + "\" -o \"" + outputPath + "/network.jpg\"";
    
    int result = system(cmd.c_str());
    if (result != 0) {
        cerr << "Error: Graphviz command failed. Make sure 'dot' is installed and in PATH.\n";
    } else {
        cout << "Graph generated at: " << outputPath << "/network.jpg" << endl;
    }
}

void visualize(const string& inputPath, const string& outputPath)
{
    PostsBuilder builder;
    visualize(inputPath, outputPath, builder);
}
#include "./CLI_h.hpp"
#include <cstdlib>  

void visualize(const string& inputPath, const string& outputPath)
{   

    Graph g;
    Reader reader(inputPath);
    reader.read();
    g = reader.getnet();
    vector<int> userIds = reader.getusers();


    string dotPath = outputPath + "/network.dot";
    string pngPath = outputPath + "/network.png";
    
    ofstream dot(dotPath);
    if (!dot.is_open()) {
        cerr << "Failed to create dot file\n";
        return;
    }

    dot << "digraph SocialNetwork {\n";
    dot << "    rankdir=LR;\n";
    dot << "    node [fontname=\"Arial\"];\n";
    dot << "    edge [fontname=\"Arial\"];\n\n";

    // ---------- USERS ----------
    dot << "    // Users\n";
    dot << "    node [shape=ellipse, style=filled, fillcolor=lightblue];\n";
    for (int uid : userIds) {
        Users u = g.getuserfromID(uid); // fetch real Users object
        dot << "    user_" << u.getId()
            << " [label=\"" << u.getName()
            << "\\n(ID: " << u.getId() << ")\"];\n";
    }
    dot << "\n";

    // ---------- FOLLOWERS ----------
    dot << "    // Followers (directed)\n";
    for (int uid : userIds) {
        Users u = g.getuserfromID(uid);
        for (int followerId : u.getFollowers()) {
            dot << "    user_" << followerId
                << " -> user_" << u.getId()
                << " [label=\"follows\"];\n"; // use '->' for directed graph
        }
    }
    dot << "\n";

    // ---------- POSTS ----------
    dot << "    // Posts\n";
    dot << "    node [shape=box, style=filled, fillcolor=lightyellow];\n";
    int postCounter = 0;
    for (int uid : userIds) {
        Users u = g.getuserfromID(uid);
        for (const auto& p : u.getPosts()) {
            string postNode = "post_" + to_string(postCounter++);
            string body = p.getcontent();
            if (body.size() > 60) body = body.substr(0, 60) + "...";

            dot << "    " << postNode
                << " [label=\"Post\\n" << body << "\"];\n";

            // Connect user to post
            dot << "    user_" << u.getId()
                << " -> " << postNode
                << " [label=\"writes\"];\n";

            // Optionally, display topics as separate nodes
            for (const auto& t : p.gettopics()) {
                string topicNode = postNode + "_topic_" + t;
                dot << "    " << topicNode
                    << " [label=\"" << t << "\", shape=note, style=filled, fillcolor=lightgreen];\n";
                dot << "    " << postNode
                    << " -> " << topicNode
                    << " [label=\"has\"];\n";
            }
        }
    }

    dot << "}\n";
    dot.close();

    cout << "GraphViz DOT file generated: " << dotPath << "\n";

    // -------- Convert DOT to PNG --------
    string cmd = "dot -Tpng \"" + dotPath + "\" -o \"" + pngPath + "\"";
    int ret = system(cmd.c_str());
    if (ret != 0)
        cerr << "Failed to generate PNG (make sure Graphviz is installed and 'dot' is in PATH)\n";
    else
        cout << "GraphViz PNG file generated: " << pngPath << "\n";
}

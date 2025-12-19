#include "../Header/CLI_h.hpp"
#include "../Parse/Reader.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0]
             << " <action> [-i input] [-o output] [-x [description...]] ...\n";
        return 1;
    }

    string action = argv[1];

    // Defaults
    string inputPath  = ".";
    string outputPath = ".";

    // Store optional flags with optional description
    vector<pair<char, string>> flags;

    for (int i = 2; i < argc; i++) {
        string arg = argv[i];

        // -i <path>
        if (arg == "-i") {
            if (i + 1 < argc) {
                inputPath = argv[++i];
            } else {
                cerr << "-i requires a path\n";
                return 1;
            }
        }
        // -o <path>
        else if (arg == "-o") {
            if (i + 1 < argc) {
                outputPath = argv[++i];
            } else {
                cerr << "-o requires a path\n";
                return 1;
            }
        }
        // any other flag with optional description
        else if (arg.size() == 2 && arg[0] == '-') {
            char flag = arg[1];
            string description = "";

            // collect all following arguments until next -<char> or end
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                if (!description.empty()) description += " ";
                description += argv[++i];
            }

            flags.push_back({flag, description});
        }
        else {
            cerr << "Invalid option: " << arg << "\n";
            return 1;
        }
    }

    // Graph graph;
    // Reader reader(inputPath);
    // reader.read();
    // graph= reader.getnet();
    // vector<int> users = reader.getusers();
    
    // for testing: print user details
    // for (const auto& user : users) {
    //     Users u = graph.getuserfromID(user);
    //     cout << "User ID: " << u.getId() << "\n";
    //     cout << "Name: " << u.getName() << "\n";
    
    //     cout << "Followers: ";
    //     for (int fid : u.getFollowers())
    //         cout << fid << " ";
    //     cout << "\n";
    
    //     cout << "Posts:\n";
    //     for (const auto& p : u.getPosts()) {
    //         string body = p.getcontent();
    //         if (body.size() > 60)
    //             body = body.substr(0, 60) + "...";
    //         cout << "  - " << body << "\n";
    
    //         cout << "    Topics: ";
    //         for (const auto& t : p.gettopics())
    //             cout << t << " ";
    //         cout << "\n";
    //     }
    //     cout << "------------------------\n";
    // }

    // visualize(outputPath, graph, users);
    // visualizeIDsOnly(outputPath, graph, users);
    Parse_input(action, inputPath, outputPath, flags);

    return 0;
}

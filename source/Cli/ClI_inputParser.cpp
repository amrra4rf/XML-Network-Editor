#include "../../Includes\CLI_h.hpp"

PostsBuilder builder;

void resetBuilder() {
    builder.reset();  // or builder = PostsBuilder();
}

void Parse_input(string action,
                 string inputPath,
                 string outputPath,
                 vector<pair<string, string>> flags)
{
    /* ================= VERIFY ================= */
    if (action == "verify") {
        vector<string> errorLog;
        vector<int> errorLines;

        checkConsistency(readFile(inputPath), errorLog, errorLines);
        if(errorLog.size()==0)cout<<"Valid"<<endl;
        else{ cout<<"invlaid"<<endl;
            cout<<"Number of errors are: "<<errorLog.size()<<endl;
        for (const auto& e : errorLog) cout << e << endl;
       
        }
        if (!flags.empty() && flags[0].first == "f" && !errorLines.empty()) {
            fixXmlToFile(readFile(inputPath), errorLines, outputPath);
        }
    }

    /* ================= FORMAT ================= */
    else if (action == "format") {
        cout << "Formatting XML File...\n";
        Format_XML_File(inputPath, outputPath);
    }

    /* ================= JSON ================= */
    else if (action == "json") {
        cout << "Converting XML to JSON...\n";
        namespace fs = std::filesystem;

        fs::create_directories(outputPath);
        string outFile = (fs::path(outputPath) / "converted.json").string();

        convert(inputPath, outFile);
        cout << "JSON created at: " << outFile << endl;
    }

    /* ================= MINI ================= */
    else if (action == "mini") {
        cout << "Minifying XML...\n";
        namespace fs = std::filesystem;

        fs::create_directories(outputPath);
        string outFile = (fs::path(outputPath) / "minified.xml").string();

        minifying(inputPath, outFile);
        cout << "Minified file: " << outFile << endl;
    }

    /* ================= COMPRESS ================= */
    else if (action == "compress") {
        cout << "Compressing XML...\n";
        namespace fs = std::filesystem;

        fs::create_directories(outputPath);
        string outFile = (fs::path(outputPath) / "compressed.comp").string();

        compress(inputPath, outFile);
        cout << "Compressed file: " << outFile << endl;
    }

    /* ================= DECOMPRESS ================= */
    else if (action == "decompress") {
        cout << "Decompressing XML...\n";
        namespace fs = std::filesystem;

        fs::create_directories(outputPath);
        string outFile = (fs::path(outputPath) / "decompressed.xml").string();

        decompress(inputPath, outFile);
        cout << "Decompressed file: " << outFile << endl;
    }

    
    /* ================= DRAW ================= */
    else if (action == "draw") {
        namespace fs = std::filesystem;
    // 1. Create the output directory if it doesn't exist
    if (!fs::exists(outputPath)) {
        fs::create_directories(outputPath); 
    }

    // 2. Define the path for the formatted temp file
    string formattedFile = outputPath + "/XML_formatted.xml";

    // 3. Format first
    Format_XML_File(inputPath, formattedFile);

    // 4. Then visualize using the formatted file
    visualize(formattedFile, outputPath, builder);
    }

    /* ================= MOST ACTIVE ================= */
    else if (action == "most_active") {
        resetBuilder();
        string formatted = outputPath + "/XML_formatted.xml";
        Format_XML_File(inputPath, formatted);

        SocialNetworkGraph network(formatted, builder);
        Users u = network.getMostActiveUser();

        cout << "most_active_user: " << u.getName() << endl;
        cout << "most_active_id: " << u.getId() << endl;
    }

    /* ================= MOST INFLUENCER ================= */
    else if (action == "most_influencer") {
        resetBuilder();
        Format_XML_File(inputPath, outputPath);

        SocialNetworkGraph network(outputPath + "/XML_formatted.xml", builder);
        Users u = network.getInfluencer();

        cout << u.getName() << " (ID: " << u.getId() << ")\n";
    }

    /* ================= MUTUAL ================= */
else if (action == "mutual") {

    // -------- get ids from flags --------
    string idsArg = "";

    for (const auto& f : flags) {
        if (f.first == "ids") {
            idsArg = f.second;   // example: "1,2,3"
            break;
        }
    }

    if (idsArg.empty()) {
        cerr << "Error: -ids is required (example: -ids 1,2,3)\n";
        return;
    }

    // -------- parse ids --------
    vector<int> ids;
    string token;
    stringstream ss(idsArg);

    while (getline(ss, token, ',')) {
        ids.push_back(stoi(token));
    }

    if (ids.size() < 2) {
        cerr << "Error: mutual requires at least 2 user IDs\n";
        return;
    }

    // -------- prepare graph --------
    resetBuilder();
    string formatted = outputPath + "/XML_formatted.xml";
    Format_XML_File(inputPath, formatted);

    SocialNetworkGraph network(formatted, builder);

    // -------- validate users --------
    for (int id : ids) {
        if (network.getGraph().getUserFromID(id).getId() == -1) {
            cerr << "Error: User ID " << id << " not found\n";
            return;
        }
    }

    // -------- mutual followers logic --------
    vector<int> mutuals = network.network.GetFollowers(
        network.getGraph().getUserFromID(ids[0])
    );

    for (size_t i = 1; i < ids.size(); ++i) {
        vector<int> nextFollowers =
            network.network.GetFollowers(network.getGraph().getUserFromID(ids[i]));

        vector<int> intersection;

        for (int a : mutuals) {
            for (int b : nextFollowers) {
                if (a == b) {
                    intersection.push_back(a);
                    break;
                }
            }
        }
        mutuals = intersection;
    }

    // -------- output --------
    cout << "Mutual followers of users: ";
    for (size_t i = 0; i < ids.size(); ++i) {
        cout << ids[i];
        if (i + 1 < ids.size()) cout << ", ";
    }
    cout << ":\n";

    if (mutuals.empty()) {
        cout << "  No mutual followers\n";
    } else {
        for (int uid : mutuals) {
            Users u = network.getGraph().getUserFromID(uid);
            cout << "  - " << u.getName()
                 << " (ID " << u.getId() << ")\n";
        }
    }
}




    /* ================= SUGGEST ================= */
   else if (action == "suggest") {
        resetBuilder();
        string formatted = outputPath + "/XML_formatted.xml";
        Format_XML_File(inputPath, formatted);

        SocialNetworkGraph network(formatted, builder);
        auto suggestions = network.getSuggestions();

        // 1. Check if user provided an ID via flags (e.g., -id 1)
        int targetId = -1;
        for (const auto& f : flags) {
            if (f.first == "id") {
                if (!f.second.empty()) {
                    try {
                        targetId = stoi(f.second);
                    } catch (...) {
                        cerr << "Error: Invalid ID format.\n";
                        return;
                    }
                }
                break;
            }
        }

        // 2. Output Logic
        if (targetId != -1) {
            // --- SINGLE USER MODE ---
            if (suggestions.find(targetId) != suggestions.end()) {
                auto list = suggestions[targetId];
                cout << "Suggestions for "
                     << network.getGraph().getUserFromID(targetId).getName() << ":\n";

                if (list.empty()) cout << "  No suggestions\n\n";
                else {
                    for (const auto& u : list)
                        cout << "  - " << u.getName() << " (ID " << u.getId() << ")\n";
                    cout << endl;
                }
            } else {
                cout << "User with ID " << targetId << " not found or has no suggestions.\n";
            }
        } else {
            // --- ALL USERS MODE (Default if no -id is provided) ---
            for (const auto& [uid, list] : suggestions) {
                cout << "Suggestions for "
                     << network.getGraph().getUserFromID(uid).getName() << ":\n";

                if (list.empty()) cout << "  No suggestions\n\n";
                else {
                    for (const auto& u : list)
                        cout << "  - " << u.getName() << " (ID " << u.getId() << ")\n";
                    cout << endl;
                }
            }
        }
    }

    /* ================= SEARCH ================= */
else if (action == "search") {
    if (flags.empty()) {
        cerr << "No search flags provided\n";
        return;
    }

    resetBuilder();

    // Make sure formatted points to a file
    string formatted = outputPath + "/XML_formatted.xml";
    Format_XML_File(inputPath, formatted);

    SocialNetworkGraph network(formatted, builder);

    for (auto& [flagChar, value] : flags) {

        // normalize "-t" → "t"
        if (!flagChar.empty() && flagChar[0] == '-')
            flagChar = flagChar.substr(1);

        if (flagChar == "w") {
            auto res = network.getPostsByWord(value);
            if (res.empty()) {
                cout << "No posts found containing \"" << value << "\"\n";
            } else {
                for (const auto& p : res)
                    cout << p.getcontent() << "\n------\n";
            }
        }
        else if (flagChar == "t") {
            auto res = network.getPostsByTopicFast(value);
            if (res.empty()) {
                cout << "No posts found with topic \"" << value << "\"\n";
            } else {
                for (const auto& p : res)
                    cout << p.getcontent() << "\n------\n";
            }
        }
        else {
            cerr << "Unknown search flag: " << flagChar << endl;
        }
    }
}


    /* ================= UNKNOWN ================= */
    else {
        cerr << "Unknown action: " << action << endl;
    }
}

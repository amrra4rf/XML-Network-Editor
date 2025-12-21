#include "../Header/CLI_h.hpp"

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

        for (const auto& e : errorLog) cout << e << endl;
        for (int l : errorLines) cout << l << endl;

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
        string outFile = (fs::path(outputPath) / "compressed.xml").string();

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
        cout << "Drawing graph...\n";

        Format_XML_File(inputPath, outputPath);
        visualize(outputPath + "/XML_formatted.xml", outputPath);
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
        resetBuilder();
        string formatted = outputPath + "/XML_formatted.xml";
        Format_XML_File(inputPath, formatted);

        SocialNetworkGraph network(formatted, builder);
        const auto& users = network.getGraph().getUsers();

        for (size_t i = 0; i < users.size(); ++i) {
            for (size_t j = i + 1; j < users.size(); ++j) {
                vector<int> ids = {users[i].getId(), users[j].getId()};
                auto mutuals = network.getMutualFollowers(ids);

                cout << users[i].getName() << " & " << users[j].getName() << ":\n";
                if (mutuals.empty()) cout << "  No mutual followers\n\n";
                else {
                    for (const auto& u : mutuals)
                        cout << "  - " << u.getName() << " (ID " << u.getId() << ")\n";
                    cout << endl;
                }
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

    /* ================= SEARCH ================= */
    else if (action == "search") {
    if (flags.empty()) {
        cerr << "No search flags provided\n";
        return;
    }

    resetBuilder();

    Format_XML_File(inputPath, outputPath);
    string formatted = outputPath + "/XML_formatted.xml";

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

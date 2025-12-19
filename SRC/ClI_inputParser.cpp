#include "../Header/CLI_h.hpp"
#include "../Parse/Reader.hpp"

void Parse_input(string action, string inputPath, string outputPath, vector<pair<char, string>> flags) {
    if (action == "verify"){

    }
    else if (action == "format"){

    }
    else if (action == "json")
    {
        /* code */
    }
    else if (action == "mini")
    {
        /* code */
        // minifying(inputPath, outputPath);
    }
    else if (action == "compress")
    {
        /* code */
        // compress(inputPath, outputPath);
    }
    else if (action == "decompress")
    {
        /* code */
        // decompress(inputPath, outputPath);

    }
    else if (action == "draw"){
        visualize(inputPath , outputPath);
    }
    else if (action == "most_active")
    {
        /* code */
        // getMostActiveUser();
    }
    else if (action == "most_influencer")
    {
        /* code */
        // getInfluencer();
    }
    else if (action == "mutual")
    {
        /* code */
        // getMutualFollowers(vector<int> goal);
    }
    else if (action == "suggest")
    {
        /* code */
        // getSuggestions();
    }
    else if (action == "search")
    {
        /* code */
        if (flags.size() > 0){
            for (const auto& flag : flags){
                char flagChar = flag.first;
                string description = flag.second;
                // Process each flag and its description

                if (flagChar == 'w'){
                    // Search by name
                }
                else if (description == "t"){
                    // Search by ID
            }
        }
    }
    else
    {
        cerr << "Unknown action: " << action << "\n";
    }
}}
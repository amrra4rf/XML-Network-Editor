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
    vector<pair<string , string>> flags;

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
        // flags: -f, -ids, -w, etc
        else if (arg[0] == '-') {
            string flagName = arg.substr(1); // remove '-'
            string value = "";

            if (i + 1 < argc && argv[i + 1][0] != '-') {
                value = argv[++i];
            }

            flags.push_back({flagName, value});
        }
        else {
            cerr << "Invalid option: " << arg << "\n";
            return 1;
        }
    }

    // cout << "action: " << action << endl;
    // cout << "inputPath: " << inputPath << endl;
    // cout << "outputPath: " << outputPath << endl;
    // cout << "flags: " << endl;
    // for (const auto& flag : flags) {
    //     cout << "first flag: " << flag.first << " second: " << flag.second << endl;
    // }

    Parse_input(action, inputPath, outputPath, flags);

    return 0;
}

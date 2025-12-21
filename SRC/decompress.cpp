#include "../Header/formatting.hpp"

void decompress(string inputfileName, string outFileName)
    {
        ifstream inputfile(inputfileName);
        if (!inputfile.is_open())
        {
            cerr << "Error: Could not open the input file " << inputfileName << endl;
            return;
        }
        ofstream outputFile(outFileName);
        if (!outputFile.is_open())
        {
            cerr << "Error: Could not open the output file " << outFileName << endl;
            return;
        }
        map<char, string> dictionary_token_decomp;
        string data_sequence((istreambuf_iterator<char>(inputfile)), istreambuf_iterator<char>());
        string line = "";
        auto it = data_sequence.begin();
        string pair = "";
        char token;
        string tmp;
        while ((pair + token) != "###")
        {
            token = *it;
            data_sequence.erase(it);
            pair = *it;
            data_sequence.erase(it);
            pair += *it;
            data_sequence.erase(it);
            if ((pair + token) != "###")
                dictionary_token_decomp[token] = pair;
        }
    
        for (auto it = dictionary_token_decomp.rbegin(); it != dictionary_token_decomp.rend(); ++it)
        {
            auto token_to_replace = it->first;
            auto original_pair = it->second;
            size_t pos = 0;
            while ((pos = data_sequence.find(token_to_replace, pos)) != string::npos)
            {
                data_sequence.replace(pos, 1, original_pair);
                pos += 2;
            }
        }
        outputFile << data_sequence;
    }
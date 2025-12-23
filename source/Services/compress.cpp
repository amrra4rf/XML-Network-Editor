#include "../../Includes/Formater.hpp"

map<string, int> pairCount;
map<char, bool> charexist;
map<char, string> dictionary_token;
map<string, char> dictionary_pair;
vector<char> unusedChar;

    struct Pair
    {
        string pairName;
        int pairCount;
    };
    
    struct Compare
    {
        bool operator()(Pair a, Pair b)
        {
            return a.pairCount < b.pairCount;
        }
    };
    priority_queue<Pair, vector<Pair>, Compare> pq;
    
    void initMaps(string &data_sequence)
    {
    
        for (int i = 33; i < 255; i++)
            charexist[i] = false;
        for (int i = 0; i < data_sequence.size() - 1; i++)
        {
            string tmp = "";
            charexist[data_sequence[i]] = true;
            charexist[data_sequence[i + 1]] = true;
            tmp += data_sequence[i];
            tmp += data_sequence[i + 1];
            pairCount[tmp]++;
        }
        for (auto &[pair, count] : pairCount)
        {
            if (count > 1)
            {
                Pair p;
                p.pairName = pair;
                p.pairCount = count;
                pq.push(p);
            }
        }
        for (auto &[c, b] : charexist)
            if (!b)
                unusedChar.push_back(c);
    }
    
    void update_pair_counts_and_pq(const std::string &data_sequence)
    {
        // Clear previous counts and queue
        pairCount.clear();
        while (!pq.empty())
            pq.pop();
    
        // Count all adjacent pairs in the new sequence
        for (int i = 0; i < data_sequence.size() - 1; i++)
        {
            std::string tmp = "";
            tmp += data_sequence[i];
            tmp += data_sequence[i + 1];
            pairCount[tmp]++;
        }
    
        for (auto &[pair, count] : pairCount)
        {
            if (count > 1)
            {
                Pair p;
                p.pairName = pair;
                p.pairCount = count;
                pq.push(p);
            }
        }
    }
    
    void merge_and_update_sequence(string &sequence, const std::string &oldPair, char newToken)
    {
        size_t pos = 0;
        while ((pos = sequence.find(oldPair, pos)) != string::npos)
        {
            sequence.replace(pos, 2, 1, newToken);
            pos += 1; 
        }
    }
    
    void compress(string inputfileName, string outFileName)
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
        string data_sequence((istreambuf_iterator<char>(inputfile)), istreambuf_iterator<char>());
        initMaps(data_sequence);
    
        while (true)
        {
            if (pq.empty())
                break;
    
            Pair current_max_pair = pq.top();
            pq.pop();
    
            if (current_max_pair.pairCount <= 1 || unusedChar.empty())
            {
                break;
            }
    
            char newToken = unusedChar.front();
            unusedChar.erase(unusedChar.begin());
    
            dictionary_token[newToken] = current_max_pair.pairName;
            dictionary_pair[current_max_pair.pairName] = newToken;
            //  Replace and generate new sequence
            merge_and_update_sequence(data_sequence, current_max_pair.pairName, newToken);
            update_pair_counts_and_pq(data_sequence);
        }
    
        for (auto &[token, pair] : dictionary_token)
        {
            outputFile << token << pair; 
        }
        outputFile << "###"; //marker
        outputFile << data_sequence;
    }
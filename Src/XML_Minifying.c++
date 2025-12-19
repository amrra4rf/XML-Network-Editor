#include <bits/stdc++.h>
using namespace std;
class XML_Minifying{

    void removeChar(int idx, string &line)
    {
        for (int i = idx; i < line.length() - 1; i++)
        {
            char tmp = line[i];
            line[i] = line[i + 1];
            line[i + 1] = tmp;
        }
        line.pop_back();
    }
    public:
    void minifying(string inputFileName, string outFileName)
    {
        ifstream inputFile(inputFileName);
        ofstream outFile(outFileName);
        // check the file if it dosnout open
        if (!inputFile.is_open())
        {
            cerr << "Error: Could not open the input file " << inputFileName << endl;
            return;
        }
        if (!outFile.is_open())
        {
            cerr << "Error: Could not open the output file " << outFileName << endl;
            return;
        }
        // minifying each line
        string line;
        while (getline(inputFile, line))
        {
            for (int i = 0; i < line.length(); i++)
            {
                if (line[i] == ' ' || line[i] == '\n')
                {
                    removeChar(i, line);
                    i--;
                }
            }
            outFile << line;
        }
    }
};





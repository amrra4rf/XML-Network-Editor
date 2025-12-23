#ifndef Formatting_hpp
#define Formatting_hpp

#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits> 
#include <cctype>
#include<stack>
#include <unordered_map>
#include <unordered_set>
#include <climits>
using namespace std;

void minifying(string inputFileName, string outFileName);

 string fixXml(const string& xml, const vector<int>& errorLines);
void Format_XML_File(const string &inputFilePath ,const string &outputFilePath);

string readFile(const string& path);
vector<string> checkConsistency(const string& xml, vector<string>& errorLog, vector<int>& errorLines);
void fixXmlToFile(const string& xml, const vector<int>& errorLines, const string& outputFilePath);

void decompress(string inputfileName, string outFileName);

void compress(string inputfileName, string outFileName);

void convert(const string& inputPath, const string& outputPath);

#endif
//cstdlib
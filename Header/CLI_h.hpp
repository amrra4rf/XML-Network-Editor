#ifndef CLI_H_HPP
#define CLI_H_HPP

#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include "../Parse/Reader.hpp"
#include "../Graph/Graphs.hpp"
#include "../Classes/User.hpp"
#include "../Classes/Posts.hpp"
#include "../Graph/Graphs.hpp"
using namespace std;

void Parse_input(string action, string inputPath, string outputPath, vector<pair<char, string>> flags , Graph& network , vector<int>& users);
void visualize(const string& outputPath, Graph& g, const vector<int>& userIds);
void visualizeIDsOnly(const string& outputPath, Graph& g, const vector<int>& userIds);

#endif
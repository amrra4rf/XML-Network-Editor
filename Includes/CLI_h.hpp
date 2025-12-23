#ifndef CLI_H_HPP
#define CLI_H_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <vector>
#include "Reader.hpp"
#include "Graphs.hpp"
#include "User.hpp"
#include "Posts.hpp"
#include "Graphs.hpp"
#include "Formater.hpp"
#include "SocialNetwork.hpp"

using namespace std;

void Parse_input(string action, string inputPath, string outputPath, vector<pair<string, string>> flags);
void visualize(const string& inputPath, const string& outputPath, PostsBuilder& builder);
void visualize(const string& inputPath, const string& outputPath);
void visualizeIDsOnly(const string& outputPath, Graph& g, const vector<int>& userIds);


extern PostsBuilder builder;   // <-- extern here
void resetBuilder();           // <-- declare only



#endif
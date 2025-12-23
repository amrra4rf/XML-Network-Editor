#ifndef VISUALIZE_HPP
#define VISUALIZE_HPP

#include <string>
#include "CLI_h.hpp"

using namespace std;

void visualize(const string& inputPath, const string& outputPath, PostsBuilder& builder);
void visualize(const string& inputPath, const string& outputPath);

#endif
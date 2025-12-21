#include "../Header/formatting.hpp"


string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

bool vectorContains(const vector<string>& v, const string& val) {
    return find(v.begin(), v.end(), val) != v.end();
}

bool vectorContainsInt(const vector<int>& v, int val) {
    return find(v.begin(), v.end(), val) != v.end();
}

bool isLeafTag(const string& tagName) {
    return (tagName == "id" || tagName == "name" || tagName == "body" || tagName == "topic");
}

bool isChildOf(const string& child, const string& parent) {
    if (parent == "user") return (child == "id" || child == "name" || child == "posts" || child == "followers");
    if (parent == "post") return (child == "body" || child == "topics");
    if (parent == "posts") return (child == "post");
    if (parent == "followers") return (child == "follower");
    if (parent == "follower") return (child == "id");
    if (parent == "topics") return (child == "topic");
    if (parent == "users") return (child == "user");
    return false;
}

string getChildName(const string& plural) {
    if (plural == "posts") return "post";
    if (plural == "followers") return "follower";
    if (plural == "topics") return "topic";
    if (plural == "users") return "user";
    if (plural.length() > 1 && plural.back() == 's') return plural.substr(0, plural.length() - 1);
    return "";
}

string extractTagName(const string& rawTag) {
    string name = rawTag;
    // Remove <, >, /
    if (name.length() > 0 && name[0] == '<') name = name.substr(1);
    if (name.length() > 0 && name[0] == '/') name = name.substr(1);
    if (name.length() > 0 && name.back() == '>') name.pop_back();
    
    // Remove attributes (e.g. "post id=1")
    size_t space = name.find(' ');
    if (space != string::npos) name = name.substr(0, space);
    
    return trim(name);
}

string readFile(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << path << endl;
        return "";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// --- CHECK CONSISTENCY ---
vector<string> checkConsistency(const string& xml, vector<string>& errorLog, vector<int>& errorLines) {
    errorLog.clear();
    errorLines.clear();

    vector<string> tagStack; 
    stringstream ss(xml);
    string line;
    int lineNum = 0;

    while (getline(ss, line)) {
        lineNum++;
        size_t pos = 0;
        while (true) {
            size_t openBracket = line.find('<', pos);
            if (openBracket == string::npos) break; 
            size_t closeBracket = line.find('>', openBracket);
            if (closeBracket == string::npos) {
                errorLog.push_back("Line " + to_string(lineNum) + ": Missing closing '>' for tag.");
                errorLines.push_back(lineNum);
                break;
            }
            
            string tagContent = line.substr(openBracket + 1, closeBracket - openBracket - 1);
            
            if (tagContent.length() > 0 && tagContent[0] != '?' && tagContent[0] != '!') {
                if (tagContent[0] == '/') {
                    string tagName = trim(tagContent.substr(1));
                    
                    if (tagStack.empty()) {
                        errorLog.push_back("Line " + to_string(lineNum) + ": Orphan closing tag </" + tagName + "> found.");
                        errorLines.push_back(lineNum);
                    } else {
                        string top = tagStack.back();
                        if (top == tagName) {
                            tagStack.pop_back(); 
                        } else {
                            if (vectorContains(tagStack, tagName)) {
                                while (!tagStack.empty() && tagStack.back() != tagName) {
                                    errorLog.push_back("Line " + to_string(lineNum) + ": Missing closing tag for <" + tagStack.back() + ">.");
                                    errorLines.push_back(lineNum);
                                    tagStack.pop_back(); 
                                }
                                tagStack.pop_back(); 
                            } else {
                                errorLog.push_back("Line " + to_string(lineNum) + ": Orphan closing tag </" + tagName + ">.");
                                errorLines.push_back(lineNum);
                            }
                        }
                    }
                } else {
                    if (tagContent.back() != '/') {
                        string tagName = tagContent;
                        size_t spacePos = tagContent.find(' ');
                        if (spacePos != string::npos) tagName = tagContent.substr(0, spacePos);
                        tagName = trim(tagName);

                        if (!tagStack.empty() && tagStack.back() == tagName) {
                            errorLog.push_back("Line " + to_string(lineNum) + ": Error. Duplicate opening tag <" + tagName + ">. Treating as closing tag.");
                            errorLines.push_back(lineNum);
                            tagStack.pop_back();
                        } 
                        else if (!tagStack.empty() && isLeafTag(tagStack.back())) {
                            errorLog.push_back("Line " + to_string(lineNum) + ": Missing closing tag for <" + tagStack.back() + ">.");
                            errorLines.push_back(lineNum);
                            tagStack.pop_back();
                            tagStack.push_back(tagName); 
                        }
                        else {
                            tagStack.push_back(tagName);
                        }
                    }
                }
            }
            pos = closeBracket + 1;
        }
    }
    
    while (!tagStack.empty()) {
        errorLog.push_back("End of file: Missing closing tag for <" + tagStack.back() + ">.");
        errorLines.push_back(lineNum); 
        tagStack.pop_back();
    }
    return errorLog;
}

/// @brief //////////////////////////////////////////////////////////////////////
/// @param xml 
/// @param errorLines 
/// @return 
void fixXmlToFile(const string& xml, const vector<int>& errorLines, const string& outputFilePath) {
    string fixedOutput = "";
    vector<string> tagStack;
    size_t pos = 0;
    int currentLine = 1;

    while (pos < xml.length()) {
        size_t openBracket = xml.find('<', pos);
        string textChunk;
        if (openBracket == string::npos) textChunk = xml.substr(pos);
        else textChunk = xml.substr(pos, openBracket - pos);

        for (char c : textChunk) if (c == '\n') currentLine++;
        fixedOutput += textChunk;

        if (openBracket == string::npos) break;

        size_t closeBracket = xml.find('>', openBracket);
        if (closeBracket == string::npos) break;

        string fullTag = xml.substr(openBracket, closeBracket - openBracket + 1);
        string tagContent = xml.substr(openBracket + 1, closeBracket - openBracket - 1);

        if (!tagContent.empty() && tagContent[0] != '?' && tagContent[0] != '!') {
            string tagName;
            bool isClosing = (tagContent[0] == '/');
            if (isClosing) tagName = trim(tagContent.substr(1));
            else {
                tagName = tagContent;
                size_t space = tagName.find(' ');
                if (space != string::npos) tagName = tagName.substr(0, space);
                tagName = trim(tagName);
            }

            if (isClosing) {
                if (!tagStack.empty() && tagStack.back() == tagName) {
                    tagStack.pop_back(); 
                    fixedOutput += fullTag;
                } else if (!tagStack.empty() && vectorContainsInt(errorLines, currentLine)) {
                    while (!tagStack.empty() && tagStack.back() != tagName) {
                        fixedOutput += "</" + tagStack.back() + ">"; 
                        tagStack.pop_back();
                    }
                    if (!tagStack.empty()) tagStack.pop_back();
                    fixedOutput += fullTag;
                } else {
                    fixedOutput += fullTag;
                }
            } else {
                // Opening tag
                if (tagContent.back() != '/') tagStack.push_back(tagName);
                fixedOutput += fullTag;
            }
        } else {
            fixedOutput += fullTag;
        }

        pos = closeBracket + 1;
    }

    // Close remaining tags
    while (!tagStack.empty()) {
        fixedOutput += "</" + tagStack.back() + ">";
        tagStack.pop_back();
    }

    // Write to file
    string fixedOutputPath = outputFilePath + "/fixed.xml";
    ofstream outFile(fixedOutputPath);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot open output file: " << fixedOutputPath << endl;
        return;
    }
    outFile << fixedOutput;
    outFile.close();

    cout << "Fixed XML written to: " << fixedOutputPath << endl;
}
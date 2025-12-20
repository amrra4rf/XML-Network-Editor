#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits> 
#include <cctype> 

using namespace std;


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
     string fixXml(const string& xml, const vector<int>& errorLines) {
        string fixedOutput = "";
        vector<string> tagStack; 
        size_t pos = 0;
        int currentLine = 1; 

        while (pos < xml.length()) {
            size_t openBracket = xml.find('<', pos);
            
            string textChunk;
            if (openBracket == string::npos) {
                textChunk = xml.substr(pos);
            } else {
                textChunk = xml.substr(pos, openBracket - pos);
            }

            for (char c : textChunk) {
                if (c == '\n') currentLine++;
            }
            fixedOutput += textChunk;

            if (openBracket == string::npos) break;
            
            size_t closeBracket = xml.find('>', openBracket);
            if (closeBracket == string::npos) break; 
            
            string tagContent = xml.substr(openBracket + 1, closeBracket - openBracket - 1);
            string fullTag = xml.substr(openBracket, closeBracket - openBracket + 1);

            if (tagContent.length() > 0 && tagContent[0] != '?' && tagContent[0] != '!') {
                if (tagContent[0] == '/') {
                    string tagName = trim(tagContent.substr(1));
                    
                    if (!tagStack.empty() && tagStack.back() == tagName) {
                        tagStack.pop_back(); fixedOutput += fullTag;
                    } 
                    else if (!tagStack.empty() && vectorContains(tagStack, tagName)) {
                        bool isErrorLine = vectorContainsInt(errorLines, currentLine);
                        if (isErrorLine) {
                            while (!tagStack.empty() && tagStack.back() != tagName) {
                                string trailingSpace = "";
                                while (!fixedOutput.empty() && isspace(fixedOutput.back())) {
                                    trailingSpace += fixedOutput.back();
                                    fixedOutput.pop_back();
                                }
                                reverse(trailingSpace.begin(), trailingSpace.end());
                                
                                fixedOutput += "</" + tagStack.back() + ">"; 
                                tagStack.pop_back();
                                fixedOutput += trailingSpace;
                            }
                            tagStack.pop_back(); fixedOutput += fullTag;
                        } else {
                             while (!tagStack.empty() && tagStack.back() != tagName) {
                                tagStack.pop_back(); 
                             }
                             tagStack.pop_back(); fixedOutput += fullTag;
                        }
                    } else {
                         if (!tagStack.empty() && isLeafTag(tagStack.back()) && vectorContainsInt(errorLines, currentLine)) {
                            string trailingSpace = "";
                            while (!fixedOutput.empty() && isspace(fixedOutput.back())) {
                                trailingSpace += fixedOutput.back();
                                fixedOutput.pop_back();
                            }
                            reverse(trailingSpace.begin(), trailingSpace.end());
                            fixedOutput += "</" + tagStack.back() + ">"; 
                            tagStack.pop_back();
                            fixedOutput += trailingSpace;
                         }
                    }
                } else {
                    if (tagContent.back() != '/') {
                        string tagName = tagContent;
                        size_t spacePos = tagContent.find(' ');
                        if (spacePos != string::npos) tagName = tagContent.substr(0, spacePos);
                        tagName = trim(tagName);

                        if (!tagStack.empty() && tagStack.back() == tagName && vectorContainsInt(errorLines, currentLine)) {
                            fixedOutput += "</" + tagName + ">"; 
                            tagStack.pop_back();
                        } 
                        else if (!tagStack.empty() && isLeafTag(tagStack.back()) && vectorContainsInt(errorLines, currentLine)) {
                            string trailingSpace = "";
                            while (!fixedOutput.empty() && isspace(fixedOutput.back())) {
                                trailingSpace += fixedOutput.back();
                                fixedOutput.pop_back();
                            }
                            reverse(trailingSpace.begin(), trailingSpace.end());

                            fixedOutput += "</" + tagStack.back() + ">"; 
                            tagStack.pop_back();
                            
                            fixedOutput += trailingSpace; 
                            tagStack.push_back(tagName);
                            fixedOutput += fullTag;
                        }
                        else {
                            tagStack.push_back(tagName);
                            fixedOutput += fullTag;
                        }
                    } else {
                        fixedOutput += fullTag;
                    }
                }
            } else {
                fixedOutput += fullTag;
            }
            pos = closeBracket + 1;
        }
        
        if (vectorContainsInt(errorLines, currentLine)) {
            while (!tagStack.empty()) {
                fixedOutput += "</" + tagStack.back() + ">"; tagStack.pop_back();
            }
        }
        
        return fixedOutput;
    }



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits> 
#include <cctype> 
#include "../../Includes/Formater.hpp"
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
                    // === CLOSING TAG ===
                    if (!tagStack.empty() && tagStack.back() == tagName) {
                        tagStack.pop_back(); fixedOutput += fullTag;
                    } 
                    else if (!tagStack.empty() && vectorContains(tagStack, tagName)) {
                        // Mismatch found (Missing closing tags)
                        if (vectorContainsInt(errorLines, currentLine)) {
                            while (!tagStack.empty() && tagStack.back() != tagName) {
                                fixedOutput += "</" + tagStack.back() + ">"; 
                                tagStack.pop_back();
                            }
                            tagStack.pop_back(); fixedOutput += fullTag;
                        } else {
                            while (!tagStack.empty() && tagStack.back() != tagName) tagStack.pop_back();
                            tagStack.pop_back(); fixedOutput += fullTag;
                        }
                    } else {
                        // === ORPHAN CLOSING TAG LOGIC ===
                        if (vectorContainsInt(errorLines, currentLine)) {
                            
                            // Case 1: Leaf Tag (e.g. </id>) - Wrap preceding text
                            if (isLeafTag(tagName)) {
                                string extractedText = "";
                                while (!fixedOutput.empty() && fixedOutput.back() != '>' ) {
                                    extractedText += fixedOutput.back();
                                    fixedOutput.pop_back();
                                }
                                reverse(extractedText.begin(), extractedText.end());
                                fixedOutput += "<" + tagName + ">" + extractedText + fullTag;
                            } 
                            // Case 2: Container Tag (e.g. </post>, </posts>) - Wrap preceding children
                            else {
                                long long cursor = fixedOutput.length();
                                long long insertPos = cursor;
                                
                                // Backtrack logic: Keep jumping over valid children until we hit a non-child
                                while (cursor > 0) {
                                    // Skip whitespace
                                    while (cursor > 0 && isspace(fixedOutput[cursor-1])) cursor--;
                                    if (cursor == 0) break;

                                    // Look for the end of the previous block (e.g. </body >)
                                    if (fixedOutput[cursor-1] == '>') {
                                        size_t lastLt = fixedOutput.rfind('<', cursor - 1);
                                        if (lastLt == string::npos) break;
                                        
                                        string foundTagRaw = fixedOutput.substr(lastLt, cursor - lastLt);
                                        string foundTagName = extractTagName(foundTagRaw);
                                        
                                        // Is this a valid child of our current orphan?
                                        if (foundTagRaw[1] == '/' && isChildOf(foundTagName, tagName)) {
                                            // Yes, it is a closing child tag (e.g. </body> inside post).
                                            // We must jump to its opening tag <body>.
                                            
                                            long long balance = 1;
                                            long long scanner = lastLt;
                                            
                                            while (scanner > 0 && balance > 0) {
                                                size_t prevGt = fixedOutput.rfind('>', scanner - 1);
                                                if (prevGt == string::npos) break;
                                                size_t prevLt = fixedOutput.rfind('<', prevGt);
                                                if (prevLt == string::npos) break;
                                                
                                                string t = fixedOutput.substr(prevLt, prevGt - prevLt + 1);
                                                // Check for nesting
                                                if (t.substr(0,2) == "</") balance++;
                                                else if (t.substr(0,1) == "<" && t[1]!='/' && t.back()!='?') balance--;
                                                
                                                scanner = prevLt;
                                            }
                                            cursor = scanner; // Jump to start of child
                                            insertPos = cursor; // Update insertion point
                                            continue; // Continue checking for more previous children
                                        }
                                    }
                                    // If we hit anything else (not a valid child), stop.
                                    break;
                                }
                                
                                fixedOutput.insert(insertPos, "<" + tagName + ">");
                                fixedOutput += fullTag;
                            }
                        }
                    }
                } else {
                    // === OPENING TAG ===
                    if (tagContent.back() != '/') { 
                        // Fix 1: Typo <id><id>
                        if (!tagStack.empty() && tagStack.back() == tagName && vectorContainsInt(errorLines, currentLine)) {
                            fixedOutput += "</" + tagName + ">"; 
                            tagStack.pop_back();
                        } 
                        // Fix 2: Leaf Violation <name>...<posts>
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




   void fixXmlToFile(const string& xml, const vector<int>& errorLines, const string& outputFolderPath) {
    // Get the fixed XML string
    string fixedXml = fixXml(xml, errorLines);

    // Ensure the output folder exists
    filesystem::create_directories(outputFolderPath);

    // Construct the full path for fixed.xml
    string fullPath = outputFolderPath + "/fixed.xml";

    // Write to file
    ofstream outFile(fullPath);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot open file " << fullPath << " for writing.\n";
        return;
    }

    outFile << fixedXml;
    outFile.close();

    cout << "Fixed XML written to: " << fullPath << endl;
}
    

#include<iostream>
#include<vector>
#include<stack>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

string trimAll0(const string& text) {
    string result;
    result.reserve(text.size());

    for (char c : text) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            continue;
        result += c;
    }
    return result;
}


string readTag(const string& text, int& pos) {
    pos++;
    string tag;
    while (pos < text.size() && text[pos] != '>') {
        tag += text[pos++];
    }
    pos++;
    return tag;
}
string readText(const string& text, int& pos) {
    string value;
    while (pos < text.size() && text[pos] != '<') {
        value += text[pos++];
    }
    return value;
}

string parseXML(const string& text, int& pos) {
    string json = "{";
    bool first = true;

    while (pos < text.size()) {
        if (text[pos] == '<') {
            if (pos + 1 < text.size() && text[pos + 1] == '/') {

                readTag(text, pos);
                break;
            }

            string tag = readTag(text, pos);

            if (!first) json += ",";
            first = false;

            json += "\"" + tag + "\":";

            // check if next is text or nested tag
            if (text[pos] == '<') {
                json += parseXML(text, pos);
            }
            else {
                string value = readText(text, pos);
                json += "\"" + value + "\"";
                readTag(text, pos); // consume closing tag
            }
        }
    }

    json += "}";
    return json;


}
string format(const string& json) {
    string result;
    int indent = 0;
    bool inString = false;

    for (size_t i = 0; i < json.size(); i++) {
        char c = json[i];

        if (c == '"' && (i == 0 || json[i - 1] != '\\')) {
            inString = !inString;
        }

        if (!inString) {
            if (c == '{' || c == '[') {
                result += c;
                result += '\n';
                indent++;
                result += string(indent * 2, ' ');
                continue;
            }

            if (c == '}' || c == ']') {
                result += '\n';
                indent--;
                result += string(indent * 2, ' ');
                result += c;
                continue;
            }

            if (c == ',') {
                result += c;
                result += '\n';
                result += string(indent * 2, ' ');
                continue;
            }

            if (c == ':') {
                result += ": ";
                continue;
            }
        }

        result += c;
    }

    return result;
}

void convert(string filePath) {

    ifstream inputFile(filePath);
    if (!inputFile)
    {
        cout << "File not found!" << endl;

    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string xml = buffer.str();
    inputFile.close();

    cout << "File contents read successfully!" << endl;

    int pos = 0;
    string xml_no_spaces = trimAll0(xml);
    string json_not_formatted = parseXML(xml_no_spaces, pos);
    string json_formatted = format(json_not_formatted);

    ofstream outputFile("data.json");
    if (!outputFile)
    {
        cout << "Failed to create JSON file!" << endl;

    }

    outputFile << json_formatted;
    outputFile.close();

    cout << "JSON file saved successfully" << endl;



}


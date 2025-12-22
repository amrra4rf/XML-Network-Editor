#include "../Header/formatting.hpp"
#include <fstream>
#include <iostream>
#include <cctype>

using namespace std;

// -------------------------------------------------
// indentation helper
// -------------------------------------------------
string indent(int level)
{
    return string(level * 4, ' '); // 4 spaces per level
}

// -------------------------------------------------
// detect closing tag
// -------------------------------------------------
bool isClosingTag(const string& tag)
{
    return tag.size() >= 2 && tag.substr(0, 2) == "</";
}

// -------------------------------------------------
// trim + normalize whitespace inside text nodes
// -------------------------------------------------
string trimAll(const string& text)
{
    string result;
    bool inSpace = false;

    for (char c : text) {
        if (isspace(static_cast<unsigned char>(c))) {
            if (!inSpace) {
                result += ' ';
                inSpace = true;
            }
        } else {
            result += c;
            inSpace = false;
        }
    }

    // remove leading space
    if (!result.empty() && result.front() == ' ')
        result.erase(result.begin());

    // remove trailing space
    if (!result.empty() && result.back() == ' ')
        result.pop_back();

    return result;
}

// -------------------------------------------------
// format XML when input is a STRING
// -------------------------------------------------
string Format_String_XML(const string& xml, const string& outputFilePath)
{
    string formatted;
    int indentLevel = 0;
    int i = 0;

    while (i < (int)xml.size()) {
        if (xml[i] == '<') {
            string tag;

            while (i < (int)xml.size() && xml[i] != '>')
                tag += xml[i++];

            tag += '>';

            if (isClosingTag(tag))
                indentLevel--;

            formatted += indent(indentLevel) + tag + "\n";

            if (!isClosingTag(tag))
                indentLevel++;
        }
        else {
            string text;

            while (i < (int)xml.size() && xml[i] != '<')
                text += xml[i++];

            string trimmed = trimAll(text);
            if (!trimmed.empty())
                formatted += indent(indentLevel) + trimmed + "\n";

            continue;
        }
        i++;
    }

    ofstream out(outputFilePath);
    if (out.is_open()) {
        out << formatted;
        out.close();
    } else {
        cout << "Error opening output file: " << outputFilePath << endl;
    }

    return formatted;
}

// -------------------------------------------------
// format XML when input is a FILE
// -------------------------------------------------
void Format_XML_File(const string& inputFilePath, const string& outputFilePath)
{
    ifstream in(inputFilePath);
    if (!in.is_open()) {
        cout << "Error opening input file" << endl;
        return;
    }

    string output_path = outputFilePath + "/XML_formatted.xml";
    ofstream out(output_path);

    string xml, line;
    while (getline(in, line))
        xml += line;

    int i = 0;
    int indentLevel = 0;

    while (i < (int)xml.size()) {
        if (xml[i] == '<') {
            string tag;

            while (i < (int)xml.size() && xml[i] != '>')
                tag += xml[i++];

            tag += '>';

            if (isClosingTag(tag))
                indentLevel--;

            out << indent(indentLevel) << tag << endl;

            if (!isClosingTag(tag))
                indentLevel++;
        }
        else {
            string text;

            while (i < (int)xml.size() && xml[i] != '<')
                text += xml[i++];

            string trimmed = trimAll(text);
            if (!trimmed.empty())
                out << indent(indentLevel) << trimmed << endl;

            continue;
        }
        i++;
    }

    in.close();
    out.close();
}

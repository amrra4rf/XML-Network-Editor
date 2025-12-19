#include<iostream>
#include<vector>
#include<stack>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

/*
    to convert xml to jason using this class
    just call jasonConverter::convert(xmlFile) this returns a string of the equivelent jason file and create a jason file and save it in the project folder
*/

class jasonConverter
{
private:
    //these methods are set private as the user doesnt need to deal with them


    static string trimAll(const string& text) {
        string result;
        result.reserve(text.size());

        for (char c : text) {
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
                continue;
            result += c;
        }
        return result;
    }


    static string readTag(const string& text, int& pos) {
        pos++;
        string tag;
        while (pos < text.size() && text[pos] != '>') {
            tag += text[pos++];
        }
        pos++;
        return tag;
    }
    static string readText(const string& text, int& pos) {
        string value;
        while (pos < text.size() && text[pos] != '<') {
            value += text[pos++];
        }
        return value;
    }

    static string parseXML(const string& text, int& pos) {
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
    static string format(const string& json) {
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


public://this method takes xml string and convert it to jason 
    static string convert(string xml) {
        int pos = 0;
        string xml_no_spaces = trimAll(xml);
        string jason_not_formatted = parseXML(xml_no_spaces, pos);
        string jason_formatted = format(jason_not_formatted);
        

        


        std::ofstream file("data.json");  // saved in project root


        file << jason_formatted;
        file.close();

        std::cout << "JSON file saved successfully\n";

        return jason_formatted;

    }



};









int main()
{


    std::string filename;
    std::cout << "Enter XML file path: ";
    std::getline(std::cin, filename);

    // Open the file
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    // Read entire file into a string
    std::stringstream buffer;
    buffer << file.rdbuf();   // copies whole file
    std::string xmlContent = buffer.str();

    // Optionally print the content
    // 
    string n = jasonConverter::convert(xmlContent);
    cout << n;
}
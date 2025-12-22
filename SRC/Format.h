#include <bits/stdc++.h>

using namespace std;

// -------------------------------------------------
// indentation
// -------------------------------------------------
    string indent(int level)
    {
        return string(level * 4, ' '); // tap for any level
    }
    
// -------------------------------------------------
// detect closing tag
// -------------------------------------------------
    bool isClosingTag(const string& tag)
    {
        return tag.substr(0, 2) == "</";
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
// handle XML if it is a String 
// -------------------------------------------------
string Format_String_XML(const string &xml,const string &outputFilePath){
    string Formatted_XML;
    int indentLevel = 0;
    int i = 0;
    //read the xml string character by character
    while(i < xml.size()){
    //handle tags 
    if (xml[i] =='<'){
        string tag;
        
        while(i < xml.size() && xml[i] != '>')
            tag += xml[i++];

        tag += '>'; // add '>' at the end of tag 

        // if this a closing tag decrease the indenrlevel before write 
        if(isClosingTag(tag))
            indentLevel--;

        Formatted_XML += indent(indentLevel) + tag + "\n";

        // if this an opening tag  increase indenrlevel after write
        if(!isClosingTag(tag))
            indentLevel++;
    }
    else{
        string text;
        //handle text
        while (i < xml.size() && xml[i] != '<')
            text += xml[i++];

        //ignore any line contian only spaces or taps or newline
        // remove whitespace-only lines
        string trimmed = trimAll(text);
        if (!trimmed.empty())
            Formatted_XML += indent(indentLevel) + trimmed + "\n";

        continue; // continue to avoid increment i twice and ignore '<' char becuase i index  at the '<' char now
    }
    i++;    // next char in xml string 

    }

    ofstream out(outputFilePath);
    if (out.is_open()){
        out << Formatted_XML;
        out.close();
    } 
    else 
        cout << "Error opening output file: " << outputFilePath << endl;

    return Formatted_XML;
    }


// -------------------------------------------------
// deal with XML if it is a input path 
// -------------------------------------------------
void Format_XML_File(const string &inputFilePath ,const string &outputFilePath){
    
    ifstream in(inputFilePath); //read from input path 
    ofstream out(outputFilePath) ; //write in out path

    //check if input file opend successfuly
    if(!in.is_open()){
        cout <<"Error Opening input file" <<endl;
    }
    
    string xml , line ;
    
    //copy data from XML file to a string 
    while (getline(in,line))
        xml += line ;
        
    int i = 0  , indentLevel =0 ;

    //read the xml string character by character
    while (i<xml.size()){
        //handle tags 
        if(xml[i] == '<'){
            string tag ; 

            while (i<xml.size() &&xml[i] != '>')  
                tag += xml[i++];    //<user
            tag += '>' ;         //<user>
            
            // if this a closing tag decrease the indenrlevel before write 
            if(isClosingTag(tag))
                indentLevel -- ;

            // write in out file with proper indentation
            out << indent(indentLevel) << tag <<endl ;  
            
            // if this an opening tag  increase indenrlevel after write
            if(!isClosingTag(tag)) 
                indentLevel++ ;

        }

        //handle Text 
        else{

            string text ;

            while (i<xml.size() && xml[i] != '<') // Eyad Tarek Nagy</user>
                text += xml[i++];           // text = Eyad Tarek Nagy
            
            //ignore any line contian only spaces or taps or newline
            string trimmed = trimAll(text);
            if (!trimmed.empty())
                out << indent(indentLevel) << trimmed << endl;

            continue ; // continue to avoid increment i twice and ignore '<' char becuase i index  at the '<' char now
        }
        i++ ; // next char in xml string 
    }

    in.close() ; // close input file 
    out.close() ; // close out file


}




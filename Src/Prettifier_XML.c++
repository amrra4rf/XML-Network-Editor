#include <bits/stdc++.h>

using namespace std;


class Pritify{
    private :
    //Helper Functions
    static string indent(int level){
        return string(level * 4, ' '); //   tap for any level 
    }
    
    static bool isClosingTag(const string &tag){
        return tag.substr(0, 2) == "</";
    }   

    static string trimAll(const string& text){
        string result;
        result.reserve(text.size());

        for (char c : text) {
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
                continue;  // skip whitespace
            result += c;
        }
	    return result;
    }

    public:
// handle XML if it is a String 
static string Format_String_XML(string &xml,string &outputFilePath){
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

    ofstream out(outputFile);
    if (out.is_open()){
        out << Formatted_XML;
        out.close();
    } 
    else 
        cout << "Error opening output file: " << outputFile << endl;

    return Formatted_XML;
    }


// deal with XML if it is a input path 
bool Format_XML_File(string &inputPath , string &outputPath){
    
    ifstream in(inputPath); //read from input path 
    ofstream out(outputPath) ; //write in out path

    //check if input file opend successfuly
    if(!in.is_open()){
        cout <<"Error Opening input file" <<endl;
        return false;
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
            if (text.find_first_not_of(" \t\n") != string::npos)
                out << indent(indentLevel) << text <<endl ;

            continue ; // continue to avoid increment i twice and ignore '<' char becuase i index  at the '<' char now
        }
        i++ ; // next char in xml string 
    }
    in.close() ; // close input file 
    out.close() ; // close out file

    return true ;
    
}


};


int main() {
    Pritify Formatter;
    string xmlString = "<users><user><id>          1              </id><name>       Ahmed Ali</name><posts><post><body>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.</body><topics><topic>economy</topic><topic>finance</topic></topics></post><post><body>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.</body><topics><topic>solar_energy</topic></topics></post></posts><followers><follower><id>2</id></follower><follower><id>3</id></follower></followers></user><user><id>2</id><name>Yasser Ahmed</name><posts><post><body>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.</body><topics><topic>education</topic></topics></post></posts><followers><follower><id>1</id></follower></followers></user><user><id>3</id><name>Mohamed Sherif</name><posts><post><body>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.</body><topics><topic>sports</topic></topics></post></posts><followers><follower><id>1</id></follower></followers></user></users>";
    string outputFile = "formatted_output.xml";

    string formated = Pritify::Format_String_XML(xmlString ,outputFile );
    cout << "Formatted XML from string:\n" << formated << endl;
    cout << "Also written to file: " << outputFile << "\n\n";
return 0;
}


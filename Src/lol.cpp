#include <bits/stdc++.h>
using namespace std;




string IsBalanced(string expression){
    stack<char> c;
    for(char x : expression){
        
        if(x ==' ')
            continue;
            
        else if( x == '(' ||x == '{' || x == '['){
                c.push(x);
            }
        else if (x == ']' || x == '}' || x == ')'){
            if(c.empty())
                return "NO";
            else if(( x == ']' && c.top() =='[') ||
                    ( x == '}' && c.top() == '{') ||
                    ( x == ')' && c.top() == '(')){
                c.pop();
            }
        }
    }
    if(c.empty())
        return "YES";
    else 
        return "NO";

}

int main(){
    int n;
    cin >> n;
    vector<string> expression(n);
    for(int i =0 ; i<n ;i++){
        cin >> expression[i];
    }

    for( string x : expression)
        cout << IsBalanced(x)<<endl;
}
#include "Parse/Reader.hpp"
#include "Graph/Graphs.hpp"
#include "Classes/User.hpp"
#include "Classes/Posts.hpp"
#include <bits/stdc++.h>






int main(){
    Graph graph;
    Reader reader("data.xml");
    reader.read();
    graph= reader.getnet();
    auto u = graph.getuserfromID(1);
    cout<<u.getFollowers().at(0);


}

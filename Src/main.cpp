#include<iostream>

using namespace std;

#include "Readers.hpp"
#include "SocialNetworkGraph.hpp"
#include "Users.hpp"
int main()
{
string s="test.xml";
Reader r(s) ;
r.read();
SocialNetworkGraph sn(r.getnet());
Users Influencer=sn.getInfluencer();
cout<<Influencer.getName();


}
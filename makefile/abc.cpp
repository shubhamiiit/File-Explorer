#include <bits/stdc++.h>
#include <dirent.h>
#include "header.h"
#include "abc.h"
using namespace std;

struct stuff{
		string permissions;
		string user;
		string group;
		string size;
		string name;
		string time;
};
void printer(vector<stuff>&v,int i){
   
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int t = w.ws_row;
	while(t--){
		cout << v[i].permissions <<setw(10)<<right<< v[i].user 
					<<setw(10)<<right<< v[i].group 
					<<setw(10)<<right<< v[i].name
					<<right<<setw(15)
					<<setprecision(2)<<fixed<<v[i].size <<right<<setw(10)
					<< v[i].time <<endl ;
					i++;
	}
}
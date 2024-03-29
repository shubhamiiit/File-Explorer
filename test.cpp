#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <bits/stdc++.h>
#include <sys/ioctl.h>
#include <fstream>

using namespace std;
template<char delimiter>
class Splitby : public std::string {};

struct stuff{
		string permissions;
		string user;
		string group;
		string size;
		string name;
		string time;
};

void printer(vector<stuff> &v,int i){
   
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int t;
	t = v.size();
	//sort(v.begin(),v.end());
    if(t > w.ws_row)t = w.ws_row;
	while(t--){
		cout << v[i].permissions <<setw(10)<<right<< v[i].user 
					<<setw(10)<<right<< v[i].group 
					<<setw(10)<<right<< v[i].name
					<<right<<setw(15)
					<<v[i].size <<right<<setw(10)
					<< v[i].time <<endl;
					i++;
	}
	
}
void create_file(vector<string>&results){
	FILE *fptr;
	char  name[120];
	for(int size = 1;size < results.size()-1;size++){
		string fpath = "";
		fpath += results[results.size()-1].c_str();
		fpath +="/";
		fpath += results[size].c_str();
		fptr = fopen(fpath.c_str(),"w");
		if(fptr == NULL){
			printf("File not exists \n");
		}
		fclose(fptr);
	}		
}

void create_dir(vector<string>&results){
	char cwd[240];
	string PATH =results[results.size()-1];
	//cerr << "here"<<endl;
	PATH += '/';
	for(int size = 1;size < results.size()-1;size++){
		PATH += results[size];
		PATH += " ";	
		int result = mkdir(PATH.c_str(), __mode_t(0777));
		//cerr << PATH<<endl;			
	}
}
void copies(string , string ,string);
void Copy_mode(vector<string>&results,string str){
	string source = str;
	string destination = "";
	for(int i=1 ;i < results.size()-1;i++){
		destination += str+results[results.size()-1];
		string name = results[i];
		copies(source+"/"+name,destination,name);
	}
	
}
int copy_file(string source , string destination){
	cerr<<source<<endl<<destination<<" "<<endl;
	cerr<<endl;
	int file_dest, file_source;
	char buffer[4096]; ssize_t xread;
	

	file_source = open(source.c_str(), O_RDONLY);
	if(file_source < 0)
		return -1;
	
	file_dest = open(destination.c_str(), O_WRONLY | O_CREAT | O_EXCL,__mode_t(0777));
	if(file_dest < 0)
		return -1;
	
	while(xread = read(file_source, buffer, sizeof buffer), xread > 0){
		char *out_ptr = buffer;
		ssize_t nwritten;

		do{
			nwritten = write(file_dest, out_ptr, xread);

			if(nwritten >= 0){
				xread -= nwritten;
				out_ptr += nwritten;
			}
			else if (errno != EINTR){
				return -1;
			}
		}while(xread>0);
	}
	if(xread == 0){
		if(close(file_dest) < 0){
			file_dest = -1;
			return -1;
		}
		close (file_source);
		return 0;
	}
}

void copies(string source, string destination, string name){
	DIR *dir; 
	struct dirent *d; 
	struct stat filestat; 
	if (!(dir = opendir((char *)source.c_str()))) 
		return; 
	destination += "/";
	destination += name; 
		//cerr << "shubham" << endl;
	if (stat((char *)destination.c_str(), &filestat) == -1) { 
		mkdir(destination.c_str(), __mode_t(0777)); 
		//cerr << "rawat" << endl;
	} 
	while ((d = readdir(dir)) != NULL) { 
		if (d->d_type == 4) {
			//cerr << "enter" <<endl;
			if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) continue; 
			string name=d->d_name; string source_new = source+"/"+d->d_name; 
			//cerr <<source<<endl<<endl;
			copies(source_new,destination,name); 
		} 
		else { 
			string source_new = source+"/"+(d->d_name);
			//cerr << "please" << endl;
			string destination_new = destination +"/" +d->d_name;
			copy_file(source_new, destination_new);
		} 
	} 
	closedir(dir); 
}
void deletes(string);

void delete_mode(vector<string>&results,string str){
	
	for(int i=1 ;i < results.size();i++){
		string destination = str+"/"+results[i];
		cerr<<"calling deletes with: "<<destination;
		deletes(destination);
	}
}

void deletes(string destination){
	cerr<<"in deletes \n";
	DIR *dir; 
	struct dirent *d; 
	struct stat filestat; 

	if(stat(destination.c_str(),&filestat)>=0){
		int m = filestat.st_mode;
		if(S_ISDIR(m)){
			if (!(dir = opendir((char *)destination.c_str()))) 
				return; 
			while ((d = readdir(dir)) != NULL) {	 
				if (d->d_type == 4) {//cerr << "enter" <<endl;
					
					if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) 
						continue; 
					string name=d->d_name; 
					string destination_new = destination+"/"+d->d_name; 
					//cerr <<source<<endl<<endl;
					deletes(destination_new); 
				}else{
					string destination_new = destination+"/"+d->d_name; 
					cerr<<"\n deleting12: "<<d->d_name;
					unlink(destination_new.c_str());
					perror("\nunlink: ");
				} 
					
			}
					cerr<<"\n deleting: "<<destination;
					rmdir(destination.c_str());
					perror("\nrmdir: ");
			
		}else{
			unlink(destination.c_str());
		}
	}
}
void move_dir(string, string,string);
void move_file(string, string);
void Move_mode(vector<string>results,string str){
		string source = str;
		string destination = "";
		if(strcmp(results[1].c_str(),"-r")==0){
			for(int i=2 ;i < results.size()-1;i++){
			destination = str+results[results.size()-1];
			string name = results[i];
			move_dir(source+"/"+name,destination,name);
			}
		}
		else{
			for(int i=1 ;i < results.size()-1;i++){
			destination = str+results[results.size()-1];
			string name = results[i];
			move_file(source+"/"+name,destination+"/"+name);
		}
	}
}
void move_dir(string source, string destination,string name){
	
	string original_source = source;
	copies( source, destination,name);
	deletes(original_source);
}

void move_file(string source, string destination){
	string original_source = source;
	copy_file(source, destination);
	deletes(original_source);
}

void searches(string , string);
void Search(vector<string>&results,string str) {
	string name = results[1];
	searches(name,str);
}
void searches(string name, string str){
	DIR *dir; 
	struct dirent *d; 
	struct stat filestat; 
	vector <string> search_str;
	string Path = str ;
	
	if(stat(Path.c_str(),&filestat)>=0){
		int m = filestat.st_mode;
		if(S_ISDIR(m)){
			if (!(dir = opendir((char *)Path.c_str()))) 
				return; 
			while ((d = readdir(dir)) != NULL) {	 
				if (d->d_type == 4) {
					
					if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) 
						continue;  
					string search_name = d->d_name;
					string Path_new = Path+"/"+d->d_name;
					//cerr << Path << " if " << Path_new<<endl ;
					if(strcmp(name.c_str(),search_name.c_str())==0){
						search_str.push_back(Path_new);
					}
					searches(Path_new,str); 
				}
				else{
					string Path_new = Path+"/"+d->d_name; 
					//cerr << "else" << Path_new << Path<<endl;
					string search_name = d->d_name;
					if(strcmp(name.c_str(),search_name.c_str())==0)
						search_str.push_back(Path_new);	
				} 
					
			}
			printf("\033[2J");
			printf("\033[;H");
			for(int i=0;i<search_str.size();i++){
				//cerr << "loop" <<endl;
				cout << search_str[i] << endl;
			}	
		}
	}
}
void snap_file(string ,string ,string);
void snapshot(vector<string>&results,string str){
	string name = str + "/" + results[1];
	string dumpfile = results[2];
	dumpfile = str +"/" + dumpfile;
	string currentPath = ".";
	snap_file(name, dumpfile,currentPath);
}

void snap_file(string name, string dumpfile, string currentPath){
	DIR *dir; 
	struct dirent *d; 
	struct stat filestat; 
	ofstream ofs;
	dir = opendir((char *)name.c_str());
	ofs.open (dumpfile.c_str(), std::ofstream::out | std::ofstream::app);
	ofs<<currentPath<<" ;"<<endl;
	while((d = readdir(dir)) != NULL){
		if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) 
			continue;  
		ofs << d->d_name<<" ";
	}
	ofs<<endl;
	ofs.close();
	dir = opendir((char *)name.c_str());
	while((d = readdir(dir)) != NULL){
		if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) 
			continue;  
		if(d->d_type==4){
			ofs.open (dumpfile.c_str(), std::ofstream::out | std::ofstream::app);
			ofs<<d->d_name<<":"<<endl;
			ofs.close();
			snap_file(string(name+"/"+string(d->d_name)),string(dumpfile),string(currentPath+"/"+string(d->d_name)));
		}
	}
}

int diropen(vector <stuff> &v,string str){
	//cerr<<"I was herer"<<str<<endl;
	DIR *d;
	struct group *grp;
	struct passwd *pwd;
	struct dirent *dir;
	struct stat file_stats;
	stat (str.c_str(), &file_stats);
	v.clear();
	d= opendir(str.c_str());
		
	if(d)
	{
		while((dir = readdir(d))!=NULL)
		{	//cerr<<"hereerr";
			string s = "";
			int fileMode;
			stuff current;
			//if(strcmp(ptr,dir->d_name)!=0 && strcmp(ptr1,dir->d_name)!=0 )
			//{ 
				string path_name = str+"/"+string(dir->d_name);
				//cerr<<path_name<<endl;
				if (!stat(path_name.c_str(), &file_stats))
            	{	
					//cout<<"opening"<<endl;
					fileMode = file_stats.st_mode;
					if (S_ISDIR(file_stats.st_mode)) s+="d"; else s+="-";
			    	if (file_stats.st_mode & S_IRUSR) s+="r"; else s+="-";
			    	if (file_stats.st_mode & S_IWUSR) s+="w" ; else s+="-";
					if (file_stats.st_mode & S_IXUSR) s+="x" ; else s+="-";
					if (file_stats.st_mode & S_IRGRP) s+="r" ;else s+="-";
					if (file_stats.st_mode & S_IWGRP) s+="w" ; else s+="-";
					if (file_stats.st_mode & S_IXGRP) s+="x" ; else s+="-";
					if (file_stats.st_mode & S_IROTH) s+="r" ; else s+="-";
					if (file_stats.st_mode & S_IWOTH) s+="w" ; else s+="-";
					if (file_stats.st_mode & S_IXOTH) s+="x" ; else s+="-";
					current.permissions = s;
					//cout << current.permissions <<"  " ;
        		}
           		else
            	{
                	printf("(stat() failed for this file)  ");
            	}

				uid_t me;
				me = getuid();
				pwd = getpwuid(me);
				//printf("%10d  ", pwd->pw_uid);
				//current.gidi = pwd->pw_uid;
				current.user = string(pwd->pw_name);//cout << current.user <<" " ;
				//printf("%10s  ", pwd->pw_name);

				grp = getgrgid(pwd->pw_gid);
				//printf("%10s  ", grp->gr_name);
				current .group = string(grp->gr_name);//cout << current.group <<" " ;
				if(S_ISDIR(file_stats.st_mode)){
					current.name = string(dir->d_name);	
					//printf("\033[34;1m%10s\033[0m  ", dir->d_name);
				}
				else if(file_stats.st_mode & S_IXOTH){
					current.name = string(dir->d_name);
					//printf("\033[32;1m%10s\033[0m  ", dir->d_name);
				}
				else{
					current.name = string(dir->d_name);
					//printf("%10s  ", dir->d_name);
				}	

				//cout << current.name <<" " ;

				if(file_stats.st_size < 1024){
					current.size = to_string(file_stats.st_size)+" Bytes ";
					//printf("%10u Bytes ", (unsigned int)file_stats.st_size);
				}
				else{
					double k = (double)file_stats.st_size/1024 ;
					//k = ceil(k*100)/100;
					current.size = to_string(k) + " KB ";
					//printf("%10.1lf KB    ", (double)k);
				}
				//cout << current.size <<" " ;
				current.time = string(ctime(&file_stats.st_mtime));
				current.time = current.time.substr(0,current.time.size()-1);
				//printf("%10s  ", ctime(&file_stats.st_mtime));
				//cout << current.time <<endl ;
			
			//}
			v.push_back(current);   
		}
		int i=0;
		printf("\033[2J");
		printf("\033[;H");
		printer(v,i);
		closedir(d);
	}
	//int x;
	//cin>>x;
	return v.size();
}

int main()
{	//cout.precision(1);
	//cout << setw(10) ;
	freopen("/home/shubham/Documents/OperAssign/log.txt","w",stderr);
	vector<stuff> v;
	printf("\033[?1049h\033[H") ;
	char choice;
	FILE *input;
	FILE *output;	
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	struct termios new_settings,initial_settings;

	if(!isatty(fileno(stdout))){
		fprintf(stderr,"You are not in a terminal, OK.\n");
	}

	input = fopen("/dev/tty", "r");
	output = fopen("/dev/tty", "w");
	//freopen("/home/shubham/Documents/OperAssign/error.txt","w",stderr);
	if(!input || !output){
		fprintf(stderr, "Unable to open /dev/tty\n");
		exit(1);
	}
	tcgetattr(fileno(input), &initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;
	//new_settings.c_lflag &= ~ISIG;
	if(tcsetattr(fileno(input), TCSANOW, &new_settings)!=0){
		fprintf(stderr, "could not set attributes\n");
	}
    int i=0;
	int cursor;
	char cwd[PATH_MAX];
	string str = getcwd(cwd, sizeof(cwd));
	int v_size = diropen(v,str);
	cursor = v_size+1;
	vector <string>pathvector;
	if(v_size > w.ws_row)
		cursor = w.ws_row;
	pathvector.push_back(str);
	int k = 0 ;
	do{     
		choice = fgetc(input);
		if(choice == 'q')break;

		
		if(choice == 10){
			string file = str +"/" + v[cursor -1].name;
			if(v[cursor-1].permissions[0] == 'd'){
				pathvector.push_back(file);
				k++;
				int v_size = diropen(v,file);
				printf("\033[2J");
				printf("\033[;H");
				cursor = v_size+1;
				int i = 0;
				if(v_size > w.ws_row)
					cursor = w.ws_row;
				printer(v,i);
			}
			else{
				if(fork()==0)
    				execlp("/usr/bin/xdg-open", "xdg-open", file.c_str() ,NULL);
			}
		}

 	    if(choice == '\033'){
    		choice = fgetc(input);
    		if(choice == '['){
				choice = fgetc(input);
				if(choice == 'A'){
					if(cursor == -1)continue; 
					if(i >=0 && cursor >=0){
						cursor--;
						printf("\033[1A");
						if(cursor <= i && i!=0){
							i--;
							printf("\033[2J"); 
							printer(v,i);
							printf("\033[;H");
						}
					}
				}
				if(choice == 'B'){
					if(cursor < v_size){
						cursor++;
						printf("\033[1B"); 
						if(cursor > w.ws_row ){
							i++;
							printf("\033[2J");
							printer(v,i);
						}
					}
				}
				if(choice == 'C'){
					if(k<pathvector.size()-1){
						int v_size = diropen(v,pathvector[++k]);
						printf("\033[2J");
						printf("\033[;H");
						cursor = v_size+1;
						if(v_size > w.ws_row)
							cursor = w.ws_row;
						printer(v,0);
					}
				}	
				if(choice == 'D'){
					if(k>=1){
						int v_size = diropen(v,pathvector[--k]);
						str = pathvector[k];
						printf("\033[2J");
						printf("\033[;H");
						cursor = v_size+1;
						if(v_size > w.ws_row)
							cursor = w.ws_row;
						printer(v,0);
					}
				}
    	   	}
	   	}
		if(choice == ':'){
			struct winsize w;
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			int x = w.ws_row;
			printf("\033[36;1H");
			cout << "command mode : ";
			string command = "";
			
			while(1){
				choice = fgetc(input);
				if(choice == 127){
					printf("\b \b");
					command.pop_back();
				}
				else if(choice == 27){
					printf("\033[2K");
					printf("\033[H");
					break;
				}

				else if(choice == 10){				
					std::istringstream iss(command);
					std::vector<std::string> results((std::istream_iterator<std::string>(iss)),istream_iterator<std::string>());				
					cerr << results.size() <<" "<< results[0]<<" "<<results[1]<<endl;;
					if(strcmp( results[0].c_str() ,"copy")== 0){
						Copy_mode(results,str);
						printf("\033[2K");
						printf("\033[36;1H");
						cout << "command mode : ";
					}

					else if(strcmp( results[0].c_str() ,"move")== 0){
						Move_mode(results,str);
						printf("\033[2K");
						printf("\033[36;1H");
						cout << "command mode : ";
					}
					
					else if(strcmp( results[0].c_str() ,"search")== 0){
						Search(results,str);
						printf("\033[2K");
						printf("\033[36;1H");
						cout << "command mode : ";
					}
					else if(strcmp( results[0].c_str() ,"create_dir")== 0){
						create_dir(results);
						//cerr << "hello"<<endl;		
						printf("\033[2K");
						printf("\033[36;1H");
						cout << "command mode : ";
					}
					else if(strcmp( results[0].c_str() ,"create_file")== 0){
						create_file(results);
						printf("\033[2K");
						printf("\033[36;1H");
						cout << "command mode : ";
					}
					else if(strcmp (results[0].c_str() ,"delete")== 0){
						delete_mode(results,str);
						printf("\033[2K");
						printf("\033[36;1H");
						cout << "command mode : ";
					}
					else if(strcmp( results[0].c_str() ,"goto")== 0){
						printf("\033[2J");
						printf("\033[;H");
						int v_sze = diropen(v,results[1].c_str());
						printf("\033[36;1H");
						cout << "command mode : ";
					}
					else if(strcmp( results[0].c_str() ,"rename")== 0){
						rename(results[1].c_str(), results[2].c_str());
						printf("\033[2K");
						cout << "command mode : ";
					}
					else if(strcmp( results[0].c_str() ,"snapshot")== 0){
						snapshot(results,str);
						printf("\033[2K");
						printf("\033[36;1H");
						
						cout << "command mode : ";
					}
					else{
						printf("\033[2K");
						printf("\033[36;1H");
						printf("Error no command found");
					}
					printf("\033[2K");
					printf("\033[36;1H");
					int v_size = diropen(v,str);
					printf("\033[36;1H");
					cout << "command mode : ";


				}
				else{
					command += choice;
					printf("%c",choice);
				}
			}
		}
		if(choice == 127){
			str = str.substr(0,str.find_last_of('/'));
			pathvector.push_back(str);
			k++;
			int v_size = diropen(v,str);
			printf("\033[2J");
			printf("\033[;H");
			cursor = v_size+1;
			if(v_size > w.ws_row)
				cursor = w.ws_row;
			printer(v,0);
		}	
		if(choice == 'h'){
			string str = ".";
			printf("\033[2J");
			printf("\033[;H");
			int i = 0;
			int v_size = diropen(v,str);
			cursor = v_size+1;
			if(v_size > w.ws_row)
				cursor = w.ws_row;
		}
	}while(1);
	
	tcsetattr(fileno(input), TCSANOW, &initial_settings);
    printf("\033[2J"); 	
    printf("\033[;H");  
    return 0;
} 

#include "header.h"
#include "abc.h"
using namespace std;

int main()
{	//cout.precision(1);
	//cout << setw(10) ;
	struct stuff{
		string permissions;
		string user;
		string group;
		string size;
		string name;
		string time;
};
	printf("\033[?1049h\033[H") ;
	char choice;
	FILE *input;
	FILE *output;
	
	vector<stuff>v;	
	
	struct termios new_settings,initial_settings;

	if(!isatty(fileno(stdout))){
		fprintf(stderr,"You are not in a terminal, OK.\n");
	}

	input = fopen("/dev/tty", "r");
	output = fopen("/dev/tty", "w");
	
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
    
	DIR *d;
	struct group *grp;
	struct passwd *pwd;
	struct dirent *dir;
	struct stat file_stats;
	stat ("alphabet", &file_stats);
	char const *ptr = ".";
	char const *ptr1 = "..";
	
	d= opendir(".");
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int cursor = w.ws_row;
	//cout << cursor << endl; 	
	if(d)
	{
		while((dir = readdir(d))!=NULL)
		{	string s = "";
			int fileMode;
			stuff current;
			if(strcmp(ptr,dir->d_name) && strcmp(ptr1,dir->d_name) )
			{ 
				if (!stat(dir->d_name, &file_stats))
            	{	
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
					k = ceil(k*100)/100;
					current.size = to_string(k) + " KB ";
					//printf("%10.1lf KB    ", (double)k);
				}
				//cout << current.size <<" " ;
				current.time = string(ctime(&file_stats.st_mtime));
				//printf("%10s  ", ctime(&file_stats.st_mtime));
				//cout << current.time <<endl ;
			
			}
			v.push_back(current);   
		}
		int i=0;
		printer(v,i);
		//cout << v.size() << " " << cursor;
		closedir(d);
	}int i=0;

	do{     
		choice = fgetc(input);
 	    if(choice == '\033'){
    		choice = fgetc(input);
    		if(choice == '['){
				choice = fgetc(input);
				if(choice == 'A'){
					cursor--; 
					if(i >=  0){
						printf("\033[1A");
						if(cursor < i){
							i--;
							printf("\033[2J"); 
							printer(v,i);
							printf("\033[;H");
						}
					}
					else
					cursor++;
				}
				if(choice == 'B'){
					cursor++;
					if(cursor <= v.size()){
						printf("\033[1B"); 
						if(cursor > w.ws_row){
							i++;
							printf("\033[2J");
							printer(v,i);
						}
					}
					else
					cursor--;
				}		
    	   	}
	   	}
				
	}while(choice != 'q');
	
	tcsetattr(fileno(input), TCSANOW, &initial_settings);
    //printf("\033[2J"); 	
    //printf("\033[?1041h");  
    return 0;
} 

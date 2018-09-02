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


int main()
{
  printf("\033[?1049h\033[H") ;
  char choice;
  int cursor = 0;
  FILE *input;
  FILE *output;
	
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
	new_settings.c_lflag &= ~ISIG;
	if(tcsetattr(fileno(input), TCSANOW, &new_settings)!=0){
		fprintf(stderr, "could not set attributes\n");
	}
    
	DIR *d;
	struct group *grp;
	struct passwd *pwd;
	struct dirent *dir;
	struct stat file_stats;
	stat ("alphabet", &file_stats);
	char *ptr = ".";
	char *ptr1 = "..";
	
	d= opendir(".");
		
	if(d)
	{
	
		while((dir = readdir(d))!=NULL)
		{
			int fileMode;
			
			if(strcmp(ptr,dir->d_name) && strcmp(ptr1,dir->d_name))
			{

				if (!stat(dir->d_name, &file_stats))
            			{	fileMode = file_stats.st_mode;
					printf( (S_ISDIR(file_stats.st_mode)) ? "d" : "-");
				    	printf( (file_stats.st_mode & S_IRUSR) ? "r" : "-");
				    	printf( (file_stats.st_mode & S_IWUSR) ? "w" : "-");
				    	printf( (file_stats.st_mode & S_IXUSR) ? "x" : "-");
				    	printf( (file_stats.st_mode & S_IRGRP) ? "r" : "-");
				    	printf( (file_stats.st_mode & S_IWGRP) ? "w" : "-");
				    	printf( (file_stats.st_mode & S_IXGRP) ? "x" : "-");
				    	printf( (file_stats.st_mode & S_IROTH) ? "r" : "-");
				    	printf( (file_stats.st_mode & S_IWOTH) ? "w" : "-");
				    	printf( (file_stats.st_mode & S_IXOTH) ? "x  " : "-  ");
					
            			}
           			else
            			{
                		printf("(stat() failed for this file)  ");
            			}
				
				uid_t me;
				me = getuid();
				pwd = getpwuid(me);
				printf("%10d  ", pwd->pw_uid);
				printf("%10s  ", pwd->pw_name);

				grp = getgrgid(pwd->pw_gid);
				printf("%10s  ", grp->gr_name);
			if(S_ISDIR(file_stats.st_mode))
				printf("\033[34;1m%10s\033[0m  ", dir->d_name);
			else if(file_stats.st_mode & S_IXOTH)
				printf("\033[32;1m%10s\033[0m  ", dir->d_name);
			else
				printf("%10s  ", dir->d_name);


			if(file_stats.st_size < 1024)
                	printf("%10u Bytes ", (unsigned int)file_stats.st_size);
			else{
			double k = (double)file_stats.st_size/1024 ;
			printf("%10.1lf KB    ", (double)k);
				}
			printf("%10s  ", ctime(&file_stats.st_mtime));
			cursor ++;
			}   
 	
		}
		closedir(d);
	}
	
	
	do{
           choice = fgetc(input);
 	   if(choice == '\033'){
			choice = fgetc(input);
		if(choice == '['){
			choice = fgetc(input);
		if(choice == 'A')printf("\033[1A");
		if(choice == 'B')printf("\033[1B");		
	   	
	}
	}
	
	printf("\033[cursorA");
	tcsetattr(fileno(input), TCSANOW, &initial_settings);
  printf("\033[2J"); 	
  printf("\033[?1041h");  
return 0;
} 

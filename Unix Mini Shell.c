/*Project title- To emulate a replica of the unix shell in C
  Author- Abhishek Banerjee
  Sem- 6 CSE A
  USN- 1PE11CS004
  OS - Ubuntu 12.04 LTS
*/

//Header Files
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<ustat.h>
#include<time.h>
#include<grp.h>
#include<pwd.h>

//Global Declarations
extern char **environ;

//Program Starts
//Function to count the number of arguments and converting the arguments in given input to tokens
int getArguments(char inp[10],char *argv[50])		
{
	char *ptr;//Declaring a pointer to count the number of arguments
	int i=0;
	ptr=strtok(inp," ");//Delimiting by single space
	while(ptr!=NULL)//Traversing arguments as long as pointer is not null and adding them to array argv
	{
		argv[i++]=ptr;//Storing succesive arguments as tokens in array argv
		ptr=strtok(NULL," ");//Initialising Last argument to null
	}	
	return i;//return the value of number of arguments
}

//Function for the link command
void myln(char *arguments[],int argcount)//Passing the number of arguments and all the individual arguments that have been recognised as input in main
{	if(strcmp(arguments[1],"-s")==0)  //comparing argument value with string -s 
         		{  	if(!symlink(arguments[2],arguments[3]))//checking if soft link can be created and creates soft link
                 			printf("soft link created\n");//printing the suitable messgaes
             			else //condition for error
                			printf("error creatng symlink\n");//error message printing

          		}
          		else if(argcount==3)//If three arguments are given
			{	 if(!link(arguments[1],arguments[2])) //checking if hard link can be created and create hard link
                    			printf("hard link created\n");//printing suitable message
                    		else //error condition
                   			printf("error creating hard link\n");//Printing error message
			}
          		else 
				printf("syntax error\n");//Syntax error condition

}

//Function for the copy command		
void mycp(char *arguments[],int argcount)//Passing the number of arguments and all the individual arguments that have been recognised as input in main
{
	int fd,fd1,r;//initialising file descriptors
	char buf[50];//iniialising buffer to store file data
	switch(argcount) //taking the number of arguments into switch case
	{
		case 1: //condition if no arguments exist
		printf("Error:No arguments!");//printing error message
		break;
		case 2://when one argument is given by the user
		printf("Error:Too few arguments for mycp command!");//printing few arguments message
		break;
		case 3://when two arguments are given by the user
	        fd=open(arguments[1],0,00777);//open the first argument
		if(fd==-1)//check if error in opening exists
		{	printf("Error:No source file!");//print suitable error message
			exit(0);//exit
		}
		fd1=creat(arguments[2],00777);//create the file which we have to copy the data of first file to	
		while((r=read(fd,buf,1))>0)//read the contents of first file in a buffer
		write(fd1,buf,1);//write the contents of first file to the second file
		printf("Contents of file %s copied to file %s\n",arguments[1],arguments[2]);//printing suitable message
		break;//break condition
		default:
		printf("Error:Excess arguments supplied!");//error condition.print suitable error message
		break;
	}
	
}

//Function for  move command
void move(char *arguments[],int argcount)//Passing the number of arguments and all the individual arguments that have been recognised as input in main
{	int fd,fd1,r;//initialise file descriptors
	char buf1[50],buf2[50];//initialise two buffers to store data 
	switch(argcount)//taking number of arguments in switch
	{	case 1://one argument.error 
		printf("Error:No arguments!");//print suitable error message
		break;
		case 2://two arguments .error
		printf("Error:Too few arguments for move command!");//print suitable message 
		break;
		case 3:
		myln(arguments,argcount);//linking the file in the first argument to the file in second
		unlink(arguments[1]);//deleting the first file
		printf("Contents of file %s moved to file %s\n",arguments[1],arguments[2]);//suitable message
		break;
		default:
		printf("Error:Excess arguments supplied!**!");//error condition.printing suitable message
		break;
	}
	
}

/*ls command start*/
	void myls(char *arguments[],int argcount)
	{
		DIR * d,*d1;
		struct dirent *con;
		int i,flag;
		char path[100];
		switch(argcount)
		{
			case 1:
			display(".",0);
			break;
			default:
			if(!strcmp(arguments[1],"-1"))
			display(".",1);
			else if(!strcmp(arguments[1],"-l"))
			{
				longlisting();
			}
			
		}
	printf("\n");
	
	}

	void display(char fullpath[],int flag)
	{
		DIR * d;
		struct dirent * con;
		d=opendir(fullpath);
		while((con=readdir(d))!=NULL)
		if(strcmp(con->d_name,".")!=0 && strcmp(con->d_name,"..")!=0)
		{
			if(flag==0)
			printf("%s\t",con->d_name);
			else printf("%s\n",con->d_name);
		}
	}
	void longlisting()
	{
		DIR *d;
		struct dirent * con;
		struct stat details;
		struct group *gr;
		struct passwd *pw;
		struct tm *t;
		char buf[100];
		t=(struct tm*)malloc(sizeof(struct tm));
		d=opendir(".");
		while((con=readdir(d))!=NULL)
		if(strcmp(con->d_name,".")!=0 && strcmp(con->d_name,"..")!=0)
		{
			stat(con->d_name,&details);
			gr=getgrgid(details.st_gid);
			pw=getpwuid(details.st_uid);
			t=localtime(&details.st_ctime);
			strftime(buf,sizeof(buf),"%b %e %t",t);
			if(S_ISDIR(details.st_mode))
				printf("d");
			else printf("-");
			if((details.st_mode & S_IRUSR)!=0)
				printf("r");
			else printf("-");
			if((details.st_mode & S_IWUSR)!=0)
				printf("w");
			else printf("-");
			if((details.st_mode & S_IXUSR)!=0)
				printf("x");
			else printf("-");
			if((details.st_mode & S_IRGRP)!=0)
				printf("r");
			else printf("-");
			if((details.st_mode & S_IWGRP)!=0)
				printf("w");
			else printf("-");
			if((details.st_mode & S_IXGRP)!=0)
				printf("x");
			else printf("-");
			if((details.st_mode & S_IROTH)!=0)
				printf("r");
			else printf("-");
			if((details.st_mode & S_IWOTH)!=0)
				printf("w");
			else printf("-");
			if((details.st_mode & S_IXOTH)!=0)
				printf("x");
			else printf("-");
			printf("%2d",details.st_nlink);
			printf("%10s",pw->pw_name);
			printf("%10s",gr->gr_name);
			printf("  %s ",buf);
			printf("%13s",con->d_name);
			printf("\n");
		}
	}
/*ls command finish*/

//Function for cat command
void mycat(char *arguments[],int argcount)//Passing the number of arguments and all the individual arguments that have been recognised as input in main
{	int i,fdesc,fdesc1,r;//Initialising file descriptors
	char buf[50];//initialising buffer to store data
	if(argcount<2)//checking number of arguments.error
	{	printf("Too few arguments!!");//print suitable error messages
		return 0;//returns 0 to main
	}
	/*else if(argcount>2)
	{	if(!strcmp(arguments[1],">"))
		{	fdesc=creat(arguments[2],00777); 
			while((r=read(0,buf,50))>0)
			write(fdesc,buf,r);
			return 0;
		}
		else if(!strcmp(arguments[2],">"))
		{	fdesc=open(arguments[1],0,00777);
			if(fdesc==-1)
			{	printf("mycat:%s:No such file or directory!**!\n",arguments[1]);
				return 0;
			}
			fdesc1=creat(arguments[3],00777);
			while((r=read(fdesc,buf,50))>0)
			write(fdesc1,buf,r);
			return 0;
		}
		else if(!strcmp(arguments[2],">>"))
		{	fdesc=open(arguments[1],O_RDWR);
			fdesc1=open(arguments[3],O_RDWR|O_APPEND);
			if(fdesc==-1)
			{	printf("error");
				return 1;
			}
			if(fdesc1==-1)
			fdesc=creat(arguments[3],00777);
			while((r=read(fdesc,buf,50))>0)
				write(fdesc1,buf,r);
				return 0;
		}	
		else
		{	for(i=1;i<argcount;i++)
			{	fdesc=open(arguments[i],O_RDWR);
				if(fdesc==-1)
					printf("mycat:%s:No such file or directory!**!\n",arguments[i]);
				else	
					while((r=read(fdesc,buf,50))>0)
						write(1,buf,r);
			}
		}
	}*/
	else //condition with proper number if arguments
	{	fdesc=open(arguments[1],0,O_RDWR);//opening the file we have to cat
		if(fdesc==-1)//error condition
			printf("mycat:%s:No such file or directory!**!\n",arguments[1]);//file not exists message
		else
			while((r=read(fdesc,buf,50))>0)//reading the contents of the file
				write(1,buf,r);//writing to buffer and display the file in terminal
	}
	return 0;
}

//mkdir command

void mymkdir(char *arguments[],int argcount)
{
	if(argcount!=2)
	{
		printf("Invalid number of arguments \n");
		return;
	}
	
	if(mkdir(arguments[1],0777)==0) //creating a new directory using mkdir api
		printf("Directory %s created! \n",arguments[1]); //printing the name of directory created
	else
		printf("Invalid path Or directory already exists \n");
}
//mkdir finished

//rmdir command
void myrmdir(char *arguments[],int argcount)
{
	if(argcount!=2)
	{
		printf("Invalid number of arguments \n");
		return;
	}
	
	if(rmdir(arguments[1])==0)//removing the directory specified in the arguments
		printf("Directory %s successfully removed!",arguments[1]);
	else
		printf("Invalid path / Directory is not empty \n");
}
//rmdir command finished

//cd command
int mycd(char *arguments[],int argcount)
{

	if(argcount!=2)//checking for number of arguments
	{
		printf("invalid!!\n");
		exit(0);
	}

	if(access(arguments[2],F_OK)==0)//checking for prior existence of directory
	{
		printf("file exits\n");
		exit(1);
	}
	
	else
	{	
		if(!chdir(arguments[1]));
		printf("directory changed\n");
		system("pwd");//displaying the present workig directory
	}
	return 0;
}

//Function for rm command
void myrm(char *arguments[],int argcount)//Passing the number of arguments and all the individual arguments that have been recognised as input in main
{	if(argcount!=2)//Checking for less arguments 
	{	printf("Not enough arguments\n");//printing suitable error message
		return ;
	}
    unlink(arguments[1]);//if proper arguments are there.delete the file given in arguments
	printf("File successfully deleted\n");//print suitable message
}

//Function for date command.Displays only the date
void mydate(char *arguments[],int argcount)//Passing the number of arguments and all the individual arguments that have been recognised as input in main
{	time_t t=time(NULL); //initialising time variable.Time_t is a variable that can hold date and time details
	struct tm tm=*localtime(&t);//defining struct tm which has time and date fields as components.Initialising it to the value of the local time
	printf("The date is: %d-%d-%d \n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday);//Printing only the date fields 
}

//time
void mytime(char *arguments[],int argcount)//Passing the number of arguments and all the individual arguments that have been recognised as input in main
{	time_t t=time(NULL);//initialising time variable.Time_t is a variable that can hold date and time details
	struct tm tm=*localtime(&t);//defining struct tm which has time and date fields as components.Initialising it to the value of the local time
	printf("The time is: %d:%d:%d \n",tm.tm_hour,tm.tm_min,tm.tm_sec);//Printing only the time fields
}

//Function for exit command
void myexit(char *arguments[],int argcount)//Passing the number of arguments and all the individual arguments that have been recognised as input in main
{	if(argcount==1)//Checking if number of arguments is only one
		_exit(0);//Calling _exit.This exits the program immmediately without performing cleanup actions
}

//Function for touch command.Changes file timesamps
void mytouch(char *arguments[],int argcount)//Passing the number of arguments and all the individual arguments that have been recognised as input in main
{	int fd1;//Declarinf file descriptors
	if(access(arguments[1],F_OK)==0)//Checking if file already exists
	{	fd1=open(arguments[1],O_RDWR,00777);//Open the file using open API
		close(fd1);//Close the file. This changes the last modification time of the file
	}
	else//Conditon for If file does not exist
	{	creat(arguments[1],00777);//Creating the file
		fd1=open(arguments[1],O_RDWR,00777);//Opening the file
		close(fd1);//Closing the file
	}
}

//Function for perm command.Shows what all permissions does the file given as argument have
void myperm(char *arguments[],int argcount)//Passing the number of arguments and all the individual arguments that have been recognised as input in main
{
	if(argcount==2)
	{	if(access(arguments[1],R_OK)==0)//checking for read permission.access checks for existence of the file with respect to the second argument
		{	printf("read permission exists\n");//printing suitable message
		}
		if(access(arguments[1],W_OK)==0)//checking for write permission
		{	printf("write permission exists\n");//printing suitable message
		}
		if(access(arguments[1],X_OK)==0)//checking for execute permisiions
		{	printf("execute permission exists\n");//printing suitable message
		}
	}
	return 0;
}

//Function for pwd command.Displays the present working directory
int mypwd(char *arguments[],int argcount)//Passing the number of arguments and all the individual arguments that have been recognised as input in main
{	if(argcount!=1)//Checking if arguments given as input are less than required
	{	printf("invalid number of arguments\n");//Printing error message
		exit(0);//exiting the program to main	
	}
	else//else condition
	{	printf("%s\n",get_current_dir_name());//In built function for Printing the name of the current directory we are woking in.
	}
	return 0;
}

//Function for env command.It displays the environment lists and variables
void myenv() //Calling the env function without any arguments as it takes vales from global declarations.
{	int i=0; //Initialising loop variable to 0
    while(environ[i]!=NULL) //Start of while loop.Checks for variables until environment list finishes
	{	puts(environ[i]);//Printing each environment variable
		i++; //Incrementing loop variable
		printf("\n"); //Line break after every variable
	}
}

//Function for echo command
void myecho(char *arguments[])//Passing the second argument as token to the function
{	printf("%s\n",arguments[1]); //Printing the second argument as is the job of UNIX echo command
}

//Main Function
int main() //main function to call all the commands as functions
{ 
	int argcount,i; //declaring variables for no. of arguments and loop variables

	char input[10],*arguments[50]; //declaring variables for input provided and the arguments that are treated as tokens later 
	do // do while loop start
	{
		printf("\nabhishekbanerjee@shell:~$"); //Shell Prompt.Will pop up every time after we execute a command in the mini shell
		gets(input); //Taking input fron the user which will be treated as different tokens separated by delimiter space

		argcount=getArguments(input,arguments);//Calling the get arguments function to get the number of arguments and individual tokens
		/*First Command- link*/if(strcmp(arguments[0],"link")==0)//Compares the value of first argument to link and calls the required function
					myln(arguments,argcount);//calling function for my link
		/*Second Command- copy*/else if(strcmp(arguments[0],"copy")==0)//Compares the value of first argument to copy and calls the required function
					mycp(arguments,argcount);//calling function for my copy
		/*Third Command- move*/else if(strcmp(arguments[0],"move")==0)//Compares the value of first argument to move and calls the required function
					move(arguments,argcount);//calling function for my move
        /*Fouth Command- ls*/ else if(strcmp(arguments[0],"ls")==0)//Compares the value of first argument to ls and calls the required function
					myls(arguments,argcount);//calling function for my ls
		/*Fifth Command- cat*/else if(strcmp(arguments[0],"cat")==0)//Compares the value of first argument to cat and calls the required function
					mycat(arguments,argcount);//calling function for my cat
        /*Sixth Command-env*/ else if(strcmp(arguments[0],"env")==0)//Compares the value of first argument to env and calls the required function
					myenv();//calling function for my env
		/*Seventh Command- mkdir*/else if(strcmp(arguments[0],"mkdir")==0)//Compares the value of first argument to mkdir and calls the required function
					mymkdir(arguments,argcount);//calling function for my mkdir
		/*Eighth Command- rmdir*/else if(strcmp(arguments[0],"rmdir")==0)//Compares the value of first argument to rmdir and calls the required function
					myrmdir(arguments,argcount);//calling function for my rmdir
	    /*Ninth Command- cd*/else if(strcmp(arguments[0],"cd")==0)//Compares the value of first argument to cd and calls the required function
					mycd(arguments,argcount);//calling function for my cd
		/*Tenth Command- rm*/else if(strcmp(arguments[0],"rm")==0)//Compares the value of first argument to rm and calls the required function
					myrm(arguments,argcount);//calling function for my rm
		/*Eleventh Command- date*/else if(strcmp(arguments[0],"date")==0)//Compares the value of first argument to date and calls the required function
					mydate(arguments,argcount);//calling function for my date
		/*Twelfth Command- time*/else if(strcmp(arguments[0],"time")==0)//Compares the value of first argument to time and calls the required function
					mytime(arguments,argcount);//calling function for my time
		/*Thirteenth Command- touch*/else if(strcmp(arguments[0],"touch")==0)//Compares the value of first argument to touch and calls the required function
					mytouch(arguments,argcount);//calling function for my touch
		/*Fourteenth Command-exit*/else if(strcmp(arguments[0],"exit")==0)//Compares the value of first argument to exit and calls the required function
					myexit(arguments,argcount);//calling function for my exit
		/*Fifteenth Command-permission*/else if(strcmp(arguments[0],"permission")==0)//Compares the value of first argument to permission and calls the required function
					myperm(arguments,argcount);//calling function for my perm
		/*Sixteenth Command-pwd*/ else if(strcmp(arguments[0],"pwd")==0)//Compares the value of first argument to pwd and calls the required function
					mypwd(arguments,argcount);//calling function for my pwd
		/*Seventeenth command - echo*/ else if(strcmp(arguments[0],"echo")==0)//Compares the value of first argument to echo and calls the required function
						myecho(arguments);//calling function for my echo
		

	}while(1);//Inifinite loop using do while.The statement inside will pop up till we choose to exit

	return 0;//returning 0 to main
}


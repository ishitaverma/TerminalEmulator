#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int mystrcmp(char const *p, char const *q)
{
    int i = 0;
    for(i = 0;q[i];i++)
    {
        if(p[i] != q[i])
            return -1;
    }
    return 0;
}

char *readline()
{
	char *inputline = NULL;
	size_t buffersize=0;
	getline(&inputline, &buffersize, stdin);
	//printf("%s\n", inputline);
	return inputline;
}

void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    printf("\n Cant terminate \n");
    fflush(stdout);
}

char **parseargs(char *line)
{
	char *token;
	token=strtok(line, " ");
	int i=0;
	char **arraytokens= (char **)(malloc(1024*sizeof(char*)));
	while(token!=NULL)
	{
		//printf("%s\n", token);
		arraytokens[i]=token;
		i++;
		token=strtok(NULL, " ");
	}
	arraytokens[i]=NULL;
	return arraytokens;
}
int pwd()
{
	size_t sizecwd=1024;
	char *cwd;
	if(getcwd(cwd,sizeof(cwd))!=NULL)
		printf("%s", cwd);
	return 1;
}
int cd(char** myargs)
{
	chdir(myargs[1]);
	return 1;
}
int startprog(char **myargs)
{
	if(mystrcmp(myargs[0], "cd")==0)
	{
		printf("cd karenge\n");
		return cd(myargs);
	}
	else if(mystrcmp(myargs[0],"pwd")==0)
	{
		printf("pwd karenge\n");
	 	return pwd();
	}
	return start(myargs);
}
int start(char** myargs)
{
	pid_t pid,wpid;
	int status;
	pid=fork();
	if(pid<0)
	{
		printf("error in forking\n" );
		exit(EXIT_SUCCESS);
	}
	else if(pid==0)
	{
		int x= execvp(myargs[0], myargs);
		printf("%d\n", x );
		if(execvp(myargs[0],myargs)!=0)
		{
			printf("error\n");
		}
		exit(EXIT_FAILURE);
	}	
	else {
		int wc= wait(NULL);
    }
	return 1;
}

int main(int argc, char *argv[])
{
	char *inputline;
	char **myargs;
	int errorstatus;
	errorstatus=1;
	//signal(SIGINT, sigintHandler);
	do 
	{
		printf("$ ");
	    inputline=readline();
		myargs=parseargs(inputline);
		errorstatus=startprog(myargs);
	}
	while(errorstatus);
	
	free(inputline);
	free(myargs);
}
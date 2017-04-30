#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//the maximum amount of tokens allowed per commmand
#define MAX_SIZE 10

//takes 3 arrays of arguments as input
//uses nested forks to run processes with these arguments concurrently
void run(char** argsArray1, char** argsArray2, char** argsArray3);

//parses arg (space delimited) and returns its conctents as
//an array ending with filename and then NULL
char** parse(char* arg, char* filename);

//Evan Pernu
//4/28/2017
//TCSS 422 Assignment 1 (Mash)
//This program takes three commands and a filename as user input.
//It then simultaneously executes these commands on the file. 
int main(){

    char arg1[255];
    char arg2[255];
    char arg3[255];
    char filename[255];

    //get user input
    printf("mash-1>");
    fgets(arg1, sizeof arg1, stdin);
    strtok(arg1, "\n");
    printf("mash-2>"); 
    fgets(arg2, sizeof arg2, stdin);
    strtok(arg2, "\n");
    printf("mash-3>");
    fgets(arg3, sizeof arg3, stdin);
    strtok(arg3, "\n");
    printf("file>");
    fgets(filename, sizeof filename, stdin);
    strtok(filename, "\n");

    char** argsArray1 = parse(arg1, filename);
    char** argsArray2 = parse(arg2, filename);
    char** argsArray3 = parse(arg3, filename);
	
    run(argsArray1, argsArray2, argsArray3);

    return 0;
}

//parses arg (space delimited) and returns its conctents as
//an array ending with filename and then NULL
char** parse(char* arg, char* filename){

    char** argsArray = (char**)malloc(sizeof(char*)*MAX_SIZE);
    char *token = strtok(arg, " ");
    int count = 0;

    while(token != NULL)
    {
        argsArray[count] = token;
        token = strtok(NULL, " ");
	count++;
    }

    argsArray[count] = filename;
    argsArray[count+1] = NULL;
    return argsArray;
}

//takes 3 arrays of arguments as input
//uses nested forks to run processes with these arguments concurrently
void run(char** argsArray1, char** argsArray2, char** argsArray3){
    
    int status;
    pid_t p1, p2, p3;

    p1 = fork();
    if (p1 == 0){
	//child 1, execute arg1
	if(execvp(argsArray1[0], argsArray1) == -1){

	    //execution failed. error
	    printf("[SHELL 1] STATUS CODE=-1\n");
	    exit(-1);
	}

    }else if(p1>0){
	p2 = fork();

	if(p2 == 0){
	    //child 2, execute arg2
	    if(execvp(argsArray2[0], argsArray2) == -1){

	        //execution failed. error
	        printf("[SHELL 2] STATUS CODE=-1\n");
	        exit(-1);
	}

	} else if(p2>0){ 
	    p3 = fork();

	    if(p3 == 0){  
		//child 3, execute arg3
		if(execvp(argsArray3[0], argsArray3) == -1){

                    //execution failed. error
	  	    printf("[SHELL 3] STATUS CODE=-1\n");
	   	    exit(-1);
	}

	    }else if(p3>0){
		//parent is done forking. wait for all children
		waitpid(p1, &status, 0);
		waitpid(p2, &status, 0);
		waitpid(p3, &status, 0);  
		printf("Done waiting on children: %ld %ld %ld.\n", p1, p2, p3); 
	    }
	}
    }
}




 

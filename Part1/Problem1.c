/*

Problem 1 
author-: Alejandro Aguilar 
Program takes in an input file filled with numbers calculates the min,max and sum
and output results to a file

*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include<unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <ctype.h>



typedef struct _input
{
  int* array;
  int size;
}input;


void *sum(void* arg);
void *max(void* arg);
void *min(void* arg);
void programC();


static sigset_t signal_maskFirst;  


void programC(){

    sigaddset (&signal_maskFirst, SIGTERM);
    sigaddset (&signal_maskFirst, SIGABRT);
    sigaddset (&signal_maskFirst, SIGTSTP);
    sigaddset (&signal_maskFirst, SIGKILL);

    
    printf("Enter file name including the extension, example 'file.txt' \n");
    char file;
    scanf("%s",&file);

    FILE *fp = fopen(&file, "r");
    
    int size = 0;
    int ch = 0;
    char *buffer;
    size_t bufsize = 32;

    if(fp == NULL)
    {
        perror("Error! File not opened");
        exit(1);
    }

    
    while(!feof(fp))
    {
        
        ch = fgetc(fp);
        if(ch == '\n')
	{	
            size++;
	}
    }

    printf("%d is size of file\n",size);
    rewind(fp);
    

    input* infile = malloc (sizeof(input)*1);
    buffer = (char *)calloc('\0',bufsize * sizeof(char));
    
    infile->size = size;
    int* arr = (int*) malloc(sizeof(int)*(size));
    
    int counter = 0;

    while(getline(&buffer,&bufsize,fp) != EOF && counter < size)
    {
     arr[counter] = atoi(buffer);
     counter++;
    }

    infile->array = arr;
    fclose(fp);
    pthread_t sum_thread, max_thread, min_thread;
 
    pthread_sigmask(SIG_BLOCK, &signal_maskFirst, NULL);
    pthread_create(&min_thread, NULL, (void*)min, (void*)infile);
  

    sigaddset (&signal_maskFirst, SIGINT);
    sigaddset (&signal_maskFirst, SIGQUIT);
    sigaddset (&signal_maskFirst, SIGSTOP);

    pthread_sigmask(SIG_BLOCK, &signal_maskFirst, NULL);

    pthread_create(&sum_thread, NULL, (void*)sum, (void*)infile);
    pthread_create(&max_thread, NULL, (void*)max, (void*)infile);
    
    pthread_kill(sum_thread,SIGQUIT);

    pthread_join(sum_thread,NULL);
    pthread_join(max_thread,NULL);
    pthread_join(min_thread,NULL);
    
}




void main(int argc, char *argv[]){

	printf("This is a Modified version of Problem 0, Part C of Project 1\n\n");
	programC();
		
}


void *sum(void* arg){
    int sum = 0;  
    int sig_caught;
    int rc = sigwait (&signal_maskFirst, &sig_caught);
    
    input* in = (input*) arg;
    for(int i = 0; i < in->size; i++)
      {
        sum += in->array[i];
      }
     printf("Sum: %d\n", sum); 
}

void *min(void* arg){
    input* in = (input*) arg;
    int min = INT_MAX;
    for(int i = 0; i < in->size; i++ )
        {
          if(min > in->array[i])
            min = in->array[i];
        }
    printf("Min: %d\n", min);
}

void *max(void* arg){
     input* in = (input*) arg;
     int max = INT_MIN;
      for(int i = 0; i< in->size; i++ )
        {
          if(max < in->array[i])
            max = in->array[i];
        }
      printf("Max: %d\n", max);
}


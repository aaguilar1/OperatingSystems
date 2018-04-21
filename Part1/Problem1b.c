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
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

static int marker; 
int calculate(int* s, int i, int size);

/*
	Each handler will return an interger that will be used in a switch case to determine min,max, and sum.
	

*/

void sigUSR1_Handler(int signum){
  marker = signum;
    return;
}
void sigUSR2_Handler(int signum){
  marker = signum;
    return;
}
void sigCONT_Handler(int signum){
  marker = signum;
    return;
}

int calculate(int *s,int q,int size)
{
   int max = INT_MIN;
   int min = INT_MAX;
   int sum = 0;
   int i = 0;
	//printf("This is the value of q: %d\n\n",q);
  switch(q)
  {
    case 0:
      {
      for(; i < size; i++)
      {
        sum += s[i];
      }
      return sum;
    }
        break;
    case 12:
        {
        for(; i < size; i++ )
        {
          if(min > s[i])
            min = s[i];
        }
        return min;
          }
        break;
    case 10:
       {
        for(; i< size; i++ )
        {
          if(max < s[i])
            max = s[i];
        }
        return max;
      }
        break;
    default:
        break;
  }
}


void programC()
{

    signal(SIGUSR1, sigUSR1_Handler);
    signal(SIGUSR2, sigUSR2_Handler);
    signal(SIGCONT, sigCONT_Handler);

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



    buffer = (char *)calloc('\0',bufsize * sizeof(char));

    int* arr = (int*) malloc(sizeof(int)*(size));
    int counter = 0;

    while(getline(&buffer,&bufsize,fp) != EOF && counter < size)
    {
      arr[counter] = atoi(buffer);
      counter++;
    }
    
    fclose(fp);


    pid_t process[4];
    int status[4];
    /*A process tree of threads is created */
    process[0] = fork();
    if(process[0] == 0)
    {
      process[3] = fork();
      if(process[3] == 0)
      {
            
            process[1] = fork();
             if(process[1]==0)
              {
                  
                  process[2] = fork();
              if(process[2] == 0)
                {
                  kill(getppid(),SIGUSR1);        
                }
              else
                {
                  waitpid(process[2],&status[2],0);
                  printf("Max: %d\n",calculate(arr,marker,size));
                  kill(getppid(),SIGUSR2);
                }
              }
          else
            { 
              waitpid(process[0],&status[0],0);
              printf("Min: %d\n",calculate(arr,marker,size));
              kill(getppid(),SIGCONT);
            }
      }
      else
      {
      waitpid(process[1],&status[1],0);
      printf("Sum: %d\n",calculate(arr,marker,size));
      raise(SIGKILL);
      }
    }
    else
    {

      waitpid(process[3],&status[3],0);
    }


}

void main(int argc, char *argv[])
{
	printf("This is a process tree of threads.\n\n");
	programC();   
}




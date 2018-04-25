/*
Problem 1) c)
Author: Alexander Riveron
Description: Program
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
#include <pthread.h>
#include <ctype.h>

static int marker; 
int calculate(int* s, int i, int size);

/*
	Each handler will return an interger that will be used in a switch case to determine min,max, and sum.

  Note for part c, a USR signal is needed, so USR2 will be replaced with USR1 from previous parts

*/

void sigUSR1_Handler(int signum){
  marker = signum;
    return;
}
/*
void sigUSR2_Handler(int signum){
  marker = signum;
    return;
}
*/
void sigCONT_Handler(int signum){
  marker = signum;
    return;
}


/*
  List of default signal handlers
*/

void INTHANDLER(int);
void QUITHANDLER(int);
void STOPHANDLER(int);
void TSTPHANDLER(int);
void ABRTHANDLER(int);
void TERMHANDLER(int);
void KILLHANDLER(int);
void USR2HANDLER(int);

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
    //signal(SIGUSR2, sigUSR2_Handler);
    signal(SIGCONT, sigCONT_Handler);

    //Part c
    signal(SIGINT, INTHANDLER);
    signal(SIGQUIT, QUITHANDLER);
    signal(SIGSTOP, STOPHANDLER);
    signal(SIGTSTP, TSTPHANDLER);
    signal(SIGABRT, ABRTHANDLER);
    signal(SIGTERM, TERMHANDLER);
    signal(SIGKILL, KILLHANDLER);
    signal(SIGUSR2, USR2HANDLER);

    printf("testing signals\n");
    printf("Current PID: %d", getpid());

    int i=0;
    while(1){
    	printf("%d\n", i);
    	i++;
    	sleep(1);
    }

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


/*
Signal Definitions
*/
void INTHANDLER(int sig){

  signal(sig, SIG_IGN);
  printf("Ctrl-C signal detected\nPID: %d\n",getpid());
  signal(sig, INTHANDLER);
}

void QUITHANDLER(int sig){

  printf("Quit signal detected\nPID: %d\n",getpid());
  signal(sig, SIG_IGN);
  signal(sig, QUITHANDLER);
}

void STOPHANDLER(int sig){

  printf("Stop signal detected\nPID: %d\n",getpid());
  signal(sig, SIG_IGN);
  signal(sig, STOPHANDLER);

}

void TSTPHANDLER(int sig){

  printf("TSTP signal detected\nPID: %d\n",getpid());
  signal(sig, SIG_IGN);
  signal(sig, TSTPHANDLER);

}

void ABRTHANDLER(int sig){

  printf("Abort signal detected\nPID: %d\n",getpid());
  signal(sig, SIG_IGN);
  signal(sig, ABRTHANDLER);

}

void KILLHANDLER(int sig){

  printf("Kill signal detected\nPID: %d\n",getpid());
  signal(sig, SIG_IGN);
  signal(sig, KILLHANDLER);

}

void TERMHANDLER(int sig){

  printf("Termination signal detected\nPID: %d\n",getpid());
  signal(sig, SIG_IGN);
  signal(sig, TERMHANDLER);

}

void USR2HANDLER(int sig){

  printf("Custom User signal detected\nPID: %d\n",getpid());
  signal(sig, SIG_IGN);
  exit(3);
}


/*
           +hhhho.                      `/yhhhy.
          sd////ohy-                  `ods+//:sm`
         .N++.`-/+ody.              `+ds+:.``+:do
         +m/:````-++sm+            -hh+/.````/:sd
         yho......./oody://++++//:/mso:......-++M`
         dyo...--.../o+yssssossssyyso:..---...o/M-
         myo..--::://o++//////++++++o+//::--..o/N-
         dyo..-/++++/////////////////+++oo/-..s+M-
         hhs-/++////////:::::///////////+++o/:ssM`
         om+++////:-----:::::::-.....:////+++ooym
        -hh+/////-`     `.:::-`       .////++++ym/
       +ds+/////: `:://`  `-`   -://.  :////++++sds`                       ``...``
      sdo////:::- :mosN/       `dssNy  .---::/++oohh.                 .:+ossooooosso/.`-.
     /Ns+/:.````   :o+:`        -oo:`        `.:+oymy              `:ss+-.`       `-/shdyo`
     :md+.                                      :ohNo             :ys-                .:`sh`
     `Ns/`             .-::::.                  .osN:            +h-         ```````     `ds:
     `ddy:`        `` -mmysddh    `            `:ydm-           oh.        `..........``  /sm:
      `-hh/`       --`.hmmdmd+`` `/           .+hm:.           :m.       `..--::::::---..```ho
        `+ds-`     `/:-:+syo:-.`:/.        `./sydho:..```     `d+        `.-:////////::--.``m+
          .sho.      .oo+os++s+-.        `-/ossooosyhhhhyyyssssN`       `.-:/++oosyso//:--./Ny`
            -ym+.     :o:-::/s`       `.:+++o++ooooooooooosssssh         .-:/+oshsooo+//:--:om`
             `No/-     :/+++/.      `-/+++++++++++oooooooooooooh.        `.-/+ohoo+++++/::-+m-
              hy::-                .////+++++++++++ooooooooooooss`        `.-:oho++++++/:/yh.
              sh:::`              -//////++++++++++++oooooooooooss.         `.-yo+++o//ohy:
              od:::.              :////////++++++++++++ooooooooooos+`         `.os++ohN+.
              od:::.              ./////////+++++++++++++++oooooooooso:+/.      -+ho++ds
              yh:::`               ./////////+++++++++++++++++++++++++oo+ss+/:--/h++++:m/
              dy::`                 .//////////+++++++++++++++++++++++++++++++++++++//:oh
              mo::                   -///////////+++++++++++++++++++++++++++++++++////:+d
              ds::.                  .//////////////+++++++++++++++++++++++++++///////:oh
              sd:::`                 .::////////////////++++++y+++++++++++++//////////-sy
              .N+:::`                -::::////////////////:--:so++++++////////////////.ys
               sd/::-                -:::::///+///////:-..:++//y//////////////////////`ho
               `mo:::`               .:::::::/o///:-.``-o+/::::++:///////////////////- ys
                /m/::`            `   ::::::::s..`` `:shm:::::::o/.:///////////////:-  +d
                 hy::.  ``        :   -:::::::s  `-oyo-`ds:::::::+/``-::////////::-`   .N-
                 -N+:.  `.:-.     /`  `::::::/y:oso:`   .ds:::::::/+:.```.......``      /d:
                  dh:.    `-:os+/://   ::::::/No-`       .sh+:::::::://:-`               -d/
                  +N/.     `.+N://hy   .:::::+N`           -sho:--------:/o+-`            :N`
                  `Ny.      `:N`  /m    :::::sm              .oho:--------dyoso/.`        `N:
                   ym`       :N`  -N`   .::::hy                .yh-------:N.  ./sy/        d/
                   :N.       :N   .N.    -::/m+                 yy----...sh      -N`       d/
                   `m/       /m   `N-     .-:N.                +m:......:m-      -N`       m/
                    h+       oh   `N-       +d                :m:.......hs       /m       `N-
                    yo       ys   .N.       do               /d/.......od`       ys       -N`
                    ho       d/   -N`      -N.              +d:......./m-       .N-       +d
                    d+      .N.   /d       sy             `sd-.......+d-        yy        d+
                   `N-      /m    ys      `N:             .hyyyyyyyssy.        :m.       :m`
                   /m`      ys   `m:      oh                                  .m:       `d+
                   odssoossyh-   -N.    `-m:                                  smoo+++ooshy
                      `.``       `+syyyys+:                                    `..---..`


*/

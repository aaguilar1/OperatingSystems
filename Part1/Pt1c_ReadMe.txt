Alexander Riveron’s notes on part 1)c
1)c)i)

With 1)b) being completed, the next task was to send default signals to the process and observe the command line after the signals are sent with the default handlers. 
I found in this case that the process is far too quick for me to properly send a signal, so I added 
while(1){
    	printf("%d\n", i);
    	i++;
    	sleep(1);
    }
to visually show the process running prior to sending the signal. The following are the results

Sending a Ctrl-C signal terminates the process with no noticeable delay and no output
Sending a SIGQUIT signal terminates the process with no noticeable delay with output “Quit (core dumped)”
Sending a SIGSTOP signal terminates the process with no noticeable delay and with output “[1]+  Stopped                 ./P1ci” with ./P1ci being the name of the gcc compiled code
Sending a SIGTSTP signal terminates the process with no noticeable delay with output: “[2]+  Stopped                 ./P1ci”
Sending a SIGABRT signal terminates the process with no noticeable delay with output “Terminated”
Sending a SIGTERM signal terminates the process with no noticeable delay with output “Aborted (core dumped)”
Sending a SIGKILL signal terminates the process with no noticeable delay with output “Killed”

1)c)ii)

CAN: SIGINT, SIGQUIT, SIGABRT,SIGTERM,SIGTSTP
CANNOT: SIGKILL,SIGSTOP
Those that cannot be blocked simply act as the signal with the default handler (see 1)c)i))
However those that do output two lines mirroring the following:


“*SOME SIGNAL* signal detected
PID: 444”

1)c)iii)

In this part, the custom user signal will be used to terminate the process as a precautionary measure.

This block of code:



                  /*
                  //Signal test
                  printf("testing signals\n");
                  printf("Current PID: %d\n", getpid());

                  int i=0;
                  while(1){
                    printf("%d\n", i);
                    i++;
                    sleep(1);
                  }
                  */
was added to stop each process prior to forking and completing. These sections can be seen in the code and can be tested by the reader of this document by simply removing the comment.

The results of sending signals to each process is reflective of the previous two experiments: 
SIGKILL,SIGSTOP cannot be blocked and terminate the process, and the rest of the signals that the process is meant to ignore are completely ignored. 
Meanwhile, any signal that the process is not listed to ignore shall act as they did in part 1)c)ii) the output shows a statement such as
“*SOME SIGNAL* signal detected
PID: 444”


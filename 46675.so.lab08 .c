# SO IS1 LAB08

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc , char **argv)
{
  
    
    if (argc >= 3 || argc <= 1)
        {
            fprintf (stderr, "nie poprawne argumenty!\n");
            return 1;
        }
    
    
    int size=0;
    while (argv[1][size] != '\0')
    {
        //printf ("%c\n",argv[1][size]);
        if (isdigit(argv[1][size]) == 0)
        {
             fprintf (stderr, "nie liczba naturalna\n");
             return 2;
        }
        size += 1;
    }
    
    int number= atoi(argv[1]);
    if(number >13 || number <= 0)
    {
       fprintf (stderr, "nie w zakresie\n");
       return 3;
    }

       
   
    int status2=0;
    int a=number-1;
    int b=number-2;  
    char liczba1[5];
    char liczba2[5];
    snprintf(liczba1, sizeof(liczba1), "%d",a);
    snprintf(liczba2, sizeof(liczba2), "%d",b);
    if(number<=2)
    {
        return 1;    
    }else if (number >= 3)
    {
        
        int pids[2];
        (pids[0] = fork()); // Creates two children

        if (pids[0] == 0) 
        {
            /* Child 1 code goes here */
            
            execlp(argv[0],argv[0],liczba1,NULL);           
            
        }

        pids[1] = fork();
        if (pids[1] == 0) 
        {
            /* Child 2 code goes here */
           
            execlp(argv[0],argv[0],liczba2,NULL);
        } else 
        {   
            
            
            int parent_pid = getpid();
            for(int c = 0; c <= 1; c++) 
                {
                      int status, child = waitpid(pids[c],&status,0);
                      status2 = status2 + WEXITSTATUS(status);
                      if (c==0)
                      {
                        fprintf(stdout,"%d  %d \t %d  %d \n" ,parent_pid, pids[c], a ,WEXITSTATUS(status));
                       }
                      else
                      {
                        fprintf(stdout,"%d  %d \t %d  %d \n" ,parent_pid, pids[c], b ,WEXITSTATUS(status));
                      }
                }
            fprintf(stdout,"%d \t \t    %d\n" ,parent_pid,status2);    
        }
        
    }




     
    fprintf(stdout,"\n \n");
    //fprintf(stdout,"Return: %d \n",status2);
    return status2;

}

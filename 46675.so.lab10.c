# SO IS1 LAB010

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

// mutex
pthread_mutex_t mutex;
double pi=1;
double iloraz(long int beg,long int n) 
{
    double wynik = 1;
    for (long int i = beg; i <= n; i++)
    {
        wynik *= ((2*i) / ((2.0 * i) - 1.0)) * ((2*i)/ ((2.0 * i) + 1.0));
    }
    
    return wynik;
}

static void *
thread_start(void *arg)
{
	pthread_t self = pthread_self();
	long int *range = (long int*) arg;
   	printf("TID: %ld, size: %ld first: %ld\n", self, range[1]-range[0]+1,range[0]);
	double thread_result=iloraz(range[0],range[1]);
	printf("TID: %ld \t thread_result: %.18f\n", self, thread_result);
	pthread_mutex_lock(&mutex);
	pi*=thread_result;
	pthread_mutex_unlock(&mutex);	   
	return (void*) range;
}

int main(int argc , char **argv)
{
 
	pthread_mutex_init(&mutex, NULL);
	   
    if (argc >= 4)
        {
            fprintf (stderr, "nie poprawna dana!\n");
            return 1;
        }
    
    printf("PID: %d\n",getpid());
    int size=0;
	
    while (argv[1][size] != '\0')
    {

        if (isdigit(argv[1][size]) == 0)
        {
             fprintf (stderr, "nie liczba\n");
             return 2;
        }
        size += 1;
    }
    
    long int number= atoi(argv[1]);
    if(number >1000000000 || number <= 1)
    {
       fprintf (stderr, "nie w zakresie\n");
       return 3;
    }

    long int threads= atoi(argv[2]);
    if(threads >= 100 || threads <= 1)
    {
       fprintf (stderr, "nie w zakresie\n");
       return 3;
    }
       
    fprintf (stderr, "liczba: %ld\n",threads);
    fprintf (stderr, "liczba: %ld\n",number);
	
	// get clock_time 
	struct timespec t1, t2,t3,t4;
	clock_gettime(CLOCK_REALTIME, &t1);
	
	// data Threads 
    pthread_t threads_TID[threads];
	pthread_attr_t *attr;
	
	long int data[threads][2];
    long int thread_size = number / threads;
    
	
	int w=1;
   	for(;w < threads ;w++)
   	{
		data[w-1][0] = 1+(w-1)*thread_size;
        data[w-1][1] = thread_size*w;	
	    pthread_create(&threads_TID[w-1], NULL,thread_start,data[w-1]);
    }
	
   if (number % threads != 0 )
    {
        data[w-1][0] = 1+(w-1)*thread_size;
		data[w-1][1] = thread_size*w+(number % threads);
		pthread_create(&threads_TID[w-1], NULL,thread_start,data[w-1]);
    }
	else
	{
		data[w-1][0] = 1+(w-1)*size;
		data[w-1][1] = size*w;	
	    pthread_create(&threads_TID[w-1], NULL,thread_start,data[w-1]);
	}
    
	for(int i=0; i<threads; i++)
	{
		long retval;
		pthread_join(threads_TID[i], (void*)&retval);
		printf("thread %d joined: %ld\n", i+1, retval);
	}
	printf("prod: %f\n", pi*2  	);
	clock_gettime(CLOCK_REALTIME, &t2);
    
	clock_gettime(CLOCK_REALTIME, &t3);
	double opi= 2*iloraz(1,number);
	clock_gettime(CLOCK_REALTIME, &t4);
	
	double time_threads = (t2.tv_sec - t1.tv_sec) * 1e9;
	time_threads = (time_threads + (t2.tv_nsec - t1.tv_nsec)) * 1e-9;
	printf("w/threads: PI=%.18f time=%.5fs\n", pi * 2, time_threads);
	
	double time_threads2 = (t4.tv_sec - t3.tv_sec) * 1e9;
	time_threads2 = (time_threads2 + (t4.tv_nsec - t3.tv_nsec)) * 1e-9;
	printf("w/threads: PI=%.18f time=%.5fs\n", pi * 2, time_threads2);
	
	
}



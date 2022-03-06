#define _XOPEN_SOURCE 600 // to make barriers work
#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h>
 
#define NUM_THREADS 2 // only two threads needed
  
//functions used for threads 
void *hello_world(){
	sleep((rand() % 2) + 1);// random time from 0 to 2
	printf("hello world\n");
	pthread_exit(NULL);
}
void *goodbye(){
	sleep((rand() % 2) + 1);// random time from 0 to 2
	printf("goodbye\n");
	pthread_exit(NULL);
} 

int main(void){
	pthread_t thr[NUM_THREADS];	// 2 threads
	int rc;
	
	for(int c = 0; c < 6;c++){ // do 6 runs for observing the change in thread order
		//creating threads 1 and 2
		//thread 1
		if ((rc = pthread_create(&thr[0], NULL, hello_world, NULL))) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return 0;
		}
		//thread 2
		if ((rc = pthread_create(&thr[1], NULL, goodbye, NULL))) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return 0;
		}
		//waits for both threads to complete
		for (int i = 0; i < NUM_THREADS; ++i) {
			pthread_join(thr[i], NULL);
		}
	}
	return 0;
}
#define _XOPEN_SOURCE 600 // to make barriers work
#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h>
 
#define NUM_THREADS 5 // only 5 threads needed

typedef struct grades{ // grade array data structure
	//int indx;
	float g;
}grades;

int counter = 1; // used for teling user which grade is which

void *bellcurve(void *arg){//bell curve function used by threads 
	printf("%d Grade ",counter);
	counter++;
	float R = *(float*)arg;//turning a pointer to float 
	printf("after bellcurved: %.3f\n",R*1.5);
	pthread_exit(NULL);
}

int main(void){
	pthread_t thr[NUM_THREADS]; // 5 threads
	int i,c,d,rc; // used for input looping and thread creation looping
	grades data[NUM_THREADS]; // holds float type grades, up to 5 
	for(i = 0;i<NUM_THREADS;i++){ // ask user for 5 grades 
		printf("Input %d grade and then press enter key: \n",i+1);
		int in = scanf("%f",&data[i].g);
		//data[i].indx = i;
		if (in != 1){ printf("Error processing INPUT\n"); return 0; }
	}
		
	//creating 5 threads for 5 grades  
	for(c = 0; c < NUM_THREADS;c++){
		sleep(2);// allows for prevoius thread to complete bf next thread starts
		if ((rc = pthread_create(&thr[c], NULL, bellcurve, &data[c].g))) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return 0;
		}
	}
	//waits for all threads to complete
	for (d = 0; d < NUM_THREADS; ++d) {
		pthread_join(thr[d], NULL);
	}
	
	return 0;	
}
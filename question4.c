#define _XOPEN_SOURCE 600 // to make barriers work
#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h>
#include <stdbool.h>
 
#define NUM_THREADS 10 // only 10 threads needed
#define WORD_LENGTH 300 // name length max

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // used for Mutex 

typedef struct Grade{ // grade array data structure
	float grade;
}Grade;

float total_grade;

void *class_total(void *arg){
	pthread_mutex_lock(&mutex);// Mutex acquire
	float R = *(float*)arg;//turning a pointer to float
	total_grade += R; // Total Grade cal
	pthread_mutex_unlock(&mutex);// Mutex release 
	pthread_exit(NULL);
}

int main(){
	pthread_t thr[NUM_THREADS]; // 10 threads
	Grade data[NUM_THREADS]; // holds float type grades
	int i,c,d,rc,in; // used for input looping and thread creation looping 
	for(i = 0;i<NUM_THREADS;i++){ // ask user for 10 grades		
		printf("Input %d grade and then press enter key: \n",i+1);
		in = scanf("%f",&data[i].grade);
		if (in != 1){ printf("Error processing INPUT\n"); return 0; }
		printf("\n");
	}
	
	//creating 10 threads for 10 student inputs  
	for(c = 0; c < NUM_THREADS;c++){
		if ((rc = pthread_create(&thr[c], NULL, class_total, &data[c]))) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return 0;
		}
	}
	//waits for all threads to complete
	for (d = 0; d < NUM_THREADS; ++d) {
		pthread_join(thr[d], NULL);
	}
	printf("Total Sum of all 10 inputed grades: %.3f\n",total_grade);
	return 0;	
}
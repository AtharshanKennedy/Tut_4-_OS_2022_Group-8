#define _XOPEN_SOURCE 600 // to make barriers work
#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
 
#define NUM_THREADS 10 // only 10 threads needed
#define WORD_LENGTH 300 // name length max

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // used for Mutex 
pthread_barrier_t barrier; // use for 10 lines coming from txt file

typedef struct Grade{ // grade array data structure
	float grade;
}Grade;
Grade data[NUM_THREADS]; // holds float type grades

int C = -1; // start at negative one to avoid miss print of first number
void *read_grades(void *arg){
	float Tt = *(float*)arg; // convert pointer to float value
	data[C].grade = Tt;// adding grade to array`
	pthread_barrier_wait(&barrier); // wait for 10 lines to be read
	pthread_exit(NULL);
}

float total_grade; // used for bf bellcurve
float total_bellcurve; // used for af bellcurve
void *save_bellcurve(void *arg){ // used for new info creation and saving new info 
	Grade *data = (Grade*)arg; // convert pointer to a array that follows the struct Grade
	pthread_mutex_lock(&mutex);// Mutex acquire
	total_grade += data->grade; // Total Grade cal
	total_bellcurve += data->grade*1.5; // Total bellcurve Grade cal
	pthread_mutex_unlock(&mutex);// Mutex release 
	pthread_exit(NULL);
}

int main(){
	pthread_t thr[NUM_THREADS]; // 10 threads
	
	int c,d,rc; // used for input looping and thread creation looping 
	
	//creating 10 threads for reading in file inputs 
	pthread_barrier_init(&barrier,NULL,10); // create barrier
	printf("Processing .txt will take 10 sec........\n"); 
	c = 0; // used for both threads
	char path[WORD_LENGTH]; // store cwd/grades.txt
	getcwd(path,sizeof(path)); // get the cwd
	strcat(path,"/grades.txt");// attach grades.txt to cwd
	FILE *pToFile = fopen(path,"r");// open grades.txt
	char temp[WORD_LENGTH]; // temp for extracted line
	float Temp; // converted string to float gets stored here for tmep purpose 
	while( fgets(temp,WORD_LENGTH,pToFile)){ // extract each line from file
		sleep(1); // sleep between each line to not cause erros with line extraction
		C++; // index through data(array that uses the struct Grade)
		Temp = atof(temp);// string to float
		c++; 
		if ((rc = pthread_create(&thr[c], NULL, read_grades, &Temp))) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return 0;
		}
		memset(temp,'\0',sizeof(temp));// make sure temp has no garbage left behind for the next itr
	}
	fclose(pToFile);// closing grades.txt
	pthread_barrier_destroy(&barrier);// destory used barrier
	printf("Processing Complete\n");
	
	//creating 10 threads for processing 
	char path1[WORD_LENGTH]; // store cwd/bellcurve.txt
	getcwd(path1,sizeof(path1)); // get the cwd
	strcat(path1,"/bellcurve.txt");// attach bellcurve.txt to cwd
	FILE *pToFile1 = fopen(path1,"wb");// open bellcurve.txt	
	for(c = 0; c < NUM_THREADS;c++){
		if ((rc = pthread_create(&thr[c], NULL, save_bellcurve, &data[c]))) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return 0;
		}
		fprintf(pToFile1,"%.3f\n",data[c].grade*1.5);//output to new bellcurve.txt file
	}
	fclose(pToFile1);// closing bellcurve.txt
	
	//waits for all threads to complete
	for (d = 0; d < NUM_THREADS; ++d) {
		pthread_join(thr[d], NULL);
	}
	
	//Print out final statments
	char Pre = '%' ;
	printf("\nTotal Grade before Bellcurve: %c%.3f\n",Pre,total_grade);
	printf("\nClass Avg before Bellcurve: %c%.3f\n",Pre,total_grade/NUM_THREADS);
	printf("\nTotal Grade after Bellcurve: %c%.3f\n",Pre,total_bellcurve);
	printf("\nClass Avg after Bellcurve: %c%.3f\n",Pre,total_bellcurve/NUM_THREADS);
	return 0;	
}
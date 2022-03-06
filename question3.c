#define _XOPEN_SOURCE 600 // to make barriers work
#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h>
#include <string.h>
 
#define NUM_THREADS 5 // only 5 threads needed
#define WORD_LENGTH 300 // name length max

typedef struct student{ // student array data structure
	char name[WORD_LENGTH];
	int student_id;
	float grade;
}student;

void *bellcurve(void *arg){//bell curve function used by threads  
	student *data = (student*)arg;
	printf("Name: %s | ID: %d | Grade Before bellcurved: %.3f | Grade after bellcurved: %.3f\n", data->name, data->student_id, data->grade, data->grade*1.5);
	pthread_exit(NULL);
}

int main(){
	pthread_t thr[NUM_THREADS]; // 5 threads
	student data[NUM_THREADS]; // holds float type grades, int type IDs, char type names up to 5
	int i,c,d,rc,in; // used for input looping and thread creation looping 
	for(i = 0;i<NUM_THREADS;i++){ // ask user for 5 names,ID,grades
		printf("Input %d name and then press enter key(FirstName_LastName): \n",i+1);
		char NameIn[WORD_LENGTH];// using a buffer to help with storing names  
		in = scanf("%s",NameIn);
		if (in != 1){ printf("Error processing INPUT\n"); return 0; }
		strcpy(data[i].name,NameIn);
		
		printf("Input %d ID and then press enter key: \n",i+1);
		in = scanf("%d",&data[i].student_id);
		if (in != 1){ printf("Error processing INPUT\n"); return 0; }
		
		printf("Input %d grade and then press enter key: \n",i+1);
		in = scanf("%f",&data[i].grade);
		if (in != 1){ printf("Error processing INPUT\n"); return 0; }
		
		printf("\n");
		
		memset(NameIn,'\0',sizeof(NameIn));
	}
	
	//clear screen to show case results
	system("clear");
	printf("Changes Applied To:\n");
	
	//creating 5 threads for 5 student inputs  
	for(c = 0; c < NUM_THREADS;c++){
		sleep(2);// allows for prevoius thread to complete bf next thread starts
		if ((rc = pthread_create(&thr[c], NULL, bellcurve, &data[c]))) {
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
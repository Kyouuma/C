//useful notions 
//A semaphore is a data structure used to help threads work together without interfering with each other. Thus Semaphore is a signaling mechanism
//allows multiple program threads to share the same resource thu protecting shared resources

#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h> // added this to cast the int

#include <pthread.h> //pthread_create, pthread_t, pthread_mutex_t, pthread_mutex_lock, pthread_mutex_unlock, pthread_exit
#include <semaphore.h> //sem_t, sem_wait. sem_post

//using namespace std;// using this to avoid name collisions between two libraries using the same function name

pthread_mutex_t count_mutex; //Name of the mutex for the tutor
sem_t chair_sem; //Name of the semaphore for the chairs
int visit; //Number of visits before a thread is done, needs to be declared here so it's consistent in threads
int students_number;
void *TutorVisit(void* threadid); //Simulates the visits to the tutor

int main(int argc, char *argv[]){
	
	
	if(argc != 4){ //Needs 4 arguments ./name_of_program,   max_number_of_students,    number_of_chairs,   max_number_of_meetings_granted_per_student
		printf ("Incorrect number of arguments \n");
		return 1; //Error
	}
	
	int numbers[3];
	for(int i = 1; i < argc; i++){ //All arguments past the name of the program must be ints
		
			numbers[i-1] = atoi(argv[i]); //Converts args(if strings presented) to numbers
		}
		
	
	students_number = numbers[0];
	visit = numbers[2]; //Sets the number of chairs globally so it could be consistent in threads
	printf("--------------------------------------------------------------------------------\n");
	printf(" This program helps organizing schedule by managing meeting time\n");
	printf(" Author: Oussama Hafsi\n");
	printf(" Students Number: %d\n",students_number);
	printf(" Maximu Meetings per student: %d\n",visit);
	printf(" Number Of Available Chairs : %d\n",numbers[1]);
	printf("--------------------------------------------------------------------------------\n");
	
	
	pthread_t threads[numbers[0]]; //Creates an array of possible threads
	sem_init(&chair_sem, 0, numbers[1]); //Creates a semaphore to represent the chairs. Returns 0 if it succeeds and -1 if anything goes wrong.
        for (int i = 1; i <= numbers[0]; i++){ //Creates threads with names for the list
		pthread_create(&threads[i], NULL, TutorVisit,(void *)(intptr_t)i); //Creates a thread per student (NULL for attributs, takes default)
	}
	
}

void *TutorVisit(void *threadid){
	int count = 0; //Initializes total number of visits to the tutor at 0
        long tid = (long)threadid; //Returns the thread id as a usable number
	printf("Hello Professor, Student (%d) reporting.\n",tid); //Thread reports its creation
	do{
		printf("Student (%d) is in the waiting room.\n",tid);
		sem_wait(&chair_sem); //lock the semaphore
		printf("Student is (%d) in a chair.\n",tid);
		pthread_mutex_lock(&count_mutex);  // Wait for a lock on the mutex. Rq: Critical Zone (here the professor is busy with  a student)
			sem_post(&chair_sem); //unlock the semaphore
			count = count + 1; // increment the number of visits for this student
		pthread_mutex_unlock(&count_mutex); // unlock the mutex
		printf("The tutor is free.\n");
	} while(count < visit); //Repeats until the count reaches the appropriate number
	printf("Student (%d) has visited the tutor the requisite number of times\n",tid);
	pthread_exit(NULL); // terminate thread calling 
}

/* Include the header file we made */
#include "p3180139-p3180208-pizza1.h"

/* Variable Declaration */
int Ncust; // Νumber of customers to get serviced
unsigned int seed; // The seed for random numbers
int A_cooks; // Available Cooks
int A_ovens; // Available Ovens
double total_order_time; // Sum of all time it takes to complete the orders
double maximum_order_time; // Maximum amount of time it took for an order to complete
int rc; // A temporary variable we're going to use to check if the thread creation worked succesfully
int random_Torder; // The amount of time it'll take for the next order to get in (randomly generated)

/* Mutexes for Variables Initialization */
pthread_mutex_t mutex_cooks = PTHREAD_MUTEX_INITIALIZER;  // Mutex for cooks
pthread_mutex_t mutex_ovens = PTHREAD_MUTEX_INITIALIZER;  // Mutex for ovens
pthread_mutex_t mutex_total_order_time = PTHREAD_MUTEX_INITIALIZER; // Mutex for screen
pthread_mutex_t mutex_maximum_order_time = PTHREAD_MUTEX_INITIALIZER; // Mutex for screen
pthread_mutex_t screen = PTHREAD_MUTEX_INITIALIZER; // Mutex for screen

/* Conditions Initialization */
pthread_cond_t cond_no_available_cooks = PTHREAD_COND_INITIALIZER; //Mutex for condition of cooks
pthread_cond_t cond_no_available_ovens = PTHREAD_COND_INITIALIZER; //Mutex for condition of ovens

//Our main function
int main(int argc,char** argv){
	if (argc != 3){ // Checking if the input data is valid
		printf("Error!\nProgram needs exactly two arguments, one for Ncust and one for seed.\nExiting. . .\n");
		exit(-1);
	}
	Ncust = atoi(argv[1]); // Converting the first command line argument to int
   	if (Ncust<= 0) { // Checking if the number of customers is valid
		printf("Error!\nNcust represents real people, so it must be a positive integer number.\nExiting. . .\n");
		exit(-1);
	}
	seed = atoi(argv[2]); // Converting the second command line argument to int
    total_order_time = 0;
    maximum_order_time = -1;
	printf("The pizzeria is officially open!\n");
    A_cooks= Ncook; // When opening the shop all cooks are available
    A_ovens= Noven; // When opening the shop all ovens are available
	//Initialization of p_thread array
	pthread_t threads[Ncust];
	//Initialization of customer array. We're giving each customer a unique id
    int id[Ncust];
	//Initialization of thread array, we need as many threads as orders
	pthread_t tid[Ncust];
    for (int i=0; i<Ncust; i++){
		id[i]=i+1;
		if (id[i]==1){ //if it's the first order ever, the time it gets on the system is t=0
				random_Torder=0;
		}
		else {
			int temp_seed=seed-id[i];
			random_Torder=(rand_r(&temp_seed))%(Torderhigh+1-Torderlow)+Torderlow;
			}
		sleep(random_Torder); // Wait for the order to get on the system
		// Meanwhile, the thread array is filling
		printf("Main: creating #%d thread.\n", id[i]);
		rc=pthread_create(&threads[i],NULL,Order,&id[i]);
		if (rc!=0){ // On success, pthread_create() returns 0; on error, it returns an error number, and the contents of threads[] are undefined
			printf ("Error code from pthread_create () is %d.\n", rc);
			exit(-1);
		}
	}
	//Waiting for the threads to join
    for (int i=0;i<Ncust;i++){
		rc = pthread_join(threads[i],NULL);
        printf("Thread #%d is done.\n",id[i]);
		if (rc!=0){ // On success, pthread_join() returns 0; on error, it returns an error number
			printf ("Error code from pthread_join () is %d\n.", rc);
			exit(-1);
		}
    }
	//Let's print the final results
	printf("Ο μέσος χρόνος ολοκλήρωσης παραγγελιών είναι: %.2f λεπτά.\n",total_order_time/Ncust);
	printf("Ο μέγιστος χρόνος που χρειάστηκε για κάποια παραγγελία είναι: %.2f λεπτά.\n",maximum_order_time );
	//Now, it's time to destroy our mutexes
    pthread_mutex_destroy(&mutex_cooks);
    pthread_mutex_destroy(&mutex_ovens);
	pthread_mutex_destroy(&mutex_total_order_time);
	pthread_mutex_destroy(&mutex_maximum_order_time);
	pthread_mutex_destroy(&screen);
    pthread_cond_destroy(&cond_no_available_cooks);
    pthread_cond_destroy(&cond_no_available_ovens);
	pthread_exit(NULL);
	return 0;
}

//The entry routine for the thread
void *Order(void *id){
	struct timespec order_received; //The time when the system has received the pizza
	struct timespec order_finished; //The time when the order has been completed
	int *tid=(int *)id; // A pointer to the id of the order
    int pizzas;// // The number of pizzas a customer ordered (randomly generated)

	/* 1st Phase: The order is getting ready to show */
	clock_gettime(CLOCK_REALTIME,&order_received); // Get the time when the order gets to the system
	unsigned int temp_seed = seed + (unsigned)tid;
	pizzas=(rand_r(&temp_seed))%(Norderhigh+1-Norderlow)+Norderlow;// The amount of pizzas the customer ordered is random

	/* 2nd Phase: Looking for a cook to prepare the pizza/s */
    rc=pthread_mutex_lock(&mutex_cooks); //Lock the mutex of cooks
	check_rc(rc);
    while(A_cooks==0){ //Wait until there's a cook available
        //printf("No cooks were found available");
        rc=pthread_cond_wait(&cond_no_available_cooks,&mutex_cooks);
		check_rc(rc);
    }
    A_cooks--; //A cook will start working on the order
	//printf("A cook is available to prepare order: #%d.\n", *tid);
	rc=pthread_mutex_unlock(&mutex_cooks); //Unlock mutex of cooks
	check_rc(rc);

	/* 3rd Phase: Waiting for the cook to prepare the order */
	sleep(Tprep*pizzas); //Count time it takes the cook to prepare the pizzas

	/* 4th Phase: Looking for an oven to bake the pizza/s */
	rc=pthread_mutex_lock(&mutex_ovens); //Lock the mutex of ovens
	check_rc(rc);
    while(A_ovens==0){ //Wait until there's an oven available
       //printf("No ovens where found");
       rc=pthread_cond_wait(&cond_no_available_ovens,&mutex_ovens);
	   check_rc(rc);
    }
    A_ovens--; //An oven will start baking the pizza/s
	//printf("An oven is availabe to make order: #%d.\n", *tid);
	rc=pthread_mutex_unlock(&mutex_ovens);//Unlock mutex
	check_rc(rc);

	/* 5th Phase: Waiting for the pizza/s to bake */
    sleep(Tbake); // Wait for the pizza/s to bake


	//Release the oven
	rc = pthread_mutex_lock(&mutex_ovens);
	check_rc(rc);
	++A_ovens;
	pthread_cond_signal(&cond_no_available_ovens);
	rc = pthread_mutex_unlock(&mutex_ovens);
	check_rc(rc);

	//Release the cook
	rc = pthread_mutex_lock(&mutex_cooks);
	check_rc(rc);
	++A_cooks;
	pthread_cond_signal(&cond_no_available_cooks);
	rc = pthread_mutex_unlock(&mutex_cooks);
	check_rc(rc);

	//Print output for order
	clock_gettime(CLOCK_REALTIME,&order_finished ); // Get the time the order is completed.
	time_t temp_time =order_finished.tv_sec-order_received.tv_sec;	// The time the order took to complete
	long int time=(long int)temp_time;
	rc = pthread_mutex_lock(&screen);
	check_rc(rc);
	print_successful_order(*tid,pizzas,time);
	rc = pthread_mutex_unlock(&screen);
	check_rc(rc);

	//Update Total Order Time
	rc = pthread_mutex_lock(&mutex_total_order_time);
	check_rc(rc);
	total_order_time += temp_time;
	rc = pthread_mutex_unlock(&mutex_total_order_time);
	check_rc(rc);

	//Update Maximum Order Time
	rc = pthread_mutex_lock(&mutex_maximum_order_time);
	check_rc(rc);
	if (temp_time>maximum_order_time){
		maximum_order_time=temp_time;
	}
	rc = pthread_mutex_unlock(&mutex_maximum_order_time);
	check_rc(rc);

	pthread_exit(NULL);
}

//Checks if a lock/unlock of a mutex returns an error code
void check_rc(int rc){
	if (rc != 0){
		printf("Error code from mutex lock/unlock is %d.\n",rc);
		pthread_exit(&rc);
	}
}

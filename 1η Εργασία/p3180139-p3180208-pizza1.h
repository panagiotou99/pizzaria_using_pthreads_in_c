#pragma once

//Library inclusions
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

//Declaration of constant variables for the pre processor
#define Ncook 6 // the number of Cooks
#define Noven 5 // the number of Ovens
#define Torderlow 1 // the minumum time to get an order
#define Torderhigh 5 // the maximum time to get an order
#define Norderlow 1 // the minumum amount of orders
#define Norderhigh 5 //the maximum amount of orders
#define Tprep 1 // the minumum time to make a single pizza
#define Tbake 10 // the maximum amount to make a single pizza

//Declaration of functions
void *Order(void *id);
void check_rc(int rc);

//For each order prints the customer's id and the amount of time (in minutes) the order took to complete*/
void  print_successful_order(int id,int pizzas,long int seconds){
	if (pizzas==1)
		printf("Η παραγγελία με αριθμό %d ετοίμασε %d πίτσα σε %ld λεπτά.\n",id,pizzas,seconds);
	else 
		printf("Η παραγγελία με αριθμό %d ετοίμασε %d πίτσες σε %ld λεπτά.\n",id,pizzas,seconds);
}
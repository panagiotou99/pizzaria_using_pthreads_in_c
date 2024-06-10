# pizzaria_using_pthreads_in_c
Pizza Ordering and Delivery System using POSIX Threads

Overview
This repository contains the implementation of a pizza ordering and delivery system using POSIX threads (pthreads). The project is divided into two parts, each building upon the previous one to add more functionality and complexity. These projects were developed as part of a mandatory course in Operating Systems at the Athens University of Economics and Business.

Project 1: Basic Pizza Ordering System
In the first part of the project, we implemented a basic system where multiple customers can place orders for pizzas. Each order is processed by a set of cooks and ovens. The primary focus was on synchronizing the cooks and ovens using pthreads to ensure that the orders are prepared and baked correctly.

Project 2: The second part of the project extends the functionality by introducing delivery personnel. Now, orders not only need to be prepared and baked, but also delivered to the customers. The synchronization complexity increases as we manage the availability of cooks, ovens, and delivery personnel.

The program accepts two parameters: the number of customers (Ncust) and the seed for the random number generator. Each customer thread represents an order being processed through the system.

Output
For each order, the system prints:
The order number
Total time taken for delivery
Time spent cooling after baking

At the end of the execution, the system calculates and prints:
The average and maximum delivery times
The average and maximum cooling times

Files
p3x-p3y-p3z-pizza1.h: Header file for Project 1
p3x-p3y-p3z-pizza1.c: Implementation file for Project 1
p3x-p3y-p3z-pizza2.h: Header file for Project 2
p3x-p3y-p3z-pizza2.c: Implementation file for Project 2
p3x-p3y-p3z-pizza2.pdf: Documentation describing the code structure and any additional features
test-res2.sh: Script to compile and run the program with 100 customers and seed 1000

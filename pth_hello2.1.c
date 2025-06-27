/* File:  
 *    pth_hello.c
 *
 * Purpose:
 *    Illustrate basic use of pthreads:  create some threads,
 *    each of which prints a message.
 *
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  gcc -g -Wall -o pth_hello pth_hello.c -lpthread
 * Usage:    ./pth_hello <thread_count>
 *
 * IPP:   Section 4.2 (p. 153 and ff.)
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <unistd.h>
#include <semaphore.h>    // semáforo

/* Global variable:  accessible to all threads */
int thread_count;  

sem_t semaphore;        // semáforo global para limitar 4 threads

void *Hello(void* rank);  /* Thread function */

int main(int argc, char* argv[]) {
   long thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles; 
   
   thread_count = 40; 

   thread_handles = malloc(thread_count * sizeof(pthread_t)); 

   sem_init(&semaphore, 0, 4);    // inicializa semáforo com valor 4

   for (thread = 0; thread < thread_count; thread++)  {
      sem_wait(&semaphore);      // espera vaga antes de criar thread
      pthread_create(&thread_handles[thread], NULL, Hello, (void*) thread);
   }
   
   printf("Hello from the main thread\n");

   for (thread = 0; thread < thread_count; thread++){ 
      pthread_join(thread_handles[thread], NULL); // Bloqueante
      printf("Thread %ld joined!\n", thread);
   }

   sem_destroy(&semaphore);        // destrói semáforo
   free(thread_handles);
   printf("Main thread exit\n");
   return 0;
}  /* main */

void *Hello(void* rank) {
   long my_rank = (long) rank;

   printf("Hello from thread %ld of %d\n", my_rank, thread_count);
   sleep(2);                      // simula trabalho, melhor visualizar limite
   printf("Thread %ld of %d exit!\n", my_rank, thread_count);

   sem_post(&semaphore);          // libera vaga no semáforo
   return NULL;
}  /* Hello */

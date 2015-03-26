/*
 * Wrapper functions for the POSIX semaphores. Rather than return
 * error codes, any failures abort the program with a message to
 * stderr.
 *
 * Sem_init
 * Sem_post
 * Sem_wait
 *
 * Must be linked with -lrt
 *
 * Created by Jerod Weinman, 16 Oct 2012
 * Revised 11 August 2014
 */
#ifndef __MYSEMAPHORE_H__
#define __MYSEMAPHORE_H__

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

void
Sem_init( sem_t *sem, int pshared, unsigned int value)
{
  int rc = sem_init(sem,pshared,value);

  if (rc)
  {
    perror("Error initializing semaphore");
    exit(EXIT_FAILURE);
  }
}


void
Sem_post(sem_t * sem)
{
  int rc = sem_post(sem);

  if (rc)
  {
    perror("Error posting to semaphore");
    exit(EXIT_FAILURE);
  }
}

void
Sem_wait(sem_t *sem)
{
  int rc = sem_wait(sem);

  if (rc)
  {
    perror("Error waiting on semaphore");
    exit(EXIT_FAILURE);
  }
}

#endif // __MYSEMAPHORE_H__

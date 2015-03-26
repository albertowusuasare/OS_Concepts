/*
 * Wrapper functions for the pthread libraries. Rather than return error codes,
 * any failures abort the program with a message to stderr.
 * 
 * Pthread_mutex_init
 * Pthread_mutex_lock
 * Pthread_mutex_unlock
 * Pthread_create
 * Pthread_join
 * Pthread_attr_init
 * Pthread_attr_destroy
 * Pthread_atr_setdetachstate
 * Pthread_cond_init
 * Pthread_cond_signal
 * Pthread_cond_wait
 *
 * Must be linked with -lpthread
 *
 * Mutex and thread routines adapted from similar routines by 
 * Remzi Arpaci-Dusseau, and the condition routines are inspired by the same.
 *
 * Created by Jerod Weinman, 8 Oct 2012
 * Revised 11 August 2014
 */
#ifndef __MYTHREADS_H__
#define __MYTHREADS_H__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void 
Pthread_mutex_init( pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
  int rc = pthread_mutex_init(mutex,attr);

  if (rc) {
      fprintf(stderr,"Error initializing mutex: %s\n", strerror(rc));
      exit(EXIT_FAILURE);
    }
}

void
Pthread_mutex_lock(pthread_mutex_t *m)
{
  int rc = pthread_mutex_lock(m);

  if (rc) {
    fprintf(stderr,"Error locking mutex: %s\n", strerror(rc));
    exit(EXIT_FAILURE);
  }
}
                                                                                
void
Pthread_mutex_unlock(pthread_mutex_t *m)
{
  int rc = pthread_mutex_unlock(m);

  if (rc) {
    fprintf(stderr,"Error unlocking mutex: %s\n", strerror(rc));
    exit(EXIT_FAILURE);
  }

}
                                                                                
void
Pthread_create(pthread_t *thread, const pthread_attr_t *attr, 	
	       void *(*start_routine)(void*), void *arg)
{
  int rc = pthread_create(thread, attr, start_routine, arg);

  if (rc) {
    fprintf(stderr,"Error creating thread: %s\n", strerror(rc));
    exit(EXIT_FAILURE);
  }
}

void
Pthread_join(pthread_t thread, void **value_ptr)
{
  int rc = pthread_join(thread, value_ptr);

  if (rc) {
    fprintf(stderr,"Error joining thread: %s\n", strerror(rc));
    exit(EXIT_FAILURE);
  }

}

void
Pthread_attr_init(pthread_attr_t *attr)
{
  int rc = pthread_attr_init(attr);

  if (rc) {
    fprintf(stderr,"Error initializing attribute: %s\n",strerror(rc));
    exit(EXIT_FAILURE);
  }
}

void
Pthread_attr_destroy(pthread_attr_t *attr)
{
  int rc = pthread_attr_destroy(attr);

  if (rc) {
    fprintf(stderr,"Error destroying attribute: %s\n",strerror(rc));
    exit(EXIT_FAILURE);
  }
}

void
Pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
  int rc = pthread_attr_setdetachstate(attr, detachstate);

  if (rc) {
    fprintf(stderr,"Error setting detach state: %s\n",strerror(rc));
    exit(EXIT_FAILURE);
  }
}

void
Pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
  int rc = pthread_cond_init(cond,attr);

  if (rc) {
    fprintf(stderr,"Error initializing condition variable: %s\n", strerror(rc));
    exit(EXIT_FAILURE);
  }

}

void
Pthread_cond_signal( pthread_cond_t *cond )
{
  int rc = pthread_cond_signal(cond);

  if (rc) {
    fprintf(stderr,"Error issuing signal: %s\n", strerror(rc));
    exit(EXIT_FAILURE);
  }
}

void
Pthread_cond_wait( pthread_cond_t *cond, pthread_mutex_t *mutex)
{
  int rc = pthread_cond_wait(cond, mutex);

  if (rc) {
    fprintf(stderr,"Error issuing wait: %s\n", strerror(rc));
    exit(EXIT_FAILURE);
  }
}

#endif // __MYTHREADS_H__

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#include <sys/time.h>

pthread_t timeThread;
pthread_mutex_t timelock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t signal = PTHREAD_COND_INITIALIZER;
#define WAIT_TIME_SECONDS 5

void *timing() {
  struct timespec ts;
  struct timeval tp;

  while(1) {
    printf("timing::set timeout deadline\n");
    pthread_mutex_lock(&timelock);

    gettimeofday(&tp, NULL);

    ts.tv_sec = tp.tv_sec;
    ts.tv_nsec = tp.tv_usec * 1000;
    ts.tv_sec += WAIT_TIME_SECONDS;     // set wait deadline

    int rc;
    printf("timing::wait for the signal or timeout!\n");
    rc = pthread_cond_timedwait(&signal, &timelock, &ts);

    if (rc == ETIMEDOUT) {
      printf("timing::timing thread timeout!\n");
    } else {
      printf("timing::timing thread is waked up by signal!\n");
    }
    pthread_mutex_unlock(&timelock);
  }
  return NULL;
}

int main(int argc, char **argv) {
  char cmd[1024];

  // start timing thread
  pthread_create(&timeThread, NULL, timing, NULL);

  while(fscanf(stdin, "%s", cmd) != EOF) {
    if(strcmp(cmd, "signal") == 0) {
      printf("type signal!\n") ;
      pthread_mutex_lock(&timelock);
      pthread_cond_signal(&signal);
      pthread_mutex_unlock(&timelock);  
    }
  }
}

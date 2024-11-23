#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
  int nbHello;
  pthread_mutex_t mutex;
  pthread_cond_t c;
} allocation;

allocation* init_allocation()
{
  allocation* a = malloc(1* sizeof(allocation));
  a->nbHello = 0;
  pthread_mutex_init(&a->mutex, NULL);
  pthread_cond_init(&a->c, NULL);
  return a;
}

void destruction(allocation* a){
  pthread_mutex_destroy(&a->mutex);
  pthread_cond_destroy(&a->c);
  free(a);
}

void incremente_libere(allocation* a){
  pthread_mutex_lock(&a->mutex);
  a->nbHello++;
  if(a->nbHello >= 10){
    pthread_cond_broadcast(&a->c);
  }
  pthread_mutex_unlock(&a->mutex);
}

void attendre10(allocation* a){
  pthread_mutex_lock(&a->mutex);
  while(a->nbHello < 10)
  {
    pthread_cond_wait(&a->c, &a->mutex);
  }
  pthread_mutex_unlock(&a->mutex);
}

void* hello_world(void* args)
{
  allocation* a = (allocation*)args;
  printf("Hello world ! ");
  incremente_libere(a);
  return NULL;
}

void* done(void* args){
  printf("Done ! ");
  return NULL;
}

int main(){
  allocation* a = init_allocation();
  pthread_t* liste = malloc(11 * sizeof(pthread_t));
  for(int i =0; i < 10; i++){
    pthread_create(&liste[i], NULL, hello_world, a);
  }
  attendre10(a);
  pthread_create(&liste[10], NULL, done, NULL);
  for(int i =0; i < 11; i++){
    pthread_join(liste[i], NULL);
  }
  free(liste);
  destruction(a);
  return EXIT_SUCCESS;
}

#include <SDL2/SDL.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "oggstream.h"
#include "stream_common.h"
#include "synchro.h"

#include <pthread.h>

pthread_mutex_t fenetre_mutex;
pthread_mutex_t texture_mutex;
pthread_cond_t fenetre_cond;
pthread_cond_t fenetre_cond_texture;
pthread_cond_t vide;
pthread_cond_t plein;

int nombre_texture;

void init_synchro() 
{
  pthread_mutex_init(&fenetre_mutex, NULL);
  pthread_mutex_init(&texture_mutex, NULL);
  pthread_mutex_init(&affiche_mutex, NULL);

  pthread_cond_init(&fenetre_cond, NULL);
  pthread_cond_init(&fenetre_cond_texture, NULL);
  pthread_cond_init(&vide, NULL);
  pthread_cond_init(&plein, NULL);

  nombre_texture = 0;
}

void clean_synchro() 
{
  pthread_mutex_destroy(&fenetre_mutex);
  pthread_mutex_destroy(&texture_mutex);
  pthread_mutex_destroy(&affiche_mutex);

  pthread_cond_destroy(&fenetre_cond);
  pthread_cond_destroy(&fenetre_cond_texture);
  pthread_cond_destroy(&vide);
  pthread_cond_destroy(&plein);
}

int main(int argc, char *argv[]) 
{
  int res;

  if (argc != 2) 
  {
    fprintf(stderr, "Usage: %s FILE", argv[0]);
    exit(EXIT_FAILURE);
  }
  assert(argc == 2);

  // Initialisation de la SDL
  res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
  atexit(SDL_Quit);
  assert(res == 0);

  // Your code HERE
  // start the two stream readers (theoraStreamReader and vorbisStreamReader)
  // each in a thread
  fprintf(stderr, "Initialisation\n");
  init_synchro();

  char* filename = (char*)argv[1];

  pthread_t theoraStreamReaderThread;
  pthread_t vorbisStreamReaderThread;

  fprintf(stderr, "Creation : ");
  pthread_create(&theoraStreamReaderThread, NULL, theoraStreamReader, (void*)filename);
  pthread_create(&vorbisStreamReaderThread, NULL, vorbisStreamReader, (void*)filename);
  fprintf(stderr, "OK\n");

  pthread_join(vorbisStreamReaderThread, NULL);
  sleep(1);

  pthread_join(theoraStreamReaderThread, NULL);
  pthread_cancel(theoraStreamReaderThread);

  fprintf(stderr, "Fin et liberation\n");
  clean_synchro();
  exit(EXIT_SUCCESS);
}

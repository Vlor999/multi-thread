#include <SDL2/SDL.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "oggstream.h"
#include "stream_common.h"
#include "synchro.h"

#include <pthread.h>

int main(int argc, char *argv[]) {
  int res;

  if (argc != 2) {
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

  char* filename = (char*)argv[1];

  pthread_t theoraStreamReaderThread;
  pthread_t vorbisStreamReaderThread;

  pthread_create(&theoraStreamReaderThread, NULL, theoraStreamReader, (void*)filename);
  pthread_create(&vorbisStreamReaderThread, NULL, vorbisStreamReader, (void*)filename);

  
  // wait for vorbis thread

  pthread_join(vorbisStreamReaderThread, NULL);
  fprintf(stderr, "la merde est la !! \n");
  // 1 seconde of sound in advance, thus wait 1 seconde
  // before leaving
  sleep(1);

  // Wait for theora and theora2sdl threads
  pthread_cancel(theoraStreamReaderThread);
  pthread_cancel(affiche);

  pthread_join(theoraStreamReaderThread, NULL);
  pthread_join(affiche, NULL);

  exit(EXIT_SUCCESS);
}

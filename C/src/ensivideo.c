#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <pthread.h>

#include "stream_common.h"
#include "oggstream.h"
#include "synchro.h"

/* Testé sur une vidéo ogg, ça marche ! */
int main(int argc, char *argv[]) {
    //init synchro variables
    pthread_mutex_init(&mutex_hmap, NULL);
    pthread_mutex_init(&mutex_textures, NULL);
    sem_init(&sem_taille,  0, 0);
    sem_init(&sem_fenetre_texture,  0, 0);
    sem_init(&sem_free_cache,  0, NBTEX);
    sem_init(&sem_cache,  0, 0);

    int res;

    if (argc != 2) {
	fprintf(stderr, "Usage: %s FILE", argv[0]);
	exit(EXIT_FAILURE);
    }
    assert(argc == 2);


    // Initialisation de la SDL
    res = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS);
    atexit(SDL_Quit);
    assert(res == 0);

    // start the two stream readers
    // theoraStreamReader video
    pthread_create(&videoStream_pid, NULL, theoraStreamReader, (void*)argv[1]);
    // vorbisStreamReader audio
    pthread_create(&audioStream_pid, NULL, vorbisStreamReader, (void*)argv[1]);
    // wait audio thread terminaison de vorbis
    pthread_join(audioStream_pid, NULL);
    // 1 seconde de garde pour le son,
    sleep(1);
    // tuer les deux threads videos si ils sont bloqués
    pthread_cancel(videoStream_pid);
    pthread_cancel(sdlStream_pid);
    // attendre les 2 threads videos
    pthread_join(videoStream_pid, NULL);
    pthread_join(sdlStream_pid, NULL);
    // destroy mutexs et semaphores
    sem_destroy(&sem_taille);
    sem_destroy(&sem_fenetre_texture);
    sem_destroy(&sem_free_cache);
    sem_destroy(&sem_cache);
    pthread_mutex_destroy(&mutex_hmap);
    pthread_mutex_destroy(&mutex_textures);
    exit(EXIT_SUCCESS);
}

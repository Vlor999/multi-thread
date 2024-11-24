#include "synchro.h"
#include "ensitheora.h"


bool fini;

/* les variables pour la synchro, ici */


/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    windowsx = buffer[0].width;
	windowsy = buffer[0].height;
    sem_post(&sem_taille);
}

void attendreTailleFenetre() {
    sem_wait(&sem_taille);
}

void signalerFenetreEtTexturePrete() {
    sem_post(&sem_fenetre_texture);
}

void attendreFenetreTexture() {
    sem_wait(&sem_fenetre_texture);
}

// Pas besoin d'exclusion mutuelle.
void debutConsommerTexture() {
    sem_wait(&sem_cache);
}

void finConsommerTexture() {
    sem_post(&sem_free_cache);
}


void debutDeposerTexture() {
    sem_wait(&sem_free_cache); //on se prépare à réduire le cache. Max NBTEXT.
}

void finDeposerTexture() {
    sem_post(&sem_cache);
}

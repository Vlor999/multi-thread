#include "synchro.h"
#include "ensitheora.h"


bool fini;

/* les variables pour la synchro, ici */


/* l'implantation des fonctions de synchro ici */
void envoiTailleFenetre(th_ycbcr_buffer buffer) 
{
    // Lock fenetre
    pthread_mutex_lock(&fenetre_mutex);

    windowsx = buffer[0].width; // modif
    windowsy = buffer[0].height; // same 
    pthread_cond_signal(&fenetre_cond); // envoie signal de changement
    
    // Unlock fenetre
    pthread_mutex_unlock(&fenetre_mutex);
}

void attendreTailleFenetre() 
{
    //Lock Fenetre
    pthread_mutex_lock(&fenetre_mutex);

    while(windowsx == 0 || windowsy == 0) // Si la fenetre n'est pas la 
    {
        pthread_cond_wait(&fenetre_cond, &fenetre_mutex); // sans la while ca focntionne quand même 
    }
    
    //Unlock fenetre
    pthread_mutex_unlock(&fenetre_mutex);    
}

void signalerFenetreEtTexturePrete() 
{
    pthread_mutex_lock(&fenetre_mutex);
    pthread_cond_signal(&fenetre_cond_texture);
    pthread_mutex_unlock(&fenetre_mutex);
}

void attendreFenetreTexture() 
{
    pthread_mutex_lock(&fenetre_mutex);
    pthread_cond_wait(&fenetre_cond_texture, &fenetre_mutex);
    pthread_mutex_unlock(&fenetre_mutex);
}

void debutConsommerTexture() 
{
    pthread_mutex_lock(&texture_mutex);
    while(nombre_texture == 0)
    {
        pthread_cond_wait(&vide, &texture_mutex);
    }
    pthread_mutex_unlock(&texture_mutex);
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

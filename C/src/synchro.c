#include "ensitheora.h"
#include "synchro.h"

/* les variables pour la synchro, ici */

pthread_t affiche;
pthread_mutex_t fenetre_mutex;
pthread_mutex_t texture_mutex;
pthread_cond_t fenetre_cond;
pthread_cond_t fenetre_cond_texture;
pthread_cond_t vide;
pthread_cond_t plein;

int nombre_texture = 0;

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

void finConsommerTexture() 
{
    pthread_mutex_lock(&texture_mutex);
    nombre_texture--; // la consoooommmaiton
    pthread_cond_signal(&plein); // pas plein
    pthread_mutex_unlock(&texture_mutex);
}

void debutDeposerTexture() 
{
    pthread_mutex_lock(&texture_mutex);
    while(nombre_texture == NBTEX)
    {
        pthread_cond_wait(&plein, &texture_mutex);
    }
    pthread_mutex_unlock(&texture_mutex);
}

void finDeposerTexture() 
{
    pthread_mutex_lock(&texture_mutex);
    nombre_texture++;
    pthread_cond_signal(&vide);
    pthread_mutex_unlock(&texture_mutex);
}

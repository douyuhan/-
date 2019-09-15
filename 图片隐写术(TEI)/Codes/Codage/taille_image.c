#include "../tifwrap/tifwrap.h"

#include "message.h"


int calcul_taille_image(bwimage_t *image)
{
        int i,j;
        int taille_image=0;

        // Calcul de la capacit¨¦ de l'image
        for(i=0; i<image->height; i++)
        {
            for(j=0; j<image->width; j++)
            {
                taille_image++;
            }
        }

    return taille_image;
}

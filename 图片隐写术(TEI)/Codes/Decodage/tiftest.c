#include "../tifwrap/tifwrap.h"

int calcul_taille_image(bwimage_t *image);

char *decodage_image(int taille_image, bwimage_t *image);

int detection(char *chainedecode, int taille_nom);

void sauvegarde(char *message);

int main()
{

    bwimage_t *image;
    error_e retval=EEA_OK;
    image=EEACreateImage();

    do
    {

        // Recherche de l'image
        if(EEA_OK != (retval=EEALoadImage("imagecode(LSB).tif", image))) break;


        // Calcul taille de l'image
        int taille_image;
        taille_image = calcul_taille_image(image);


        //*************************decodage****************************
        char *pch,chainedecode[taille_image];
        pch=decodage_image(taille_image, image);

        int g=0;
        for(g=0; *pch!='\0'; g++)
        {
            chainedecode[g]=*pch;
            pch++;
        }

        int taille_nom,taille_data;
        taille_nom=(int)chainedecode[1];
        taille_data=(int)chainedecode[2];


        int count;
        count=detection(chainedecode, taille_nom);
        //printf("count=%d\n",count);


        //Affichier les details du message secret
        if(count!=0)
        {
            printf("\nCRC:\n%x\n\nTaille du nom:\n%d\n\nTaille de donnees:\n%d\n\n",chainedecode[0],chainedecode[1],chainedecode[2]);

            printf("Nom du fichier:\n");
            int i;
            for(i=3;i<3+taille_nom;i++)
            {
                printf("%c",chainedecode[i]);
            }

            printf("\n\nDonnees du fichier:\n");
            for(i=3+taille_nom;i<3+taille_nom+taille_data;i++)
            {
                printf("%c",chainedecode[i]);
            }
            printf("\n\n");


            //demander a l'utiliser le nom du fichier
            char message_data[taille_data];
            for(i=0;i<taille_data;i++)
            {
                message_data[i]=chainedecode[i+3+taille_nom];
            }

            sauvegarde(message_data);

        }





    }
    while(0);

    switch(retval)
    {
    case EEA_OK:
        break;
    case EEA_ENOFILE:
        fprintf(stderr, "Cannot open file\n");
        break;
    case EEA_EBADBPS:
        fprintf(stderr, "Number of bits per sample must be equal to 8\n");
        break;
    case EEA_EBADPHOTOMETRIC:
        fprintf(stderr, "Not a colormap image\n");
        break;
    case EEA_ENOCOLORMAP:
        fprintf(stderr, "Image does not have a colormap\n");
        break;
    case EEA_ENOTGRAY:
        fprintf(stderr, "At least one entry in the colormap is not gray\n");
    case EEA_ENOMEM:
        fprintf(stderr, "Cannot allocate memory\n");
        break;
    default:
        ;/* Can't happen */
    }
    EEAFreeImage(image);

    return 0;
}

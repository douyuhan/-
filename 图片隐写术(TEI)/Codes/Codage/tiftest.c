#include "../tifwrap/tifwrap.h"

#include "message.h"


//int CRC(char *Adresse_tab, int Taille_max);
int CRC(char *ptr, int taille_data);

char *chaine_info(message info);

message init_message(int nb_char_msg, int taille_image, FILE * fichier, char nom [30]);

void codage_image(message message_code, int taille_image, bwimage_t *image);

int calcul_taille_image(bwimage_t *image);

/*void decodage_image(bwimage_t *image);

void detection_message(char *pch);

void sauvegarde(char *message);
*/

int main()
{

    bwimage_t *image;
    error_e retval=EEA_OK;
    image=EEACreateImage();

    do
    {

        // Recherche de l'image
        if(EEA_OK != (retval=EEALoadImage("lena.tif", image))) break;


        // Calcul taille de l'image
        int taille_image;
        taille_image = calcul_taille_image(image);
        printf("Taille de l'image est de %d * %d = %d pixels.\n",image->height,image->width,taille_image);


        //Recherche du fichier secret
        char nom[30];
        printf("quel est le nom du fichier\n");
        scanf("%s",nom);
        FILE *fichier;

        char str[5]=".txt";
        strcat(nom,str);
        fichier=fopen(nom,"r");

        // Calcul de la taille du message
        int nb_char_msg=0;
        while(fgetc(fichier)!=EOF)nb_char_msg++;
        fclose(fichier);
        //printf("la taille: %d\n",nb_char_msg);


        // Création de la structure message
        message message_code = init_message(nb_char_msg, taille_image, fichier, nom);


        // CODAGE DE L'IMAGE
        codage_image( message_code,  taille_image, image);


        // Création de la nouvelle image codée
        if(EEA_OK != (retval=EEADumpImage("imagecode(LSB).tif", image))) break;


/*
        printf("\n\n*************************decodage****************************\n");
        decodage_image(image);*/



        // Après décodage :
        // Test pour la détection de présence d'un message caché
        // char *pch = "123aaabbbzzzzzzzzzzz";
        // detection_message(pch);


/////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Demander quel est le nom du fichier à décoder (image


        // DECODAGE DE L'IMAGE

    /*    int i,j;
        int newdata[image->height][image->width];

        char chainedecode[taille_image/8];

        for(i=0; i<image->height; i++)
        {
            for(j=0; j<image->width; j++)
            {
                newdata[i][j]=0x01&image->data[i][j];
            }
        }

        //decodage sur CRC, taille du nom, taille de donnees
        int m,n;
        for(m=0; m<3; m++)
        {
            chainedecode[m]=0;
            for(n=0; n<8; n++)
            {
                newdata[0][8*m+n]=newdata[0][8*m+n]<<(7-n);
                chainedecode[m]=chainedecode[m]|newdata[0][8*m+n];
            }
        }


        //decodage sur filename et data
        int taille_nom,taille_data;
        taille_nom=(int)chainedecode[1];
        taille_data=(int)chainedecode[2];


        //Detection (en fonction de tqille_data)
        detection_message(chainedecode);
        */

    /*    printf("\nCRC:\n%d\n\nTaille du nom:\n%d\n\nTaille de donnees:\n%d\n\n",chainedecode[0],chainedecode[1],chainedecode[2]);

        // Décodage de la totlite du message
        int nb_cara_par_ligne=(image->width)/8;
        int k,I,M;
        I=(3+taille_nom+taille_data)/nb_cara_par_ligne;
        M=(3+taille_nom+taille_data)%nb_cara_par_ligne;


        for(k=0; k<3+taille_nom+taille_data; k++)
        {
            chainedecode[k]=0;
        }

        for(i=0; i<I; i++)
        {
            for(m=0; m<nb_cara_par_ligne; m++)
            {
                for(n=0; n<8; n++)
                {
                    newdata[i][8*m+n]=newdata[i][8*m+n]<<(7-n);
                    chainedecode[nb_cara_par_ligne*i+m]=chainedecode[nb_cara_par_ligne*i+m]|newdata[i][8*m+n];
                }
            }
        }

        for(m=0; m<M; m++)
        {
            for(n=0; n<8; n++)
            {
                newdata[I][8*m+n]=newdata[I][8*m+n]<<(7-n);
                chainedecode[nb_cara_par_ligne*I+m]=chainedecode[nb_cara_par_ligne*I+m]|newdata[I][8*m+n];
            }
        }


        // Affichage du décodage
        printf("Nom du fichier:\n");
        for(k=3; k<3+taille_nom; k++)
        {
            printf("%c",chainedecode[k]);
        }
        printf("\n\nDonnees du fichier:\n");
        for(k=3+taille_nom; k<3+taille_nom+taille_data; k++)
        {
            printf("%c",chainedecode[k]);

        }
        printf("\n");


        char message_data[taille_data];
        for(i=0;i<taille_data;i++)
        {
            message_data[i]=chainedecode[i+3+taille_nom];
        }

        sauvegarde(message_data);
*/


       /* int crc_recu, crc_compare;
        char crc_data[taille_data];

        for(k=3+taille_nom; k<3+taille_nom+taille_data; k++)
        {

            for(i=0; i<taille_data; i++)
            {
                crc_data[i] = chainedecode[k];
            }
        }

        printf("\n");

        crc_recu = (int)chainedecode[0];

        crc_compare = CRC(crc_data,taille_data);

        printf("\n \n En décodage final :: CRC: %d \n\n crc_compare : %d \n\n",crc_recu, crc_compare);

*/

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

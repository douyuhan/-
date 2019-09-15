#include "../tifwrap/tifwrap.h"
#include <string.h>

int detection(char *chainedecode, int taille_nom)
{
    char *nom_message1="fichier_secret_1.txt";
    char *nom_message2="fichier_secret_2.txt";
    char *nom_message3="fichier_secret_3.txt";
    char nom_message[taille_nom];

    int i;
    for(i=0;i<taille_nom;i++)
    {
        nom_message[i]=chainedecode[i+3];
        //printf("dans subfonction,%c\n",nom_message_decode[i]);
    }
    nom_message[taille_nom]='\0';


    /*printf("\nlength of nom_message_decode=%d\n",strlen(nom_message));
    printf("\nsize of nom_message_decode=%d\n",sizeof(nom_message));
    printf("dans subfonction,nom_message_decode=%s\n",nom_message);
    printf("\n");*/


    int flag[3],count;
    flag[0]=strcmp(nom_message1,nom_message);
    flag[1]=strcmp(nom_message2,nom_message);
    flag[2]=strcmp(nom_message3,nom_message);

    //printf("flag1=%d    flag2=%d    flag3=%d\n",flag[0],flag[1],flag[3]);


    if(flag[0]==0)
        count=1;
    else if(flag[1]==0)
        count=2;
    else if(flag[2]==0)
        count=3;
    else
        count=0;

    switch(count)
    {
        case 0:
            printf("L'image ne contient aucune donnee secrete.\n");
            break;
        case 1:
            printf("L'image contient de donnees secretes.\nLe nom du fichier est:\n");
            for(i=0;i<20;i++)
            {
                printf("%c",nom_message1[i]);
            }
            break;
        case 2:
            printf("L'image contient de donnees secretes.\nLe nom du fichier est:\n");
            for(i=0;i<20;i++)
            {
                printf("%c",nom_message2[i]);
            }
            break;
        case 3:
            printf("L'image contient de donnees secretes.\nLe nom du fichier est:\n");
            for(i=0;i<20;i++)
            {
                printf("%c",nom_message3[i]);
            }
            break;
        default:
            printf("Error!\n");
            break;
    }

    return count;
    /*
    //Affichier les detailes du message secret
    if(count!=0)
    {
        printf("\nCRC:\n%x\n\nTaille du nom:\n%d\n\nTaille de donnees:\n%d\n\n",chainedecode[0],chainedecode[1],chainedecode[2]);

        printf("Nom du fichier:\n");
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

    }
    */
}

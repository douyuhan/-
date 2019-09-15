#include "message.h"

#include "../tifwrap/tifwrap.h"

//int CRC(char *Adresse_tab, int Taille_max);
int CRC(char *ptr, int taille_data);


// Mettre une struct message sous forme de pointeur sur chaine
/*char *chaine_info(message info)
{
    // Création de la chaine
    char infotot[info.size_of_filename+info.size_of_data+3];

    // 1er octet = CRC
    infotot[0]= info.crc;
    // 2eme octet = size_of_filename
    infotot[1]= info.size_of_filename;

    // 3eme octet = size_of_data
    infotot[2]= info.size_of_data;

    // 4-n-eme octet = filename
    int i;
    for(i=3; i<(3+info.size_of_filename); i++)
    {
        infotot[i]=*info.filename++;
    }

    // n-m-eme octet = data
    for(i=3+info.size_of_filename; i<(info.size_of_filename+info.size_of_data+3); i++)
    {
        infotot[i]=*info.data++;
    }

    // Affichage données
    printf("%d %d %d\n",infotot[0],infotot[1],infotot[2]);

    for(i=3; i<(3+info.size_of_filename); i++)
    {
        printf("%c",infotot[i]);
    }

    printf("\n");

    for(i=3+info.size_of_filename; i<(info.size_of_filename+info.size_of_data+3); i++)
    {
        printf("%c",infotot[i]);
    }

    printf("\n");

    char *infos=infotot;

    return infos;
}*/



message init_message(int nb_char_msg, int taille_image, FILE * fichier, char nom [30])
{
    message message_code;

    char data[taille_image];

    int size_struct_msg = sizeof(message_code.crc) + sizeof(message_code.size_of_filename) + sizeof(message_code.size_of_data) + sizeof(message_code.filename) + sizeof(message_code.data);


    // Vérification de la taille du message à envoyer
    if ( nb_char_msg>(taille_image-size_struct_msg) )
    {
        printf("Message secret trop long\n");
    }

    // Récupération de la chaine de caractère du message secret à envoyer
    else
    {
        fichier=fopen(nom,"r");
        int i =0;
        char caractereActuel;

        // Boucle de lecture des caractères un à un
        do
        {
            caractereActuel = fgetc(fichier);
            data[i]=caractereActuel;
            i++;
        }// On continue tant que fgetc n'a pas retourné EOF (fin de fichier)
        while (caractereActuel != EOF);

        // Affichage du message secret
        /*for(i=0; i<nb_char_msg; i++)
        {
            printf("%c", data[i]);
        }*/
    }

    // Mettre sous forme de structure le message secret du fichier
    message_code.crc = CRC(data,nb_char_msg);
    message_code.size_of_filename = strlen(nom);
    message_code.size_of_data = nb_char_msg;
    message_code.filename = nom;
    message_code.data = data;

    return message_code;
}



/*void detection_message(char *pch)
{

    // Récupère la size_of_data de la chaine
    if( pch[2] == '0')
    {
        printf("\n Le fichier ne contient aucune donnée secrète ! \n");

    }
    else
    {
        printf("\n Le fichier contient un fichier dissimulé : \n");

        // 2eme octet = size_of_filename
        int size_of_filename = pch[1];


        // 4-n-eme octet = filename
        int i;
        for(i=3; i<(3+size_of_filename); i++)
        {
            // Affichage du nom du fichier
            printf(" %c ", pch[i]);
        }
        printf("\n");
    }
}*/

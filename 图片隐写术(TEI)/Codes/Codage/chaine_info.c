
#include "message.h"

#include "../tifwrap/tifwrap.h"


char *chaine_info(message info)
{
    char infotot[info.size_of_filename+info.size_of_data+3];

    infotot[0]= info.crc;
    infotot[1]= info.size_of_filename;
    infotot[2]= info.size_of_data;

    int i;
    for(i=3; i<3+info.size_of_filename; i++)
    {
        infotot[i]=*info.filename++;
    }

    for(i=3+info.size_of_filename; i<info.size_of_filename+info.size_of_data+3; i++)
    {
        infotot[i]=*info.data++;
    }


    printf("\nCRC:\n%x\n\nTaille du nom:\n%d\n\nTaille de donnees:\n%d\n",infotot[0],infotot[1],infotot[2]);

    printf("\nNom du fichier:\n");
    for(i=3; i<(3+info.size_of_filename); i++)
    {
        printf("%c",infotot[i]);
    }

    printf("\n\nDonnees du fichier:\n");

    for(i=3+info.size_of_filename; i<(info.size_of_filename+info.size_of_data+3); i++)
    {
        printf("%c",infotot[i]);
    }

    printf("\n");

    char *infos=infotot;

    return infos;
}



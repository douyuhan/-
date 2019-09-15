#include "message.h"

#include "../tifwrap/tifwrap.h"


char *chaine_info(message info);


void codage_image(message message_code, int taille_image, bwimage_t *image)
{


    // Mettre la structure message sous forme de chaine de caractère
    unsigned int i,j;

    int taille_message;

    taille_message = 3 + message_code.size_of_filename + message_code.size_of_data;

    char ch[taille_message],*pch;

    //pch=&ch[0];

    pch=chaine_info(message_code);
    //printf("%s",pch);
    int g=0;
    for(g=0; *pch!='\0'; g++)
    {
        ch[g]=*pch;
        pch++;
    }


    // CODAGE DE L'IMAGE
    int m,n,I,J;
    int temp[8*taille_message];

    for(m=0; m<taille_message; m++)
    {
        for(n=0; n<8; n++)
            temp[8*m+n]=((0x80&(ch[m]<<n))>>7)&0x01;
    }

    I=(taille_message*8)/image->width;
    J=(taille_message*8)%image->width;
    for(i=0; i<I; i++)
    {
        for(j=0; j<image->width; j++)
        {
            image->data[i][j]=0xfe&image->data[i][j];
            image->data[i][j]=image->data[i][j]+temp[(image->width)*i+j];
        }
    }
    for(j=0; j<J; j++)
    {
        image->data[I][j]=0xfe&image->data[i][j];
        image->data[I][j]=image->data[I][j]+temp[(image->width)*I+j];
    }

}












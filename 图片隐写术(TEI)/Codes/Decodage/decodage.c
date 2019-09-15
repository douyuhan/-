#include "../tifwrap/tifwrap.h"

//void sauvegarde(char *message);

char *decodage_image(int taille_image, bwimage_t *image)
{
    int i,j;
    int newdata[image->height][image->width];

    char chainedecode[taille_image/8];

    for(i=0; i<image->height; i++)
    {
      for(j=0; j<image->width; j++)
      {
          newdata[i][j]=0x01&image->data[i][j];
      }
    }

    //decodage sur CRC, taille du nom, taille de donn¨¦es
    int m,n;
    for(m=0;m<3;m++)
    {
        chainedecode[m]=0;
        for(n=0;n<8;n++)
        {
            newdata[0][8*m+n]=newdata[0][8*m+n]<<(7-n);
            chainedecode[m]=chainedecode[m]|newdata[0][8*m+n];
        }
    }
    //printf("dans subfonction,crc=%x,taille_nom=%d,taille_data=%d\n",chainedecode[0],chainedecode[1],chainedecode[2]);

    //printf("\nCRC:\n%x\n\nTaille du nom:\n%d\n\nTaille de donnees:\n%d\n\n",chainedecode[0],chainedecode[1],chainedecode[2]);
    //decodage sur filename et data
    int taille_nom,taille_data;
    taille_nom=(int)chainedecode[1];
    taille_data=(int)chainedecode[2];

    char subchaine[3+taille_nom+taille_data];

    int nb_cara_par_ligne=(image->width)/8;
    int I,M;
    I=(3+taille_nom+taille_data)/nb_cara_par_ligne;
    M=(3+taille_nom+taille_data)%nb_cara_par_ligne;


    /*for(k=0;k<3+taille_nom+taille_data;k++)
    {
        chainedecode[k]=0;
    }*/



   /* //decodage sur l'aure du premier ligne
    for(m=3;m<nb_cara_par_ligne-3;m++)
    {
        for(n=0;n<8;n++)
        {
            newdata[0][8*m+n]=newdata[0][8*m+n]<<(7-n);
            chainedecode[m]=chainedecode[m]|newdata[0][8*m+n];
        }
    }*/

    //decodage sur l'autre(commencer par le deuxieme ligne)
    for(i=0;i<I;i++)
    {
        for(m=0;m<nb_cara_par_ligne;m++)
        {
            subchaine[nb_cara_par_ligne*i+m]=0;
            for(n=0;n<8;n++)
            {
                newdata[i][8*m+n]=newdata[i][8*m+n]<<(7-n);
                subchaine[nb_cara_par_ligne*i+m]=subchaine[nb_cara_par_ligne*i+m]|newdata[i][8*m+n];
            }
        }
    }

    for(m=0;m<M;m++)
    {
        subchaine[nb_cara_par_ligne*I+m]=0;
        for(n=0;n<8;n++)
        {
            newdata[I][8*m+n]=newdata[I][8*m+n]<<(7-n);
            subchaine[nb_cara_par_ligne*I+m]=subchaine[nb_cara_par_ligne*I+m]|newdata[I][8*m+n];
        }
    }

    for(i=3;i<3+taille_nom+taille_data;i++)
    {
        chainedecode[i]=subchaine[i];
    }

    /*printf("dans subfonction,crc=%x,taille_nom=%d,taille_data=%d\n",chainedecode[0],chainedecode[1],chainedecode[2]);
    printf("%s",chainedecode);
    printf("\n");*/

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

    char *p=chainedecode;
    return p;
}

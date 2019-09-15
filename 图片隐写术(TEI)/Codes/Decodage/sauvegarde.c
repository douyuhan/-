#include "../tifwrap/tifwrap.h"

#include "message.h"

void sauvegarde(char *message)
{
     char nom[100];
     char hz[]=".txt";
     printf("Quel nom voulez-vous sauvegardez le fichier?\n");
     gets(nom);
     strcat(nom,hz);
     FILE  *fp ;
     fp=fopen(nom,"w+");//这句话就是程序直接以nom新建文件，不管文件之前是否存在
     fprintf(fp,message);//这句话就可以往文件里写入数据
     fclose(fp);//这句话关闭了程序对文件的控制，这样做避免了以后的无操作改变文件内容
}

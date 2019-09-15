#include "message.h"

#include "../tifwrap/tifwrap.h"


//int CRC(char *Adresse_tab, int Taille_max)
int CRC(char *ptr, int taille_data)
{
    int i;
    int crc=0; // 计算的初始crc值

    while(taille_data--)
    {
        crc ^= *ptr++;  //每次先与需要计算的数据异或,计算完指向下一数据

         //下面这段计算过程与计算一个字节crc一样
        for (i=8; i>0; --i) //数据往左移了8位，需要计算8次
        {
            if (crc & 0x80)  //判断最高位是否为1，最高位为1，不需要异或，往左移一位，然后与0x31异或
                crc = (crc << 1) ^ 0x31;  // 0x31(多项式：x8+x5+x4+1，100110001)，最高位不需要异或，直接去掉
            else
                crc = (crc << 1); //最高位为0时，不需要异或，整体数据往左移一位
        }
    }

    return (crc);
}

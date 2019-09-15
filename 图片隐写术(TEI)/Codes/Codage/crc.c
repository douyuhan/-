#include "message.h"

#include "../tifwrap/tifwrap.h"


//int CRC(char *Adresse_tab, int Taille_max)
int CRC(char *ptr, int taille_data)
{
    int i;
    int crc=0; // ����ĳ�ʼcrcֵ

    while(taille_data--)
    {
        crc ^= *ptr++;  //ÿ��������Ҫ������������,������ָ����һ����

         //������μ�����������һ���ֽ�crcһ��
        for (i=8; i>0; --i) //������������8λ����Ҫ����8��
        {
            if (crc & 0x80)  //�ж����λ�Ƿ�Ϊ1�����λΪ1������Ҫ���������һλ��Ȼ����0x31���
                crc = (crc << 1) ^ 0x31;  // 0x31(����ʽ��x8+x5+x4+1��100110001)�����λ����Ҫ���ֱ��ȥ��
            else
                crc = (crc << 1); //���λΪ0ʱ������Ҫ�����������������һλ
        }
    }

    return (crc);
}

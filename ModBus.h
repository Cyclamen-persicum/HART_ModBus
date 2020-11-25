#pragma once
#ifndef MODBUS__H
#define MODBUS__H

#include <string>

class ModBus
{
    //������תΪ�������ַ���
    void change2str(int);
    //�������ո�����
    double calc(std::string);
    //��׼CRCת��
    void calccrc();
public:
    ModBus(std::string str);
    //�õ�ת���������
    double getRes() { return calc(numTwo); }
    ~ModBus();
    //��modbus����ת��Ϊ�ַ����飬����CRC����
    void change2char(std::string);
    //�ȶ�CRC
    bool CRC(unsigned char*, int);
private:
    unsigned char data[25];
    std::string numTwo;
    int num_;
    std::string str_;
    unsigned short crcStd = 0;
};
#endif // !MODBUS__H



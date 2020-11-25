#pragma once
#ifndef MODBUS__H
#define MODBUS__H

#include <string>

class ModBus
{
    //将数据转为二进制字符串
    void change2str(int);
    //计算最终浮点数
    double calc(std::string);
    //标准CRC转换
    void calccrc();
public:
    ModBus(std::string str);
    //得到转换后的数据
    double getRes() { return calc(numTwo); }
    ~ModBus();
    //将modbus报文转换为字符数组，便于CRC计算
    void change2char(std::string);
    //比对CRC
    bool CRC(unsigned char*, int);
private:
    unsigned char data[25];
    std::string numTwo;
    int num_;
    std::string str_;
    unsigned short crcStd = 0;
};
#endif // !MODBUS__H



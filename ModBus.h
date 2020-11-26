#pragma once
#ifndef MODBUS_H
#define MODBUS_H

#include <string>
#include <unordered_map>
#include <vector>

class ModBus
{
    //将数据转为二进制字符串
    void change2str(int);
    //计算最终浮点数
    void calc();
    //标准CRC转换
    void calccrc();
public:
    ModBus(std::string str);
    ~ModBus();
    //将modbus报文转换为字符数组，便于CRC计算
    void change2char(std::string);
    //比对CRC
    bool CRC(unsigned char*, int);
    //将报文中的数据转为5个int，调用calc
    void data2int();
public:
private:
    bool flagCRC;
    unsigned char data_[25];
    std::string numTwo_;
    int num_;
    std::string str_;
    unsigned short crcStd;
    std::vector<double> res_;
    std::unordered_map<char, unsigned short> stdMap
        = { {'0',0},{'1',1},{'2',2},{'3',3},
            {'4',4},{'5',5},{'6',6},{'7',7},
            {'8',8},{'9',9},{'a',10},{'b',11},
            {'c',12},{'d',13},{'e',14},{'f',15},
            {'A',10},{'B',11},{'C',12},{'D',13},
            {'E',14},{'F',15} };
};
#endif // !MODBUS__H



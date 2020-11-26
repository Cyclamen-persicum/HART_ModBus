#include "modbus.h"
#include <cmath>

void ModBus::change2str(int num_)
{
    for (int i = 31; i >= 0; --i)
        numTwo_ += (((num_ >> i) & 1) + '0');
}

ModBus::~ModBus()
{
    numTwo_.clear();
    str_.clear();
    res_.clear();
    stdMap.clear();
}

void ModBus::change2char(std::string str)
{
    int size = str.size();
    int cur = 0;
    for (int i = 0; i < size; i += 2)
    {
        int temp;
        temp = stdMap[str[i]];
        temp = stdMap[str[i + 1]] + 16 * temp;
        temp += 0x00;
        data_[cur++] = temp;
    }
}

void ModBus::calc()
{
    // p指向当前位置
    auto p = 0;
    // flag真则为正，假则为负
    bool flag = numTwo_[0] == '0' ? 1 : 0;
    p++;
    // power为小数点移动的位数
    std::string power_str = numTwo_.substr(p, 8);
    int power = stoi(power_str, nullptr, 2) - 127;
    p = 9;
    std::string integer_str;
    int integer = 0;
    std::string decimal_str;
    double decimal = 0.0;
    //幂数大于等于0
    //注：等于0情况未经检验
    //TODO:验证幂数=0情况
    if (power >= 0)
    {
        integer_str = "1" + numTwo_.substr(p, power);
        p += power;
        integer = stoi(integer_str, nullptr, 2);
        for (; p != 32; ++p)
        {
            if (numTwo_[p] == '1')
                decimal += pow(2, -(p - 8 - power));
        }
    }
    //幂数小于0
    //这段代码未经检验，谨慎使用
    //TODO:验证幂数<0的情况
    else if (power < 0)
    {
        decimal_str = std::string(-(power - 1), '0') + "1" + numTwo_.substr(9);
        int size = decimal_str.size();
        for (int i = 0; p < size; ++i)
        {
            if (numTwo_[i] == '1')
                decimal += pow(2, -(i + 1));
        }
    }
    res_.emplace_back(flag ? double(integer) + decimal : -double(integer) + decimal);
}

void ModBus::calccrc()
{
    std::string crcStd_str = str_.substr(48, 2) + str_.substr(46, 2);
    for (int i = 0; i < 4; ++i)
    {
        unsigned short temp;
        temp = stdMap[crcStd_str[i]];
        crcStd += pow(16, 3 - i) * temp;
    }
}

ModBus::ModBus(std::string str) : str_(str)
{
    flagCRC = false;
    crcStd = 0;
    calccrc();
    change2char(str_);
    if (CRC(data_, 23))
    {
        flagCRC = true;
        data2int();
    }
}

bool ModBus::CRC(unsigned char* ptr, int len)
{
    unsigned char i;
    unsigned short crc = 0xFFFF;
    if (len == 0)
        len = 1;
    while (len--)
    {
        crc ^= *ptr;
        for (i = 0; i < 8; i++)
        {
            if (crc & 1)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
                crc >>= 1;
        }
        ptr++;
    }
    if (crc == crcStd)
        return true;
    return false;
}

void ModBus::data2int()
{
    auto p = 6;
    while (p < 46)
    {
        int temp = 0;
        for (int i = 0; i < 8; ++i)
            temp += pow(16, 7 - i) * stdMap[str_[p++]];
        change2str(temp);
        if (temp == 0)
            res_.emplace_back(0.0);
        else
            calc();
        numTwo_ = "";
    }
}

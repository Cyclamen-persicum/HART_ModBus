#include "ModBus.h"

void ModBus::change2str(int num_)
{
    for (int i = 31; i >= 0; --i)
        numTwo += (((num_ >> i) & 1) + '0');
    numTwo += '\0';
}

ModBus::~ModBus()
{
    numTwo.clear();
    //TODO:这里以后要清理数组
}

void ModBus::change2char(std::string str)
{
    int size = str.size();
    int cur = 0;
    for (int i = 0; i < size; i += 2)
    {
        int temp;
        if (str[i] >= '0' && str[i] <= '9')
            temp = str[i] - '0';
        else if (str[i] >= 'a' && str[i] <= 'z')
            temp = str[i] - 'a' + 10;
        if (str[i + 1] >= '0' && str[i + 1] <= '9')
            temp = str[i + 1] - '0' + 16 * temp;
        else if (str[i + 1] >= 'a' && str[i + 1] <= 'z')
            temp = str[i + 1] - 'a' + 10 + 16 * temp;
        temp += 0x00;
        data[cur++] = temp;
    }
}

double ModBus::calc(std::string numTwo)
{
    // p指向当前位置
    auto p = 0;
    // flag真则为正，假则为负
    bool flag = numTwo[0] == '0' ? 1 : 0;
    p++;
    // power为小数点移动的位数
    std::string power_str = numTwo.substr(p, 8);
    int power = stoi(power_str, nullptr, 2) - 127;
    p = 9;
    std::string integer_str;
    int integer = 0;
    std::string decimal_str;
    double decimal = 0.0;
    //幂数大于0
    if (power > 0)
    {
        integer_str = "1" + numTwo.substr(p, power);
        p += power;
        integer = stoi(integer_str, nullptr, 2);
        for (; p != 32; ++p)
        {
            if (numTwo[p] == '1')
                decimal += pow(2, -(p - 8 - power));
        }
    }
    //幂数小于0
    else if (power < 0)
    {
        decimal_str = std::string(-(power - 1), '0') + "1" + numTwo.substr(9);
        int size = decimal_str.size();
        for (int i=0; p < size; ++i)
        {
            if (numTwo[i] == '1')
                decimal += pow(2, -(i + 1));
        }
    }
    // TODO: ==0情况视情况而定
    return flag ? double(integer) + decimal : -double(integer) + decimal;
}

void ModBus::calccrc()
{
    std::string crcStd_str = str_.substr(48, 2) + str_.substr(46, 2);
    for (int i = 0; i < 4; ++i)
    {
        unsigned short temp;
        if (crcStd_str[i] >= '0' && crcStd_str[i] <= '9')
            temp = crcStd_str[i] - '0';
        else if (crcStd_str[i] >= 'a' && crcStd_str[i] <= 'z')
            temp = crcStd_str[i] - 'a' + 10;
        crcStd += pow(16, 3 - i) * temp;
    }
}

ModBus::ModBus(std::string str) : str_(str)
{
    calccrc();
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
    if(crc==crcStd)
        return true;
    return false;
}

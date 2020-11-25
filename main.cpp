#define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "ModBus.h"

using namespace std;

int main()
{
    int num = 0x40800000;
    string crctest = "01031440800000000000000000000041bb461041b58f4a65f9";
    ModBus modbus(crctest);
    //double n = modbus.getRes();
    //cout << n;
    modbus.change2char(crctest);
    //modbus.CRC(modbus.data,23);
    return 0;
}
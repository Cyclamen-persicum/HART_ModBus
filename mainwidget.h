#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

/*-------用户添加头文件-----------------------------*/
#include <QSerialPort> //端口
#include <QDebug>      //debug消息显示
#include <QMessageBox>
#include <QPalette>
#include <vector>
#include <string>
#include <QByteArray>
#include "modbus.h"
/*------------------------------------------------*/

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    /*-----------------函数声明-------------------*/
    //系统初始化
    void system_init();
    /*------------------槽函数--------------------*/
    bool button_open_port(bool isSuccess);  //打开端口
    bool button_send_data(bool isSuccess);  //发送数据
    bool receive_data();                    //接收数据
    bool button_close_port(bool isSuccess); //关闭端口
    bool button_clear_log(bool isSuccess);  //清除日志
    /*-----------------变量声明-------------------*/
    QSerialPort *globalPort; //端口变量指针
    bool isOpen;             //端口打开标记
    QByteArray qbaDataSend;  //发送字符串
};
#endif // MAINWIDGET_H

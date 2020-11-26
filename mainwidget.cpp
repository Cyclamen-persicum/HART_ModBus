#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    system_init();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::system_init()
{
    //配置端口
    isOpen=false;
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->labelConnectionStatus->setText("未连接");
    ui->labelConnectionStatus->setPalette(pe);
    globalPort=new QSerialPort(this);
    globalPort->setParity(QSerialPort::NoParity);       //设置为无奇偶校验
    globalPort->setDataBits(QSerialPort::Data8);        //设置为8位传输
    globalPort->setStopBits(QSerialPort::OneStop);      //设置为1位停止位
    //connect连接区
    connect(ui->pushButtonOpen,&QPushButton::clicked,this,&MainWidget::button_open_port);
    connect(ui->pushButtonSend,&QPushButton::clicked,this,&MainWidget::button_send_data);
    connect(globalPort,&QSerialPort::readyRead,this,&MainWidget::receive_data);
    connect(ui->pushButtonClose,&QPushButton::clicked,this,&MainWidget::button_close_port);
    connect(ui->pushButtonClearLog,&QPushButton::clicked,this,&MainWidget::button_clear_log);
}

/*----------------------------------------------------------------
 *          槽函数
 * ---------------------------------------------------------------*/

/*----------------打开端口-------------------*/
bool MainWidget::button_open_port(bool isSuccess)
{
    if(isOpen)
    {
        QMessageBox::warning(this,"警告","端口已被开启");
        return false;
    }
    //端口名设置
    globalPort->setPortName(ui->comboBoxPort->currentText());
    //端口波特率设置
    globalPort->setBaudRate(ui->comboBoxBaud->currentText().toInt());
    //打开端口
#ifdef __DEBUG_CONFIG_
    globalPort->write("1"); //向打开的端口写一个1
#endif
#ifdef __QDEBUG_CONFIG_
    qDebug()<<globalPort->portName()<<globalPort->baudRate();
    qDebug()<<"执行button_open_port";
#endif
    auto ans=globalPort->open(QIODevice::ReadWrite);
    if(ans)
    {
        isOpen=true;
        QPalette pe;
        pe.setColor(QPalette::WindowText,Qt::green);
        ui->labelConnectionStatus->setText("已连接");
        ui->labelConnectionStatus->setPalette(pe);
        ui->plainTextEditSysinfo->appendPlainText("打开端口成功！");
        ui->plainTextEditSysinfo->appendPlainText("当前打开端口为"+ui->comboBoxPort->currentText()+"\n");
        ui->plainTextEditSysinfo->appendPlainText("波特率为"+ui->comboBoxBaud->currentText()+"\n");
        ui->plainTextEditSysinfo->appendPlainText("无奇偶校验位\n8位传输\n1位停止位\n");
    }
    else
    {
        ui->plainTextEditSysinfo->appendPlainText("打开端口失败！");
    }
    return ans;
}

/*--------------发送数据--------------------*/
bool MainWidget::button_send_data(bool isSucess)
{
    if(!isOpen)
    {
        QMessageBox::warning(this,"警告","端口未打开");
        return false;
    }
    auto ans=globalPort->write(ui->lineEditSend->text().toLatin1());
    if(ans!=-1)
    {
        ui->plainTextEditSysinfo->appendPlainText("发送字符:\n");
        ui->plainTextEditSysinfo->appendPlainText(ui->lineEditSend->text().toLatin1());
        ui->plainTextEditSysinfo->appendPlainText("成功！\n");
    }
    else
    {
        ui->plainTextEditSysinfo->appendPlainText("发送失败！\n");
    }
#ifdef __QDEBUG_CONFIG_
    qDebug()<<ans;
#endif
    return ans;
}
/*----------------接收数据------------------*/
bool MainWidget::receive_data()
{
    auto resArray=globalPort->readAll();
    if(resArray.isEmpty())
    {
        return false;
    }
    auto str=QString(resArray);
#ifdef __QDEBUG_CONFIG_
    qDebug()<<resArray;
#endif
    ui->plainTextEditSysinfo->appendPlainText("接收到字符:\n");
    ui->plainTextEditSysinfo->appendPlainText(str+" ");
    std::string modbus_str=str.toStdString();
    ModBus modbus(modbus_str);
    ui->textEditData1->setText(QString::number(modbus.res_[0],10,4));
    ui->textEditData2->setText(QString::number(modbus.res_[1],10,4));
    ui->textEditData3->setText(QString::number(modbus.res_[2],10,4));
    ui->textEditData4->setText(QString::number(modbus.res_[3],10,4));
    ui->textEditData5->setText(QString::number(modbus.res_[4],10,4));
    return true;
}
/*-----------------关闭串口-----------------*/
bool MainWidget::button_close_port(bool isSuccess)
{
    if(!isOpen)
    {
        QMessageBox::warning(this,"警告","端口未打开");
        return false;
    }
    auto portName=globalPort->portName();
    globalPort->close();
    isOpen=false;
    ui->plainTextEditSysinfo->appendPlainText("串口"+portName+"已关闭\n");
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->labelConnectionStatus->setText("未连接");
    ui->labelConnectionStatus->setPalette(pe);
    return true;
}
/*------------------清除日志----------------*/
bool MainWidget::button_clear_log(bool isSuccess)
{
    ui->plainTextEditSysinfo->clear();
    return true;
}

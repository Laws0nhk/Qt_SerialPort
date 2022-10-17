#include "mainwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QRect>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    layoutInit();           //组件初始化
    scanSerialPort();       //扫描串口
    baudRateItemInit();     //波特率初始化
    dataBitsItemInit();     //数据位初始化
    parityItemInit();       //校验位初始化
    stopBitsItemInit();     //停止位初始化
}

MainWindow::~MainWindow()
{

}

void MainWindow::layoutInit()                                                   //布局初始化
{
    QList<QScreen *> list_screen = QGuiApplication::screens();
#if __arm__
    this->resize(list_screen.at(0)->geometry().width(),
                 list_screen.at(0)->geometry().height());
#else
    this->resize(800, 480);
#endif

    textBrowser = new QTextBrowser;                                             //组件初始化
    textEdit = new QTextEdit;
    gridLayout = new QGridLayout;
    vBoxLayout = new QVBoxLayout;
    funcWidget = new QWidget;
    mainWidget = new QWidget;
    serialPort = new QSerialPort(this);

    QList<QString> list_label;
    list_label << "串口号：" << "波特率：" << "数据位：" << "校验位：" << "停止位：";
    for(int i = 0; i != 5; ++i){                                                //QLabel、QComboBox
        label[i] = new QLabel(list_label[i]);
        label[i]->setMinimumSize(80, 30);
        label[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        gridLayout->addWidget(label[i], 0, i);

        comboBox[i] = new QComboBox;
        comboBox[i]->setMinimumSize(80, 30);
        comboBox[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        gridLayout->addWidget(comboBox[i], 1, i);
    }

    QList<QString> list_pushButton;                                             //QPushButton
    list_pushButton << "发送" << "打开串口";
    for(int i = 0; i != 2; ++i){
        pushButton[i] = new QPushButton(list_pushButton[i]);
        pushButton[i]->setMinimumSize(80, 30);
        pushButton[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        gridLayout->addWidget(pushButton[i], i, 5);
    }
    pushButton[0]->setEnabled(false);

    funcWidget->setLayout(gridLayout);
    vBoxLayout->addWidget(textBrowser);
    vBoxLayout->addWidget(textEdit);
    vBoxLayout->addWidget(funcWidget);
    mainWidget->setLayout(vBoxLayout);
    this->setCentralWidget(mainWidget);

    textBrowser->setPlaceholderText("接收到的消息...");
    textEdit->setText("发送的信息...");

    connect(pushButton[0], &QPushButton::click, this,
            &MainWindow::sendPushButtonClicked);
    connect(pushButton[1], &QPushButton::clicked, this,
            &MainWindow::openSerialPortPushButtonClicked);
    connect(serialPort, &QSerialPort::readyRead, this,
            &MainWindow::serialPortReadyRead);
}

void MainWindow::scanSerialPort()                                                   //扫描串口
{
    //查找可用串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            comboBox[0]->addItem(info.portName());
    }
}

void MainWindow::baudRateItemInit()                                                 //波特率初始化
{
    QList<QString> list_baudRate;
    list_baudRate << "1200" << "2400" << "14800" << "9600"
                   << "19200" << "38400" << "57600" << "115200"
                   << "230400" << "460800" << "921600";
    for(int i = 0; i != 11; ++i){
        comboBox[1]->addItem(list_baudRate[i]);
    }
    comboBox[1]->setCurrentIndex(7);
}

void MainWindow::dataBitsItemInit()                                                 //数据位初始化
{
    QList<QString> list_dataBits;
    list_dataBits << "5" << "6" << "7" << "8";
    for(int i = 0; i != 4; ++i){
        comboBox[2]->addItem(list_dataBits[i]);
    }
    comboBox[2]->setCurrentIndex(3);
}

void MainWindow::parityItemInit()                                                   //校验位初始化
{
    QList<QString> list_parityItem;
    list_parityItem << "NONE" << "Even" << "Odd" << "Space" << "Mark";
    for(int i =0; i != 5; ++i){
        comboBox[3]->addItem(list_parityItem[i]);
    }
    comboBox[3]->setCurrentIndex(0);
}

void MainWindow::stopBitsItemInit()                                                 //停止位初始化
{
    QList<QString> list_stopBitsItem;
    list_stopBitsItem << "1" << "2";
    for(int i =0; i != 2; ++i){
        comboBox[4]->addItem(list_stopBitsItem[i]);
    }
    comboBox[4]->setCurrentIndex(0);
}

void MainWindow::sendPushButtonClicked()                                            //发送按钮
{
    QByteArray data = textEdit->toPlainText().toUtf8();
    serialPort->write(data);
}

void MainWindow::openSerialPortPushButtonClicked()                                  //打开串口按钮
{
    if (pushButton[1]->text() == "打开串口") {
        serialPort->setPortName(comboBox[0]->currentText());
        serialPort->setBaudRate(comboBox[1]->currentText().toInt());

        switch (comboBox[2]->currentText().toInt()) {
        case 5:
            serialPort->setDataBits(QSerialPort::Data5);
            break;
        case 6:
            serialPort->setDataBits(QSerialPort::Data6);
            break;
        case 7:
            serialPort->setDataBits(QSerialPort::Data7);
            break;
        case 8:
            serialPort->setDataBits(QSerialPort::Data8);
            break;
        default:
            break;
        }

        switch (comboBox[3]->currentIndex()) {
        case 0:
            serialPort->setParity(QSerialPort::NoParity);
            break;
        case 1:
            serialPort->setParity(QSerialPort::EvenParity);
            break;
        case 2:
            serialPort->setParity(QSerialPort::OddParity);
            break;
        case 3:
            serialPort->setParity(QSerialPort::SpaceParity);
            break;
        case 4:
            serialPort->setParity(QSerialPort::MarkParity);
            break;
        default:
            break;
        }

        switch (comboBox[4]->currentText().toInt()) {
        case 1:
            serialPort->setStopBits(QSerialPort::OneStop);
            break;
        case 2:
            serialPort->setStopBits(QSerialPort::TwoStop);
            break;
        default:
            break;
        }

        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        if(!serialPort->open(QIODevice::ReadWrite)){
            QMessageBox::about(this, "错误", "串口无法打开！可能串口已经被占用");
        } else {
            for(int i = 0; i != 5; ++i){
                comboBox[i]->setEnabled(false);
            }
            pushButton[0]->setEnabled(true);
            pushButton[1]->setText("关闭串口");
        }
    } else {
        serialPort->close();
        for(int i = 0; i != 5; ++i){
            comboBox[i]->setEnabled(true);
        }
        pushButton[0]->setEnabled(false);
        pushButton[1]->setText("打开串口");
    }

}

void MainWindow::serialPortReadyRead()                                                  //串口读取
{
    QByteArray buf = textEdit->toPlainText().toUtf8();
    textBrowser->insertPlainText(QString(buf));
}

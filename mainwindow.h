#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QWidget>

#include <QSerialPort>
#include <QSerialPortInfo>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QSerialPort *serialPort;

    QTextBrowser *textBrowser;
    QTextEdit *textEdit;
    QLabel *label[5];
    QComboBox *comboBox[5];
    QPushButton *pushButton[2];
    QGridLayout *gridLayout;
    QVBoxLayout *vBoxLayout;
    QWidget *funcWidget;
    QWidget *mainWidget;

    void layoutInit();
    void scanSerialPort();
    void baudRateItemInit();
    void dataBitsItemInit();
    void parityItemInit();
    void stopBitsItemInit();

private slots:
    void sendPushButtonClicked();
    void openSerialPortPushButtonClicked();
    void serialPortReadyRead();

};

#endif // MAINWINDOW_H

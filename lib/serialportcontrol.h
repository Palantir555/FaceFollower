/***************************************************************************
 *   Copyright (C) 2013 by Juan Carlos Jiménez Caballero                   *
 *                                                                         *
 *   jcjc.dev@gmail.com                                                    *
 *   http://jcjc-dev.com                                                   *
 ***************************************************************************/

#ifndef SERIALPORTCONTROL_H
#define SERIALPORTCONTROL_H

#include <QObject>
#include <QIODevice>
#include <stdio.h>
#include <QDebug>

#include <qextserialenumerator.h>
#include "qextserialport.h"

#define BUFFER_SIZE 10000

class SerialPortControl : public QObject
{
    Q_OBJECT
private:
    QextSerialPort *serialPort;
    QextSerialEnumerator *serialEnum;
    QString         port_name;
    BaudRateType    baud_rate;
    DataBitsType    data_bits;
    ParityType      parity_bits;
    StopBitsType    stop_bits;
    FlowType        flow_control;
    QIODevice::OpenMode open_mode;
    QextSerialPort::QueryMode query_mode;

    void processBuffer (void);

public:
    explicit SerialPortControl(QObject *parent = 0);
    ~SerialPortControl(void);
    //Setters:
    void set_portName   (QString portName);
    void set_baudRate   (BaudRateType baudRate);
    void set_dataBits   (DataBitsType dataBits);
    void set_parityBits (ParityType parityBits);
    void set_stopBits   (StopBitsType stopBits);
    void set_flowControl(FlowType flowControl);
    void set_openMode   (QIODevice::OpenMode openMode);
    void set_queryMode  (QextSerialPort::QueryMode queryMode);

    //Getters:
    bool port_isOpen (void);

    //Orders:
    void portInit   (void);
    bool openPort   (void);
    void closePort  (void);
    void cleanBuffer(void);
    bool sendQString(QString str);
    bool sendHex    (QString str);
    QByteArray translateHex (QChar ch);


    //Buffer:
    QByteArray *buffer;
    QByteArray *input;
    //Public and accessed directly... Hmm....

signals:
    void stringProcessed (void);
    void bufferModified (QByteArray buff);
    void portOpen (void);
    void portClosed (void);
    void portsListChanged (void);
    //void controlSystemOverride (void);

public slots:
    void onDataAvailable (void);
    void devicesListModified (void);
};

#endif // SERIALPORTCONTROL_H

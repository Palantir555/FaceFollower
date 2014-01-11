/***************************************************************************
 *   Copyright (C) 2013 by Juan Carlos Jiménez Caballero                   *
 *                                                                         *
 *   jcjc.dev@gmail.com                                                    *
 *   http://jcjc-dev.com                                                   *
 ***************************************************************************/

#include "serialportcontrol.h"

SerialPortControl::SerialPortControl(QObject *parent) :
    QObject(parent)
{
    serialPort = new QextSerialPort(QextSerialPort::EventDriven, this);
    serialEnum = new QextSerialEnumerator;
    //The following only works in OSX and Win. For other OSs use refresh button:
    serialEnum->setUpNotifications();
    buffer = new QByteArray;
    buffer->clear();
    input = new QByteArray;
    input->clear();


    //Inicializamos a los valores por defecto:
    this->set_baudRate (BAUD9600);
    this->set_dataBits (DATA_8);
    this->set_parityBits (PAR_NONE);
    this->set_stopBits (STOP_1);
    this->set_flowControl (FLOW_OFF);
    this->set_openMode (QIODevice::ReadWrite);

    connect (serialPort, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    connect (serialEnum, SIGNAL(deviceDiscovered(QextPortInfo)) ,this
             ,SLOT(devicesListModified()));
    connect (serialEnum, SIGNAL(deviceRemoved(QextPortInfo))    ,this
             ,SLOT(devicesListModified()));
}

SerialPortControl::~SerialPortControl()
{
    delete serialPort;
}

void SerialPortControl::devicesListModified (void)
{
    qDebug("SerialPortContrl: devices list modified");
    emit portsListChanged();
}

void SerialPortControl::set_portName    (QString portName)
{
    this->port_name = portName;
}
void SerialPortControl::set_baudRate    (BaudRateType baudRate)
{
    this->baud_rate = baudRate;
}
void SerialPortControl::set_dataBits    (DataBitsType dataBits)
{
    this->data_bits = dataBits;
}
void SerialPortControl::set_parityBits  (ParityType parityBits)
{
    this->parity_bits = parityBits;
}
void SerialPortControl::set_stopBits    (StopBitsType stopBits)
{
    this->stop_bits = stopBits;
}
void SerialPortControl::set_flowControl (FlowType flowControl)
{
    this->flow_control = flowControl;
}
void SerialPortControl::set_openMode    (QIODevice::OpenMode openMode)
{
    this->open_mode = openMode;
}
void SerialPortControl::set_queryMode   (QextSerialPort::QueryMode queryMode)
{
    this->query_mode = queryMode;
}

void SerialPortControl::cleanBuffer (void)
{
    buffer->clear();
}
void SerialPortControl::closePort (void)
{
    if (serialPort->isOpen())
    {
        serialPort->close();
        this->cleanBuffer();
        emit portClosed();
    }
    else
        qDebug("The port is already closed");
}
bool SerialPortControl::openPort (void)
{
    this->portInit();
    bool result = 0;
    result = serialPort->open(open_mode);
    if (result)
        emit portOpen();
    return(result);
}
bool SerialPortControl::port_isOpen (void)
{
    return(serialPort->isOpen());
}

void SerialPortControl::portInit (void)
{
    qDebug("Initializating the port");

    if(serialPort->isOpen())
        serialPort->close ();
    serialPort->setPortName   (this->port_name);
    serialPort->setQueryMode  (QextSerialPort::EventDriven);
    serialPort->setBaudRate   (this->baud_rate);
    serialPort->setDataBits   (this->data_bits);
    serialPort->setParity     (this->parity_bits);
    serialPort->setStopBits   (this->stop_bits);
    serialPort->setFlowControl(this->flow_control);
}

void SerialPortControl::onDataAvailable (void)
{
    input->clear();
    if(serialPort->isOpen())
        input->append (serialPort->readAll());
    if((buffer->size() + input->size()) < BUFFER_SIZE)
        buffer->append (*input);
    else   //If buffer is full-> Assume unrecognized strs -> clean it
    {
        qDebug("Buffer emptied");
        this->cleanBuffer();
    }
    emit bufferModified (*buffer);
    this->processBuffer ();
}

void SerialPortControl::processBuffer (void)
{
    this->cleanBuffer();
}

bool SerialPortControl::sendQString (QString str)
{
    if (!port_isOpen())
    {
        qDebug() << "Error: The port is closed";
        return(0); //Shoud alert about the port being closed.
    }
    int res = -1;   //Set default to the error value.
    res = serialPort->write(str.toUtf8(), (str.toUtf8().size()));
    if (res == -1)
    {
        qDebug() << "Failed to write the message to the serial port";
        return(0);
    }
    else
        return(1);
}

bool SerialPortControl::sendHex (QString str)
{
    if (!port_isOpen())
        return(0); //Shoud alert about the port being closed.
    int res = -1;   //Set default to the error value.
    //int i=0;
    bool ok = false;
    //unsigned short int num;
    //const char *ch;
    //ch = new char;
    QByteArray BA;
    BA[0] = 0xAA;
    BA[1] = 0x0D;
    BA[2] = 0x00;
    BA[3] = 0x00;
    BA[4] = 0x00;
    BA[5] = 0x00;
    res = serialPort->write(BA);
    //res = serialPort->write(QByteArray().number(str.toULongLong(&ok, 16)));
    /*
    for(i=0; i < str.size(); i+=sizeof(short int)*2*2) // 1num = 4bytes = 8hex characters
    {
        num = str.mid(i, sizeof(short int)*2).toUInt(&ok, 16);
        ch = (char*)&num;
        qDebug(ch);
        res = serialPort->write( ch );
        ch++;
        res = serialPort->write( ch );
    }
    */
    if (res == -1)
    {
        qDebug() << "Failed to write the message to the serial port";
        return(0);
    }
    else
    {
        qDebug() << QString().number(res);
        return(1);
    }
}

QByteArray SerialPortControl::translateHex(QChar ch)
{

}

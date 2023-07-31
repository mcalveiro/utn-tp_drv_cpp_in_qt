#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>
#include <vector>

// Interfaz para el puerto serie
class ISerialPort {
public:
    virtual ~ISerialPort() {}
    virtual bool Open() = 0;
    virtual void Close() = 0;
    virtual bool Read(std::vector<char>& data) = 0;
};

#ifdef _WIN32
#include <Windows.h>

// Implementación de la clase para el puerto serie en Windows
class SerialPortWindows : public ISerialPort {
public:
    SerialPortWindows(const std::wstring& portName);
    ~SerialPortWindows() override;

    bool Open() override;
    void Close() override;
    bool Read(std::vector<char>& data) override;

private:
    HANDLE hComm;
    std::wstring portName;
};
#else
#include <fcntl.h>
#include <termios.h>

// Implementación de la clase para el puerto serie en sistemas Unix (Linux y macOS)
class SerialPortUnix : public ISerialPort {
public:
    SerialPortUnix(const std::string& portName);
    ~SerialPortUnix() override;

    bool Open() override;
    void Close() override;
    bool Read(std::vector<char>& data) override;

private:
    int fd;
    std::string portName;
};
#endif

#endif // SERIALPORT_H

#ifndef _WIN32
#include "serialport.h"
#include <iostream>
#include <unistd.h>

SerialPortUnix::SerialPortUnix(const std::string& portName)
    : fd(-1), portName(portName) {}

SerialPortUnix::~SerialPortUnix() {
    Close();
}

bool SerialPortUnix::Open() {
    fd = open(portName.c_str(), O_RDWR | O_NOCTTY);
    if (fd == -1) {
        std::cout << "Error al abrir el puerto serie." << std::endl;
        return false;
    }

    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        std::cout << "Error al obtener la configuración del puerto serie." << std::endl;
        Close();
        return false;
    }

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 10;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cout << "Error al configurar el puerto serie." << std::endl;
        Close();
        return false;
    }

    return true;
}

void SerialPortUnix::Close() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

bool SerialPortUnix::Read(std::vector<char>& data) {
    char incomingData[256];
    ssize_t bytesRead = read(fd, incomingData, sizeof(incomingData));
    if (bytesRead > 0) {
        data.assign(incomingData, incomingData + bytesRead);
        return true;
    }
    return false;
}
#endif

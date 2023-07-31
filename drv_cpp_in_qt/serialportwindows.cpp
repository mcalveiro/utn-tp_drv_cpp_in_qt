#ifdef _WIN32
#include "serialport.h"
#include <iostream>

SerialPortWindows::SerialPortWindows(const std::wstring& portName)
    : hComm(INVALID_HANDLE_VALUE), portName(portName) {}

SerialPortWindows::~SerialPortWindows() {
    Close();
}

bool SerialPortWindows::Open() {
    hComm = CreateFileW(portName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hComm == INVALID_HANDLE_VALUE) {
        std::wcout << L"Error al abrir el puerto COM." << std::endl;
        return false;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hComm, &dcbSerialParams)) {
        std::wcout << L"Error al obtener la configuración del puerto COM." << std::endl;
        Close();
        return false;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hComm, &dcbSerialParams)) {
        std::wcout << L"Error al configurar el puerto COM." << std::endl;
        Close();
        return false;
    }

    return true;
}

void SerialPortWindows::Close() {
    if (hComm != INVALID_HANDLE_VALUE) {
        CloseHandle(hComm);
        hComm = INVALID_HANDLE_VALUE;
    }
}

bool SerialPortWindows::Read(std::vector<char>& data) {
    char incomingData[256];
    DWORD bytesRead = 0;

    if (ReadFile(hComm, incomingData, sizeof(incomingData), &bytesRead, NULL)) {
        // Copiar los datos leídos al vector
        data.assign(incomingData, incomingData + bytesRead);
        return true;
    }

    return false;
}
#endif

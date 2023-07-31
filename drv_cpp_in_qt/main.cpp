/*****************************************************
 * Archivo: main.cpp
 *
 * Autor: Miguel A. Calveiro
 * Clase: Programación
 *
 *
 * Descripción
 * ***********
 *
 * Este archivo contiene el programa principal que interactúa con un puerto serie.
 * El programa está diseñado para funcionar tanto en Windows como en sistemas Unix (Linux y macOS).
 * Utiliza la clase SerialPortWindows en caso de estar en Windows, y SerialPortUnix en otros sistemas.
 * El programa lee datos desde el puerto serie y los muestra en la consola en tiempo real.
 * Además, verifica si se ha presionado la barra espaciadora para detener la lectura de datos.
 * En Windows, el programa mostrará mensajes en formato wide (std::wcout) debido al uso de wstring.
 *
 *****************************************************/

#include <iostream>

/* Clase de contenedor de la biblioteca estándar de C++.
   Es útil para almacenar y manipular colecciones de elementos de manera eficiente. */
#include <vector>

/* Clase para medir y manipular intervalos de tiempo,medir el tiempo de ejecución de una sección de código
   o establecer retardos en el programa.
   Por ejemplo, se utiliza aquí para agregar una pausa de 100 milisegundos entre las lecturas de datos desde el puerto serie. */
#include <chrono>

/* El espacio de nombres std::this_thread contiene funciones para trabajar con el hilo actual,
 * como std::this_thread::sleep_for, que permite pausar la ejecución del hilo actual durante un cierto período de tiempo.
 * En el código, se usa para pausar la ejecución del bucle principal durante 100 milisegundos
 * entre cada lectura de datos desde el puerto serie.*/
#include <thread>

// API para interactuar con un puerto serie (COM) desde la aplicación.
#include "serialport.h"

#ifdef _WIN32

/* Este encabezado es específico de Windows y proporciona algunas funciones de consola que son útiles
 * para leer entradas del usuario sin esperar a que presione Enter, como GetAsyncKeyState
 * que se utiliza aquí para verificar si se ha presionado la barra espaciadora.
 * No es una parte estándar de C++ y no está disponible en sistemas no Windows.
 * Se utiliza para habilitar el manejo de la barra espaciadora para detener la lectura de datos en Windows. */
#include <conio.h>

#define PRINT_WAITING_MSG() std::wcout << L"Esperando datos..." << std::endl
#define PRINT_STOPPED_MSG() std::wcout << L"\nPrograma detenido por la barra espaciadora." << std::endl
#else
#define PRINT_WAITING_MSG() /* std::cout << "Esperando datos..." << std::endl */
#define PRINT_STOPPED_MSG() /* std::cout << "\nPrograma detenido por la barra espaciadora." << std::endl */
#endif

// Función para verificar si se ha presionado la barra espaciadora (solo en Windows)
bool isSpaceKeyPressed() {
#ifdef _WIN32
    return (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
#else
    return false; // En otras plataformas, no se verifica la barra espaciadora
#endif
}

// Función para manejar la señal de interrupción (CTRL+C)
#ifdef _WIN32
BOOL CtrlHandler(DWORD fdwCtrlType) {
    if (fdwCtrlType == CTRL_C_EVENT) {
        std::wcout << L"\nPrograma detenido por la señal de interrupción (CTRL+C)." << std::endl;
        return TRUE;
    }
    return FALSE;
}
#endif

// Función principal del programa
int main() {
#ifdef _WIN32
    // Nombre del puerto COM en Windows
    std::wstring portName = L"COM6";
#else
    // Nombre del puerto en sistemas Unix (Linux y macOS)
    std::string portName = "/dev/ttyS0";
#endif

    // Crear el objeto del puerto serie según el sistema operativo
#ifdef _WIN32
    SerialPortWindows serialPort(portName); // Clase para Windows
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {
        std::wcout << L"Error al registrar el manejador de señales." << std::endl;
        return 1;
    }
#else
    /* SerialPortUnix serialPort(portName); // Clase para sistemas Unix */
#endif

    // Intentar abrir el puerto serie
    if (!serialPort.Open()) {
#ifdef _WIN32
        std::wcout << L"No se pudo abrir el puerto serie." << std::endl;
#else
        /* std::cout << "No se pudo abrir el puerto serie." << std::endl; */
#endif
        return 1; // Salir del programa con código de error
    }

    // Mostrar mensaje de espera (dependiendo del sistema operativo)
    PRINT_WAITING_MSG();

    while (true) {
        // Verificar si se ha presionado la barra espaciadora (solo en Windows)
        if (isSpaceKeyPressed()) {
            std::cout << "\nPrograma detenido por la barra espaciadora." << std::endl;
            break; // Salir del bucle si se ha presionado la barra espaciadora
        }

        std::vector<char> receivedData;
        if (serialPort.Read(receivedData)) {
            // Imprimir los datos recibidos desde el puerto serie
            for (char c : receivedData) {
                std::cout << c;
            }
            std::cout << std::endl;
        }

        // Añadir una pausa de 100 ms para no sobrecargar el procesador
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }


#ifdef _WIN32
    // En Windows, esperar a que el usuario presione Enter antes de salir
    std::cout << "\nPresione Enter para salir...";
    std::cin.ignore();
#endif

    // Mostrar mensaje de programa detenido por la barra espaciadora
    PRINT_STOPPED_MSG();

    return 0; // Salir del programa con éxito
}

## Programa de Interacción con Puerto Serie

**Archivo:** main.cpp

**Autor:** Miguel A. Calveiro

**Clase:** Programación

### Descripción

Este archivo contiene un programa de consola que interactúa con un puerto serie (COM). El programa está diseñado para funcionar tanto en sistemas Windows como en sistemas Unix (Linux y macOS). Se adapta a la plataforma mediante el uso de la clase `SerialPortWindows` en Windows y `SerialPortUnix` en otros sistemas.

El objetivo principal del programa es leer datos desde el puerto serie y mostrarlos en tiempo real en la consola. Además, verifica si se ha presionado la barra espaciadora para detener la lectura de datos.

En la versión de Windows, el programa utiliza `std::wcout` para mostrar mensajes en formato wide debido al uso de `wstring`.

### Estructura del Código

El programa consta de las siguientes partes principales:

1. Inclusión de bibliotecas: Incluye las bibliotecas necesarias para el funcionamiento del programa, como las bibliotecas estándar de C++ para entrada/salida, manejo de tiempos y manipulación de vectores, así como la biblioteca personalizada `"serialport.h"` que proporciona la interfaz para interactuar con el puerto serie.

2. Macros de mensajes: Define macros para mostrar mensajes en la consola. Dependiendo del sistema operativo, estos mensajes pueden estar en formato wide en el caso de Windows o en formato estándar en otros sistemas.

3. Funciones auxiliares:
   - `isSpaceKeyPressed()`: Verifica si se ha presionado la barra espaciadora (solo en Windows) utilizando `GetAsyncKeyState(VK_SPACE)`.
   - `CtrlHandler()`: Maneja la señal de interrupción (CTRL+C) en Windows.

4. Función `main()`: La función principal del programa que realiza las siguientes acciones:
   - Define el nombre del puerto serie según el sistema operativo (`portName`).
   - Crea un objeto del puerto serie (`SerialPortWindows` en Windows y `SerialPortUnix` en otros sistemas).
   - Intenta abrir el puerto serie usando el método `Open()`.
   - Si el puerto no se puede abrir, muestra un mensaje de error y finaliza el programa con un código de error (1).
   - Muestra un mensaje de espera en la consola utilizando la macro `PRINT_WAITING_MSG()`.
   - Entra en un bucle infinito que realiza lo siguiente:
     - Verifica si se ha presionado la barra espaciadora (solo en Windows) utilizando la función `isSpaceKeyPressed()`. Si se ha presionado, muestra un mensaje y sale del bucle.
     - Lee los datos desde el puerto serie utilizando el método `Read(receivedData)` y los almacena en un vector de caracteres (`receivedData`).
     - Imprime los datos recibidos en la consola.
     - Pausa la ejecución del bucle durante 100 milisegundos usando `std::this_thread::sleep_for()` para evitar sobrecargar el procesador.
   - En Windows, muestra un mensaje para esperar a que el usuario presione Enter antes de salir.
   - Muestra un mensaje que indica que el programa ha sido detenido por la barra espaciadora.
   - Finaliza el programa con éxito (código de retorno 0).

### Uso y Plataformas Compatibles

El programa ha sido diseñado para ser utilizado en sistemas Windows, Linux y macOS. Al utilizar la clase `SerialPortWindows` para Windows y `SerialPortUnix` para otros sistemas, se adapta automáticamente al sistema operativo en el que se ejecute.

Para utilizar el programa, simplemente compílalo y ejecútalo. Asegúrate de tener conectado un dispositivo al puerto serie (COM) desde el cual puedas recibir datos. El programa leerá y mostrará los datos en tiempo real en la consola.

### Notas

- El programa funciona adecuadamente en sistemas Windows, Linux y macOS, siempre que se utilice el archivo `"serialport.h"` apropiado para cada plataforma.
- En sistemas Windows, los mensajes se muestran en formato wide debido al uso de `wstring`. Esto permite la correcta visualización de caracteres no ASCII, como los acentos y caracteres especiales.
- El programa verifica si se ha presionado la barra espaciadora en Windows para detener la lectura de datos desde el puerto serie.




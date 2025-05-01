#include "gestorCliente.h"


using namespace std;

void mostrarClientes(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Limpiar buffers
    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));

    // Enviar comando al servidor
    strcpy(sendBuff, "GET_CLIENTS");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Esperar respuesta del servidor
    int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación
        cout << recvBuff << endl;
        mostrarMenuPrincipalCliente(s);
    } else if (bytes == 0) {
        cout << "El servidor ha cerrado la conexión" << endl;
    } else {
        cout << "Error al recibir datos: " << WSAGetLastError() << endl;
    }
}

void mostrarHabitaciones(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Limpiar buffers
    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));

    // Enviar comando al servidor
    strcpy(sendBuff, "GET_ROOMS");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Esperar respuesta del servidor
    int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación
        cout << recvBuff << endl;
        mostrarMenuPrincipalCliente(s);
    } else if (bytes == 0) {
        cout << "El servidor ha cerrado la conexión" << endl;
    } else {
        cout << "Error al recibir datos: " << WSAGetLastError() << endl;
    }
}

void crearReserva(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];
    string input;

    // Enviar comando al servidor
    strcpy(sendBuff, "CREATE_RESERVATION");
    send(s, sendBuff, strlen(sendBuff), 0);

    bool continuar = true;
    while (continuar) {
        // Recibir mensaje del servidor
        int bytes = recv(s, recvBuff, sizeof(recvBuff), 0);
        if (bytes > 0) {
            recvBuff[bytes] = '\0';

            // Verificar tipo de mensaje
            if (strncmp(recvBuff, "INPUT|", 6) == 0) {
                // Es una solicitud de entrada
                printf("%s", recvBuff + 6); // Mostrar mensaje sin prefijo

                // Leer entrada del usuario
                getline(cin, input);

                // Enviar respuesta al servidor
                strcpy(sendBuff, input.c_str());
                send(s, sendBuff, strlen(sendBuff), 0);
            }
            else if (strncmp(recvBuff, "INFO|", 5) == 0) {
                // Es un mensaje informativo
                printf("%s\n", recvBuff + 5); // Mostrar mensaje sin prefijo
                continuar = false; // Finalizar el ciclo
            }
            else {
                // Otro tipo de mensaje
                printf("%s\n", recvBuff);
                continuar = false; // Finalizar el ciclo
            }
        }
        else {
            printf("Error al recibir datos del servidor\n");
            continuar = false;
        }
    }
}

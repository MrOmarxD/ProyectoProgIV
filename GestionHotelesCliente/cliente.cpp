#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <string>

#include "modulos/gestorUsuarios.h"
#include "modulos/gestorHabitaciones.h"
#include "modulos/gestorReservas.h"
#include "modulos/gestorFacturas.h"
#include "modulos/gestorRegistros.h"
#include "modulos/gestorMenus.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
using namespace std;

char menu() {
    printf("\n==========================================================\n");
    printf(" SISTEMA DE GESTION DE HOTELES - CLIENTE REMOTO\n");
    printf("==========================================================\n");
    printf("1. Ver listado de clientes\n");
    printf("2. Ver listado de habitaciones\n");
    printf("3. Crear nueva reserva\n");
    printf("4. Salir\n\n");
    printf("Seleccione una opcion: ");

    char opcion = getchar();
    char c;
    while ((c = getchar()) != '\n' && c != EOF);

    return opcion;
}

void mostrarClientes(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Enviar comando al servidor
    strcpy(sendBuff, "GET_CLIENTS");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Recibir respuesta del servidor
    int bytes = recv(s, recvBuff, sizeof(recvBuff), 0);
    if (bytes > 0) {
        recvBuff[bytes] = '\0';
        printf("%s\n", recvBuff);
    }
}

void pedirHabitacion(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Enviar comando al servidor
    strcpy(sendBuff, "GET_ROOMS");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Recibir respuesta del servidor
    int bytes = recv(s, recvBuff, sizeof(recvBuff), 0);
    if (bytes > 0) {
        recvBuff[bytes] = '\0';
        printf("%s\n", recvBuff);
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

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET s;
    struct sockaddr_in server;
    char sendBuff[512], recvBuff[512];

    printf("\nInitialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return -1;
    }

    printf("Initialised.\n");

    //SOCKET creation
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    //CONNECT to remote server
    if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        printf("Connection error: %d", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return -1;
    }

    printf("Connection stablished with: %s (%d)\n", inet_ntoa(server.sin_addr),
           ntohs(server.sin_port));

    /*EMPIEZA EL PROGRAMA DEL CLIENTE*/
    char c;
    do {
        c = menu();


        switch(c) {
            case '1':
                mostrarClientes(s);
                break;

            case '2':
                pedirHabitacion(s);
                break;

            case '3':
                crearReserva(s);
                break;

            case '4':
                strcpy(sendBuff, "EXIT");
                send(s, sendBuff, strlen(sendBuff), 0);  // Modificado: usar strlen en lugar de sizeof
                printf("Desconectando del servidor...\n");
                break;

            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                break;
        }

    } while(c != '4');

    /*ACABA EL PROGRAMA DEL CLIENTE*/

    // CLOSING the socket and cleaning Winsock...
    closesocket(s);
    WSACleanup();

    return 0;
}

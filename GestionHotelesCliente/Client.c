#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000


char menu() {
    printf("\n==========================================================\n");
    printf("    SISTEMA DE GESTION DE HOTELES - CLIENTE REMOTO\n");
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

    printf("\n--- LISTADO DE CLIENTES ---\n");
    printf("DNI          | Nombre                | Apellido              | Telefono         | Email\n");
    printf("-----------------------------------------------------------------------------------------\n");


    char sendBuff[512];
    strcpy(sendBuff, "GET_CLIENTS");
    send(s, sendBuff, sizeof(sendBuff), 0);


    recv(s, recvBuff, sizeof(recvBuff), 0);
    if (strcmp(recvBuff, "CLIENT_LIST_START") != 0) {
        printf("Error en la comunicacion con el servidor.\n");
        return;
    }


    while (1) {
        recv(s, recvBuff, sizeof(recvBuff), 0);
        if (strcmp(recvBuff, "CLIENT_LIST_END") == 0) {
            break;
        }

        char *dni = strtok(recvBuff, "|");
        char *nombre = strtok(NULL, "|");
        char *apellido = strtok(NULL, "|");
        char *telefono = strtok(NULL, "|");
        char *email = strtok(NULL, "|");

        if (dni && nombre && apellido && telefono && email) {
            printf("%-13s| %-22s| %-22s| %-17s| %s\n",
                  dni, nombre, apellido, telefono, email);
        } else {
            printf("Error al procesar datos del cliente: formato incorrecto\n");
        }
    }

    printf("-----------------------------------------------------------------------------------------\n");
}


void pedirHabitacion(SOCKET s) {
    char recvBuff[512];

    printf("\n--- LISTADO DE HABITACIONES ---\n");
    printf("Numero   | Tipo        | Estado      | Precio (EUR) | Capacidad | Descripcion\n");
    printf("-------------------------------------------------------------------------------\n");


    char sendBuff[512];
    strcpy(sendBuff, "GET_ROOMS");
    send(s, sendBuff, sizeof(sendBuff), 0);


    recv(s, recvBuff, sizeof(recvBuff), 0);
    if (strcmp(recvBuff, "ROOM_LIST_START") != 0) {
        printf("Error en la comunicacion con el servidor.\n");
        return;
    }


    while (1) {
        recv(s, recvBuff, sizeof(recvBuff), 0);
        if (strcmp(recvBuff, "ROOM_LIST_END") == 0) {
            break;
        }


        char *numero = strtok(recvBuff, "|");
        char *tipo = strtok(NULL, "|");
        char *estado = strtok(NULL, "|");
        char *precio = strtok(NULL, "|");
        char *capacidad = strtok(NULL, "|");
        char *descripcion = strtok(NULL, "|");

        if (numero && tipo && estado && precio && capacidad) {

            char desc_short[31] = "";
            if (descripcion) {
                strncpy(desc_short, descripcion, 30);
                if (strlen(descripcion) > 30) {
                    strcat(desc_short, "...");
                }
            }

            printf("%-9s| %-12s| %-11s| %-12s | %-9s | %s\n",
                  numero, tipo, estado, precio, capacidad, desc_short);
        } else {
            printf("Error al procesar datos de la habitación: formato incorrecto\n");
        }
    }

    printf("-------------------------------------------------------------------------------\n");
}


void client_crearReserva(SOCKET s) {
    char sendBuff[512], recvBuff[512];
    char temp[100];

    printf("\n--- CREAR NUEVA RESERVA ---\n");


    strcpy(sendBuff, "CREATE_RESERVATION");
    send(s, sendBuff, sizeof(sendBuff), 0);

    // Mandar Dni cliente
    printf("Ingrese DNI del cliente: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = 0;
    strcpy(sendBuff, temp);
    send(s, sendBuff, sizeof(sendBuff), 0);

    // Mandar numero habitacion
    printf("Ingrese numero de habitacion: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = 0;
    strcpy(sendBuff, temp);
    send(s, sendBuff, sizeof(sendBuff), 0);

    // Fecha entrada
    printf("Ingrese fecha de entrada (AAAA/MM/DD): ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = 0;
    strcpy(sendBuff, temp);
    send(s, sendBuff, sizeof(sendBuff), 0);

    // Fecha salida
    printf("Ingrese fecha de salida (AAAA/MM/DD): ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = 0;
    strcpy(sendBuff, temp);
    send(s, sendBuff, sizeof(sendBuff), 0);

    // Observaciones
    printf("Observaciones: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = 0; // Remove newline
    strcpy(sendBuff, temp);
    send(s, sendBuff, sizeof(sendBuff), 0);

    // Send end marker
    strcpy(sendBuff, "CREATE_RESERVATION_END");
    send(s, sendBuff, sizeof(sendBuff), 0);

    // Receive response
    recv(s, recvBuff, sizeof(recvBuff), 0);

    // Parse response (format: "STATUS|Message|Details")
    char *status = strtok(recvBuff, "|");
    char *message = strtok(NULL, "|");
    char *details = strtok(NULL, "|");

    if (strcmp(status, "SUCCESS") == 0) {
        printf("\n¡Reserva creada exitosamente!\n");
        if (details) printf("%s\n", details);
    } else {
        printf("\nError al crear la reserva: %s\n", message ? message : "Error desconocido");
    }

    printf("Presione Enter para continuar...");
    getchar();
}

int client_main(int argc, char *argv[]) {
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

    // crear SOCKET
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    // CONECTAR
    printf("Conectando al servidor %s:%d...\n", SERVER_IP, SERVER_PORT);
    if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        printf("Connection error: %d", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return -1;
    }

    printf("Conexion establecida con: %s (%d)\n",
           inet_ntoa(server.sin_addr), ntohs(server.sin_port));

    // Main cliente
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
                client_crearReserva(s);
                break;

            case '4':

                strcpy(sendBuff, "EXIT");
                send(s, sendBuff, sizeof(sendBuff), 0);
                printf("Desconectando del servidor...\n");
                break;

            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                break;
        }

    } while(c != '4');

    // Cerrar socket
    closesocket(s);
    WSACleanup();

    printf("Conexion terminada. Gracias por usar el sistema.\n");
    return 0;
}

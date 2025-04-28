#include <stdio.h>
#include <winsock2.h>
#include <iostream>

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
   /*


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

    printf("-----------------------------------------------------------------------------------------\n");*/
    // Como la funcion listar cliente no devuelve nada esto no funciona, esto es por si devuelve un char, tendremos
    //que cambiar la funcion para que sea asi y en el server una vez termine de mandar todos los datos
    // este tiene que mandar CLIENT_LIST_END para saber que ha terminado
}
void crearReserva(SOCKET s){}
void perdirHabitacion(SOCKET s){}

int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in server;
	char sendBuff[512], recvBuff[512];

	/*
	 * En sendBuff guardaremos lo que el cliente le env�a al servidor
	 * En recvBuff guardaremos lo que el servidor le env�a al cliente
	 * */
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

	server.sin_addr.s_addr = inet_addr(SERVER_IP); //INADDR_ANY;
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
	                send(s, sendBuff, sizeof(sendBuff), 0);
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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "bd/gestionBD.h"
#include "modulos/gestorUsuarios.h"
#include "modulos/gestorClientes.h"
#include "modulos/gestorHabitaciones.h"
#include "modulos/gestorReservas.h"
#include "modulos/gestorFacturas.h"
#include "modulos/gestorRegistros.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

void procesarPeticion(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    memset(sendBuff, 0, 512);

    if (strcmp(recvBuff, "GET_USERS") == 0) {
		// Obtener la lista de usuarios desde la BD
    	listarUsuarios(comm_socket, recvBuff, sendBuff);
	} else if (strcmp(recvBuff, "BUSCAR_USUARIO") == 0) {
		buscarUsuario(comm_socket, recvBuff, sendBuff);
	} else if (strcmp(recvBuff, "MODIFICAR_USUARIO") == 0) {
		modificarUsuario(comm_socket, recvBuff, sendBuff);
    } else if (strcmp(recvBuff, "GET_CLIENTS") == 0) {
    	obtenrClientes(comm_socket, recvBuff, sendBuff);
    } else if (strcmp(recvBuff, "DELETE_USER") == 0) {
    	eliminarUsuario(comm_socket, recvBuff, sendBuff);
    } else if (strcmp(recvBuff, "CREATE_RESERVATION") == 0) {
    	crearReserva(comm_socket, recvBuff, sendBuff);
    } else if (strcmp(recvBuff, "GET_ROOMS") == 0) {
    	obtenerHabitaciones(comm_socket, recvBuff, sendBuff);
    } else if (strcmp(recvBuff, "CREATE_CLIENT") == 0) {
    	crearCliente(comm_socket, recvBuff, sendBuff);
    } else if (strcmp(recvBuff, "MODIFICAR_CLIENTE") == 0) {
    	modificarCliente(comm_socket, recvBuff, sendBuff);
    } else if (strcmp(recvBuff, "2") == 0) {
    	crearUsuario(comm_socket, recvBuff, sendBuff);
    } else if (strcmp(recvBuff, "1") == 0) {
        iniciarSesion(comm_socket, recvBuff, sendBuff);
    } else if (strcmp(recvBuff, "SALIR") == 0) {
    	// Cliente solicita terminar la conexión
    	strcpy(sendBuff, "Cerrando conexión");
    	send(comm_socket, sendBuff, strlen(sendBuff), 0);
    	printf("Cliente solicitó terminar la conexión\n");
    } else {
    	// Unknown command
    	sprintf(sendBuff, "Comando desconocido: %s", recvBuff);
    	send(comm_socket, sendBuff, strlen(sendBuff), 0);
    	printf("Comando desconocido recibido: %s\n", recvBuff);
    }
}

int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET conn_socket; //el que lleva la conexion
	SOCKET comm_socket; //el que lo comunica
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512]; // lo que yo envio, lo que yo recibo
	int bytes_recibidos;

	printf("\nInitialising Winsock...\n"); // inicializa la libreria
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation creacion del socket( la primera estructura
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");
	// cual es la ip y cual es el puerto
	server.sin_addr.s_addr = inet_addr(SERVER_IP); //INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server,
			sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	printf("Bind done.\n"); //DEJAR EL SOCKET EN ESPERA

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		printf("Listen failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	printf("Waiting for incoming connections...\n");
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
	// Using comm_socket is able to send/receive data to/from connected client
	if (comm_socket == INVALID_SOCKET) {
		printf("accept failed with error code : %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	printf("Incomming connection from: %s (%d)\n", inet_ntoa(client.sin_addr),
			ntohs(client.sin_port));

	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);

	int fin = 0;
	abrirBd();
	printf("Base de datos abierta correctamente\n");

	do {
		// Limpiar los buffers
		memset(recvBuff, 0, 512);
		memset(sendBuff, 0, 512);

		// Recibir datos del cliente
		bytes_recibidos = recv(comm_socket, recvBuff, 512, 0);

		if (bytes_recibidos > 0) {
			// Asegurar que el buffer termina en NULL
			recvBuff[bytes_recibidos] = '\0';

			printf("Mensaje recibido: %s\n", recvBuff);

			// Procesar la petición
			procesarPeticion(comm_socket, recvBuff, sendBuff);

			// Si el cliente envía "SALIR", terminamos el bucle
			if (strcmp(recvBuff, "SALIR") == 0) {
				fin = 1;
				printf("Recibida señal de salida del cliente\n");
			}
		} else if (bytes_recibidos == 0) {
			// El cliente ha cerrado la conexión
			printf("Cliente desconectado\n");
			fin = 1;
		} else {
			// Error en la recepción
			printf("Error en recv: %d\n", WSAGetLastError());
			fin = 1;
		}

	} while (fin == 0);

	printf("Cerrando la base de datos...\n");
	cerrarBd();
	printf("Base de datos cerrada correctamente\n");

	// CLOSING the sockets and cleaning Winsock...
	closesocket(comm_socket);
	WSACleanup();
	printf("Servidor finalizado.\n");

    return 0;
}

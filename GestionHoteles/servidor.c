#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <winsock2.h>

#include "bd/gestionBD.h"
#include "modulos/gestorUsuarios.h"
#include "modulos/gestorHabitaciones.h"
#include "modulos/gestorReservas.h"
#include "modulos/gestorFacturas.h"
#include "modulos/gestorRegistros.h"
#include "modulos/gestorMenus.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

// Modifica procesarPeticion para que reciba el socket


void procesarPeticion(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    printf("Procesando petición: '%s'\n", recvBuff);

    // Clear the send buffer before populating it
    memset(sendBuff, 0, 512);

    if (strcmp(recvBuff, "GET_CLIENTS") == 0) {
        // Populate the sendBuff with client list
        strcpy(sendBuff, "Lista de clientes: Cliente1, Cliente2, Cliente3");
        printf("Manda lista de clientes\n");
    } else if (strcmp(recvBuff, "CREATE_RESERVATION") == 0) {
        crearReserva(comm_socket, recvBuff, sendBuff);
        printf("Reserva procesada\n");
    } else if (strcmp(recvBuff, "GET_ROOMS") == 0) {
        // Populate the sendBuff with room list
        strcpy(sendBuff, "Lista de habitaciones: Hab101, Hab102, Hab103");
        printf("Manda la lista de habitaciones\n");
    } else if (strcmp(recvBuff, "BUSCAR_CLIENTE") == 0) {
        // Clear and receive new data in recvBuff
        memset(recvBuff, 0, 512);
        recv(comm_socket, recvBuff, 512, 0);
        buscarClientesBD(recvBuff, comm_socket);
    } else if (strcmp(recvBuff, "MODIFICAR_CLIENTE") == 0) {
        // Solicitar el DNI del cliente a modificar
        strcpy(sendBuff, "ENVIAR_DNI");
        send(comm_socket, sendBuff, strlen(sendBuff), 0);

        // Recibir el DNI
        memset(recvBuff, 0, 512);
        int bytes = recv(comm_socket, recvBuff, 512, 0);
        if (bytes > 0) {
            recvBuff[bytes] = '\0'; // Asegurar terminación
            modificarCliente(recvBuff, comm_socket);
        }
    } else if (strcmp(recvBuff, "1") == 0) {
        printf("Procesando opción 1 (login)...\n");

        // Clear buffers for fresh receipt
        char usu[20] = {0}, con[20] = {0};
        int resul;

        // Get username
        memset(recvBuff, 0, 512);
        int bytes_username = recv(comm_socket, recvBuff, 512, 0);
        if (bytes_username <= 0) {
            strcpy(sendBuff, "Error al recibir el nombre de usuario");
            return;
        }

        strncpy(usu, recvBuff, sizeof(usu)-1);
        usu[sizeof(usu)-1] = '\0';

        // Get password
        memset(recvBuff, 0, 512);
        int bytes_password = recv(comm_socket, recvBuff, 512, 0);
        if (bytes_password <= 0) {
            strcpy(sendBuff, "Error al recibir la contraseña");
            return;
        }

        strncpy(con, recvBuff, sizeof(con)-1);
        con[sizeof(con)-1] = '\0';

        // Debug output
        printf("Usuario recibido: %s, Contraseña recibida: %s\n", usu, con);

        // Send confirmation
        sprintf(sendBuff, "Servidor: Recibido %s %s", usu, con);
        send(comm_socket, sendBuff, strlen(sendBuff), 0);

        // This part needs fixing - u is not initialized properly
        Usuario *u = (Usuario*) malloc(sizeof(Usuario));
        if (recuperarUsuarioBD(usu, u) != 0) {
            if (strcmp(con, u->password) == 0) resul = 0;
            else resul = 1;
        } else {
            resul = 2;
        }
        free(u);

        // Clear buffer and send result
        memset(sendBuff, 0, 512);
        sprintf(sendBuff, "%d", resul);
        send(comm_socket, sendBuff, strlen(sendBuff), 0);
        printf("Verificación de credenciales completada: %d\n", resul);
    } else {
        // Unknown command
        sprintf(sendBuff, "Comando desconocido: %s", recvBuff);
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

			// Enviar respuesta al cliente
			send(comm_socket, sendBuff, strlen(sendBuff), 0);

			// Si el cliente envía "SALIR", terminamos el bucle
			if (strcmp(recvBuff, "SALIR") == 0) {
				fin = 1;
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
		cerrarBd();

		// CLOSING the sockets and cleaning Winsock...
		closesocket(comm_socket);
		WSACleanup();

	    return 0;
	}


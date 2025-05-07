#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "bd/gestionBD.h"
#include "modulos/gestorUsuarios.h"
#include "modulos/gestorHabitaciones.h"
#include "modulos/gestorReservas.h"
#include "modulos/gestorFacturas.h"
#include "modulos/gestorRegistros.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

void procesarPeticion(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    memset(sendBuff, 0, 512);

    if (strcmp(recvBuff, "GET_CLIENTS") == 0) {
        char* listaDeClientes = listarClientes();
        strncpy(sendBuff, listaDeClientes, 511);
        sendBuff[511] = '\0'; // Aseguramos que termine con nulo
        send(comm_socket, sendBuff, strlen(sendBuff), 0);
    } else if (strcmp(recvBuff, "DELETE_USER") == 0) {
        // Recibir el nombre de usuario a eliminar
        memset(recvBuff, 0, 512);
        int bytes = recv(comm_socket, recvBuff, 512, 0);
        if (bytes > 0) {
            recvBuff[bytes] = '\0'; // Asegurar terminación
            // Verificar si el usuario existe
            if (!comprobarUsuario(recvBuff)) {
                strcpy(sendBuff, "ERROR: El usuario no existe en la base de datos");
                printf("Usuario %s no existe en la BD\n", recvBuff);
            } else {
                // Eliminar usuario de la BD
                if (eliminarUsuarioBD(recvBuff) != 1) {
                    strcpy(sendBuff, "Usuario no eliminado correctamente");
                    printf("Error al eliminar el usuario %s de la BD\n", recvBuff);
                } else {
                    strcpy(sendBuff, "Usuario eliminado correctamente");
                    printf("Usuario %s eliminado de la BD\n", recvBuff);
                }
            }

            send(comm_socket, sendBuff, strlen(sendBuff), 0);
        }
    }
    else if (strcmp(recvBuff, "CREATE_RESERVATION") == 0) {
    	// Procesar registro de nuevo usuario
		memset(recvBuff, 0, 512);
		int bytes = recv(comm_socket, recvBuff, 512, 0);
		if (bytes > 0) {
			recvBuff[bytes] = '\0'; // Asegurar terminación
			printf("Datos de la reserva recibidos: %s\n", recvBuff);

			// Extraer datos de la reserva
			Reserva *nuevaReserva = (Reserva*) malloc(sizeof(Reserva));
			memset(nuevaReserva, 0, sizeof(Reserva));

			// Parsear los datos separados por '|'
			char *token;
			char *rest = recvBuff;

			token = strtok_s(rest, "|", &rest);
			if (token != NULL) strcpy(nuevaReserva->dni_cliente, token);

			token = strtok_s(rest, "|", &rest);
			if (token != NULL) strcpy(nuevaReserva->fecha_entrada, token);

			token = strtok_s(rest, "|", &rest);
			if (token != NULL) strcpy(nuevaReserva->fecha_salida, token);

			token = strtok_s(rest, "|", &rest);
			if (token != NULL) strcpy(nuevaReserva->estado, token);

			token = strtok_s(rest, "|", &rest);
			if (token != NULL) strcpy(nuevaReserva->observaciones, token);

			token = strtok_s(rest, "|", &rest);
			if (token != NULL) nuevaReserva->id_habitacion = atoi(token);

			token = strtok_s(rest, "|", &rest);
			if (token != NULL) nuevaReserva->monto = atoi(token);

			printf("Datos extraídos - dni_cliente: %s, fecha_entrada: %s, fecha_salida: %s, estado: %s, observaciones: %s, id_habitacion: %d, monto: %d\n",
				  nuevaReserva->dni_cliente, nuevaReserva->fecha_entrada, nuevaReserva->fecha_salida,
				  nuevaReserva->estado, nuevaReserva->observaciones, nuevaReserva->id_habitacion, nuevaReserva->monto);

			// Verificar si el cliente existe
			Cliente *cli = (Cliente*) malloc(sizeof(Cliente));
			if (recuperarClienteBD(nuevaReserva->dni_cliente, cli) == 0) {
				strcpy(sendBuff, "ERROR: El dni_cliente no existe");
				printf("DNI %s no existe en la BD\n", nuevaReserva->dni_cliente);
			} else {
				// Crear reserva en la BD
				if (crearReservaBD(nuevaReserva) != 1) {
					strcpy(sendBuff, "Reserva no registrada correctamente");
					printf("Reserva del cliente: %s no a quedado registrada en la BD\n", nuevaReserva->dni_cliente);
				} else {
					strcpy(sendBuff, "Reserva registrada correctamente");
					printf("Reserva del cliente: %s registrada en la BD\n", nuevaReserva->dni_cliente);

				}
			}

			free(nuevaReserva);
			free(cli);
			send(comm_socket, sendBuff, strlen(sendBuff), 0);
		}

    } else if (strcmp(recvBuff, "GET_ROOMS") == 0) {
    	char* listaDeHabitaciones = listarHabitaciones();
		strncpy(sendBuff, listaDeHabitaciones, 511);
		sendBuff[511] = '\0'; // Aseguramos que termine con nulo
		send(comm_socket, sendBuff, strlen(sendBuff), 0);
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
    } else if (strcmp(recvBuff, "2") == 0) {
        // Procesar registro de nuevo usuario
        memset(recvBuff, 0, 512);
        int bytes = recv(comm_socket, recvBuff, 512, 0);
        if (bytes > 0) {
            recvBuff[bytes] = '\0'; // Asegurar terminación
            printf("Datos de usuario recibidos: %s\n", recvBuff);

            // Extraer datos del usuario
            Usuario *nuevoUsuario = (Usuario*) malloc(sizeof(Usuario));
            memset(nuevoUsuario, 0, sizeof(Usuario));

            // Parsear los datos separados por '|'
            char *token;
            char *rest = recvBuff;

            // Nombre
            token = strtok_s(rest, "|", &rest);
            if (token != NULL) strcpy(nuevoUsuario->nombre, token);

            // Rol
            token = strtok_s(rest, "|", &rest);
            if (token != NULL) strcpy(nuevoUsuario->rol, token);

            // Usuario
            token = strtok_s(rest, "|", &rest);
            if (token != NULL) strcpy(nuevoUsuario->usuario, token);

            // Password
            token = strtok_s(rest, "|", &rest);
            if (token != NULL) strcpy(nuevoUsuario->password, token);

            // Turno
            token = strtok_s(rest, "|", &rest);
            if (token != NULL) strcpy(nuevoUsuario->turno, token);

            // Salario
            token = strtok_s(rest, "|", &rest);
            if (token != NULL) nuevoUsuario->salario = atoi(token);

            printf("Datos extraídos - Nombre: %s, Rol: %s, Usuario: %s, Password: %s, Turno: %s, Salario: %d\n",
                  nuevoUsuario->nombre, nuevoUsuario->rol, nuevoUsuario->usuario,
                  nuevoUsuario->password, nuevoUsuario->turno, nuevoUsuario->salario);

            // Verificar si el usuario ya existe
            if (comprobarUsuario(nuevoUsuario->usuario)) {
                strcpy(sendBuff, "ERROR: El nombre de usuario ya existe");
                printf("Usuario %s ya existe en la BD\n", nuevoUsuario->usuario);
            } else {
                // Crear usuario en la BD
                if (crearUsuarioBD(nuevoUsuario) != 1) {
                    strcpy(sendBuff, "Usuario no registrado correctamente");
                    printf("Usuario %s no a quedado registrado en la BD\n", nuevoUsuario->usuario);
                } else {
                    strcpy(sendBuff, "Usuario registrado correctamente");
                    printf("Usuario %s registrado en la BD\n", nuevoUsuario->usuario);

				}
            }

            free(nuevoUsuario);
            send(comm_socket, sendBuff, strlen(sendBuff), 0);
        }
    } else if (strcmp(recvBuff, "1") == 0) {
        char usu[20] = {0}, con[20] = {0};
        int resul;

        // Get username
        memset(recvBuff, 0, 512);
        int bytes_username = recv(comm_socket, recvBuff, 512, 0);
        if (bytes_username <= 0) {
            strcpy(sendBuff, "Error al recibir el nombre de usuario");
            send(comm_socket, sendBuff, strlen(sendBuff), 0);
            return;
        }

        strncpy(usu, recvBuff, sizeof(usu)-1);
        usu[sizeof(usu)-1] = '\0';

        // Get password
        memset(recvBuff, 0, 512);
        int bytes_password = recv(comm_socket, recvBuff, 512, 0);
        if (bytes_password <= 0) {
            strcpy(sendBuff, "Error al recibir la contraseña");
            send(comm_socket, sendBuff, strlen(sendBuff), 0);
            return;
        }

        strncpy(con, recvBuff, sizeof(con)-1);
        con[sizeof(con)-1] = '\0';

        // Send confirmation
        sprintf(sendBuff, "Servidor: Recibido %s %s", usu, con);
        send(comm_socket, sendBuff, strlen(sendBuff), 0);

        // Properly initialize and use the Usuario structure
        Usuario *u = (Usuario*) malloc(sizeof(Usuario));
        memset(u, 0, sizeof(Usuario)); // Initialize with zeros

        if (recuperarUsuarioBD(usu, u) != 0) {
            if (strcmp(con, u->password) == 0) {
                resul = 0;

                // Preparar para enviar los datos del usuario al cliente después del resultado
                memset(sendBuff, 0, 512);
                sprintf(sendBuff, "%d", resul);
                send(comm_socket, sendBuff, strlen(sendBuff), 0);

                // Enviar los datos del usuario como una cadena delimitada por '|'
                memset(sendBuff, 0, 512);
                sprintf(sendBuff, "%s|%s|%s|%s|%s|%d",
                    u->nombre, u->rol, u->usuario, u->password, u->turno, u->salario);
                send(comm_socket, sendBuff, strlen(sendBuff), 0);
            } else {
                resul = 1;
                // Solo enviar el resultado (error de contraseña)
                memset(sendBuff, 0, 512);
                sprintf(sendBuff, "%d", resul);
                send(comm_socket, sendBuff, strlen(sendBuff), 0);
            }
        } else {
            resul = 2;
            // Solo enviar el resultado (usuario no encontrado)
            memset(sendBuff, 0, 512);
            sprintf(sendBuff, "%d", resul);
            send(comm_socket, sendBuff, strlen(sendBuff), 0);
        }

        free(u);
        printf("Verificación de credenciales completada: %d\n", resul);
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

#include "gestorClientes.h"

void obtenrClientes(SOCKET comm_socket, char *recvBuff, char *sendBuff){
	char* listaDeClientes = listarClientes();
	strncpy(sendBuff, listaDeClientes, 511);
	sendBuff[511] = '\0'; // Aseguramos que termine con nulo
	send(comm_socket, sendBuff, strlen(sendBuff), 0);
}

void crearCliente(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
	// Informar al cliente que estamos listos para recibir datos
	strcpy(sendBuff, "SOLICITAR_DATOS");
	send(comm_socket, sendBuff, strlen(sendBuff), 0);

	// Recibir los datos del cliente a crear
	memset(recvBuff, 0, 512);
	int bytes = recv(comm_socket, recvBuff, 512, 0);
	if (bytes > 0) {
		recvBuff[bytes] = '\0'; // Asegurar terminación
		printf("Datos del cliente recibidos: %s\n", recvBuff);

		// Extraer datos del cliente
		Cliente *nuevoCliente = (Cliente*) malloc(sizeof(Cliente));
		memset(nuevoCliente, 0, sizeof(Cliente));

		// Parsear los datos separados por '|'
		char *token;
		char *rest = recvBuff;

		// DNI
		token = strtok_s(rest, "|", &rest);
		if (token != NULL) strcpy(nuevoCliente->dni, token);

		// Nombre
		token = strtok_s(rest, "|", &rest);
		if (token != NULL) strcpy(nuevoCliente->nombre, token);

		// Apellido
		token = strtok_s(rest, "|", &rest);
		if (token != NULL) strcpy(nuevoCliente->apellido, token);

		// Teléfono
		token = strtok_s(rest, "|", &rest);
		if (token != NULL) strcpy(nuevoCliente->telefono, token);

		// Email
		token = strtok_s(rest, "|", &rest);
		if (token != NULL) strcpy(nuevoCliente->email, token);

		printf("Datos extraídos - DNI: %s, Nombre: %s, Apellido: %s, Teléfono: %s, Email: %s\n",
			  nuevoCliente->dni, nuevoCliente->nombre, nuevoCliente->apellido,
			  nuevoCliente->telefono, nuevoCliente->email);

		// Verificar si el cliente ya existe por DNI
		if (comprobarCliente(nuevoCliente->email)) {
			strcpy(sendBuff, "ERROR|El cliente con este email ya existe");
			printf("Cliente con email %s ya existe en la BD\n", nuevoCliente->email);
		} else {
			// Crear cliente en la BD
			if (crearClienteBD(nuevoCliente) != 1) {
				strcpy(sendBuff, "ERROR|Cliente no registrado correctamente");
				printf("Cliente %s no ha quedado registrado en la BD\n", nuevoCliente->dni);
			} else {
				strcpy(sendBuff, "OK|Cliente registrado correctamente");
				printf("Cliente %s registrado en la BD\n", nuevoCliente->dni);
			}
		}

		free(nuevoCliente);
		send(comm_socket, sendBuff, strlen(sendBuff), 0);
	}
}

void modificarCliente(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
	// Solicitar el DNI del cliente a modificar
	memset(recvBuff, 0, 512);
	int bytes = recv(comm_socket, recvBuff, 512, 0);
	if (bytes > 0) {
		recvBuff[bytes] = '\0'; // Asegurar terminación
		char dni[20];
		strncpy(dni, recvBuff, sizeof(dni)-1);
		dni[sizeof(dni)-1] = '\0';

		// Verificar si el cliente existe
		Cliente *c = (Cliente*) malloc(sizeof(Cliente));
		memset(c, 0, sizeof(Cliente));

		if (recuperarClienteBD(dni, c) == 0) {
			strcpy(sendBuff, "ERROR|El cliente no existe en la base de datos");
			printf("Cliente con DNI %s no existe en la BD\n", dni);
			free(c);
		} else {
			// Enviar los datos actuales del cliente al cliente (frontend)
			memset(sendBuff, 0, 512);
			sprintf(sendBuff, "DATOS_CLIENTE|%s|%s|%s|%s|%s",
				c->dni, c->nombre, c->apellido, c->telefono, c->email);
			send(comm_socket, sendBuff, strlen(sendBuff), 0);

			// Recibir los datos actualizados del cliente
			memset(recvBuff, 0, 512);
			bytes = recv(comm_socket, recvBuff, 512, 0);
			if (bytes > 0) {
				recvBuff[bytes] = '\0'; // Asegurar terminación
				printf("Datos actualizados del cliente recibidos: %s\n", recvBuff);

				// Verificar si recibimos el formato esperado
				if (strncmp(recvBuff, "UPDATE_CLIENTE|", 15) == 0) {
					// Parsear los datos separados por '|'
					char *token;
					char *rest = recvBuff + 15; // Saltar el prefijo "UPDATE_CLIENTE|"

					// Nombre
					token = strtok_s(rest, "|", &rest);
					if (token != NULL) strcpy(c->nombre, token);

					// Apellido
					token = strtok_s(rest, "|", &rest);
					if (token != NULL) strcpy(c->apellido, token);

					// Teléfono
					token = strtok_s(rest, "|", &rest);
					if (token != NULL) strcpy(c->telefono, token);

					// Email
					token = strtok_s(rest, "|", &rest);
					if (token != NULL) strcpy(c->email, token);

					// DNI no cambia, es la clave primaria

					// Actualizar el cliente en la BD
					// SOLUCIÓN 1: Asumir que modificarClienteBD ahora devuelve un entero
					if (modificarClienteBD(c) != 1) {
						strcpy(sendBuff, "ERROR|Cliente no modificado correctamente");
						printf("Error al modificar el cliente %s en la BD\n", c->dni);
					} else {
						strcpy(sendBuff, "OK|Cliente modificado correctamente");
						printf("Cliente %s modificado en la BD\n", c->dni);
					}

					/* SOLUCIÓN 2 (alternativa): Si no se puede modificar la función
					modificarClienteBD(c);
					// Asumimos que la modificación fue exitosa si llegamos aquí
					strcpy(sendBuff, "OK|Cliente modificado correctamente");
					printf("Cliente %s modificado en la BD\n", c->dni);
					*/
				} else {
					strcpy(sendBuff, "ERROR|Formato de datos incorrecto");
					printf("Formato de datos incorrecto al modificar el cliente %s\n", c->dni);
				}

				send(comm_socket, sendBuff, strlen(sendBuff), 0);
			}
		}

		free(c);
		send(comm_socket, sendBuff, strlen(sendBuff), 0);
	}
}

void buscarCliente(SOCKET comm_socket, char *recvBuff, char *sendBuff){
}



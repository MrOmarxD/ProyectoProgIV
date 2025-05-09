#include "gestorReservas.h"
#include "gestorClientes.h"
#include "gestorRegistros.h"
#include <stdio.h>

void crearReserva(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
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
}

void modificarReserva(Reserva *r){

	char idUReserva[10];

	printf("\n--- MODEFICAR RESERVA ---\n");
	printf("Ingrese el id de la reserva a modificar: ");
	fflush(stdout);

	while (getchar() != '\n');

	fgets(idUReserva, 10, stdin);
	idUReserva[strcspn(idUReserva, "\n")] = '\0';

	if (!recuperarReservaBD(idUReserva, r)) {
		return;
	}

	printf("Reserva encontrada. Dejar en blanco para no modificar.\n");

	printf("Ingrese nuevo id usuario: ");
		fflush(stdout);
		char nuevoId[20];


		fgets(nuevoId, 20, stdin);
		nuevoId[strcspn(nuevoId, "\n")] = '\0';
		if (strlen(nuevoId) > 0) {
			strcpy(r->dni_cliente, nuevoId);
		}

	printf("Id habitacion actual: %d\n", r->id_habitacion);
	printf("Ingrese nuevo id habitacion: ");
	fflush(stdout);
		char idHaStr[20];
		fgets(idHaStr, 20, stdin);
		idHaStr[strcspn(idHaStr, "\n")] = '\0'; // Eliminar el salto de línea
		sscanf(idHaStr, "%d", &r->id_habitacion);
	printf("fecha entrada actual: %s\n", r->fecha_entrada);
			printf("Ingrese nueva fecha entrada: ");
			fflush(stdout);
			char fE[50];

			fgets(fE, 50, stdin);
			fE[strcspn(fE, "\n")] = '\0';
			if (strlen(fE) > 0) {
				strcpy(r->fecha_entrada, fE);
			}
   printf("fecha salida actual: %s\n", r->fecha_salida);
					printf("Ingrese nueva fecha salida: ");
					fflush(stdout);
					char fS[50];

					fgets(fS, 50, stdin);
					fS[strcspn(fS, "\n")] = '\0';
					if (strlen(fS) > 0) {
						strcpy(r->fecha_salida, fS);
					}

	printf("Estado actual: %s\n", r->estado);
	printf("Elija el nuevo estado de la reserva.\n");
	printf("1. Confirmada\n");
	printf("2. Pendiente\n");
	printf("Seleccione una opcion: ");
	fflush(stdout);
	int opcion;
	if (scanf("%d", &opcion) == 1) {
		while (getchar() != '\n'); // Limpiar el buffer de entrada
		switch(opcion) {
			case 1:
				strcpy(r->estado, "Confirmada");
				break;
			case 2:
				strcpy(r->estado, "Pendiente");
				break;
			default:
				printf("No se modificará el rol.\n");
				fflush(stdout);
				break;
		}
	} else {
		while (getchar() != '\n'); // Limpiar el buffer de entrada
	}

	printf("Ingrese monto en euros €: ");
	fflush(stdout);
	char montoStr[20];
	fgets(montoStr, 20, stdin);
	montoStr[strcspn(montoStr, "\n")] = '\0'; // Eliminar el salto de línea
	sscanf(montoStr, "%d", &r->monto);

	printf("Ingrese nueva observacion: ");
	fflush(stdout);
	char observacionStr[20];


	fgets(observacionStr, 20, stdin);
	observacionStr[strcspn(observacionStr, "\n")] = '\0';
	if (strlen(observacionStr) > 0) {
		strcpy(r->observaciones, observacionStr);
	}

	modificarReservaBD(r);
}
void buscarReservas(Reserva *r){
	char id_usuario[20];

	printf("\n--- BUSCAR RESERVA ---\n");
	printf("Ingrese el dni del cliente que quiera buscar: ");
	fflush(stdout);

	while (getchar() != '\n');

	fgets(id_usuario, 20, stdin);
	id_usuario[strcspn(id_usuario, "\n")] = '\0';

	buscarReservaBD(id_usuario);
}



#include "gestorReservas.h"
#include "gestorRegistros.h"
#include <stdio.h>

void crearReserva(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    Reserva nueva_reserva;
    memset(&nueva_reserva, 0, sizeof(Reserva));

    // Solicitar DNI del cliente
    strcpy(sendBuff, "INPUT|Ingrese DNI del cliente: ");
    send(comm_socket, sendBuff, strlen(sendBuff), 0);
    recv(comm_socket, recvBuff, 512, 0);
    strncpy(nueva_reserva.dni_cliente, recvBuff, strlen(recvBuff) + 1);

    // Solicitar ID de habitación
    strcpy(sendBuff, "INPUT|Ingrese ID de habitación: ");
    send(comm_socket, sendBuff, strlen(sendBuff), 0);
    recv(comm_socket, recvBuff, 512, 0);
    nueva_reserva.id_habitacion = atoi(recvBuff);

    // Solicitar fecha de entrada
    strcpy(sendBuff, "INPUT|Ingrese fecha de entrada (formato YYYY-MM-DD): ");
    send(comm_socket, sendBuff, strlen(sendBuff), 0);
    recv(comm_socket, recvBuff, 512, 0);
    strncpy(nueva_reserva.fecha_entrada, recvBuff, strlen(recvBuff) + 1);

    // Solicitar fecha de salida
    strcpy(sendBuff, "INPUT|Ingrese fecha de salida (formato YYYY-MM-DD): ");
    send(comm_socket, sendBuff, strlen(sendBuff), 0);
    recv(comm_socket, recvBuff, 512, 0);
    strncpy(nueva_reserva.fecha_salida, recvBuff, strlen(recvBuff) + 1);

    // Solicitar estado de la reserva
    strcpy(sendBuff, "INPUT|Ingrese estado de la reserva (confirmada/pendiente/cancelada): ");
    send(comm_socket, sendBuff, strlen(sendBuff), 0);
    recv(comm_socket, recvBuff, 512, 0);
    strncpy(nueva_reserva.estado, recvBuff, strlen(recvBuff) + 1);

    // Solicitar monto
    strcpy(sendBuff, "INPUT|Ingrese monto de la reserva: ");
    send(comm_socket, sendBuff, strlen(sendBuff), 0);
    recv(comm_socket, recvBuff, 512, 0);
    nueva_reserva.monto = atoi(recvBuff);

    // Solicitar observaciones
    strcpy(sendBuff, "INPUT|Ingrese observaciones (opcional): ");
    send(comm_socket, sendBuff, strlen(sendBuff), 0);
    recv(comm_socket, recvBuff, 512, 0);
    strncpy(nueva_reserva.observaciones, recvBuff, strlen(recvBuff) + 1);

    // Guardar la reserva en la base de datos
    crearReservaBD(&nueva_reserva);

    // Informar al cliente del resultado
    sprintf(sendBuff, "INFO|Reserva ID %d creada exitosamente", nueva_reserva.id);

    printf("Reserva creada con ID: %d\n", nueva_reserva.id);
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



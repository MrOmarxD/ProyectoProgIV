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

void modificarReserva(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    // Solicitar el ID de la reserva a modificar
    memset(recvBuff, 0, 512);
    int bytes = recv(comm_socket, recvBuff, 512, 0);
    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación
        int id_reserva = atoi(recvBuff);

        // Verificar si la reserva existe
        Reserva *r = (Reserva*) malloc(sizeof(Reserva));
        memset(r, 0, sizeof(Reserva));

        if (recuperarReservaPorIdBD(id_reserva, r) == 0) {
            strcpy(sendBuff, "ERROR: No existe reserva con ese ID");
            printf("Reserva con ID %d no existe en la BD\n", id_reserva);
            free(r);
        } else {
            // Enviar los datos actuales de la reserva al cliente
            memset(sendBuff, 0, 512);
            sprintf(sendBuff, "%d|%s|%d|%s|%s|%s|%d|%s",
                r->id, r->dni_cliente, r->id_habitacion, r->fecha_entrada,
                r->fecha_salida, r->estado, r->monto, r->observaciones);
            send(comm_socket, sendBuff, strlen(sendBuff), 0);

            // Recibir los datos actualizados de la reserva
            memset(recvBuff, 0, 512);
            bytes = recv(comm_socket, recvBuff, 512, 0);
            if (bytes > 0) {
                recvBuff[bytes] = '\0'; // Asegurar terminación
                printf("Datos actualizados de la reserva recibidos: %s\n", recvBuff);

                // Parsear los datos separados por '|'
                char *token;
                char *rest = recvBuff;

                // ID (no cambia, es la clave primaria)
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) r->id = atoi(token);

                // DNI Cliente
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) strcpy(r->dni_cliente, token);

                // ID Habitación
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) r->id_habitacion = atoi(token);

                // Fecha Entrada
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) strcpy(r->fecha_entrada, token);

                // Fecha Salida
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) strcpy(r->fecha_salida, token);

                // Estado
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) strcpy(r->estado, token);

                // Monto
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) r->monto = atoi(token);

                // Observaciones
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) strcpy(r->observaciones, token);

                // Actualizar la reserva en la BD
                if (modificarReservaBD(r) != 1) {
                    strcpy(sendBuff, "Reserva no modificada correctamente");
                    printf("Error al modificar la reserva con ID %d en la BD\n", r->id);
                } else {
                    strcpy(sendBuff, "Reserva modificada correctamente");
                    printf("Reserva con ID %d modificada en la BD\n", r->id);
                }
            }
            free(r);
        }

        send(comm_socket, sendBuff, strlen(sendBuff), 0);
    }
}

void buscarReserva(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    // Recibir el DNI del cliente cuya reserva se busca
    memset(recvBuff, 0, 512);
    int bytes = recv(comm_socket, recvBuff, 512, 0);
    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación
        printf("Búsqueda de reserva para el cliente con DNI: %s\n", recvBuff);

        // Verificar si la reserva existe
        if (!comprobarReserva(recvBuff)) {
            strcpy(sendBuff, "ERROR: No existe reserva para el cliente con ese DNI");
            printf("Reserva para el cliente %s no existe en la BD\n", recvBuff);
        } else {
            // Buscar la reserva en la BD
            Reserva *r = (Reserva*) malloc(sizeof(Reserva));
            memset(r, 0, sizeof(Reserva));

            if (recuperarReservaPorIdBD(recvBuff, r) == 1) {
                // Formatear los datos de la reserva para enviarlos al cliente
                memset(sendBuff, 0, 512);
                sprintf(sendBuff, "%d|%s|%d|%s|%s|%s|%d|%s",
                    r->id, r->dni_cliente, r->id_habitacion, r->fecha_entrada,
                    r->fecha_salida, r->estado, r->monto, r->observaciones);
                printf("Reserva encontrada para el cliente %s\n", recvBuff);
            } else {
                strcpy(sendBuff, "ERROR: Error al recuperar la reserva");
                printf("Error al recuperar la reserva para el cliente %s\n", recvBuff);
            }
            free(r);
        }

        send(comm_socket, sendBuff, strlen(sendBuff), 0);
        printf("Resultados de búsqueda enviados al cliente\n");
    }
}

void listarReservas(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    printf("Ejecutando listarReservas en el servidor...\n");

    // Inicializar buffer de envío
    memset(sendBuff, 0, sizeof(sendBuff)); // Asegúrate de que sendBuff tenga tamaño adecuado

    // Obtener lista de reservas directamente
    char* listaReservas = listarReservasBD();

    if (listaReservas == NULL || strlen(listaReservas) == 0) {
        printf("No se obtuvieron datos de la base de datos\n");
        strcpy(sendBuff, "No hay datos disponibles.");
    } else {
        printf("Datos obtenidos de la BD (%d bytes)\n", (int)strlen(listaReservas));

        // Copiar al buffer de envío, asegurando no exceder su tamaño
        size_t maxCopy = sizeof(sendBuff) - 1; // Asume que sendBuff es un arreglo
        if (strlen(listaReservas) < maxCopy) {
            strcpy(sendBuff, listaReservas);
        } else {
            strncpy(sendBuff, listaReservas, maxCopy);
            sendBuff[maxCopy] = '\0'; // Asegurar terminación
            printf("Advertencia: Datos truncados al copiar al buffer\n");
        }
    }

    // Enviar datos al cliente
    int sentBytes = send(comm_socket, sendBuff, strlen(sendBuff), 0);

    if (sentBytes == SOCKET_ERROR) {
        printf("Error al enviar datos: %d\n", WSAGetLastError());
    } else {
        printf("Datos enviados correctamente (%d bytes)\n", sentBytes);
    }
}

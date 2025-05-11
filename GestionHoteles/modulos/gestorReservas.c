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
        int idReserva = atoi(recvBuff);

        // Verificar si la reserva existe
        Reserva *r = (Reserva*) malloc(sizeof(Reserva));
        memset(r, 0, sizeof(Reserva));

        if (recuperarReservaBD(idReserva, r) == 0) {
            strcpy(sendBuff, "ERROR: La reserva no existe en la base de datos");
            printf("Reserva con ID %d no existe en la BD\n", idReserva);
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

                // ID (último parámetro para confirmar)
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) r->id = atoi(token);

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

void listarReservas(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    // Obtener la lista de reservas desde la BD
    char* listaDeReservas = listaReservasBD();

    // Buffer más grande para la respuesta
    static char responseBuffer[4096];

    // Copiar la respuesta al buffer local
    strncpy(responseBuffer, listaDeReservas, sizeof(responseBuffer) - 1);
    responseBuffer[sizeof(responseBuffer) - 1] = '\0'; // Garantizar terminación

    // Si la respuesta es demasiado grande para enviarse de una vez,
    // dividirla en partes (chunks)
    const int MAX_SEND_SIZE = 1024;
    int remaining = strlen(responseBuffer);
    int sent = 0;

    while (remaining > 0) {
        int chunk_size = (remaining > MAX_SEND_SIZE) ? MAX_SEND_SIZE : remaining;

        // Enviar un fragmento de la respuesta
        int result = send(comm_socket, responseBuffer + sent, chunk_size, 0);

        if (result == SOCKET_ERROR) {
            printf("Error al enviar datos al cliente: %d\n", WSAGetLastError());
            break;
        }

        sent += result;
        remaining -= result;
    }

    printf("Enviada lista de reservas al cliente (%d bytes)\n", sent);
}


void buscarReservas(SOCKET comm_socket, char *recvBuff, char *sendBuff){
	// Recibir el criterio de búsqueda
	memset(recvBuff, 0, 512);
	int bytes = recv(comm_socket, recvBuff, 512, 0);
	if (bytes > 0) {
		recvBuff[bytes] = '\0'; // Asegurar terminación
		printf("Búsqueda de reservas por criterio: %s\n", recvBuff);

		// Convertir el criterio a entero para buscar por ID
		int id_reserva = atoi(recvBuff);

		// Realizar la búsqueda en la base de datos
		char* resultadoBusqueda = buscarReservaBD(id_reserva);

		// Enviar resultados al cliente
		memset(sendBuff, 0, 512);
		strncpy(sendBuff, resultadoBusqueda, 511);
		sendBuff[511] = '\0'; // Asegurar terminación

		send(comm_socket, sendBuff, strlen(sendBuff), 0);
		printf("Resultados de búsqueda enviados al cliente\n");
	}
}

void eliminarReserva(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    // Recibir id reserva a eliminar
    memset(recvBuff, 0, 512);
    int bytes = recv(comm_socket, recvBuff, 512, 0);
    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación
        int id_reserva = atoi(recvBuff);

        // Verificar si la reserva existe
        if (comprobarReserva(id_reserva) == 0) {
            strcpy(sendBuff, "ERROR: La reserva no existe en la base de datos");
            printf("Reserva con ID %d no existe en la BD\n", id_reserva);
        } else {
            // Eliminar la reserva de la BD
            if (eliminarReservaBD(id_reserva) != 1) {  // Aquí está la corrección principal
                strcpy(sendBuff, "Reserva no eliminada correctamente");
                printf("Error al eliminar la reserva con ID %d de la BD\n", id_reserva);
            } else {
                strcpy(sendBuff, "Reserva eliminada correctamente");  // Corregido el mensaje
                printf("Reserva con ID %d eliminada de la BD\n", id_reserva);
            }
        }

        send(comm_socket, sendBuff, strlen(sendBuff), 0);
    }
}

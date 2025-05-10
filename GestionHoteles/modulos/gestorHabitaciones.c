#include "gestorHabitaciones.h"
#include <stdio.h>
#include <stdlib.h>

void obtenerHabitaciones(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    // Obtener la lista de habitaciones desde la BD
    char* listaDeHabitaciones = listarHabitaciones();

    // Asegurarse de que no exceda el tamaño del buffer
    strncpy(sendBuff, listaDeHabitaciones, 511);
    sendBuff[511] = '\0'; // Garantizar terminación con NULL

    // Enviar la respuesta al cliente
    send(comm_socket, sendBuff, strlen(sendBuff), 0);

    printf("Enviada lista de habitaciones al cliente\n");
}

void crearHabitacion(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    // Procesar registro de nueva habitación
    memset(recvBuff, 0, 512);
    int bytes = recv(comm_socket, recvBuff, 512, 0);
    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación
        printf("Datos de habitación recibidos: %s\n", recvBuff);

        // Extraer datos de la habitación
        Habitacion *nuevaHabitacion = (Habitacion*) malloc(sizeof(Habitacion));
        memset(nuevaHabitacion, 0, sizeof(Habitacion));

        // Parsear los datos separados por '|'
        char *token;
        char *rest = recvBuff;

        // Número
        token = strtok_s(rest, "|", &rest);
        if (token != NULL) strcpy(nuevaHabitacion->numero, token);

        // Tipo
        token = strtok_s(rest, "|", &rest);
        if (token != NULL) strcpy(nuevaHabitacion->tipo, token);

        // Precio
        token = strtok_s(rest, "|", &rest);
        if (token != NULL) nuevaHabitacion->precio = atoi(token);

        // Estado
        token = strtok_s(rest, "|", &rest);
        if (token != NULL) strcpy(nuevaHabitacion->estado, token);

        // Capacidad
        token = strtok_s(rest, "|", &rest);
        if (token != NULL) nuevaHabitacion->capacidad = atoi(token);

        // Descripción
        token = strtok_s(rest, "|", &rest);
        if (token != NULL) strcpy(nuevaHabitacion->descripcion, token);

        printf("Datos extraídos - Número: %s, Tipo: %s, Precio: %d, Estado: %s, Capacidad: %d, Descripción: %s\n",
              nuevaHabitacion->numero, nuevaHabitacion->tipo, nuevaHabitacion->precio,
              nuevaHabitacion->estado, nuevaHabitacion->capacidad, nuevaHabitacion->descripcion);

        // Verificar si la habitación ya existe - Esta función la debes implementar en gestorBD.c
        Habitacion habitacionExistente;
        if (recuperarHabitacionBD(nuevaHabitacion->numero, &habitacionExistente)) {
            strcpy(sendBuff, "ERROR: El número de habitación ya existe");
            printf("Habitación %s ya existe en la BD\n", nuevaHabitacion->numero);
        } else {
            // Crear habitación en la BD
            crearHabitacionBD(nuevaHabitacion);
            strcpy(sendBuff, "Habitación registrada correctamente");
            printf("Habitación %s registrada en la BD\n", nuevaHabitacion->numero);
        }

        free(nuevaHabitacion);
        send(comm_socket, sendBuff, strlen(sendBuff), 0);
    }
}

void modificarHabitacion(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    // Solicitar el número de habitación a modificar
    memset(recvBuff, 0, 512);
    int bytes = recv(comm_socket, recvBuff, 512, 0);
    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación
        char numeroHabitacion[10];
        strncpy(numeroHabitacion, recvBuff, sizeof(numeroHabitacion)-1);
        numeroHabitacion[sizeof(numeroHabitacion)-1] = '\0';

        // Verificar si la habitación existe
        Habitacion *h = (Habitacion*) malloc(sizeof(Habitacion));
        memset(h, 0, sizeof(Habitacion));

        if (recuperarHabitacionBD(numeroHabitacion, h) == 0) {
            strcpy(sendBuff, "ERROR: La habitación no existe en la base de datos");
            printf("Habitación %s no existe en la BD\n", numeroHabitacion);
            free(h);
        } else {
            // Enviar los datos actuales de la habitación al cliente
            memset(sendBuff, 0, 512);
            sprintf(sendBuff, "%s|%s|%d|%s|%d|%s",
                h->numero, h->tipo, h->precio, h->estado, h->capacidad, h->descripcion);
            send(comm_socket, sendBuff, strlen(sendBuff), 0);

            // Recibir los datos actualizados de la habitación
            memset(recvBuff, 0, 512);
            bytes = recv(comm_socket, recvBuff, 512, 0);
            if (bytes > 0) {
                recvBuff[bytes] = '\0'; // Asegurar terminación
                printf("Datos actualizados de la habitación recibidos: %s\n", recvBuff);

                // Parsear los datos separados por '|'
                char *token;
                char *rest = recvBuff;

                // Número (no cambia, es la clave primaria)

                // Tipo
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) strcpy(h->tipo, token);

                // Precio
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) h->precio = atoi(token);

                // Estado
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) strcpy(h->estado, token);

                // Capacidad
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) h->capacidad = atoi(token);

                // Descripción
                token = strtok_s(rest, "|", &rest);
                if (token != NULL) strcpy(h->descripcion, token);

                // Actualizar la habitación en la BD
                modificarHabitacionBD(h);
                strcpy(sendBuff, "Habitación modificada correctamente");
                printf("Habitación %s modificada en la BD\n", h->numero);
            }
            free(h);
        }

        send(comm_socket, sendBuff, strlen(sendBuff), 0);
    }
}

void cambiarEstadoHabitacion(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    // Solicitar el número de habitación para cambiar su estado
    memset(recvBuff, 0, 512);
    int bytes = recv(comm_socket, recvBuff, 512, 0);
    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación
        char numeroHabitacion[10];
        strncpy(numeroHabitacion, recvBuff, sizeof(numeroHabitacion)-1);
        numeroHabitacion[sizeof(numeroHabitacion)-1] = '\0';

        // Verificar si la habitación existe
        Habitacion *h = (Habitacion*) malloc(sizeof(Habitacion));
        memset(h, 0, sizeof(Habitacion));

        if (recuperarHabitacionBD(numeroHabitacion, h) == 0) {
            strcpy(sendBuff, "ERROR: La habitación no existe en la base de datos");
            printf("Habitación %s no existe en la BD\n", numeroHabitacion);
            free(h);
        } else {
            // Enviar los datos actuales de la habitación al cliente (principalmente el estado actual)
            memset(sendBuff, 0, 512);
            sprintf(sendBuff, "%s|%s", h->numero, h->estado);
            send(comm_socket, sendBuff, strlen(sendBuff), 0);

            // Recibir el nuevo estado
            memset(recvBuff, 0, 512);
            bytes = recv(comm_socket, recvBuff, 512, 0);
            if (bytes > 0) {
                recvBuff[bytes] = '\0'; // Asegurar terminación
                printf("Nuevo estado recibido: %s\n", recvBuff);

                // Actualizar el estado
                strcpy(h->estado, recvBuff);

                // Actualizar la habitación en la BD
                modificarHabitacionBD(h);
                strcpy(sendBuff, "Estado de habitación modificado correctamente");
                printf("Estado de habitación %s modificado a %s en la BD\n", h->numero, h->estado);
            }
            free(h);
        }

        send(comm_socket, sendBuff, strlen(sendBuff), 0);
    }
}

void buscarHabitacionBD(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    // Recibir el número de habitación a buscar
    memset(recvBuff, 0, 512);
    int bytes = recv(comm_socket, recvBuff, 512, 0);
    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación
        char numeroHabitacion[10];
        strncpy(numeroHabitacion, recvBuff, sizeof(numeroHabitacion)-1);
        numeroHabitacion[sizeof(numeroHabitacion)-1] = '\0';

        // Verificar si la habitación existe
        Habitacion *h = (Habitacion*) malloc(sizeof(Habitacion));
        memset(h, 0, sizeof(Habitacion));

        if (recuperarHabitacionBD(numeroHabitacion, h) == 0) {
            strcpy(sendBuff, "ERROR: La habitación no existe en la base de datos");
            printf("Habitación %s no existe en la BD\n", numeroHabitacion);
        } else {
            // Enviar la información de la habitación
            memset(sendBuff, 0, 512);
            sprintf(sendBuff, "Número: %s\nTipo: %s\nPrecio: %d €\nEstado: %s\nCapacidad: %d\nDescripción: %s",
                h->numero, h->tipo, h->precio, h->estado, h->capacidad, h->descripcion);
            printf("Información de habitación %s enviada al cliente\n", h->numero);
        }
        free(h);

        send(comm_socket, sendBuff, strlen(sendBuff), 0);
    }
}

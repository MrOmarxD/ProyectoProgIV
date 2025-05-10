#ifndef GESTORHABITACIONES_H_
#define GESTORHABITACIONES_H_

#include "gestorRegistros.h"
#include <winsock2.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char numero[10];
    char tipo[20];
    int precio;
    char estado[15]; // Disponible, Ocupada, Mantenimiento
    int capacidad;
    char descripcion[100];
} Habitacion;

void obtenerHabitaciones(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void crearHabitacion(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void modificarHabitacion(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void cambiarEstadoHabitacion(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void buscarHabitacionBD(SOCKET comm_socket, char *recvBuff, char *sendBuff);

#endif /* MODULOS_GESTORHABITACIONES_H_ */

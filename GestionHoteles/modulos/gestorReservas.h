#ifndef GESTORRESERVAS_H_
#define GESTORRESERVAS_H_

#include "gestorRegistros.h"
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int id;
    char dni_cliente[10];
    int id_habitacion;
    char fecha_entrada[20];
    char fecha_salida[20];
    char estado[20];
    int monto;
    char observaciones[100];

} Reserva;

void crearReserva(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void modificarReserva(SOCKET comm_socket, char *recvBuff, char *sendBuff);
void buscarReserva(SOCKET comm_socket, char *recvBuff, char *sendBuff);

#endif /* MODULOS_GESTORRESERVAS_H_ */

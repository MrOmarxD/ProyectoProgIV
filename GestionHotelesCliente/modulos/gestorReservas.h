#ifndef GESTORRESERVAS_H_
#define GESTORRESERVAS_H_

//#include "gestorRegistros.h"
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

void gestionReservas(SOCKET s);
void modificarReserva(SOCKET s);
void eliminarReservaBD(SOCKET s);
void buscarReservas(SOCKET s);
void listarReserva(SOCKET s);



#endif /* MODULOS_GESTORRESERVAS_H_ */

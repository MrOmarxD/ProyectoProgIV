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

void gestionReservas(int usuario_actual, const char* LOG_FILE);
void crearReserva(SOCKET s);
void modificarReserva(Reserva *r);
void buscarReservas(Reserva *r);
#endif /* MODULOS_GESTORRESERVAS_H_ */

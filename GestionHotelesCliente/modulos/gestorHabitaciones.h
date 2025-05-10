#ifndef GESTORHABITACIONES_H_
#define GESTORHABITACIONES_H_

//#include "gestorRegistros.h"
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

void gestionHabitaciones(SOCKET s);
void crearHabitacion(SOCKET s);
void modificarHabitacion(SOCKET s);
void establecerEstadoHabitacion(SOCKET s);
void listarHabitaciones(SOCKET s);
void buscarHabitacion(SOCKET s);

#endif /* MODULOS_GESTORHABITACIONES_H_ */

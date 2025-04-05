#ifndef GESTORHABITACIONES_H_
#define GESTORHABITACIONES_H_

#include "gestorRegistros.h"
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

void gestionHabitaciones(int usuario_actual, const char* LOG_FILE);

void crearHabitacion(Habitacion *habitacion);
void modificarHabitacion(Habitacion *habitacion);
void establecerEstadoHabitacion(Habitacion *habitacion);
void buscarHabitacion(Habitacion *habitacion);

#endif /* MODULOS_GESTORHABITACIONES_H_ */

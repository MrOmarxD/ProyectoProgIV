#ifndef GESTORHABITACIONES_H_
#define GESTORHABITACIONES_H_
#include <stdbool.h>

typedef struct {
    int id;
    char tipo[20];
    float precio;
    char estado[15]; // Disponible, Ocupada, Mantenimiento
    int capacidad;
} Habitacion;

void gestionHabitaciones(int usuario_actual, char LOG_FILE, bool ejecutar);

#endif /* MODULOS_GESTORHABITACIONES_H_ */

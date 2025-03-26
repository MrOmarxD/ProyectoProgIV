#include "gestorHabitaciones.h"
#include "gestorRegistros.h"
#include <stdio.h>

void gestionHabitaciones(int usuario_actual, char LOG_FILE) {
    int opcion;
    printf("\n--- GESTIÓN DE HABITACIONES ---\n");
    printf("1. Añadir nueva habitación\n");
    printf("2. Modificar habitación\n");
    printf("3. Cambiar estado de habitación\n");
    printf("4. Listar habitaciones\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

    registrarActividad(usuario_actual, "Acceso a gestión de habitaciones", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

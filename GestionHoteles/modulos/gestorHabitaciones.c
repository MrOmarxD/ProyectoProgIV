#include "gestorHabitaciones.h"
#include "gestorRegistros.h"
#include "menuPrincipal.h"
#include <stdio.h>

void gestionHabitaciones(int usuario_actual, char LOG_FILE, bool ejecutar) {
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

    switch (opcion) {
                case 1:
                	printf("Añadir nueva habitación\n");
                	fflush(stdout);
                    break;
                case 2:
                	printf("Modificar habitación\n");
                	fflush(stdout);
                    break;
                case 3:
                	printf("Cambiar estado de habitación\n");
                	fflush(stdout);
                    break;
                case 4:
                	printf("Listar habitaciones\n");
                	fflush(stdout);
                    break;
                case 0:
                	mostrarMenuPrincipal(ejecutar, usuario_actual, LOG_FILE);
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }

    registrarActividad(usuario_actual, "Acceso a gestión de habitaciones", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

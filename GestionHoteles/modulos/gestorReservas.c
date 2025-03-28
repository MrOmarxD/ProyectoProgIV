#include "gestorReservas.h"
#include "gestorRegistros.h"
#include "menuPrincipal.h"
#include <stdio.h>

void gestionReservas(int usuario_actual, char LOG_FILE, bool ejecutar) {
    int opcion;
    printf("\n--- GESTIÓN DE RESERVAS ---\n");
    printf("1. Crear nueva reserva\n");
    printf("2. Modificar reserva\n");
    printf("3. Cancelar reserva\n");
    printf("4. Buscar reservas por cliente\n");
    printf("5. Listar reservas activas\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

    switch (opcion) {
                case 1:
                	printf("Crear nueva reserva\n");
                	fflush(stdout);
                    break;
                case 2:
                	printf("Modificar reserva\n");
                	fflush(stdout);
                    break;
                case 3:
                	printf("Cancelar reserva\n");
                	fflush(stdout);
                    break;
                case 4:
                	printf("Buscar reservas por cliente\n");
                	fflush(stdout);
                    break;
                case 5:
                	printf("Listar reservas activas\n");
                	fflush(stdout);
                    break;
                case 0:
                	mostrarMenuPrincipal(ejecutar, usuario_actual, LOG_FILE);
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }

    registrarActividad(usuario_actual, "Acceso a gestión de reservas", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

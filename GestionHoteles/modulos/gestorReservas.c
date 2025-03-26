#include "gestorReservas.h"
#include "gestorRegistros.h"
#include <stdio.h>

void gestionReservas(int usuario_actual, char LOG_FILE) {
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

    registrarActividad(usuario_actual, "Acceso a gestión de reservas", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

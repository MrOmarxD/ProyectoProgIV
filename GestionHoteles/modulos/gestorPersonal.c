#include "gestorPersonal.h"

void gestionPersonal() {
    int opcion;
    printf("\n--- GESTIÓN DE PERSONAL ---\n");
    printf("1. Contratar nuevo empleado\n");
    printf("2. Modificar datos de empleado\n");
    printf("3. Dar de baja a empleado\n");
    printf("4. Listar personal\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    scanf("%d", &opcion);

    registrarActividad(usuario_actual, "Acceso a gestión de personal");

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

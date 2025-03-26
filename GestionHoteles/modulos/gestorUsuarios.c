#include "gestorUsuarios.h"
#include "gestorRegistros.h"
#include <stdio.h>

void gestionUsuarios(int usuario_actual, char LOG_FILE) {
    int opcion;
    printf("\n--- GESTIÓN DE USUARIOS ---\n");
    printf("1. Crear nuevo usuario\n");
    printf("2. Modificar usuario existente\n");
    printf("3. Eliminar usuario\n");
    printf("4. Listar usuarios\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);


    registrarActividad(usuario_actual, "Acceso a gestión de usuarios", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

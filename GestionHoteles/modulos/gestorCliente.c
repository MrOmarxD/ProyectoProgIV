#include "gestorClientes.h"
#include "gestorRegistros.h"
#include <stdio.h>

void gestionClientes(int usuario_actual, char LOG_FILE) {
    int opcion;
    printf("\n--- GESTIÓN DE CLIENTES ---\n");
    printf("1. Registrar nuevo cliente\n");
    printf("2. Modificar datos de cliente\n");
    printf("3. Buscar cliente\n");
    printf("4. Listar todos los clientes\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

    registrarActividad(usuario_actual, "Acceso a gestión de clientes", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

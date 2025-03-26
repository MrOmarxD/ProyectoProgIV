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

    switch (opcion) {
                case 1:
                	printf("Registrar nuevo cliente\n");
                	fflush(stdout);
                    break;
                case 2:
                	printf("Modificar datos de cliente\n");
                	fflush(stdout);
                    break;
                case 3:
                	printf("Buscar cliente\n");
                	fflush(stdout);
                    break;
                case 4:
                	printf("Listar todos los clientes\n");
                	fflush(stdout);
                    break;
                case 0:
                	main();
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }

    registrarActividad(usuario_actual, "Acceso a gestión de clientes", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

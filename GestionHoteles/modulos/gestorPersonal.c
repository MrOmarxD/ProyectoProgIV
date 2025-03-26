#include "gestorPersonal.h"
#include "gestorRegistros.h"
#include <stdio.h>

void gestionPersonal(int usuario_actual, char LOG_FILE) {
    int opcion;
    printf("\n--- GESTIÓN DE PERSONAL ---\n");
    printf("1. Contratar nuevo empleado\n");
    printf("2. Modificar datos de empleado\n");
    printf("3. Dar de baja a empleado\n");
    printf("4. Listar personal\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

    switch (opcion) {
                case 1:
                	printf("Contratar nuevo empleado\n");
                	fflush(stdout);
                    break;
                case 2:
                	printf("Modificar datos de empleado\n");
                	fflush(stdout);
                    break;
                case 3:
                	printf("Dar de baja a empleado\n");
                	fflush(stdout);
                    break;
                case 4:
                	printf("Listar personal\n");
                	fflush(stdout);
                    break;
                case 0:
                	main();
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }

    registrarActividad(usuario_actual, "Acceso a gestión de personal", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

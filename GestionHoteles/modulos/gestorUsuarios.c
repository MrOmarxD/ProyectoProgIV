#include "gestorUsuarios.h"

void gestionUsuarios(int usuario_actual, const char* LOG_FILE) {
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

    switch (opcion) {
                case 1:
                	printf("Crear nuevo usuario\n");
                	fflush(stdout);
                    break;
                case 2:
                	printf("Modificar usuario existente\n");
                	fflush(stdout);
                    break;
                case 3:
                	printf("Eliminar usuario\n");
                	fflush(stdout);
                    break;
                case 4:
                	listaUsuarios();
                	fflush(stdout);
                    break;
                case 0:
                	mostrarMenuPrincipal(usuario_actual, LOG_FILE);
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }


    registrarActividad(usuario_actual, "Acceso a gestión de usuarios", LOG_FILE);

}

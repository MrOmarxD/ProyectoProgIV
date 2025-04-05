#include "gestorClientes.h"


void gestionClientes(int usuario_actual, const char* LOG_FILE) {
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
                	crearCliente(&usuario_actual);
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
                	mostrarMenuPrincipal(usuario_actual, LOG_FILE);
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }

    registrarActividad(usuario_actual, "Acceso a gestión de clientes", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

void crearCliente(Cliente *client) {
    printf("Ingrese DNI: ");
    fflush(stdout);

    while (getchar() != '\n');

    fgets(client->dni, 50, stdin);
    client->dni[strcspn(client->dni, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Ingrese nombre completo: ");
    fflush(stdout);

    //while (getchar() != '\n');

    fgets(client->nombre, 50, stdin);
    client->nombre[strcspn(client->nombre, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Ingrese apellido: ");
    fflush(stdout);

    //while (getchar() != '\n');

    fgets(client->apellido, 50, stdin);
    client->apellido[strcspn(client->apellido, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Ingrese telefono: ");
    fflush(stdout);

    //while (getchar() != '\n');

    fgets(client->telefono, 50, stdin);
    client->telefono[strcspn(client->telefono, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Ingrese email: ");
    fflush(stdout);

    //while (getchar() != '\n');

    fgets(client->email, 50, stdin);
    client->email[strcspn(client->email, "\n")] = '\0'; // Eliminar el salto de línea




    crearClienteBD(client);
}



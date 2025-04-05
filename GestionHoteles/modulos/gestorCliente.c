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
                	modificarCliente(&usuario_actual);
                	fflush(stdout);
                    break;
                case 3:
                	buscarCliente(&usuario_actual);
                	fflush(stdout);
                    break;
                case 4:
                	listarClientes();
                	fflush(stdout);
                    break;
                case 0:
                	mostrarMenuPrincipal();
                	break;
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }

    registrarActividad(usuario_actual, "Acceso a gestión de clientes", LOG_FILE);
}

void crearCliente(Cliente *client) {
    printf("Ingrese DNI: ");
    fflush(stdout);

    while (getchar() != '\n');

    fgets(client->dni, 50, stdin);
    client->dni[strcspn(client->dni, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Ingrese nombre completo: ");
    fflush(stdout);


    fgets(client->nombre, 50, stdin);
    client->nombre[strcspn(client->nombre, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Ingrese apellido: ");
    fflush(stdout);

    fgets(client->apellido, 50, stdin);
    client->apellido[strcspn(client->apellido, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Ingrese telefono: ");
    fflush(stdout);


    fgets(client->telefono, 50, stdin);
    client->telefono[strcspn(client->telefono, "\n")] = '\0'; // Eliminar el salto de línea

    do {
		printf("Ingrese email: ");
		fflush(stdout);

		//while (getchar() != '\n');

		fgets(client->email, 50, stdin);
		client->email[strcspn(client->email, "\n")] = '\0'; // Eliminar el salto de línea
		if (comprobarUsuario(client->email)) {
			printf("El nombre de usuario ya existe. Por favor, elija otro.\n\n");
			fflush(stdout);
		} else {
			break;
		}
	} while (1);


    crearClienteBD(client);
}

void modificarCliente(Cliente *client) {
    char dni[20];

	printf("\n--- MODEFICAR CLIENTE ---\n");
	printf("Ingrese el dni de cliente a modificar: ");
    fflush(stdout);

    while (getchar() != '\n');

    fgets(dni, 20, stdin);
    dni[strcspn(dni, "\n")] = '\0';

    if (!recuperarClienteBD(dni, client)) {
        return;
    }

    printf("Usuario encontrado. Dejar en blanco para no modificar.\n");

    printf("Nombre actual: %s\n", client->nombre);
    printf("Ingrese nuevo nombre: ");
    fflush(stdout);
    char nuevoNombre[50];

    fgets(nuevoNombre, 50, stdin);
    nuevoNombre[strcspn(nuevoNombre, "\n")] = '\0';
    if (strlen(nuevoNombre) > 0) {
        strcpy(client->nombre, nuevoNombre);
    }

    printf("Apellido actual: %s\n", client->apellido);
	printf("Ingrese nuevo apellido: ");
	fflush(stdout);
	char nuevoApellido[50];

	fgets(nuevoApellido, 50, stdin);
	nuevoApellido[strcspn(nuevoApellido, "\n")] = '\0';
	if (strlen(nuevoApellido) > 0) {
		strcpy(client->apellido, nuevoApellido);
	}

    printf("Telefono actual: %s\n", client->telefono);
	printf("Ingrese nuevo telefono: ");
	fflush(stdout);
	char nuevoTelefono[50];

	fgets(nuevoTelefono, 50, stdin);
	nuevoTelefono[strcspn(nuevoTelefono, "\n")] = '\0';
	if (strlen(nuevoTelefono) > 0) {
		strcpy(client->telefono, nuevoTelefono);
	}

    printf("Email actual: %s\n", client->email);
    do {
		printf("Ingrese nuevo email: ");
		fflush(stdout);
		char nuevoEmail[50];

		fgets(nuevoEmail, 50, stdin);
		nuevoEmail[strcspn(nuevoEmail, "\n")] = '\0';
		if (strlen(nuevoEmail) > 0) {
			strcpy(client->email, nuevoEmail);
		}
		if (comprobarCliente(client->email)) {
			printf("El email ya existe. Por favor, elija otro.\n\n");
			fflush(stdout);
		} else {
			break;
		}
	} while (1);


    modificarClienteBD(client);
}

void buscarCliente(Cliente *client){
	char dni[20];

	printf("\n--- BUSCAR CLIENTE ---\n");
	printf("Ingrese el dni de cliente que quiera buscar: ");
	fflush(stdout);

	while (getchar() != '\n');

	fgets(dni, 20, stdin);
	dni[strcspn(dni, "\n")] = '\0';

	buscarClientesBD(dni);
}



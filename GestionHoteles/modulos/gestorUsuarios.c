#include "gestorUsuarios.h"

void gestionUsuarios(int usuario_actual, const char* LOG_FILE) {
	Usuario usuario;
    int opcion;
    printf("\n--- GESTIÓN DE USUARIOS ---\n");
    printf("1. Crear nuevo usuario\n");
    printf("2. Modificar usuario existente\n");
    printf("3. Eliminar usuario\n");
    printf("4. Listar usuarios\n");
    printf("5. Buscar usuario\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

    switch (opcion) {
                case 1:
                	printf("\n---CREAR NUEVO USUARIO---\n");
                	crearUsuario(&usuario);
                	fflush(stdout);
                    break;
                case 2:
                	modificarUsuario(&usuario);
                	fflush(stdout);
                    break;
                case 3:
                	eliminarUsuarioBD();
                	fflush(stdout);
                    break;
                case 4:
                	listaUsuarios();
                	fflush(stdout);
                    break;
                case 5:
                	buscarUsuario(&usuario);
					fflush(stdout);
					break;
                case 0:
                	mostrarMenuPrincipal();
                	break;
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }


    registrarActividad(usuario_actual, "Acceso a gestión de usuarios", LOG_FILE);
}

void crearUsuario(Usuario *user) {
    printf("Ingrese nombre completo: \n");
    fflush(stdout);

    while (getchar() != '\n');

    fgets(user->nombre, 50, stdin);
    user->nombre[strcspn(user->nombre, "\n")] = '\0'; // Eliminar el salto de línea

    int opcion;
    do {
        printf("Elija el rol del nuevo Usuario\n");
        printf("1. Administrador\n");
        printf("2. Recepcionista\n");
        printf("3. Limpieza\n");
        printf("4. Mantenimiento\n");
        printf("Seleccione una opcion: ");
        fflush(stdout);
        scanf("%d", &opcion);

        // Limpiar el buffer de entrada
        while (getchar() != '\n');

        switch(opcion) {
            case 1:
                printf("\nHa seleccionado: Administrador\n\n");
                fflush(stdout);
                strcpy(user->rol, "Administrador");
                break;
            case 2:
                printf("\nHa seleccionado: Recepcionista\n\n");
                fflush(stdout);
                strcpy(user->rol, "Recepcionista");
                break;
            case 3:
                printf("\nHa seleccionado: Limpieza\n\n");
                fflush(stdout);
                strcpy(user->rol, "Limpieza");
                break;
            case 4:
                printf("\nHa seleccionado: Mantenimiento\n\n");
                fflush(stdout);
                strcpy(user->rol, "Mantenimiento");
                break;
            default:
                printf("\nOpcion no valida. Por favor, intente de nuevo.\n");
                fflush(stdout);
                opcion = 0;
                break;
        }
    } while(opcion == 0);

    int turno;
	do {
		printf("Elija el turno del nuevo Usuario\n");
		printf("1. Mañana\n");
		printf("2. Tarde\n");
		printf("3. Noche\n");
		printf("Seleccione un turno: ");
		fflush(stdout);
		scanf("%d", &turno);

		// Limpiar el buffer de entrada
		while (getchar() != '\n');

		switch(turno) {
			case 1:
				printf("\nHa seleccionado: Mañana\n\n");
				fflush(stdout);
				strcpy(user->turno, "Mañana");
				break;
			case 2:
				printf("\nHa seleccionado: Tarde\n\n");
				fflush(stdout);
				strcpy(user->turno, "Tarde");
				break;
			case 3:
				printf("\nHa seleccionado: Noche\n\n");
				fflush(stdout);
				strcpy(user->turno, "Noche");
				break;
			default:
				printf("\nTurno no valida. Por favor, intente de nuevo.\n");
				fflush(stdout);
				turno = 0;
				break;
			}
      } while(turno == 0);

	printf("Ingrese salario en euros €: ");
	fflush(stdout);
	char salarioStr[20];
	fgets(salarioStr, 20, stdin);
	salarioStr[strcspn(salarioStr, "\n")] = '\0'; // Eliminar el salto de línea
	sscanf(salarioStr, "%d", &user->salario);

	do {
		printf("Ingrese nombre de usuario: ");
		fflush(stdout);

		fgets(user->usuario, 20, stdin);
		user->usuario[strcspn(user->usuario, "\n")] = '\0';

		if (comprobarUsuario(user->usuario)) {
			printf("El nombre de usuario ya existe. Por favor, elija otro.\n\n");
			fflush(stdout);
		} else {
			break;
		}
	} while (1);


    printf("Ingrese contraseña: ");
    fflush(stdout);

    fgets(user->password, 20, stdin);
    user->password[strcspn(user->password, "\n")] = '\0';

    crearUsuarioBD(user);
}

void modificarUsuario(Usuario *user) {
    char nombreUsuario[20];

	printf("\n--- MODEFICAR USUARIO ---\n");
	printf("Ingrese el nombre de usuario a modificar: ");
    fflush(stdout);

    while (getchar() != '\n');

    fgets(nombreUsuario, 20, stdin);
    nombreUsuario[strcspn(nombreUsuario, "\n")] = '\0';

    if (!recuperarUsuarioBD(nombreUsuario, user)) {
        return;
    }

    printf("Usuario encontrado. Dejar en blanco para no modificar.\n");

    printf("Nombre completo actual: %s\n", user->nombre);
    printf("Ingrese nuevo nombre completo: ");
    fflush(stdout);
    char nuevoNombre[50];

    fgets(nuevoNombre, 50, stdin);
    nuevoNombre[strcspn(nuevoNombre, "\n")] = '\0';
    if (strlen(nuevoNombre) > 0) {
        strcpy(user->nombre, nuevoNombre);
    }

    printf("Rol actual: %s\n", user->rol);
    printf("Elija el nuevo rol del Usuario Dejar en blanco para no modificar.\n");
    printf("1. Administrador\n");
    printf("2. Recepcionista\n");
    printf("3. Limpieza\n");
    printf("4. Mantenimiento\n");
    printf("Seleccione una opcion: ");
    fflush(stdout);
    int opcion;
    if (scanf("%d", &opcion) == 1) {
        while (getchar() != '\n'); // Limpiar el buffer de entrada
        switch(opcion) {
            case 1:
                strcpy(user->rol, "Administrador");
                break;
            case 2:
                strcpy(user->rol, "Recepcionista");
                break;
            case 3:
                strcpy(user->rol, "Limpieza");
                break;
            case 4:
                strcpy(user->rol, "Mantenimiento");
                break;
            default:
                printf("No se modificará el rol.\n");
                fflush(stdout);
                break;
        }
    } else {
        while (getchar() != '\n'); // Limpiar el buffer de entrada
    }

    printf("Turno actual: %s\n", user->turno);
	printf("Elija el nuevo turno del Usuario Dejar en blanco para no modificar.\n");
	printf("1. Mañana\n");
	printf("2. Tarde\n");
	printf("3. Noche\n");
	printf("Seleccione una opcion: ");
	fflush(stdout);
	int turno;
	if (scanf("%d", &turno) == 1) {
		while (getchar() != '\n'); // Limpiar el buffer de entrada
		switch(turno) {
			case 1:
				strcpy(user->turno, "Mañana");
				break;
			case 2:
				strcpy(user->turno, "Tarde");
				break;
			case 3:
				strcpy(user->turno, "Noche");
				break;
			default:
				printf("No se modificará el turno.\n");
				fflush(stdout);
				break;
		}
	} else {
		while (getchar() != '\n'); // Limpiar el buffer de entrada
	}

	printf("Ingrese nuevo salario en euros €: ");
	fflush(stdout);
	char salarioStr[20];
	fgets(salarioStr, 20, stdin);
	salarioStr[strcspn(salarioStr, "\n")] = '\0'; // Eliminar el salto de línea
	sscanf(salarioStr, "%d", &user->salario);

    printf("Ingrese nueva contraseña: ");
    fflush(stdout);
    char nuevaPassword[20];


    fgets(nuevaPassword, 20, stdin);
    nuevaPassword[strcspn(nuevaPassword, "\n")] = '\0';
    if (strlen(nuevaPassword) > 0) {
        strcpy(user->password, nuevaPassword);
    }

    modificarUsuarioBD(user);
}

void buscarUsuario(Usuario *usuario){
	char nombre_usuario[20];

	printf("\n--- BUSCAR USUARIO ---\n");
	printf("Ingrese el nombre de usuario que quiera buscar: ");
	fflush(stdout);

	while (getchar() != '\n');

	fgets(nombre_usuario, 20, stdin);
	nombre_usuario[strcspn(nombre_usuario, "\n")] = '\0';

	buscarUsuarioBD(nombre_usuario);
}


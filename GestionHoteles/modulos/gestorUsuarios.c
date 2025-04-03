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
                	crearUsuario(&usuario_actual);
                	fflush(stdout);
                    break;
                case 2:
                	printf("Modificar usuario existente\n");
                	//modificarUsuario(&usuario_actual);
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

void crearUsuario(Usuario *user) {
    printf("Ingrese nombre completo: \n");

	fflush(stdout);
    fgets(user->nombre, 50, stdin);
    user->nombre[strcspn(user->nombre, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Elija el rol del nuevo Uusario\n");
	printf("1. Administrador\n");
	printf("2. Recepcionista\n");
	printf("3. Limpieza\n");
	printf("4. Mantenimiento\n");
	printf("Seleccione una opcion: ");
	fflush(stdout);
	int opcion;
	do{
		scanf("%d", &opcion);
		switch(opcion) {
			case 1:
				printf("\nHa seleccionado: Administrador\n");
				fflush(stdout);
				strcpy(user->rol, "Administrador");
				break;
			case 2:
				printf("\nHa seleccionado: Recepcionista\n");
				fflush(stdout);
				strcpy(user->rol, "Recepcionista");
				break;
			case 3:
				printf("\nHa seleccionado: Limpieza\n");
				fflush(stdout);
				strcpy(user->rol, "Limpieza");
				break;
			case 4:
				printf("\nHa seleccionado: Mantenimiento\n");
				fflush(stdout);
				strcpy(user->rol, "Mantenimiento");
				break;
			default:
				printf("\nOpcion no valida. Por favor, intente de nuevo.\n");
				fflush(stdout);
				opcion = 0;
				break;
		}
	}while(opcion == 0);

    printf("Ingrese nombre de usuario: ");
	fflush(stdout);
    fgets(user->usuario, 20, stdin);
    user->usuario[strcspn(user->usuario, "\n")] = '\0';

    printf("Ingrese contraseña: ");
	fflush(stdout);
    fgets(user->password, 20, stdin);
    user->password[strcspn(user->password, "\n")] = '\0';

    printf("\nUsuario creado exitosamente!\n");
	fflush(stdout);
}

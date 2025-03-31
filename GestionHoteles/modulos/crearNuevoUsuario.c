#include "crearNuevoUsuario.h"

void crearUsuario(Usuario *user) {
    printf("Ingrese ID del usuario: ");
    scanf("%d", &user->id);
    getchar(); // Limpiar el buffer de entrada

    printf("Ingrese nombre completo: ");
    fgets(user->nombre, 50, stdin);
    user->nombre[strcspn(user->nombre, "\n")] = '\0'; // Eliminar el salto de línea

    printf("Ingrese rol del usuario: ");
    fgets(user->rol, 20, stdin);
    user->rol[strcspn(user->rol, "\n")] = '\0';

    printf("Ingrese nombre de usuario: ");
    fgets(user->usuario, 20, stdin);
    user->usuario[strcspn(user->usuario, "\n")] = '\0';

    printf("Ingrese contraseña: ");
    fgets(user->password, 20, stdin);
    user->password[strcspn(user->password, "\n")] = '\0';

    printf("\nUsuario creado exitosamente!\n");
}

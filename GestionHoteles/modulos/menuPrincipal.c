#include "menuPrincipal.h"
#include "gestorRegistros.h"
#include <stdio.h>

void mostrarMenuPrincipal(bool ejecutar, int usuario_actual, char LOG_FILE) {
	int opcion;
    printf("\n=============================================================\n");
    printf("               MENÚ PRINCIPAL\n");
    printf("=============================================================\n");
    printf("1. Gestión de Usuarios\n");
    printf("2. Gestión de Clientes\n");
    printf("3. Gestión de Personal\n");
    printf("4. Gestión de Habitaciones\n");
    printf("5. Gestión de Reservas\n");
    printf("6. Facturación\n");
    printf("7. Ver Registros de Actividad\n");
    printf("8. Configuración del Sistema\n");
    printf("0. Salir\n");
    printf("=============================================================\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

            switch (opcion) {
                case 1:
                    gestionUsuarios(usuario_actual, LOG_FILE);
                    break;
                case 2:
                    gestionClientes(usuario_actual, LOG_FILE);
                    break;
                case 3:
                    gestionPersonal(usuario_actual, LOG_FILE);
                    break;
                case 4:
                    gestionHabitaciones(usuario_actual, LOG_FILE);
                    break;
                case 5:
                    gestionReservas(usuario_actual, LOG_FILE);
                    break;
                case 6:
                    gestionFacturacion(usuario_actual, LOG_FILE);
                    break;
                case 7:
                    verRegistrosActividad(usuario_actual, LOG_FILE);
                    break;
                case 8:
                    configuracionSistema();
                    break;
                case 0:
                    printf("Cerrando sesión y saliendo del sistema...\n");
                    fflush(stdout);
                    registrarActividad(usuario_actual, "Cierre de sesión", LOG_FILE);
                    ejecutar = false;
                    break;
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }
}

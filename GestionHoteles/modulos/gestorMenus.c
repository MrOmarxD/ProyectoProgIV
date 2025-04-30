#include "gestorMenus.h"

/* Variables globales */
char CONFIG_FILE[100] = "config.dat";
const char* LOG_FILE = "actividad.log";
int usuario_actual = 0; // ID del usuario que ha iniciado sesión

int mostrarMenuPrincipal() {
	int opcion;
    printf("\n=============================================================\n");
    printf("               MENÚ PRINCIPAL\n");
    printf("=============================================================\n");
    printf("1. Gestión de Usuarios\n");
    printf("2. Gestión de Clientes\n");
    printf("3. Gestión de Habitaciones\n");
    printf("4. Gestión de Reservas\n");
    printf("5. Facturación\n");
    printf("6. Ver Registros de Actividad\n");
    printf("7. Configuración del Sistema\n");
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
                    //gestionClientes(usuario_actual, LOG_FILE);
                    break;
                case 3:
                    gestionHabitaciones(usuario_actual, LOG_FILE);
                    break;
                case 4:
                    //gestionReservas(usuario_actual, LOG_FILE);
                    break;
                case 5:
                    gestionFacturacion(usuario_actual, LOG_FILE);
                    break;
                case 6:
                    verRegistrosActividad(usuario_actual, LOG_FILE);
                    break;
                case 7:
                    configuracionSistema();
                    break;
                case 0:
                    printf("Cerrando sesión y saliendo del sistema...\n");
                    fflush(stdout);
                    registrarActividad(usuario_actual, "Cierre de sesión", LOG_FILE);
                    return 0;
                    break;
            }
            return 1;
}

void eleccionInicial(){
	int respuesta = 99;
    int ejecutar = 1;

    printf("\n=============================================================\n");
    printf("      SISTEMA DE GESTIÓN DE HOTELES - GRUPO 12\n");
    printf("=============================================================\n");
    fflush(stdout);

    do{
		printf("Si eres usuario pulsa el 1, si eres administrador pulsa 2:\n");
		fflush(stdout);
		scanf("%i",&respuesta);

		switch (respuesta) {
			case 1:
				while (ejecutar != 0) {
					ejecutar = mostrarMenuPrincipalCliente();
				}
				break;
			case 2:
				while (ejecutar != 0) {
					ejecutar = mostrarMenuPrincipal();
				}
				break;
			default:
				printf("Opción no válida. Intente nuevamente.\n");
				respuesta = 0;
		}
    }while(respuesta == 0);
}

/* Implementación básica de las funciones de gestión */
void configuracionSistema() {
    int opcion;
    printf("\n--- CONFIGURACIÓN DEL SISTEMA ---\n");
    printf("1. Cambiar rutas de archivos\n");
    printf("2. Configurar parámetros de conexión\n");
    printf("3. Hacer copia de seguridad\n");
    printf("4. Restaurar desde copia de seguridad\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

    switch (opcion) {
                case 1:
                	printf("Cambiar rutas de archivos\n");
                	fflush(stdout);
                    break;
                case 2:
                	printf("Configurar parámetros de conexión\n");
                	fflush(stdout);
                    break;
                case 3:
                	printf("Hacer copia de seguridad\n");
                	fflush(stdout);
                    break;
                case 4:
                	printf("Restaurar desde copia de seguridad\n");
                	fflush(stdout);
                    break;
                case 0:
                	mostrarMenuPrincipal();
                	break;
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }

    registrarActividad(usuario_actual, "Acceso a configuración del sistema", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
    fflush(stdout);
}

int mostrarMenuPrincipalCliente(){
	printf("En Proceso...");
	return 0;
}

#include <stdio.h>
#include "sqlite3.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "modulos/gestorUsuarios.h"
#include "modulos/gestorClientes.h"
#include "modulos/gestorPersonal.h"
#include "modulos/gestorHabitaciones.h"
#include "modulos/gestorReservas.h"
#include "modulos/gestorFacturas.h"
#include "modulos/gestorRegistros.h"

/* Prototipos de funciones */
void mostrarMenuPrincipal();
void configuracionSistema();

/* Funciones para manejo de archivos */

/* Variables globales */
char CONFIG_FILE[100] = "config.dat";
char LOG_FILE[100] = "actividad.log";
int usuario_actual = 0; // ID del usuario que ha iniciado sesión

int main() {
    sqlite3 *db;

    int result = sqlite3_open("hoteles.sqlite", &db);
    if (result != SQLITE_OK) {
        printf("Error al abrir la BBDD\n");
        fflush(stdout);
        return result;
    }

    printf("BBDD abierta\n");
    fflush(stdout);

    result = sqlite3_close(db);
    if (result != SQLITE_OK) {
        printf("Error al cerrar la BBDD\n");
        printf("%s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return result;
    }

    printf("BBDD cerrada\n");
    fflush(stdout);

    int opcion;
    bool ejecutar = true;

    printf("\n=============================================================\n");
    printf("      SISTEMA DE GESTIÓN DE HOTELES - GRUPO 12\n");
    printf("=============================================================\n");
    fflush(stdout);

    // Verificar archivos de configuración...

    // Simulación de inicio de sesión...

    // Bucle principal del programa...
    while (ejecutar) {
        mostrarMenuPrincipal();
        printf("Seleccione una opción: ");
        fflush(stdout);
        if (scanf("%d", &opcion) != 1) {
                    while (getchar() != '\n'); // Limpiar el búfer de entrada
                    printf("Entrada no válida. Intente nuevamente.\n");
                    fflush(stdout);
                    continue;
                }

        switch (opcion) {
            case 1:
                gestionUsuarios();
                break;
            case 2:
                gestionClientes();
                break;
            case 3:
                gestionPersonal();
                break;
            case 4:
                gestionHabitaciones();
                break;
            case 5:
                gestionReservas();
                break;
            case 6:
                gestionFacturacion();
                break;
            case 7:
                verRegistrosActividad();
                break;
            case 8:
                configuracionSistema();
                break;
            case 0:
                printf("Cerrando sesión y saliendo del sistema...\n");
                registrarActividad(usuario_actual, "Cierre de sesión");
                ejecutar = false;
                break;
            default:
                printf("Opción no válida. Intente nuevamente.\n");
                fflush(stdout);
        }
    }

    return 0;
}


void mostrarMenuPrincipal() {
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
    fflush(stdout);
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

    registrarActividad(usuario_actual, "Acceso a configuración del sistema");

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
    fflush(stdout);
}

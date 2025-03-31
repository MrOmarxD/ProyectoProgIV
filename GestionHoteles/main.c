#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bd/gestionBD.h"
#include "modulos/gestorUsuarios.h"
//#include "modulos/gestorClientes.h"
//#include "modulos/gestorPersonal.h"
#include "modulos/gestorHabitaciones.h"
#include "modulos/gestorReservas.h"
#include "modulos/gestorFacturas.h"
#include "modulos/gestorRegistros.h"
#include "modulos/menuPrincipal.h"
#include "gestorUsuarios.h"
#include "gestorClientes.h"

/* Prototipos de funciones */

void configuracionSistema();

/* Funciones para manejo de archivos */

/* Variables globales */
char CONFIG_FILE[100] = "config.dat";
const char* LOG_FILE = "actividad.log";
int usuario_actual = 0; // ID del usuario que ha iniciado sesión

int main() {

	abrirBd();

	int respuesta = 99;
    int ejecutar = 1;

    printf("\n=============================================================\n");
    printf("      SISTEMA DE GESTIÓN DE HOTELES - GRUPO 12\n");
    printf("=============================================================\n");
    fflush(stdout);
    printf("Si eres usuario pulsa el 1, si eres administrador pulsa 2 y si no estas registrado pulsa 0:\n");
    scanf("%i",&respuesta);
    if(respuesta == 1){
    // Verificar archivos de configuración...

    // Simulación de inicio de sesión...

    // Bucle principal del programa...
    while (ejecutar != 0) {
       /* if (scanf("%d", &opcion) != 1) {
                            while (getchar() != '\n'); // Limpiar el búfer de entrada
                            printf("Entrada no válida. Intente nuevamente.\n");
                            fflush(stdout);
                            continue;
                        }*/
        ejecutar = gestionClientes(usuario_actual, LOG_FILE);

    }
    cerrarBd();
    return 0;}
    else if(respuesta == 2){
    while (ejecutar != 0) {
           /* if (scanf("%d", &opcion) != 1) {
                                while (getchar() != '\n'); // Limpiar el búfer de entrada
                                printf("Entrada no válida. Intente nuevamente.\n");
                                fflush(stdout);
                                continue;
                            }*/
            ejecutar = gestionUsuarios(usuario_actual, LOG_FILE);

        }
        cerrarBd();
        return 0;}
    else{ while (ejecutar != 0) {
        /* if (scanf("%d", &opcion) != 1) {
                             while (getchar() != '\n'); // Limpiar el búfer de entrada
                             printf("Entrada no válida. Intente nuevamente.\n");
                             fflush(stdout);
                             continue;
                         }*/
         ejecutar = mostrarMenuPrincipal(usuario_actual, LOG_FILE);

     }
     cerrarBd();
     return 0;}
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
                	main();
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }

    registrarActividad(usuario_actual, "Acceso a configuración del sistema", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
    fflush(stdout);
}

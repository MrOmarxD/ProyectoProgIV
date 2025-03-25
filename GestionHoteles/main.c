#include <stdio.h>
#include "sqlite3.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct {
    int id;
    char nombre[50];
    char rol[20];
    char usuario[20];
    char password[20];
} Usuario;

typedef struct {
    char dni[10];
    char nombre[50];
    char apellidos[50];
    char telefono[15];
    char email[50];
} Cliente;

typedef struct {
    int id;
    char nombre[50];
    char cargo[30];
    char turno[20];
    float salario;
} Personal;

typedef struct {
    int id;
    char tipo[20];
    float precio;
    char estado[15]; // Disponible, Ocupada, Mantenimiento
    int capacidad;
} Habitacion;

typedef struct {
    int id;
    char dni_cliente[10];
    int id_habitacion;
    char fecha_entrada[11];
    char fecha_salida[11];
    float monto;
} Reserva;

typedef struct {
    int id;
    int id_reserva;
    char dni_cliente[10];
    float monto;
    char metodo_pago[20];
    char fecha[11];
} Factura;

typedef struct {
    int id;
    int id_usuario;
    char fecha[20];
    char operacion[100];
} Registro;

/* Prototipos de funciones */
void mostrarMenuPrincipal();
void gestionUsuarios();
void gestionClientes();
void gestionPersonal();
void gestionHabitaciones();
void gestionReservas();
void gestionFacturacion();
void verRegistrosActividad();
void configuracionSistema();

/* Funciones para manejo de archivos */
bool guardarDatos(const char* archivo, void* datos, size_t tamano, int cantidad);
bool cargarDatos(const char* archivo, void* datos, size_t tamano, int* cantidad);
void registrarActividad(int id_usuario, const char* operacion);

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

void gestionUsuarios() {
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


    registrarActividad(usuario_actual, "Acceso a gestión de usuarios");

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
    fflush(stdout);
}

void gestionClientes() {
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

    registrarActividad(usuario_actual, "Acceso a gestión de clientes");

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
    fflush(stdout);
}

void gestionPersonal() {
    int opcion;
    printf("\n--- GESTIÓN DE PERSONAL ---\n");
    printf("1. Contratar nuevo empleado\n");
    printf("2. Modificar datos de empleado\n");
    printf("3. Dar de baja a empleado\n");
    printf("4. Listar personal\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

    registrarActividad(usuario_actual, "Acceso a gestión de personal");

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
    fflush(stdout);
}

void gestionHabitaciones() {
    int opcion;
    printf("\n--- GESTIÓN DE HABITACIONES ---\n");
    printf("1. Añadir nueva habitación\n");
    printf("2. Modificar habitación\n");
    printf("3. Cambiar estado de habitación\n");
    printf("4. Listar habitaciones\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

    registrarActividad(usuario_actual, "Acceso a gestión de habitaciones");

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
    fflush(stdout);
}

void gestionReservas() {
    int opcion;
    printf("\n--- GESTIÓN DE RESERVAS ---\n");
    printf("1. Crear nueva reserva\n");
    printf("2. Modificar reserva\n");
    printf("3. Cancelar reserva\n");
    printf("4. Buscar reservas por cliente\n");
    printf("5. Listar reservas activas\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

    registrarActividad(usuario_actual, "Acceso a gestión de reservas");

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
    fflush(stdout);
}

void gestionFacturacion() {
    int opcion;
    printf("\n--- FACTURACIÓN ---\n");
    printf("1. Generar nueva factura\n");
    printf("2. Buscar factura\n");
    printf("3. Listar facturas por fecha\n");
    printf("4. Generar informe de facturación\n");
    printf("0. Volver al menú principal\n");
    printf("Seleccione una opción: ");
    fflush(stdout);
    scanf("%d", &opcion);

    registrarActividad(usuario_actual, "Acceso a facturación");

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
    fflush(stdout);
}

void verRegistrosActividad() {
    printf("\n--- REGISTROS DE ACTIVIDAD ---\n");
    fflush(stdout);

    FILE* log = fopen(LOG_FILE, "r");
    if (log == NULL) {
        printf("No hay registros de actividad disponibles.\n");
        return;
    }

    char linea[200];
    printf("Últimas actividades registradas:\n");
    printf("----------------------------------------------------------\n");

    while (fgets(linea, sizeof(linea), log) != NULL) {
        printf("%s", linea);
    }

    fclose(log);
    printf("----------------------------------------------------------\n");
    printf("Presione Enter para continuar...");
    fflush(stdout);
    getchar(); // Capturar el enter anterior
    getchar(); // Esperar a que el usuario presione Enter
}

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

/* Implementación básica de las funciones de archivo */

bool guardarDatos(const char* archivo, void* datos, size_t tamano, int cantidad) {
    FILE* file = fopen(archivo, "wb");
    if (file == NULL) {
        return false;
    }

    size_t escritos = fwrite(datos, tamano, cantidad, file);
    fclose(file);

    return escritos == cantidad;
}

bool cargarDatos(const char* archivo, void* datos, size_t tamano, int* cantidad) {
    FILE* file = fopen(archivo, "rb");
    if (file == NULL) {
        *cantidad = 0;
        return false;
    }

    *cantidad = fread(datos, tamano, 100, file); // Suponemos un máximo de 100 registros
    fclose(file);

    return true;
}

void registrarActividad(int id_usuario, const char* operacion) {
    FILE* log = fopen(LOG_FILE, "a");
    if (log == NULL) {
        printf("Error al registrar actividad.\n");
        return;
    }

    /* Obtener fecha y hora actual */



    fclose(log);
}


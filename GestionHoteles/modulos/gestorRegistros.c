#include "gestorRegistros.h"
#include <stdio.h>

void verRegistrosActividad(int usuario_actual, char LOG_FILE) {
    printf("\n--- REGISTROS DE ACTIVIDAD ---\n");

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

void registrarActividad(int id_usuario, const char* operacion, char LOG_FILE) {
    FILE* log = fopen(LOG_FILE, "a");
    if (log == NULL) {
        printf("Error al registrar actividad.\n");
        return;
    }

    /* Obtener fecha y hora actual */

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

    fclose(log);
}

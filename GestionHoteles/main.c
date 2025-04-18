#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bd/gestionBD.h"
#include "modulos/gestorUsuarios.h"
#include "modulos/gestorHabitaciones.h"
#include "modulos/gestorReservas.h"
#include "modulos/gestorFacturas.h"
#include "modulos/gestorRegistros.h"
#include "modulos/gestorMenus.h"
int client_main(int argc, char *argv[]);
int server_main(int argc, char *argv[]);

int main() {
    abrirBd();

    int opcion;
    fflush(stdout);
    printf("1. Iniciar sistema de gestión\n");
    printf("2. Iniciar servidor\n");
    printf("3. Iniciar cliente\n");
    printf("Seleccione una opción: ");
    scanf("%d", &opcion);

    switch(opcion) {
        case 1:
            eleccionInicial();
            break;
        case 2:
            server_main(0, NULL);
            break;
        case 3:
            client_main(0, NULL);
            break;
        default:
            printf("Opción no válida\n");
    }

    cerrarBd();
    return 0;
}

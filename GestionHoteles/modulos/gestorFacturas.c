#include "gestorFacturas.h"
#include "gestorRegistros.h"
#include <stdio.h>

void gestionFacturacion(int usuario_actual, char LOG_FILE) {
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

    registrarActividad(usuario_actual, "Acceso a facturación", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

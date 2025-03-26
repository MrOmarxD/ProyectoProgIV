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

    switch (opcion) {
                case 1:
                	printf("Generar nueva factura\n");
                	fflush(stdout);
                    break;
                case 2:
                	printf("Buscar factura\n");
                	fflush(stdout);
                    break;
                case 3:
                	printf("Listar facturas por fecha\n");
                	fflush(stdout);
                    break;
                case 4:
                	printf("Generar informe de facturación\n");
                	fflush(stdout);
                    break;
                case 0:
                	main();
                default:
                    printf("Opción no válida. Intente nuevamente.\n");
                    fflush(stdout);
            }

    registrarActividad(usuario_actual, "Acceso a facturación", LOG_FILE);

    /* Aquí iría la implementación de cada opción */
    printf("Funcionalidad en desarrollo...\n");
}

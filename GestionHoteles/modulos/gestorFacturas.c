#include "gestorFacturas.h"
#include "gestorRegistros.h"
#include "gestorMenus.h"
#include "../bd/gestionBD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

void generarNuevaFactura(int* usuario_actual) {
    Factura nueva_factura;
    Cliente cliente;
    
    // Obtener fecha actual
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(nueva_factura.fecha, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
    
    // Pedir datos de la factura
    printf("\n--- GENERAR NUEVA FACTURA ---\n");
    
    printf("Ingrese DNI del cliente: ");
	fflush(stdout);

	while (getchar() != '\n');

    fgets(nueva_factura.dni_cliente, 10, stdin);
    nueva_factura.dni_cliente[strcspn(nueva_factura.dni_cliente, "\n")] = '\0';
    
    // Verificar si el cliente existe
    if (!recuperarClienteBD(nueva_factura.dni_cliente, &cliente)) {
        printf("Error: El cliente con DNI %s no existe en la base de datos.\n", nueva_factura.dni_cliente);
        fflush(stdout);
        return;
    }

    printf("Ingrese numero de Facturacion: ");
	fflush(stdout);

	while (getchar() != '\n');

	fgets(nueva_factura.numero_Factura, 10, stdin);
	    nueva_factura.numero_Factura[strcspn(nueva_factura.numero_Factura, "\n")] = '\0';

    printf("Ingrese monto: ");
    fflush(stdout);
    scanf("%f", &nueva_factura.monto);
    
    printf("Ingrese id Reserva: ");
	fflush(stdout);
	char idReservaStr[20];
	fgets(idReservaStr, 20, stdin);
	idReservaStr[strcspn(idReservaStr, "\n")] = '\0'; // Eliminar el salto de línea
	sscanf(idReservaStr, "%d", &nueva_factura.id_reserva);
	while (getchar() != '\n');

    int metodo_pago;
    do {
        printf("Elija el metodo de pago\n");
        printf("1. Efectivo\n");
        printf("2. Transferencia\n");
        printf("3. Tarjeta de credito\n");
        printf("Seleccione un metodo de pago: ");
        fflush(stdout);
        scanf("%d", &metodo_pago);

        // Limpiar el buffer de entrada
        while (getchar() != '\n');

        switch(metodo_pago) {
            case 1:
                printf("\nHa seleccionado: Efectivo\n\n");
                fflush(stdout);
                strcpy(nueva_factura.metodo_pago, "Efectivo");
                break;
            case 2:
                printf("\nHa seleccionado: Transferencia\n\n");
                fflush(stdout);
                strcpy(nueva_factura.metodo_pago, "Transferencia");
                break;
            case 3:
                printf("\nHa seleccionado: Tarjeta de credito\n\n");
                fflush(stdout);
                strcpy(nueva_factura.metodo_pago, "Tarjeta de credito");
                break;
            default:
                printf("\nMetodo de pago no valido. Por favor, intente de nuevo.\n");
                fflush(stdout);
                metodo_pago = 0;
                break;
        }
    } while(metodo_pago == 0);
    
    int estado_factura;

    do {
        printf("Elija el estado de la factura\n");
        printf("1. Pendiente\n");
        printf("2. Pagada\n");
        printf("Seleccione un estado: ");
        fflush(stdout);
        scanf("%d", &estado_factura);

        // Limpiar el buffer de entrada
        while (getchar() != '\n');

        switch(estado_factura) {
            case 1:
                printf("\nHa seleccionado: Pendiente\n\n");
                fflush(stdout);
                strcpy(nueva_factura.estado, "Pendiente");
                break;
            case 2:
                printf("\nHa seleccionado: Pagada\n\n");
                fflush(stdout);
                strcpy(nueva_factura.estado, "Pagada");
                break;
            default:
                printf("\nEstado no valido. Por favor, intente de nuevo.\n");
                fflush(stdout);
                estado_factura = 0;
                break;
        }
    } while(estado_factura == 0);

    crearFacturaBD(&nueva_factura);
    
    // Crear archivo de factura
    char nombre_archivo[50];
    sprintf(nombre_archivo, "factura_%d.txt", nueva_factura.numero_Factura);
    
    FILE* archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("Error al crear el archivo de factura.\n");
        fflush(stdout);
        return;
    }
    
    // Escribir datos de la factura en el archivo
    fprintf(archivo, "===================================\n");
    fprintf(archivo, "           FACTURA #%d\n", nueva_factura.id);
    fprintf(archivo, "===================================\n\n");
    fprintf(archivo, "Numero de Factura: %s\n", nueva_factura.numero_Factura);
    fprintf(archivo, "Estado de Factura: %s\n", nueva_factura.estado);
    fprintf(archivo, "ID de Reserva: %d\n", nueva_factura.id_reserva);
    fprintf(archivo, "DNI del Cliente: %s\n", nueva_factura.dni_cliente);
    fprintf(archivo, "Nombre: %s %s\n", cliente.nombre, cliente.apellido);
    fprintf(archivo, "Fecha: %s\n", nueva_factura.fecha);
    fprintf(archivo, "Metodo de Pago: %s\n\n", nueva_factura.metodo_pago);
    fprintf(archivo, "-----------------------------------\n");
    fprintf(archivo, "Monto Total: %.2f €\n", nueva_factura.monto);
    fprintf(archivo, "===================================\n");
    
    fclose(archivo);
    
    printf("\nFactura #%d generada correctamente en el archivo '%s'.\n", nueva_factura.id, nombre_archivo);
    fflush(stdout);
}

void buscarFactura(int* usuario_actual) {
    char *numero_factura = (char *)malloc(20 * sizeof(char));
    if (numero_factura == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        exit(1);
    }

    Factura factura_encontrada;
    Cliente cliente;

    printf("\n--- BUSCAR Factura ---\n");
    printf("Ingrese el numero de factura que quiera buscar: ");
    fflush(stdout);

    while (getchar() != '\n');

    fgets(numero_factura, 20, stdin);
    numero_factura[strcspn(numero_factura, "\n")] = '\0';

    if (buscarFacturaBD(numero_factura, &factura_encontrada)) {
        if (recuperarClienteBD(factura_encontrada.dni_cliente, &cliente)) {
            printf("\n=== FACTURA ENCONTRADA ===\n");
            printf("ID: %d\n", factura_encontrada.id);
            printf("DNI Cliente: %s\n", factura_encontrada.dni_cliente);
            printf("Nombre Cliente: %s %s\n", cliente.nombre, cliente.apellido);
            printf("Monto: %.2f €\n", factura_encontrada.monto);
            printf("Metodo de Pago: %s\n", factura_encontrada.metodo_pago);
            printf("Fecha: %s\n", factura_encontrada.fecha);
            printf("Numero Factura: %s\n", factura_encontrada.numero_Factura);
            printf("Estado: %s\n", factura_encontrada.estado);
            printf("=========================\n");
            fflush(stdout);
        } else {
            printf("Error: No se pudo encontrar informacion del cliente con DNI %s.\n", factura_encontrada.dni_cliente);
            fflush(stdout);
        }
    } else {
        printf("No se encontrado ninguna factura con el numero %s.\n", factura_encontrada.numero_Factura);
        fflush(stdout);
    }
    free(numero_factura);
}

void gestionFacturacion(int usuario_actual, const char* LOG_FILE) {
    int opcion;
    printf("\n--- FACTURACION ---\n");
    printf("1. Generar nueva factura\n");
    printf("2. Buscar factura\n");
    printf("0. Volver al menu principal\n");
    printf("Seleccione una opcion: ");
    fflush(stdout);
    scanf("%d", &opcion);

    switch (opcion) {
    case 1:
        printf("Generar nueva factura\n");
        fflush(stdout);
        generarNuevaFactura(&usuario_actual);
        break;
    case 2:
        buscarFactura(&usuario_actual);
        break;
    case 0:
        mostrarMenuPrincipal();
        break;
    default:
        printf("Opcion no valida. Intente nuevamente.\n");
        fflush(stdout);
    }

    registrarActividad(usuario_actual, "Acceso a facturacion", LOG_FILE);
}

#include "gestorFacturas.h"
#include "gestorRegistros.h"
#include "gestorMenus.h"
#include "gestionBD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Funci�n para generar una nueva factura - Case 1
void generarNuevaFactura(int* usuario_actual) {
    Factura nueva_factura;
    Cliente cliente;
    
    // Abrir la base de datos
    abrirBd();
    
    // Generar ID de factura (siguiente al �ltimo en la BD)
    nueva_factura.id = obtenerUltimoIDFacturaBD() + 1;
    
    // Obtener fecha actual
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(nueva_factura.fecha, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
    
    // Pedir datos de la factura
    printf("\n--- GENERAR NUEVA FACTURA ---\n");
    
    printf("Ingrese ID de reserva: ");
    fflush(stdout);
    scanf("%d", &nueva_factura.id_reserva);
    
    printf("Ingrese DNI del cliente: ");
    fflush(stdout);
    
    while (getchar() != '\n');
    
    fgets(nueva_factura.dni_cliente, 10, stdin);
    nueva_factura.dni_cliente[strcspn(nueva_factura.dni_cliente, "\n")] = '\0';
    
    // Verificar si el cliente existe
    if (!recuperarClienteBD(nueva_factura.dni_cliente, &cliente)) {
        printf("Error: El cliente con DNI %s no existe en la base de datos.\n", nueva_factura.dni_cliente);
        fflush(stdout);
        cerrarBd();
        return;
    }
    
    printf("Ingrese monto: ");
    fflush(stdout);
    scanf("%f", &nueva_factura.monto);
    
    printf("Ingrese metodo de pago: ");
    fflush(stdout);
    
    while (getchar() != '\n');
    
    fgets(nueva_factura.metodo_pago, 20, stdin);
    nueva_factura.metodo_pago[strcspn(nueva_factura.metodo_pago, "\n")] = '\0';
    
    // Guardar la factura en la base de datos
    crearFacturaBD(&nueva_factura);
    
    // Crear archivo de factura
    char nombre_archivo[50];
    sprintf(nombre_archivo, "factura_%d.txt", nueva_factura.id);
    
    FILE* archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("Error al crear el archivo de factura.\n");
        fflush(stdout);
        cerrarBd();
        return;
    }
    
    // Escribir datos de la factura en el archivo
    fprintf(archivo, "===================================\n");
    fprintf(archivo, "           FACTURA #%d\n", nueva_factura.id);
    fprintf(archivo, "===================================\n\n");
    fprintf(archivo, "ID de Factura: %d\n", nueva_factura.id);
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
    
    // Cerrar la base de datos
    cerrarBd();
}

// Funci�n para buscar factura - Case 2
void buscarFactura(int* usuario_actual) {
    int id_factura;
    Factura factura_encontrada;
    Cliente cliente;
    
    printf("\n--- BUSCAR FACTURA ---\n");
    printf("Ingrese el ID de la factura: ");
    fflush(stdout);
    scanf("%d", &id_factura);
    
    // Abrir la base de datos
    abrirBd();
    
    // Buscar la factura en la base de datos
    printf("\nBuscando factura #%d...\n", id_factura);
    fflush(stdout);
    
    if (buscarFacturaBD(id_factura, &factura_encontrada)) {
        // Obtener informaci�n del cliente
        if (recuperarClienteBD(factura_encontrada.dni_cliente, &cliente)) {
            // Mostrar los datos de la factura
            printf("\n=== FACTURA ENCONTRADA ===\n");
            printf("ID: %d\n", factura_encontrada.id);
            printf("ID Reserva: %d\n", factura_encontrada.id_reserva);
            printf("DNI Cliente: %s\n", factura_encontrada.dni_cliente);
            printf("Nombre Cliente: %s %s\n", cliente.nombre, cliente.apellido);
            printf("Monto: %.2f €\n", factura_encontrada.monto);
            printf("Metodo de Pago: %s\n", factura_encontrada.metodo_pago);
            printf("Fecha: %s\n", factura_encontrada.fecha);
            printf("=========================\n");
            fflush(stdout);
            
            // Opci�n para generar archivo
            char opcion;
            printf("\n¿Desea generar un archivo con esta factura? (s/n): ");
            fflush(stdout);
            
            while (getchar() != '\n');
            scanf("%c", &opcion);
            
            if (opcion == 's' || opcion == 'S') {
                char nombre_archivo[50];
                sprintf(nombre_archivo, "factura_%d.txt", factura_encontrada.id);
                
                FILE* archivo = fopen(nombre_archivo, "w");
                if (archivo == NULL) {
                    printf("Error al crear el archivo de factura.\n");
                    fflush(stdout);
                    cerrarBd();
                    return;
                }
                
                // Escribir datos de la factura en el archivo
                fprintf(archivo, "===================================\n");
                fprintf(archivo, "           FACTURA #%d\n", factura_encontrada.id);
                fprintf(archivo, "===================================\n\n");
                fprintf(archivo, "ID de Factura: %d\n", factura_encontrada.id);
                fprintf(archivo, "ID de Reserva: %d\n", factura_encontrada.id_reserva);
                fprintf(archivo, "DNI del Cliente: %s\n", factura_encontrada.dni_cliente);
                fprintf(archivo, "Nombre: %s %s\n", cliente.nombre, cliente.apellido);
                fprintf(archivo, "Fecha: %s\n", factura_encontrada.fecha);
                fprintf(archivo, "Metodo de Pago: %s\n\n", factura_encontrada.metodo_pago);
                fprintf(archivo, "-----------------------------------\n");
                fprintf(archivo, "Monto Total: %.2f €\n", factura_encontrada.monto);
                fprintf(archivo, "===================================\n");
                
                fclose(archivo);
                
                printf("\nFactura guardada en '%s'.\n", nombre_archivo);
                fflush(stdout);
            }
        } else {
            printf("Error: No se pudo encontrar informaci�n del cliente con DNI %s.\n", factura_encontrada.dni_cliente);
            fflush(stdout);
        }
    } else {
        printf("No se encontr� ninguna factura con el ID %d.\n", id_factura);
        fflush(stdout);
    }
    
    // Cerrar la base de datos
    cerrarBd();
}

// Funci�n para listar facturas por fecha - Case 3
void listarFacturasPorFecha(int* usuario_actual) {
    char fecha[11];
    
    printf("\n--- LISTAR FACTURAS POR FECHA ---\n");
    printf("Ingrese la fecha (DD/MM/AAAA): ");
    fflush(stdout);
    
    while (getchar() != '\n');
    
    fgets(fecha, 11, stdin);
    fecha[strcspn(fecha, "\n")] = '\0';
    
    // Abrir la base de datos
    abrirBd();
    
    // Buscar facturas en la base de datos para la fecha especificada
    int count = listarFacturasPorFechaBD(fecha);
    
    if (count > 0) {
        printf("---------------------------------------------------\n");
        printf("Total: %d facturas encontradas\n", count);
    } else {
        printf("---------------------------------------------------\n");
        printf("No se encontraron facturas para el %s\n", fecha);
    }
    fflush(stdout);
    
    // Cerrar la base de datos
    cerrarBd();
}

// Funci�n para generar informe de facturaci�n - Case 4
void generarInformeFacturacion(int* usuario_actual) {
    char fecha_inicio[11], fecha_fin[11];
    
    printf("\n--- GENERAR INFORME DE FACTURACI�N ---\n");
    printf("Fecha de inicio (DD/MM/AAAA): ");
    fflush(stdout);
    
    while (getchar() != '\n');
    
    fgets(fecha_inicio, 11, stdin);
    fecha_inicio[strcspn(fecha_inicio, "\n")] = '\0';
    
    printf("Fecha de fin (DD/MM/AAAA): ");
    fflush(stdout);
    
    fgets(fecha_fin, 11, stdin);
    fecha_fin[strcspn(fecha_fin, "\n")] = '\0';
    
    // Abrir la base de datos
    abrirBd();
    
    // Crear archivo de informe
    char nombre_archivo[50];
    sprintf(nombre_archivo, "informe_%s_%s.txt", fecha_inicio, fecha_fin);
    
    FILE* archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        printf("Error al crear el archivo de informe.\n");
        fflush(stdout);
        cerrarBd();
        return;
    }
    
    // Obtener fecha actual
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char fecha_actual[20];
    sprintf(fecha_actual, "%02d/%02d/%04d %02d:%02d", tm->tm_mday, tm->tm_mon + 1, 
            tm->tm_year + 1900, tm->tm_hour, tm->tm_min);
    
    // Generar informe de facturaci�n
    int total_facturas = generarInformeFacturacionBD(fecha_inicio, fecha_fin);
    
    if (total_facturas > 0) {
        fprintf(archivo, "===================================================\n");
        fprintf(archivo, "              INFORME DE FACTURACI�N\n");
        fprintf(archivo, "                %s - %s\n", fecha_inicio, fecha_fin);
        fprintf(archivo, "===================================================\n\n");
        fprintf(archivo, "Fecha de generacion: %s\n\n", fecha_actual);
        fprintf(archivo, "Total de facturas: %d\n", total_facturas);
        fprintf(archivo, "===================================================\n");
    } else {
        fprintf(archivo, "===================================================\n");
        fprintf(archivo, "              INFORME DE FACTURACI�N\n");
        fprintf(archivo, "                %s - %s\n", fecha_inicio, fecha_fin);
        fprintf(archivo, "===================================================\n\n");
        fprintf(archivo, "Fecha de generacion: %s\n\n", fecha_actual);
        fprintf(archivo, "No se encontraron facturas en el per�odo especificado.\n");
        fprintf(archivo, "===================================================\n");
    }
    
    fclose(archivo);
    
    printf("\nInforme generado correctamente en el archivo '%s'.\n", nombre_archivo);
    fflush(stdout);
    
    // Cerrar la base de datos
    cerrarBd();
}

// Funci�n principal de gesti�n de facturaci�n
void gestionFacturacion(int usuario_actual, const char* LOG_FILE) {
    int opcion;
    printf("\n--- FACTURACION ---\n");
    printf("1. Generar nueva factura\n");
    printf("2. Buscar factura\n");
    printf("3. Listar facturas por fecha\n");
    printf("4. Generar informe de facturacion\n");
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
        printf("Buscar factura\n");
        fflush(stdout);
        buscarFactura(&usuario_actual);
        break;
    case 3:
        printf("Listar facturas por fecha\n");
        fflush(stdout);
        listarFacturasPorFecha(&usuario_actual);
        break;
    case 4:
        printf("Generar informe de facturacion\n");
        fflush(stdout);
        generarInformeFacturacion(&usuario_actual);
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

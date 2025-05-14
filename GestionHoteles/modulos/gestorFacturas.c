#include "gestorFacturas.h"
#include "gestorRegistros.h"

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
	scanf(idReservaStr, "%d", &nueva_factura.id_reserva);
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

void buscarFactura(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    char numero_factura[20];

    // Recibir número de factura desde el cliente
    memset(recvBuff, 0, 512);
    int bytesRecibidos = recv(comm_socket, recvBuff, 511, 0);

    if (bytesRecibidos > 0) {
        recvBuff[bytesRecibidos] = '\0';
        strcpy(numero_factura, recvBuff);

        // Buscar factura en la BD
        Factura factura;
        if (buscarFacturaBD(numero_factura, &factura)) {
            // Recuperar información del cliente
            Cliente cliente;
            if (recuperarClienteBD(factura.dni_cliente, &cliente)) {
                // Formatear datos para enviar al cliente
                sprintf(sendBuff,
                        "%s|%s|%s|%s|%s|%s %s|%s|%s|%.2f",
                        factura.numero_Factura,
                        factura.estado,
                        factura.id_reserva,
                        factura.dni_cliente,
                        cliente.nombre,
                        cliente.apellido,
                        factura.fecha,
                        factura.metodo_pago,
                        factura.monto);
            } else {
                // Si no se encuentra el cliente, enviar datos sin nombre
                sprintf(sendBuff,
                        "%s|%s|%s|%s|NO_ENCONTRADO|NO_ENCONTRADO|%s|%s|%.2f",
                        factura.numero_Factura,
                        factura.estado,
                        factura.id_reserva,
                        factura.dni_cliente,
                        factura.fecha,
                        factura.metodo_pago,
                        factura.monto);
            }
        } else {
            // Factura no encontrada
            strcpy(sendBuff, "NOT_FOUND");
        }
    } else {
        strcpy(sendBuff, "ERROR_RECV");
    }

    // Enviar resultado al cliente
    send(comm_socket, sendBuff, strlen(sendBuff), 0);
}

void crearFactura(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    // Variables para almacenar los datos de la factura
    int id_reserva, monto_total;
    char dni_cliente[10], fecha_emision[20], metodo_pago[20], estado[20], detalles[100];
    char id_reserva_str[20]; // Para almacenar el id_reserva como texto

    // Recibir datos de factura del cliente
    memset(recvBuff, 0, 512);
    int bytesRecibidos = recv(comm_socket, recvBuff, 511, 0);

    if (bytesRecibidos > 0) {
        recvBuff[bytesRecibidos] = '\0'; // Asegurar terminación

        // Parsear los datos recibidos (formato: id_reserva|dni_cliente|fecha_emision|metodo_pago|estado|monto_total|detalles)
        sscanf(recvBuff, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]",
               &id_reserva, dni_cliente, fecha_emision, metodo_pago, estado, &monto_total, detalles);

        // Convertir id_reserva a string para la estructura Factura
        sprintf(id_reserva_str, "%d", id_reserva);

        // Crear una estructura de factura
        Factura nueva_factura;
        nueva_factura.id = 0; // El ID se generará automáticamente en la BD
        strcpy(nueva_factura.id_reserva, id_reserva_str);
        strcpy(nueva_factura.dni_cliente, dni_cliente);
        strcpy(nueva_factura.fecha, fecha_emision);
        strcpy(nueva_factura.metodo_pago, metodo_pago);
        strcpy(nueva_factura.estado, estado);
        nueva_factura.monto = (float)monto_total;

        // Generar un número único para la factura
        time_t t = time(NULL);
        sprintf(nueva_factura.numero_Factura, "F%ld", t);

        // Verificar si el cliente existe
        Cliente cliente;
        if (!recuperarClienteBD(nueva_factura.dni_cliente, &cliente)) {
            strcpy(sendBuff, "ERROR|El cliente con el DNI proporcionado no existe en la base de datos.");
            send(comm_socket, sendBuff, strlen(sendBuff), 0);
            return;
        }

        // Guardar la factura en la base de datos
        crearFacturaBD(&nueva_factura);

        // Enviar los datos necesarios para que el cliente cree el archivo
        sprintf(sendBuff, "OK|%s|%s|%s|%s|%s|%s|%s|%s|%.2f|%s",
                nueva_factura.numero_Factura,
                nueva_factura.estado,
                nueva_factura.id_reserva,
                nueva_factura.dni_cliente,
                cliente.nombre,
                cliente.apellido,
                nueva_factura.fecha,
                nueva_factura.metodo_pago,
                nueva_factura.monto,
                detalles);
    } else {
        strcpy(sendBuff, "ERROR|Error al recibir los datos de la factura.");
    }

    // Enviar respuesta al cliente
    send(comm_socket, sendBuff, strlen(sendBuff), 0);
}

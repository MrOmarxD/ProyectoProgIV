#include "gestorFacturas.h"
//#include "gestorRegistros.h"
#include "gestorMenus.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

using namespace std;

void gestionFacturacion(SOCKET s) {
    int opcion;
    cout<<"\n--- FACTURACION ---\n";
    cout<<"1. Generar nueva factura\n";
    cout<<"2. Buscar factura\n";
    cout<<"0. Volver al menu principal\n";
    cout<<"Seleccione una opcion: "<<endl;
    cin >> opcion;
    
    switch (opcion) {
    case 1:
        cout<<"Generar nueva factura\n"<<endl;
        crearFactura(s);
        break;
    case 2:
    	buscarFactura(s);
    	break;
    case 0:
        mostrarMenuPrincipal(s);
        break;
    default:
        cout<<"Opcion no valida. Intente nuevamente.\n"<<endl;
    }

    //registrarActividad(usuario_actual, "Acceso a facturacion", LOG_FILE);
}

void crearFactura(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];
    string input;

    // Enviar comando al servidor
    strcpy(sendBuff, "CREAR_FACTURA");
    send(s, sendBuff, strlen(sendBuff), 0);

    int id_reserva, monto_total;
    char dni_cliente[10], metodo_pago[20], fecha_emision[20], estado[20], detalles[100];

    // Limpiar el buffer de entrada antes de usar getline
    cin.ignore(1000, '\n');

    cout << "=== REGISTRO DE NUEVA FACTURA ===" << endl;

    cout << "Introduce el ID de la reserva: ";
    cin >> id_reserva;

    // Limpiar buffer después de usar cin >>
    cin.ignore(1000, '\n');

    cout << "Introduce el DNI del cliente: ";
    cin.getline(dni_cliente, 10);

    cout << "Introduce la fecha de emisión (DD/MM/AAAA): ";
    cin.getline(fecha_emision, 20);

    int opcion;
    do {
        cout << "Elija el método de pago\n";
        cout << "1. Efectivo\n";
        cout << "2. Tarjeta de Crédito\n";
        cout << "3. Tarjeta de Débito\n";
        cout << "4. Transferencia\n";
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        switch(opcion) {
            case 1:
                cout << "\nHa seleccionado: Efectivo\n\n";
                strcpy(metodo_pago, "Efectivo");
                break;
            case 2:
                cout << "\nHa seleccionado: Tarjeta de Crédito\n\n";
                strcpy(metodo_pago, "Tarjeta de Crédito");
                break;
            case 3:
                cout << "\nHa seleccionado: Tarjeta de Débito\n\n";
                strcpy(metodo_pago, "Tarjeta de Débito");
                break;
            case 4:
                cout << "\nHa seleccionado: Transferencia\n\n";
                strcpy(metodo_pago, "Transferencia");
                break;
            default:
                cout << "\nOpcion no valida. Por favor, intente de nuevo.\n";
                opcion = 0;
                break;
        }
    } while(opcion == 0);

    // Limpiar buffer después de usar cin >>
    cin.ignore(1000, '\n');

    opcion = 0;
    do {
        cout << "Elija el estado de la factura\n";
        cout << "1. Pagada\n";
        cout << "2. Pendiente\n";
        cout << "3. Cancelada\n";
        cout << "Seleccione una opcion: ";

        cin >> opcion;

        switch(opcion) {
            case 1:
                cout << "\nHa seleccionado: Pagada\n\n";
                strcpy(estado, "Pagada");
                break;
            case 2:
                cout << "\nHa seleccionado: Pendiente\n\n";
                strcpy(estado, "Pendiente");
                break;
            case 3:
                cout << "\nHa seleccionado: Cancelada\n\n";
                strcpy(estado, "Cancelada");
                break;
            default:
                cout << "\nOpcion no valida. Por favor, intente de nuevo.\n";
                opcion = 0;
                break;
        }
    } while(opcion == 0);

    cout << "Introduce el monto total: ";
    cin >> monto_total;

    // Limpiar buffer después de usar cin >>
    cin.ignore(1000, '\n');

    cout << "Introduce detalles adicionales: ";
    cin.getline(detalles, 100);

    // Enviar los datos de la factura al servidor
    memset(sendBuff, 0, sizeof(sendBuff));
    sprintf(sendBuff, "%d|%s|%s|%s|%s|%d|%s", id_reserva, dni_cliente, fecha_emision,
            metodo_pago, estado, monto_total, detalles);
    send(s, sendBuff, strlen(sendBuff), 0);

    // Recibir respuesta del servidor
    memset(recvBuff, 0, sizeof(recvBuff));
    int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

    if (bytes > 0) {
        recvBuff[bytes] = '\0';

        // Verificar el estado de la respuesta
        char status[10];
        sscanf(recvBuff, "%[^|]", status);

        if (strcmp(status, "OK") == 0) {
            // Formato esperado: "OK|numero_factura|estado|id_reserva|dni_cliente|nombre|apellido|fecha|metodo_pago|monto|detalles"
            char numero_factura[20], estado_factura[20], id_reserva_str[20], dni_cliente_factura[10];
            char nombre[50], apellido[50], fecha[20], metodo_pago_factura[20], detalles_factura[100];
            float monto;

            // Usar strtok para extraer cada campo
            char* token = strtok(recvBuff, "|");  // Este es "OK"

            token = strtok(NULL, "|");
            strcpy(numero_factura, token);  // numero_factura

            token = strtok(NULL, "|");
            strcpy(estado_factura, token);  // estado

            token = strtok(NULL, "|");
            strcpy(id_reserva_str, token);  // id_reserva

            token = strtok(NULL, "|");
            strcpy(dni_cliente_factura, token);  // dni_cliente

            token = strtok(NULL, "|");
            strcpy(nombre, token);  // nombre

            token = strtok(NULL, "|");
            strcpy(apellido, token);  // apellido

            token = strtok(NULL, "|");
            strcpy(fecha, token);  // fecha

            token = strtok(NULL, "|");
            strcpy(metodo_pago_factura, token);  // metodo_pago

            token = strtok(NULL, "|");
            monto = atof(token);  // monto

            token = strtok(NULL, "|");
            if (token != NULL) {
                strcpy(detalles_factura, token);  // detalles
            } else {
                strcpy(detalles_factura, "");
            }

            // Crear el archivo de la factura en el cliente
            char nombre_archivo[50];
            sprintf(nombre_archivo, "factura_%s.txt", numero_factura);

            FILE* archivo = fopen(nombre_archivo, "w");
            if (archivo == NULL) {
                cout << "Error al crear el archivo de factura." << endl;
            } else {
                // Escribir datos de la factura en el archivo
                fprintf(archivo, "===================================\n");
                fprintf(archivo, "           FACTURA #%s\n", numero_factura);
                fprintf(archivo, "===================================\n\n");
                fprintf(archivo, "Numero de Factura: %s\n", numero_factura);
                fprintf(archivo, "Estado de Factura: %s\n", estado_factura);
                fprintf(archivo, "ID de Reserva: %s\n", id_reserva_str);
                fprintf(archivo, "DNI del Cliente: %s\n", dni_cliente_factura);
                fprintf(archivo, "Nombre: %s %s\n", nombre, apellido);
                fprintf(archivo, "Fecha: %s\n", fecha);
                fprintf(archivo, "Metodo de Pago: %s\n\n", metodo_pago_factura);
                fprintf(archivo, "Detalles: %s\n\n", detalles_factura);
                fprintf(archivo, "-----------------------------------\n");
                fprintf(archivo, "Monto Total: %.2f €\n", monto);
                fprintf(archivo, "===================================\n");

                fclose(archivo);

                cout << "\nFactura #" << numero_factura << " generada correctamente." << endl;
                cout << "Se ha guardado en '" << nombre_archivo << "'" << endl;
            }
        } else {
            // El formato esperado para errores es "ERROR|mensaje"
            char error_msg[200];
            char* token = strtok(recvBuff, "|");  // Este es "ERROR"
            token = strtok(NULL, "|");
            if (token != NULL) {
                strcpy(error_msg, token);
                cout << error_msg << endl;
            } else {
                cout << "Error desconocido al crear la factura." << endl;
            }
        }

        // Añadir una pausa para que el usuario pueda leer el mensaje
        cout << "Presiona Enter para continuar...";
        cin.get();
    } else if (bytes == 0) {
        cout << "El servidor ha cerrado la conexión" << endl;
    } else {
        cout << "Error al recibir datos: " << WSAGetLastError() << endl;
    }
}
void buscarFactura(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];
    string criterio;

    // Limpiar buffers
    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));

    cout << "\n=== BÚSQUEDA DE FACTURA ===\n";
    cout << "Introduce el número de la factura a buscar: ";
    cin.ignore(1000, '\n'); // Limpiar buffer de entrada
    getline(cin, criterio);

    // Enviar comando al servidor
    strcpy(sendBuff, "BUSCAR_FACTURA");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Pequeña pausa para asegurar que el servidor procese el comando
    Sleep(100);

    // Enviar criterio de búsqueda
    memset(sendBuff, 0, sizeof(sendBuff));
    strcpy(sendBuff, criterio.c_str());
    send(s, sendBuff, strlen(sendBuff), 0);

    // Recibir resultados del servidor
    memset(recvBuff, 0, sizeof(recvBuff));
    int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación

        // Verificar si se encontró la factura
        if (strcmp(recvBuff, "NOT_FOUND") == 0) {
            cout << "\nNo se encontró ninguna factura con el número: " << criterio << endl;
        }
        else if (strcmp(recvBuff, "ERROR_RECV") == 0) {
            cout << "\nError al procesar la solicitud." << endl;
        }
        else {
            // Parsear los datos recibidos (formato: numero_factura|estado|id_reserva|dni_cliente|nombre|apellido|fecha|metodo_pago|monto)
            char numero_factura[20], estado[20], id_reserva[20], dni_cliente[10],
                 nombre[50], apellido[50], fecha[20], metodo_pago[20];
            float monto;

            // Parsear los campos usando strtok
            char *token = strtok(recvBuff, "|");
            strcpy(numero_factura, token);

            token = strtok(NULL, "|");
            strcpy(estado, token);

            token = strtok(NULL, "|");
            strcpy(id_reserva, token);

            token = strtok(NULL, "|");
            strcpy(dni_cliente, token);

            token = strtok(NULL, "|");
            strcpy(nombre, token);

            token = strtok(NULL, "|");
            strcpy(apellido, token);

            token = strtok(NULL, "|");
            strcpy(fecha, token);

            token = strtok(NULL, "|");
            strcpy(metodo_pago, token);

            token = strtok(NULL, "|");
            monto = atof(token);

            // Mostrar información de la factura formateada
            cout << "\nResultados de la búsqueda:" << endl;
            cout << "===================================" << endl;
            cout << "           FACTURA #" << numero_factura << endl;
            cout << "===================================" << endl << endl;
            cout << "Numero de Factura: " << numero_factura << endl;
            cout << "Estado de Factura: " << estado << endl;
            cout << "ID de Reserva: " << id_reserva << endl;
            cout << "DNI del Cliente: " << dni_cliente << endl;

            // Mostrar nombre del cliente si está disponible
            if (strcmp(nombre, "NO_ENCONTRADO") != 0) {
                cout << "Nombre: " << nombre << " " << apellido << endl;
            } else {
                cout << "Nombre: Cliente no encontrado" << endl;
            }

            cout << "Fecha: " << fecha << endl;
            cout << "Metodo de Pago: " << metodo_pago << endl << endl;
            cout << "-----------------------------------" << endl;
            cout << "Monto Total: " << monto << " €" << endl;
            cout << "===================================" << endl;
        }
    } else if (bytes == 0) {
        cout << "El servidor ha cerrado la conexión" << endl;
    } else {
        cout << "Error al recibir datos: " << WSAGetLastError() << endl;
    }

    // Añadir una pausa para que el usuario pueda leer el mensaje
    cout << "\nPresiona Enter para continuar...";
    cin.get();
}

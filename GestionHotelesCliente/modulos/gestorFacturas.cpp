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
    strcpy(sendBuff, "CREAR_FACTURA"); // Asumiendo que "2" es el comando para crear facturas
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
        cout << recvBuff << endl;

        // Añadir una pausa para que el usuario pueda leer el mensaje
        cout << "Presiona Enter para continuar...";
        cin.get();
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
    cout << "Introduce el id de la factura a buscar: ";
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
        cout << "\nResultados de la búsqueda:\n";
        cout << "------------------------\n";
        cout << recvBuff << endl;
    } else if (bytes == 0) {
        cout << "El servidor ha cerrado la conexión" << endl;
    } else {
        cout << "Error al recibir datos: " << WSAGetLastError() << endl;
    }

    // Añadir una pausa para que el usuario pueda leer el mensaje
    cout << "\nPresiona Enter para continuar...";
    cin.get();
}

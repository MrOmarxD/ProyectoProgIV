#include "gestorReservas.h"
#include "gestorMenus.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

void gestionReservas(SOCKET s) {
    int opcion;

    cout<<"\n--- GESTIÓN DE RESERVAS ---\n";
    cout<<"1. Crear nueva reserva\n";
    cout<<"2. Modificar reserva\n";
    cout<<"3. Borrar reserva\n";
    cout<<"4. Buscar reservas por cliente\n";
    cout<<"5. Listar reservas\n";
    cout<<"0. Volver al menú principal\n";
    cout<<"Seleccione una opción: " <<endl;
	cin >> opcion;

    switch (opcion) {
                case 1:
                	crearReserva(s);
					break;
                case 2:
                	modificarReserva(s);
                    break;
                case 3:
                	//eliminarReservaBD(s);
                    break;
                case 4:
                	buscarReserva(s);
                    break;
                case 5:
                	listaReservas(s);
                    break;
                case 0:
                	mostrarMenuPrincipal(s);
                	break;
                default:
                    cout<<"Opción no válida. Intente nuevamente.\n" <<endl;
            }

    //registrarActividad(usuario_actual, "Acceso a gestión de reservas", LOG_FILE);
}

void modificarReserva(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Enviar comando al servidor
    strcpy(sendBuff, "MODIFICAR_RESERVA");
    send(s, sendBuff, strlen(sendBuff), 0);

    int idReserva;
    cout << "\n=== MODIFICAR RESERVA ===\n";
    cout << "Introduce el ID de la reserva que desea modificar: ";
    cin >> idReserva;

    // Limpiar buffer después de usar cin >>
    cin.ignore(1000, '\n');

    // Enviar el ID de la reserva al servidor
    memset(sendBuff, 0, sizeof(sendBuff));
    sprintf(sendBuff, "%d", idReserva);
    send(s, sendBuff, strlen(sendBuff), 0);

    // Recibir respuesta del servidor
    memset(recvBuff, 0, sizeof(recvBuff));
    int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

    if (bytes > 0) {
        recvBuff[bytes] = '\0';

        // Verificar si hubo error (la reserva no existe)
        if (strncmp(recvBuff, "ERROR:", 6) == 0) {
            cout << recvBuff << endl;
            cout << "Presiona Enter para continuar...";
            cin.get();
            return;
        }

        // Parsear los datos recibidos de la reserva
        char dni_cliente[10], fecha_entrada[20], fecha_salida[20], estado[20], observaciones[100];
        int id_habitacion, monto, id;

        // Separar la cadena por '|'
        char *token;
        char *rest = recvBuff;

        // ID
        token = strtok(rest, "|");
        if (token != NULL) id = atoi(token);

        // DNI Cliente
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(dni_cliente, token);

        // ID Habitación
        token = strtok(NULL, "|");
        if (token != NULL) id_habitacion = atoi(token);

        // Fecha Entrada
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(fecha_entrada, token);

        // Fecha Salida
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(fecha_salida, token);

        // Estado
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(estado, token);

        // Monto
        token = strtok(NULL, "|");
        if (token != NULL) monto = atoi(token);

        // Observaciones
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(observaciones, token);

        cout << "\nDatos actuales de la reserva:\n";
        cout << "ID: " << id << " (no se puede modificar)\n";
        cout << "DNI Cliente: " << dni_cliente << endl;
        cout << "ID Habitación: " << id_habitacion << endl;
        cout << "Fecha Entrada: " << fecha_entrada << endl;
        cout << "Fecha Salida: " << fecha_salida << endl;
        cout << "Estado: " << estado << endl;
        cout << "Monto: " << monto << endl;
        cout << "Observaciones: " << observaciones << endl;

        cout << "\nIntroduce los nuevos datos (deja en blanco para mantener el valor actual):\n";

        char nuevoDato[100];

        cout << "Nuevo DNI Cliente [" << dni_cliente << "]: ";
        cin.getline(nuevoDato, 10);
        if (strlen(nuevoDato) > 0) {
            strcpy(dni_cliente, nuevoDato);
        }

        cout << "Nuevo ID Habitación [" << id_habitacion << "]: ";
        cin.getline(nuevoDato, 10);
        if (strlen(nuevoDato) > 0) {
            id_habitacion = atoi(nuevoDato);
        }

        cout << "Nueva Fecha Entrada [" << fecha_entrada << "]: ";
        cin.getline(nuevoDato, 20);
        if (strlen(nuevoDato) > 0) {
            strcpy(fecha_entrada, nuevoDato);
        }

        cout << "Nueva Fecha Salida [" << fecha_salida << "]: ";
        cin.getline(nuevoDato, 20);
        if (strlen(nuevoDato) > 0) {
            strcpy(fecha_salida, nuevoDato);
        }

        int opcion;
        cout << "¿Desea cambiar el estado? (1: Sí, 0: No): ";
        cin >> opcion;

        if (opcion == 1) {
            do {
                cout << "Elija el nuevo estado de la Reserva\n";
                cout << "1. Pendiente\n";
                cout << "2. Confirmada\n";
                cout << "3. Cancelada\n";
                cout << "4. Completada\n";
                cout << "Seleccione una opcion: ";

                cin >> opcion;

                switch(opcion) {
                    case 1:
                        cout << "\nHa seleccionado: Pendiente\n\n";
                        strcpy(estado, "Pendiente");
                        break;
                    case 2:
                        cout << "\nHa seleccionado: Confirmada\n\n";
                        strcpy(estado, "Confirmada");
                        break;
                    case 3:
                        cout << "\nHa seleccionado: Cancelada\n\n";
                        strcpy(estado, "Cancelada");
                        break;
                    case 4:
                        cout << "\nHa seleccionado: Completada\n\n";
                        strcpy(estado, "Completada");
                        break;
                    default:
                        cout << "\nOpcion no valida. Por favor, intente de nuevo.\n";
                        opcion = 0;
                        break;
                }
            } while(opcion == 0);
        }

        // Limpiar el buffer de entrada antes de usar getline
        cin.ignore(1000, '\n');

        cout << "Nuevo Monto [" << monto << "]: ";
        cin.getline(nuevoDato, 10);
        if (strlen(nuevoDato) > 0) {
            monto = atoi(nuevoDato);
        }

        cout << "Nuevas Observaciones [" << observaciones << "]: ";
        cin.getline(nuevoDato, 100);
        if (strlen(nuevoDato) > 0) {
            strcpy(observaciones, nuevoDato);
        }

        // Confirmar la modificación
        char confirmacion;
        cout << "\n¿Está seguro de que desea modificar la reserva con ID " << id << "? (S/N): ";
        cin >> confirmacion;

        if (toupper(confirmacion) != 'S') {
            cout << "Operación cancelada." << endl;
            cout << "Presiona Enter para continuar...";
            cin.ignore(1000, '\n'); // Limpiar buffer
            cin.get();
            return;
        }

        // Limpiar buffer después de usar cin >>
        cin.ignore(1000, '\n');

        // Enviar los datos actualizados al servidor
        memset(sendBuff, 0, sizeof(sendBuff));
        sprintf(sendBuff, "%s|%d|%s|%s|%s|%d|%s|%d",
                dni_cliente, id_habitacion, fecha_entrada, fecha_salida,
                estado, monto, observaciones, id);
        send(s, sendBuff, strlen(sendBuff), 0);

        // Recibir respuesta final del servidor
        memset(recvBuff, 0, sizeof(recvBuff));
        bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
        if (bytes > 0) {
            recvBuff[bytes] = '\0';
            cout << recvBuff << endl;

            // Añadir una pausa para que el usuario pueda leer el mensaje
            cout << "Presiona Enter para continuar...";
            cin.get();
        }
    }
}

void listaReservas(SOCKET s) {
    char recvBuff[8192]; // Buffer más grande para recibir listas largas
    char sendBuff[512];

    // Limpiar buffers
    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));

    // Enviar comando al servidor
    strcpy(sendBuff, "LISTAR_RESERVAS");
    int sendResult = send(s, sendBuff, strlen(sendBuff), 0);

	if (sendResult == SOCKET_ERROR) {
		cout << "Error al enviar solicitud al servidor: " << WSAGetLastError() << endl;
		return;
	}

	// Esperar respuesta del servidor
	int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

	if (bytes > 0) {
		recvBuff[bytes] = '\0'; // Asegurar terminación
		cout << recvBuff << endl;

		// Añadir una pausa para que el usuario pueda leer el mensaje
		cout << "Presiona Enter para continuar...";
		cin.ignore(1000, '\n');
		cin.get();
	} else if (bytes == 0) {
		cout << "El servidor ha cerrado la conexión" << endl;
	} else {
		cout << "Error al recibir datos: " << WSAGetLastError() << endl;
	}
}

void buscarReserva(SOCKET s) {
    char recvBuff[4096];
    char sendBuff[4096];
    string criterio;

    // Limpiar buffers
    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));

    cout << "\n=== BÚSQUEDA DE RESERVAS ===\n";
    cout << "Introduce el id de la reserva a buscar: ";
    cin.ignore(1000, '\n'); // Limpiar buffer de entrada
    getline(cin, criterio);

    // Enviar comando al servidor
    strcpy(sendBuff, "BUSCAR_RESERVAS");
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

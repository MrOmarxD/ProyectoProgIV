#include "gestorHabitaciones.h"
#include "gestorMenus.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

void gestionHabitaciones(SOCKET s) {
    int opcion;

    cout<<"\n--- GESTIÓN DE HABITACIONES ---\n";
    cout<<"1. Añadir nueva habitación\n";
    cout<<"2. Modificar habitación\n";
    cout<<"3. Cambiar estado de habitación\n";
    cout<<"4. Listar habitaciones\n";
    cout<<"5. Buscar habitaciones\n";
    cout<<"0. Volver al menú principal\n";
    cout<<"Seleccione una opción: "<<endl;
    cin >> opcion;

    switch (opcion) {
                case 1:
                	crearHabitacion(s);
                    break;
                case 2:
                	modificarHabitacion(s);
                    break;
                case 3:
                	establecerEstadoHabitacion(s);
                    break;
                case 4:
                	listarHabitaciones(s);
                    break;
                case 5:
					buscarHabitacion(s);
					break;
                case 0:
                	mostrarMenuPrincipal(s);
                	break;
                default:
                    cout<<"Opción no válida. Intente nuevamente.\n" <<endl;
            }

    //registrarActividad(usuario_actual, "Acceso a gestión de habitaciones", LOG_FILE);
}

void crearHabitacion(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];
    string input;

    // Enviar comando al servidor
    strcpy(sendBuff, "CREATE_ROOM");
    send(s, sendBuff, strlen(sendBuff), 0);

    char numero[10], tipo[20], estado[15], descripcion[100];
    int precio, capacidad;

    // Limpiar el buffer de entrada antes de usar getline
    cin.ignore(1000, '\n');

    cout << "=== REGISTRO DE NUEVA HABITACIÓN ===" << endl;
    cout << "Introduce el número de habitación: ";
    cin.getline(numero, 10);

    int opcion;
    do {
        cout<<"Elija el tipo de habitación\n";
        cout<<"1. Individual\n";
        cout<<"2. Doble\n";
        cout<<"3. Suite\n";
        cout<<"4. Familiar\n";
        cout<<"Seleccione una opción: ";

        cin>>opcion;

        switch(opcion) {
            case 1:
                cout<<"\nHa seleccionado: Individual\n\n";
                strcpy(tipo, "Individual");
                break;
            case 2:
                cout<<"\nHa seleccionado: Doble\n\n";
                strcpy(tipo, "Doble");
                break;
            case 3:
                cout<<"\nHa seleccionado: Suite\n\n";
                strcpy(tipo, "Suite");
                break;
            case 4:
                cout<<"\nHa seleccionado: Familiar\n\n";
                strcpy(tipo, "Familiar");
                break;
            default:
                cout<<"\nOpción no válida. Por favor, intente de nuevo.\n";
                opcion = 0;
                break;
        }
    } while(opcion == 0);

    cout << "Introduce el precio por noche: ";
    cin >> precio;

    opcion = 0;
    do {
        cout<<"Elija el estado inicial de la habitación\n";
        cout<<"1. Disponible\n";
        cout<<"2. Ocupada\n";
        cout<<"3. Mantenimiento\n";
        cout<<"Seleccione una opción: ";

        cin>>opcion;

        switch(opcion) {
            case 1:
                cout<<"\nHa seleccionado: Disponible\n\n";
                strcpy(estado, "Disponible");
                break;
            case 2:
                cout<<"\nHa seleccionado: Ocupada\n\n";
                strcpy(estado, "Ocupada");
                break;
            case 3:
                cout<<"\nHa seleccionado: Mantenimiento\n\n";
                strcpy(estado, "Mantenimiento");
                break;
            default:
                cout<<"\nOpción no válida. Por favor, intente de nuevo.\n";
                opcion = 0;
                break;
        }
    } while(opcion == 0);

    cout << "Introduce la capacidad de personas: ";
    cin >> capacidad;

    // Limpiar buffer después de usar cin >>
    cin.ignore(1000, '\n');

    cout << "Introduce una descripción de la habitación: ";
    cin.getline(descripcion, 100);

    // Enviar los datos de la habitación al servidor
    memset(sendBuff, 0, sizeof(sendBuff));
    sprintf(sendBuff, "%s|%s|%d|%s|%d|%s", numero, tipo, precio, estado, capacidad, descripcion);
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

void modificarHabitacion(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Enviar comando al servidor
    strcpy(sendBuff, "MODIFICAR_HABITACION");
    send(s, sendBuff, strlen(sendBuff), 0);

    char idHabitacion[10];
    // Limpiar el buffer de entrada antes de usar getline
    cin.ignore(1000, '\n');

    cout << "\n=== MODIFICAR HABITACIÓN ===\n";
    cout << "Introduce el ID o número de la habitación que desea modificar: ";
    cin.getline(idHabitacion, 10);

    // Enviar el ID de la habitación al servidor
    memset(sendBuff, 0, sizeof(sendBuff));
    strcpy(sendBuff, idHabitacion);
    send(s, sendBuff, strlen(sendBuff), 0);

    // Recibir respuesta del servidor
    memset(recvBuff, 0, sizeof(recvBuff));
    int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

    if (bytes > 0) {
        recvBuff[bytes] = '\0';

        // Verificar si hubo error (la habitación no existe)
        if (strncmp(recvBuff, "ERROR:", 6) == 0) {
            cout << recvBuff << endl;
            cout << "Presiona Enter para continuar...";
            cin.get();
            return;
        }

        // Parsear los datos recibidos de la habitación
        char numero[10], tipo[20], estado[15], descripcion[100];
        int precio, capacidad;

        // Separar la cadena por '|'
        char *token;
        char *rest = recvBuff;

        // Tipo
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(tipo, token);

        // Precio
        token = strtok(NULL, "|");
        if (token != NULL) precio = atoi(token);

        // Estado
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(estado, token);

        // Capacidad
        token = strtok(NULL, "|");
        if (token != NULL) capacidad = atoi(token);

        // Descripción
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(descripcion, token);

        cout << "\nDatos actuales de la habitación:\n";
        cout << "Número: " << numero << endl;
        cout << "Tipo: " << tipo << endl;
        cout << "Precio: " << precio << endl;
        cout << "Estado: " << estado << endl;
        cout << "Capacidad: " << capacidad << endl;
        cout << "Descripción: " << descripcion << endl;

        cout << "\nIntroduce los nuevos datos (deja en blanco para mantener el valor actual):\n";

        char nuevoDato[100];

        int opcion;
        cout << "¿Desea cambiar el tipo? (1: Sí, 0: No): ";
        cin >> opcion;

        if (opcion == 1) {
            do {
                cout << "Elija el nuevo tipo de habitación\n";
                cout << "1. Individual\n";
                cout << "2. Doble\n";
                cout << "3. Suite\n";
                cout << "4. Familiar\n";
                cout << "Seleccione una opción: ";

                cin >> opcion;

                switch(opcion) {
                    case 1:
                        cout << "\nHa seleccionado: Individual\n\n";
                        strcpy(tipo, "Individual");
                        break;
                    case 2:
                        cout << "\nHa seleccionado: Doble\n\n";
                        strcpy(tipo, "Doble");
                        break;
                    case 3:
                        cout << "\nHa seleccionado: Suite\n\n";
                        strcpy(tipo, "Suite");
                        break;
                    case 4:
                        cout << "\nHa seleccionado: Familiar\n\n";
                        strcpy(tipo, "Familiar");
                        break;
                    default:
                        cout << "\nOpción no válida. Por favor, intente de nuevo.\n";
                        opcion = 0;
                        break;
                }
            } while(opcion == 0);
        }

        // Limpiar el buffer de entrada
        cin.ignore(1000, '\n');

        cout << "Nuevo precio [" << precio << "]: ";
        cin.getline(nuevoDato, 10);
        if (strlen(nuevoDato) > 0) {
            precio = atoi(nuevoDato);
        }

        cout << "¿Desea cambiar el estado? (1: Sí, 0: No): ";
        cin >> opcion;

        if (opcion == 1) {
            do {
                cout << "Elija el nuevo estado de la habitación\n";
                cout << "1. Disponible\n";
                cout << "2. Ocupada\n";
                cout << "3. Mantenimiento\n";
                cout << "Seleccione una opción: ";

                cin >> opcion;

                switch(opcion) {
                    case 1:
                        cout << "\nHa seleccionado: Disponible\n\n";
                        strcpy(estado, "Disponible");
                        break;
                    case 2:
                        cout << "\nHa seleccionado: Ocupada\n\n";
                        strcpy(estado, "Ocupada");
                        break;
                    case 3:
                        cout << "\nHa seleccionado: Mantenimiento\n\n";
                        strcpy(estado, "Mantenimiento");
                        break;
                    default:
                        cout << "\nOpción no válida. Por favor, intente de nuevo.\n";
                        opcion = 0;
                        break;
                }
            } while(opcion == 0);
        }

        // Limpiar buffer
        cin.ignore(1000, '\n');

        cout << "Nueva capacidad [" << capacidad << "]: ";
        cin.getline(nuevoDato, 10);
        if (strlen(nuevoDato) > 0) {
            capacidad = atoi(nuevoDato);
        }

        cout << "Nueva descripción [" << descripcion << "]: ";
        cin.getline(nuevoDato, 100);
        if (strlen(nuevoDato) > 0) {
            strcpy(descripcion, nuevoDato);
        }

        // Confirmar la modificación
        char confirmacion;
        cout << "\n¿Está seguro de que desea modificar la habitación '" << numero << "'? (S/N): ";
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
        sprintf(sendBuff, "%s|%s|%d|%s|%d|%s", numero, tipo, precio, estado, capacidad, descripcion);
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

void establecerEstadoHabitacion(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Enviar comando al servidor
    strcpy(sendBuff, "CAMBIAR_ESTADO_HABITACION");
    send(s, sendBuff, strlen(sendBuff), 0);

    char idHabitacion[10];
    // Limpiar el buffer de entrada antes de usar getline
    cin.ignore(1000, '\n');

    cout << "\n=== CAMBIAR ESTADO DE HABITACIÓN ===\n";
    cout << "Introduce el ID o número de la habitación: ";
    cin.getline(idHabitacion, 10);

    // Enviar el ID de la habitación al servidor
    memset(sendBuff, 0, sizeof(sendBuff));
    strcpy(sendBuff, idHabitacion);
    send(s, sendBuff, strlen(sendBuff), 0);

    // Recibir respuesta del servidor
    memset(recvBuff, 0, sizeof(recvBuff));
    int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

    if (bytes > 0) {
        recvBuff[bytes] = '\0';

        // Verificar si hubo error (la habitación no existe)
        if (strncmp(recvBuff, "ERROR:", 6) == 0) {
            cout << recvBuff << endl;
            cout << "Presiona Enter para continuar...";
            cin.get();
            return;
        }

        // Mostrar estado actual
        cout << "\nEstado actual: " << recvBuff << endl;

        // Seleccionar nuevo estado
        int opcion;
        char nuevoEstado[15];

        do {
            cout << "Elija el nuevo estado de la habitación\n";
            cout << "1. Disponible\n";
            cout << "2. Ocupada\n";
            cout << "3. Mantenimiento\n";
            cout << "Seleccione una opción: ";

            cin >> opcion;

            switch(opcion) {
                case 1:
                    cout << "\nHa seleccionado: Disponible\n\n";
                    strcpy(nuevoEstado, "Disponible");
                    break;
                case 2:
                    cout << "\nHa seleccionado: Ocupada\n\n";
                    strcpy(nuevoEstado, "Ocupada");
                    break;
                case 3:
                    cout << "\nHa seleccionado: Mantenimiento\n\n";
                    strcpy(nuevoEstado, "Mantenimiento");
                    break;
                default:
                    cout << "\nOpción no válida. Por favor, intente de nuevo.\n";
                    opcion = 0;
                    break;
            }
        } while(opcion == 0);

        // Confirmar el cambio
        char confirmacion;
        cout << "¿Está seguro de que desea cambiar el estado de la habitación? (S/N): ";
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

        // Enviar el nuevo estado al servidor
        memset(sendBuff, 0, sizeof(sendBuff));
        strcpy(sendBuff, nuevoEstado);
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

void listarHabitaciones(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Limpiar buffers
    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));

    // Enviar comando al servidor
    strcpy(sendBuff, "GET_ROOMS");
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

void buscarHabitacion(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];
    string criterio;
    int tipoBusqueda;

    // Limpiar buffers
    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));

    cout << "\n=== BÚSQUEDA DE HABITACIONES ===\n";
    cout << "1. Buscar por número\n";
    cout << "2. Buscar por tipo\n";
    cout << "3. Buscar por estado\n";
    cout << "Seleccione una opción: ";
    cin >> tipoBusqueda;

    // Limpiar buffer de entrada
    cin.ignore(1000, '\n');

    switch(tipoBusqueda) {
        case 1:
            cout << "Introduce el número de habitación a buscar: ";
            break;
        case 2:
            cout << "Introduce el tipo de habitación a buscar (Individual, Doble, Suite, Familiar): ";
            break;
        case 3:
            cout << "Introduce el estado a buscar (Disponible, Ocupada, Mantenimiento): ";
            break;
        default:
            cout << "Opción no válida. Volviendo al menú anterior." << endl;
            return;
    }

    getline(cin, criterio);

    // Enviar comando al servidor
    strcpy(sendBuff, "BUSCAR_HABITACION");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Pequeña pausa para asegurar que el servidor procese el comando
    Sleep(100);

    // Enviar tipo de búsqueda
    memset(sendBuff, 0, sizeof(sendBuff));
    sprintf(sendBuff, "%d", tipoBusqueda);
    send(s, sendBuff, strlen(sendBuff), 0);

    // Pequeña pausa para asegurar que el servidor procese el tipo
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

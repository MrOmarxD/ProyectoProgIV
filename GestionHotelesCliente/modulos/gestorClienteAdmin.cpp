#include "gestorClienteAdmin.h"

using namespace std;

void gestionClienteAdmin(SOCKET s) {
    int opcion;
    cout<<"\n--- GESTIÓN DE CLIENTES ---\n";
    cout<<"1. Crear nuevo cliente\n";
    cout<<"2. Modificar cliente existente\n";
    cout<<"3. Listar clientes\n";
    cout<<"4. Buscar cliente\n";
    cout<<"0. Volver al menú principal\n";
    cout<<"Seleccione una opción: " << endl;
    cin >> opcion;

    switch (opcion) {
                case 1:
                	crearClienteAd(s);
                    break;
                case 2:
                	modificarClienteAd(s);
                    break;
                case 3:
                	listarClientesAd(s);
                    break;
                case 4:
                	buscarClienteAd(s);
					break;
                case 0:
                	mostrarMenuPrincipal(s);
                	break;
                default:
                    cout<<"Opción no válida. Intente nuevamente.\n"<< endl;
            }


    //registrarActividad(usuario_actual, "Acceso a gestión de usuarios", LOG_FILE);
}

void crearClienteAd(SOCKET s) {
    string dni, nombre, apellido, telefono, email;
    char sendBuff[512], recvBuff[512];

    cout << "\n--- CREAR NUEVO CLIENTE ---\n";

    // Limpiamos el buffer de entrada
    cin.ignore();

    cout << "DNI: ";
    getline(cin, dni);

    cout << "Nombre: ";
    getline(cin, nombre);

    cout << "Apellido: ";
    getline(cin, apellido);

    cout << "Teléfono: ";
    getline(cin, telefono);

    cout << "Email: ";
    getline(cin, email);

    // Enviar comando al servidor
    strcpy(sendBuff, "CREATE_CLIENT");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Recibir respuesta del servidor que confirma que está listo para recibir datos
    memset(recvBuff, 0, sizeof(recvBuff));
    recv(s, recvBuff, sizeof(recvBuff), 0);

    // Si el servidor está listo, enviar los datos del cliente
    if (strcmp(recvBuff, "SOLICITAR_DATOS") == 0) {
        string datosCliente = dni + "|" + nombre + "|" + apellido + "|" + telefono + "|" + email;

        memset(sendBuff, 0, sizeof(sendBuff));
        strcpy(sendBuff, datosCliente.c_str());
        send(s, sendBuff, strlen(sendBuff), 0);

        // Recibir respuesta final del servidor
        memset(recvBuff, 0, sizeof(recvBuff));
        recv(s, recvBuff, sizeof(recvBuff), 0);

        // Verificar el resultado
        if (strncmp(recvBuff, "OK|", 3) == 0) {
            cout << "\nCliente creado correctamente.\n";
        } else if (strncmp(recvBuff, "ERROR|", 6) == 0) {
            cout << "\nError: " << (recvBuff + 6) << "\n";
        } else {
            cout << "\nRespuesta desconocida del servidor.\n";
        }
    } else {
        cout << "\nError: El servidor no está listo para recibir datos.\n";
    }

    // Volver al menú de gestión de clientes
    cout << "\nPresione Enter para continuar...";
    cin.get();
    gestionClienteAdmin(s);
}

void modificarClienteAd(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Enviar comando al servidor
    strcpy(sendBuff, "MODIFICAR_CLIENTE");
    send(s, sendBuff, strlen(sendBuff), 0);

    char dni[20];
    // Limpiar el buffer de entrada antes de usar getline
    cin.ignore(1000, '\n');

    cout << "\n=== MODIFICAR CLIENTE ===\n";
    cout << "Introduce el DNI del cliente que desea modificar: ";
    cin.getline(dni, 20);

    // Enviar el DNI del cliente al servidor
    memset(sendBuff, 0, sizeof(sendBuff));
    strcpy(sendBuff, dni);
    send(s, sendBuff, strlen(sendBuff), 0);

    // Recibir respuesta del servidor
    memset(recvBuff, 0, sizeof(recvBuff));
    int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

    if (bytes > 0) {
        recvBuff[bytes] = '\0';

        // Verificar si hubo error (el cliente no existe)
        if (strncmp(recvBuff, "ERROR|", 6) == 0) {
            cout << recvBuff + 6 << endl;
            cout << "Presiona Enter para continuar...";
            cin.get();
            return;
        }

        // Verificar si recibimos los datos del cliente
        if (strncmp(recvBuff, "DATOS_CLIENTE|", 14) == 0) {
            // Parsear los datos recibidos del cliente
            char dni[20], nombre[50], apellido[50], telefono[20], email[50];

            // Separar la cadena por '|'
            char *token;
            char *rest = recvBuff + 14; // Saltar el prefijo "DATOS_CLIENTE|"

            // DNI
            token = strtok(rest, "|");
            if (token != NULL) strcpy(dni, token);

            // Nombre
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(nombre, token);

            // Apellido
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(apellido, token);

            // Teléfono
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(telefono, token);

            // Email
            token = strtok(NULL, "|");
            if (token != NULL) strcpy(email, token);

            cout << "\nDatos actuales del cliente:\n";
            cout << "DNI: " << dni << " (no se puede modificar)\n";
            cout << "Nombre: " << nombre << endl;
            cout << "Apellido: " << apellido << endl;
            cout << "Teléfono: " << telefono << endl;
            cout << "Email: " << email << endl;

            cout << "\nIntroduce los nuevos datos (deja en blanco para mantener el valor actual):\n";

            char nuevoDato[50];

            cout << "Nuevo nombre [" << nombre << "]: ";
            cin.getline(nuevoDato, 50);
            if (strlen(nuevoDato) > 0) {
                strcpy(nombre, nuevoDato);
            }

            cout << "Nuevo apellido [" << apellido << "]: ";
            cin.getline(nuevoDato, 50);
            if (strlen(nuevoDato) > 0) {
                strcpy(apellido, nuevoDato);
            }

            cout << "Nuevo teléfono [" << telefono << "]: ";
            cin.getline(nuevoDato, 20);
            if (strlen(nuevoDato) > 0) {
                strcpy(telefono, nuevoDato);
            }

            cout << "Nuevo email [" << email << "]: ";
            cin.getline(nuevoDato, 50);
            if (strlen(nuevoDato) > 0) {
                strcpy(email, nuevoDato);
            }

            // Confirmar la modificación
            char confirmacion;
            cout << "\n¿Está seguro de que desea modificar el cliente con DNI '" << dni << "'? (S/N): ";
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
            sprintf(sendBuff, "UPDATE_CLIENTE|%s|%s|%s|%s", nombre, apellido, telefono, email);
            send(s, sendBuff, strlen(sendBuff), 0);

            // Recibir respuesta final del servidor
            memset(recvBuff, 0, sizeof(recvBuff));
            bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
            if (bytes > 0) {
                recvBuff[bytes] = '\0';

                if (strncmp(recvBuff, "OK|", 3) == 0) {
                    cout << recvBuff + 3 << endl;
                } else if (strncmp(recvBuff, "ERROR|", 6) == 0) {
                    cout << "Error: " << recvBuff + 6 << endl;
                } else {
                    cout << recvBuff << endl;
                }

                // Añadir una pausa para que el usuario pueda leer el mensaje
                cout << "Presiona Enter para continuar...";
                cin.get();
            }
        } else {
            cout << "Error: Respuesta inesperada del servidor" << endl;
            cout << "Presiona Enter para continuar...";
            cin.get();
        }
    }
}

void listarClientesAd(SOCKET s) {
    char sendBuff[512], recvBuff[4096]; // Buffer más grande para la lista

    cout << "\n--- LISTA DE CLIENTES ---\n";

    // Enviar comando al servidor
    strcpy(sendBuff, "GET_CLIENTS");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Recibir lista de clientes
    memset(recvBuff, 0, sizeof(recvBuff));
    recv(s, recvBuff, sizeof(recvBuff), 0);

    // Mostrar la lista de clientes
    cout << recvBuff << endl;

    // Volver al menú de gestión de clientes
    cout << "\nPresione Enter para continuar...";
    cin.ignore();
    cin.get();
    gestionClienteAdmin(s);
}

void buscarClienteAd(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];
    string criterio;

    // Limpiar buffers
    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));

    cout << "\n=== BÚSQUEDA DE CLIENTES ===\n";
    cout << "Introduce el dni del cliente a buscar: ";
    cin.ignore(1000, '\n'); // Limpiar buffer de entrada
    getline(cin, criterio);

    // Enviar comando al servidor
    strcpy(sendBuff, "BUSCAR_CLIENTE");
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

#include "gestorUsuarios.h"

using namespace std;

void gestionUsuarios(SOCKET s) {
    int opcion;
    cout<<"\n--- GESTIÓN DE USUARIOS ---\n";
    cout<<"1. Crear nuevo usuario\n";
    cout<<"2. Modificar usuario existente\n";
    cout<<"3. Eliminar usuario\n";
    cout<<"4. Listar usuarios\n";
    cout<<"5. Buscar usuario\n";
    cout<<"0. Volver al menú principal\n";
    cout<<"Seleccione una opción: " << endl;
    cin >> opcion;

    switch (opcion) {
                case 1:
                	crearUsuario(s);
                    break;
                case 2:
                	modificarUsuario(s);
                    break;
                case 3:
                	eliminarUsuario(s);
                    break;
                case 4:
                	listaUsuarios(s);
                    break;
                case 5:
                	buscarUsuario(s);
					break;
                case 0:
                	mostrarMenuPrincipal(s);
                	break;
                default:
                    cout<<"Opción no válida. Intente nuevamente.\n"<< endl;
            }


    //registrarActividad(usuario_actual, "Acceso a gestión de usuarios", LOG_FILE);
}

void crearUsuario(SOCKET s){
	char recvBuff[512];
	char sendBuff[512];
	string input;

	// Enviar comando al servidor
	strcpy(sendBuff, "2");
	send(s, sendBuff, strlen(sendBuff), 0);

	char nombre[50], rol[20], usuario[20], password[20], turno[20];
	int salario;

	// Limpiar el buffer de entrada antes de usar getline
	cin.ignore(1000, '\n');

	cout << "=== REGISTRO DE NUEVO USUARIO ===" << endl;
	cout << "Introduce el nombre completo: ";
	cin.getline(nombre, 50);

	int opcion;
	do {
		cout<<"Elija el rol del nuevo Usuario\n";
		cout<<"1. Administrador\n";
		cout<<"2. Recepcionista\n";
		cout<<"3. Limpieza\n";
		cout<<"4. Mantenimiento\n";
		cout<<"Seleccione una opcion: ";

		cin>>opcion;

		switch(opcion) {
			case 1:
				cout<<"\nHa seleccionado: Administrador\n\n";
				strcpy(rol, "Administrador");
				break;
			case 2:
				cout<<"\nHa seleccionado: Recepcionista\n\n";
				strcpy(rol, "Recepcionista");
				break;
			case 3:
				cout<<"\nHa seleccionado: Limpieza\n\n";
				strcpy(rol, "Limpieza");
				break;
			case 4:
				cout<<"\nHa seleccionado: Mantenimiento\n\n";
				strcpy(rol, "Mantenimiento");
				break;
			default:
				cout<<"\nOpcion no valida. Por favor, intente de nuevo.\n";
				opcion = 0;
				break;
		}
	} while(opcion == 0);

	// Limpiar el buffer de entrada antes de usar getline
	cin.ignore(1000, '\n');

	cout << "Introduce el nombre de usuario: ";
	cin.getline(usuario, 20);

	cout << "Introduce la contraseña: ";
	cin.getline(password, 20);

	opcion = 0;
	do {
		cout<<"Elija el turno del nuevo Usuario\n";
		cout<<"1. Mañana\n";
		cout<<"2. Tarde\n";
		cout<<"3. Noche\n";
		cout<<"Seleccione una opcion: ";

		cin>>opcion;

		switch(opcion) {
			case 1:
				cout<<"\nHa seleccionado: Mañana\n\n";
				strcpy(turno, "Mañana");
				break;
			case 2:
				cout<<"\nHa seleccionado: Tarde\n\n";
				strcpy(turno, "Tarde");
				break;
			case 3:
				cout<<"\nHa seleccionado: Noche\n\n";
				strcpy(turno, "Noche");
				break;
			default:
				cout<<"\nOpcion no valida. Por favor, intente de nuevo.\n";
				opcion = 0;
				break;
		}
	} while(opcion == 0);

	cout << "Introduce el salario: ";
	cin >> salario;

	// Limpiar buffer después de usar cin >>
	cin.ignore(1000, '\n');

	// Enviar los datos del registro al servidor
	memset(sendBuff, 0, sizeof(sendBuff));
	sprintf(sendBuff, "%s|%s|%s|%s|%s|%d", nombre, rol, usuario, password, turno, salario);
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

void eliminarUsuario(SOCKET s){
    char recvBuff[512];
    char sendBuff[512];

    // Enviar comando al servidor
    strcpy(sendBuff, "DELETE_USER");
    send(s, sendBuff, strlen(sendBuff), 0);

    char usuario[20];
    // Limpiar el buffer de entrada antes de usar getline
    cin.ignore(1000, '\n');

    cout << "\n=== ELIMINAR USUARIO ===\n";
    cout << "Introduce el nombre de usuario que desea eliminar: ";
    cin.getline(usuario, 20);

    // Confirmar la eliminación
    char confirmacion;
    cout << "¿Está seguro de que desea eliminar el usuario '" << usuario << "'? (S/N): ";
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

    // Enviar el nombre de usuario al servidor
    memset(sendBuff, 0, sizeof(sendBuff));
    strcpy(sendBuff, usuario);
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
void listaUsuarios(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Limpiar buffers
    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));

    // Enviar comando al servidor
    strcpy(sendBuff, "GET_USERS");
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

void modificarUsuario(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Enviar comando al servidor
    strcpy(sendBuff, "MODIFICAR_USUARIO");
    send(s, sendBuff, strlen(sendBuff), 0);

    char nombreUsuario[20];
    // Limpiar el buffer de entrada antes de usar getline
    cin.ignore(1000, '\n');

    cout << "\n=== MODIFICAR USUARIO ===\n";
    cout << "Introduce el nombre de usuario que desea modificar: ";
    cin.getline(nombreUsuario, 20);

    // Enviar el nombre de usuario al servidor
    memset(sendBuff, 0, sizeof(sendBuff));
    strcpy(sendBuff, nombreUsuario);
    send(s, sendBuff, strlen(sendBuff), 0);

    // Recibir respuesta del servidor
    memset(recvBuff, 0, sizeof(recvBuff));
    int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

    if (bytes > 0) {
        recvBuff[bytes] = '\0';

        // Verificar si hubo error (el usuario no existe)
        if (strncmp(recvBuff, "ERROR:", 6) == 0) {
            cout << recvBuff << endl;
            cout << "Presiona Enter para continuar...";
            cin.get();
            return;
        }

        // Parsear los datos recibidos del usuario
        char nombre[50], rol[20], usuario[20], password[20], turno[20];
        int salario;

        // Separar la cadena por '|'
        char *token;
        char *rest = recvBuff;

        // Nombre
        token = strtok(rest, "|");
        if (token != NULL) strcpy(nombre, token);

        // Rol
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(rol, token);

        // Usuario
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(usuario, token);

        // Password
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(password, token);

        // Turno
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(turno, token);

        // Salario
        token = strtok(NULL, "|");
        if (token != NULL) salario = atoi(token);

        cout << "\nDatos actuales del usuario:\n";
        cout << "Nombre: " << nombre << endl;
        cout << "Rol: " << rol << endl;
        cout << "Usuario: " << usuario << " (no se puede modificar)\n";
        cout << "Contraseña: " << password << endl;
        cout << "Turno: " << turno << endl;
        cout << "Salario: " << salario << endl;

        cout << "\nIntroduce los nuevos datos (deja en blanco para mantener el valor actual):\n";

        char nuevoDato[50];

        cout << "Nuevo nombre [" << nombre << "]: ";
        cin.getline(nuevoDato, 50);
        if (strlen(nuevoDato) > 0) {
            strcpy(nombre, nuevoDato);
        }

        int opcion;
        cout << "¿Desea cambiar el rol? (1: Sí, 0: No): ";
        cin >> opcion;

        if (opcion == 1) {
            do {
                cout << "Elija el nuevo rol del Usuario\n";
                cout << "1. Administrador\n";
                cout << "2. Recepcionista\n";
                cout << "3. Limpieza\n";
                cout << "4. Mantenimiento\n";
                cout << "Seleccione una opcion: ";

                cin >> opcion;

                switch(opcion) {
                    case 1:
                        cout << "\nHa seleccionado: Administrador\n\n";
                        strcpy(rol, "Administrador");
                        break;
                    case 2:
                        cout << "\nHa seleccionado: Recepcionista\n\n";
                        strcpy(rol, "Recepcionista");
                        break;
                    case 3:
                        cout << "\nHa seleccionado: Limpieza\n\n";
                        strcpy(rol, "Limpieza");
                        break;
                    case 4:
                        cout << "\nHa seleccionado: Mantenimiento\n\n";
                        strcpy(rol, "Mantenimiento");
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

        cout << "Nueva contraseña [" << password << "]: ";
        cin.getline(nuevoDato, 20);
        if (strlen(nuevoDato) > 0) {
            strcpy(password, nuevoDato);
        }

        cout << "¿Desea cambiar el turno? (1: Sí, 0: No): ";
        cin >> opcion;

        if (opcion == 1) {
            do {
                cout << "Elija el nuevo turno del Usuario\n";
                cout << "1. Mañana\n";
                cout << "2. Tarde\n";
                cout << "3. Noche\n";
                cout << "Seleccione una opcion: ";

                cin >> opcion;

                switch(opcion) {
                    case 1:
                        cout << "\nHa seleccionado: Mañana\n\n";
                        strcpy(turno, "Mañana");
                        break;
                    case 2:
                        cout << "\nHa seleccionado: Tarde\n\n";
                        strcpy(turno, "Tarde");
                        break;
                    case 3:
                        cout << "\nHa seleccionado: Noche\n\n";
                        strcpy(turno, "Noche");
                        break;
                    default:
                        cout << "\nOpcion no valida. Por favor, intente de nuevo.\n";
                        opcion = 0;
                        break;
                }
            } while(opcion == 0);
        }

        cout << "Nuevo salario [" << salario << "]: ";
        cin >> nuevoDato;
        if (strlen(nuevoDato) > 0) {
            salario = atoi(nuevoDato);
        }

        // Limpiar buffer después de usar cin >>
        cin.ignore(1000, '\n');

        // Confirmar la modificación
        char confirmacion;
        cout << "\n¿Está seguro de que desea modificar el usuario '" << usuario << "'? (S/N): ";
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
        sprintf(sendBuff, "%s|%s|%s|%s|%d", nombre, rol, password, turno, salario);
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

void buscarUsuario(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];
    string criterio;

    // Limpiar buffers
    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));

    cout << "\n=== BÚSQUEDA DE USUARIOS ===\n";
    cout << "Introduce el nombre de usuario a buscar: ";
    cin.ignore(1000, '\n'); // Limpiar buffer de entrada
    getline(cin, criterio);

    // Enviar comando al servidor
    strcpy(sendBuff, "BUSCAR_USUARIO");
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

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
                	//modificarUsuario(&usuario);
                    break;
                case 3:
                	eliminarUsuario(s);
                    break;
                case 4:
                	listaUsuarios(s);
                    break;
                case 5:
                	//buscarUsuario(&usuario);
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
        send(s, sendBuff, strlen(sendBuff), 0);

        // Esperar respuesta del servidor
        int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

        if (bytes > 0) {
            recvBuff[bytes] = '\0'; // Asegurar terminación
            cout << recvBuff << endl;
            //mostrarMenuPrincipalUsuario(s);
        } else if (bytes == 0) {
            cout << "El servidor ha cerrado la conexión" << endl;
        } else {
            cout << "Error al recibir datos: " << WSAGetLastError() << endl;
        }
    }


#include "gestorCliente.h"


using namespace std;

void mostrarClientes(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Limpiar buffers
    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));

    // Enviar comando al servidor
    strcpy(sendBuff, "GET_CLIENTS");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Esperar respuesta del servidor
    int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación
        cout << recvBuff << endl;
        mostrarMenuPrincipalCliente(s);
    } else if (bytes == 0) {
        cout << "El servidor ha cerrado la conexión" << endl;
    } else {
        cout << "Error al recibir datos: " << WSAGetLastError() << endl;
    }
}

void mostrarHabitaciones(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];

    // Limpiar buffers
    memset(recvBuff, 0, sizeof(recvBuff));
    memset(sendBuff, 0, sizeof(sendBuff));

    // Enviar comando al servidor
    strcpy(sendBuff, "GET_ROOMS");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Esperar respuesta del servidor
    int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);

    if (bytes > 0) {
        recvBuff[bytes] = '\0'; // Asegurar terminación
        cout << recvBuff << endl;
        mostrarMenuPrincipalCliente(s);
    } else if (bytes == 0) {
        cout << "El servidor ha cerrado la conexión" << endl;
    } else {
        cout << "Error al recibir datos: " << WSAGetLastError() << endl;
    }
}

void crearReserva(SOCKET s) {
    char recvBuff[512];
    char sendBuff[512];
    string input;

    // Enviar comando al servidor
    strcpy(sendBuff, "CREATE_RESERVATION");
    send(s, sendBuff, strlen(sendBuff), 0);

	char dni_cliente[10], fecha_entrada[20], fecha_salida[20], estado[20], observaciones[100];
	int id_habitacion, monto;

	// Limpiar el buffer de entrada antes de usar getline
	cin.ignore(1000, '\n');

	cout << "=== CREAR NUEVA RESERVA ===" << endl;
	cout << "Introduce el dni del cliente: ";
	cin.getline(dni_cliente, 10);


	cout << "Introduce el id de la habitacion: ";
	cin>>id_habitacion;

	cout << "Introduce el monto en €: ";
	cin>>monto;

	// Limpiar el buffer de entrada antes de usar getline
	cin.ignore(1000, '\n');

	cout << "Introduce la fecha de entrada(formato YYYY-MM-DD): ";
	cin.getline(fecha_entrada, 20);

	cout << "Introduce la fecha de salida(formato YYYY-MM-DD): ";
	cin.getline(fecha_salida, 20);

	int opcion;
	do {
		cout<<"Elija el estado de la reserva\n";
		cout<<"1. Pendiente\n";
		cout<<"2. Confirmada\n";
		cout<<"3. Cancelada\n";
		cout<<"4. Completada\n";
		cout<<"Seleccione una opcion: ";

		cin>>opcion;

		switch(opcion) {
			case 1:
				cout<<"\nHa seleccionado: Pendiente\n\n";
				strcpy(estado, "Pendiente");
				break;
			case 2:
				cout<<"\nHa seleccionado: Confirmada\n\n";
				strcpy(estado, "Confirmada");
				break;
			case 3:
				cout<<"\nHa seleccionado: Cancelada\n\n";
				strcpy(estado, "Cancelada");
				break;
			case 4:
				cout<<"\nHa seleccionado: Completada\n\n";
				strcpy(estado, "Completada");
				break;
			default:
				cout<<"\nOpcion no valida. Por favor, intente de nuevo.\n";
				opcion = 0;
				break;
		}
	} while(opcion == 0);
	// Limpiar buffer después de usar cin >>
	cin.ignore(1000, '\n');

	cout << "Introduce las observaciones: ";
	cin >> observaciones;

	// Limpiar buffer después de usar cin >>
	cin.ignore(1000, '\n');

	// Enviar los datos del registro al servidor
	memset(sendBuff, 0, sizeof(sendBuff));
	sprintf(sendBuff, "%s|%s|%s|%s|%s|%d|%d", dni_cliente, fecha_entrada, fecha_salida, estado, observaciones, id_habitacion, monto);
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

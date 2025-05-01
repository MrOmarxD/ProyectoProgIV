#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <string>

//#include "modulos/gestorUsuarios.h"
//#include "modulos/gestorHabitaciones.h"
//#include "modulos/gestorReservas.h"
//#include "modulos/gestorFacturas.h"
//#include "modulos/gestorRegistros.h"
#include "modulos/gestorMenus.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
using namespace std;

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET s;
    struct sockaddr_in server;
    char sendBuff[512], recvBuff[512];

    printf("\nInitialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return -1;
    }

    printf("Initialised.\n");

    //SOCKET creation
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    //CONNECT to remote server
    if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        printf("Connection error: %d", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return -1;
    }

    printf("Connection stablished with: %s (%d)\n", inet_ntoa(server.sin_addr),
           ntohs(server.sin_port));

    /*EMPIEZA EL PROGRAMA DEL CLIENTE*/
    char opcion;
    do {
		opcion = eleccionInicial();

		memset(sendBuff, 0, sizeof(sendBuff));
		memset(recvBuff, 0, sizeof(recvBuff));

		sprintf(sendBuff, "%c", opcion);
		send(s, sendBuff, strlen(sendBuff), 0);

		if (opcion == '1') {
			char usu[20], con[20];
			int resul;

			cout << "Introduce tu nombre de usuario: ";
			cin >> usu;
			cout << "Introduce tu contraseña: ";
			cin >> con;

			// Clear input buffer
			cin.ignore(1000, '\n');

			// Send nombre de usuario
			memset(sendBuff, 0, sizeof(sendBuff));
			strcpy(sendBuff, usu);
			send(s, sendBuff, strlen(sendBuff), 0);

			// Send password
			memset(sendBuff, 0, sizeof(sendBuff));
			strcpy(sendBuff, con);
			send(s, sendBuff, strlen(sendBuff), 0);

			// Receive confirmation
			memset(recvBuff, 0, sizeof(recvBuff));
			int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
			if (bytes > 0) {
				recvBuff[bytes] = '\0';
				cout << recvBuff << endl;

				// Receive authentication result
				memset(recvBuff, 0, sizeof(recvBuff));
				bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
				if (bytes > 0) {
					recvBuff[bytes] = '\0';
					sscanf(recvBuff, "%d", &resul);

					if (resul == 0) {
						cout << "Sesión iniciada correctamente" << endl;
						if (strstr(usu, "admin"))
							mostrarMenuPrincipal();
						else
							mostrarMenuPrincipalCliente(s);
					} else if (resul == 1) {
						cout << "La contraseña no es correcta" << endl;
					} else {
						cout << "El nombre de usuario no es correcto" << endl;
					}
				}
			}
		} else if (opcion == '2') {
			char nombre[50], rol[20], usuario[20], password[20], turno[20];
			int salario;

			// Limpiar el buffer de entrada antes de usar getline
			cin.ignore(1000, '\n');

			cout << "=== REGISTRO DE NUEVO USUARIO ===" << endl;
			cout << "Introduce el nombre completo: ";
			cin.getline(nombre, 50);

			cout << "Introduce el rol (admin/empleado): ";
			cin.getline(rol, 20);

			cout << "Introduce el nombre de usuario: ";
			cin.getline(usuario, 20);

			cout << "Introduce la contraseña: ";
			cin.getline(password, 20);

			cout << "Introduce el turno (mañana/tarde/noche): ";
			cin.getline(turno, 20);

			cout << "Introduce el salario: ";
			cin >> salario;

			// Limpiar buffer después de usar cin >>
			cin.ignore(1000, '\n');

			// Enviar señal de registro
			strcpy(sendBuff, "2");
			send(s, sendBuff, strlen(sendBuff), 0);

			// Crear cadena con los datos separados por '|'
			memset(sendBuff, 0, sizeof(sendBuff));
			sprintf(sendBuff, "%s|%s|%s|%s|%s|%d", nombre, rol, usuario, password, turno, salario);
			send(s, sendBuff, strlen(sendBuff), 0);

			// Recibir respuesta del servidor
			memset(recvBuff, 0, sizeof(recvBuff));
			int bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
			if (bytes > 0) {
				recvBuff[bytes] = '\0';
				cout << recvBuff << endl;
			}
		} else if (opcion == '0') {
			cout << "Saliendo del programa..." << endl;
		} else {
			cout << "Opción no válida" << endl;
		}

	} while (opcion != '0');


    /*ACABA EL PROGRAMA DEL CLIENTE*/

    // CLOSING the socket and cleaning Winsock...
    closesocket(s);
    WSACleanup();

    return 0;
}

#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>

#include "modulos/gestorMenus.h"

using namespace std;

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET s;
    struct sockaddr_in server;
    char sendBuff[512], recvBuff[512];

    // Cargar la configuración desde el archivo
    if (!loadConfig(CONFIG_FILE, &g_config)) {
        cout << "No se pudo cargar la configuración. Se utilizarán valores por defecto." << endl;
        setDefaultConfig(&g_config);
    }

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

    // Usar IP y puerto desde la configuración
    server.sin_addr.s_addr = inet_addr(g_config.server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(g_config.server_port);

    printf("Conectando a %s:%d...\n", g_config.server_ip, g_config.server_port);

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
            Usuario *usuarioActual = (Usuario*) malloc(sizeof(Usuario));

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

                        // Recibir datos del usuario desde el servidor
                        memset(recvBuff, 0, sizeof(recvBuff));
                        bytes = recv(s, recvBuff, sizeof(recvBuff) - 1, 0);
                        if (bytes > 0) {
                            recvBuff[bytes] = '\0';

                            // Parsear los datos del usuario
                            char *token;
                            char *rest = recvBuff;

                            token = strtok(rest, "|");
                            if (token != NULL) strcpy(usuarioActual->nombre, token);

                            token = strtok(NULL, "|");
                            if (token != NULL) strcpy(usuarioActual->rol, token);

                            token = strtok(NULL, "|");
                            if (token != NULL) strcpy(usuarioActual->usuario, token);

                            token = strtok(NULL, "|");
                            if (token != NULL) strcpy(usuarioActual->password, token);

                            token = strtok(NULL, "|");
                            if (token != NULL) strcpy(usuarioActual->turno, token);

                            token = strtok(NULL, "|");
                            if (token != NULL) usuarioActual->salario = atoi(token);

                            cout << "Bienvenido, " << usuarioActual->nombre << " (" << usuarioActual->rol << ")" << endl;

                            int salir = 1;
                            do{
                            	// Comprobar el rol en lugar del nombre de usuario
								if (strcmp(usuarioActual->rol, "Administrador") == 0)
									salir = mostrarMenuPrincipal(s);
								else
									salir = mostrarMenuPrincipalCliente(s);
                            }while(salir !=0);
                        }
					} else if (resul == 1) {
						cout << "La contraseña no es correcta" << endl;
					} else {
						cout << "El nombre de usuario no es correcto" << endl;
					}
				}
			}

            // Liberar memoria
            free(usuarioActual);
		} else if (opcion == '2') {
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
		} else if (opcion == '0') {
			// Enviar señal de salida al servidor
			memset(sendBuff, 0, sizeof(sendBuff));
			strcpy(sendBuff, "SALIR");
			send(s, sendBuff, strlen(sendBuff), 0);
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

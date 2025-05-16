#include "gestorRegistro.h"

void mostrarMenuRegistros(SOCKET s) {
    char opcion;
    char sendBuff[512], recvBuff[8192]; // Buffer de recepción más grande para registros
    bool salir = false;

    while (!salir) {
        cout << "\n===== GESTOR DE REGISTROS DE ACTIVIDAD =====" << endl;
        cout << "1. Listar todos los registros" << endl;
        cout << "2. Buscar registros por ID de usuario" << endl;
        cout << "3. Buscar registros por rango de fechas" << endl;
        cout << "0. Volver al menú principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore(); // Limpiar buffer

        switch (opcion) {
            case '1':
                listarRegistros(s);
                break;

            case '2':
                buscarRegistrosPorUsuario(s);
                break;

            case '3':
                obtenerRegistrosPorFecha(s);
                break;

            case '0':
                salir = true;
                break;

            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
                break;
        }
    }
}

void procesarRespuestaServidor(SOCKET s, char* recvBuff, char* sendBuff) {
    int bytesRead = recv(s, recvBuff, 8192, 0);

    if (bytesRead > 0) {
        recvBuff[bytesRead] = '\0';

        // Verificar si hay error
        if (strncmp(recvBuff, "ERROR", 5) == 0) {
            cout << "\n" << recvBuff << endl;

            // Pausa para que el usuario pueda leer el mensaje
            cout << "Presione Enter para continuar...";
            cin.get();
            return;
        }

        // Verificar si no se encontraron registros
        if (strncmp(recvBuff, "No se encontraron", 17) == 0) {
            cout << "\n" << recvBuff << endl;

            // Pausa para que el usuario pueda leer el mensaje
            cout << "Presione Enter para continuar...";
            cin.get();
            return;
        }

        // Mostrar los registros recibidos
        cout << "\nRESULTADOS:" << endl;
        cout << recvBuff << endl;

        // Si la respuesta es muy grande, puede que necesitemos confirmar recepción
        // y seguir recibiendo más datos
        while (bytesRead == 8191) { // Buffer lleno, puede haber más datos
            // Enviar confirmación para recibir más datos
            strcpy(sendBuff, "CONTINUAR");
            send(s, sendBuff, strlen(sendBuff), 0);

            // Recibir más datos
            bytesRead = recv(s, recvBuff, 8192, 0);
            if (bytesRead > 0) {
                recvBuff[bytesRead] = '\0';
                cout << recvBuff << endl;
            } else {
                break;
            }
        }

        // Pausa para que el usuario pueda leer los resultados
        cout << "Presione Enter para continuar...";
        cin.get();
    } else {
        cout << "\nError al recibir respuesta del servidor." << endl;

        // Pausa para que el usuario pueda leer el mensaje
        cout << "Presione Enter para continuar...";
        cin.get();
    }
}

void listarRegistros(SOCKET s) {
    char sendBuff[512], recvBuff[8192];

    // Enviar comando para listar registros
    memset(sendBuff, 0, sizeof(sendBuff));
    strcpy(sendBuff, "LISTAR_REGISTROS");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Enviar parámetros vacíos (no se necesitan para listar todos)
    memset(sendBuff, 0, sizeof(sendBuff));
    send(s, sendBuff, strlen(sendBuff), 0);

    // Procesar la respuesta
    memset(recvBuff, 0, sizeof(recvBuff));
    procesarRespuestaServidor(s, recvBuff, sendBuff);
}

void buscarRegistrosPorUsuario(SOCKET s) {
    char sendBuff[512], recvBuff[8192];
    int id_usuario;

    cout << "\nIngrese el ID del usuario a buscar: ";
    cin >> id_usuario;
    cin.ignore(); // Limpiar buffer

    // Enviar comando para buscar por usuario
    memset(sendBuff, 0, sizeof(sendBuff));
    strcpy(sendBuff, "BUSCAR_REGISTROS_USUARIO");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Enviar el ID de usuario
    memset(sendBuff, 0, sizeof(sendBuff));
    sprintf(sendBuff, "%d", id_usuario);
    send(s, sendBuff, strlen(sendBuff), 0);

    // Procesar la respuesta
    memset(recvBuff, 0, sizeof(recvBuff));
    procesarRespuestaServidor(s, recvBuff, sendBuff);
}

void obtenerRegistrosPorFecha(SOCKET s) {
    char sendBuff[512], recvBuff[8192];
    string fecha_inicio, fecha_fin;

    cout << "\nBuscar registros por rango de fechas" << endl;
    cout << "Formato de fecha: YYYY-MM-DD" << endl;
    cout << "Fecha de inicio: ";
    getline(cin, fecha_inicio);

    cout << "Fecha de fin: ";
    getline(cin, fecha_fin);

    // Enviar comando para buscar por fecha
    memset(sendBuff, 0, sizeof(sendBuff));
    strcpy(sendBuff, "BUSCAR_REGISTROS_FECHA");
    send(s, sendBuff, strlen(sendBuff), 0);

    // Enviar el rango de fechas
    memset(sendBuff, 0, sizeof(sendBuff));
    string fechas = fecha_inicio + " " + fecha_fin;
    strcpy(sendBuff, fechas.c_str());
    send(s, sendBuff, strlen(sendBuff), 0);

    // Procesar la respuesta
    memset(recvBuff, 0, sizeof(recvBuff));
    procesarRespuestaServidor(s, recvBuff, sendBuff);
}

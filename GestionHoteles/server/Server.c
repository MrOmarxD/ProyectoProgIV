#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>


#include "../bd/gestionBD.h"
#include "../modulos/gestorUsuarios.h"
#include "../modulos/gestorClientes.h"
#include "../modulos/gestorHabitaciones.h"
#include "../modulos/gestorReservas.h"
#include "../modulos/gestorFacturas.h"
#include "../modulos/gestorRegistros.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
#define LOG_FILE "server_activity.log"
sqlite3 *db;


void mandarListaClientes(SOCKET comm_socket) {
    char sendBuff[512];
    Cliente cliente;
    sqlite3_stmt *stmt;
    int result;

    // Open database connection
    abrirBd();

    // Send header indicating start of client list
    strcpy(sendBuff, "CLIENT_LIST_START");
    send(comm_socket, sendBuff, sizeof(sendBuff), 0);

    // Prepare SQL statement to fetch all clients
    char sql[] = "SELECT dni, nombre, apellido, telefono, email FROM clientes";

    if (sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        strcpy(sendBuff, "CLIENT_LIST_END");
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        cerrarBd();
        return;
    }


    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {

        const char *dni = (const char*)sqlite3_column_text(stmt, 0);
        const char *nombre = (const char*)sqlite3_column_text(stmt, 1);
        const char *apellido = (const char*)sqlite3_column_text(stmt, 2);
        const char *telefono = (const char*)sqlite3_column_text(stmt, 3);
        const char *email = (const char*)sqlite3_column_text(stmt, 4);


        sprintf(sendBuff, "%s|%s|%s|%s|%s",
                dni ? dni : "",
                nombre ? nombre : "",
                apellido ? apellido : "",
                telefono ? telefono : "",
                email ? email : "");
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
    }


    sqlite3_finalize(stmt);


    strcpy(sendBuff, "CLIENT_LIST_END");
    send(comm_socket, sendBuff, sizeof(sendBuff), 0);


    cerrarBd();
}


void mandarListaHabitaciones(SOCKET comm_socket) {
    char sendBuff[512];
    Habitacion habitacion;
    sqlite3_stmt *stmt;
    int result;


    abrirBd();


    strcpy(sendBuff, "ROOM_LIST_START");
    send(comm_socket, sendBuff, sizeof(sendBuff), 0);


    char sql[] = "SELECT numero, tipo, precio, estado, capacidad, descripcion FROM habitaciones";

    if (sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        fflush(stdout);
        strcpy(sendBuff, "ROOM_LIST_END");
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        cerrarBd();
        return;
    }


    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {

        const char *numero = (const char*)sqlite3_column_text(stmt, 0);
        const char *tipo = (const char*)sqlite3_column_text(stmt, 1);
        int precio = sqlite3_column_int(stmt, 2);
        const char *estado = (const char*)sqlite3_column_text(stmt, 3);
        int capacidad = sqlite3_column_int(stmt, 4);
        const char *descripcion = (const char*)sqlite3_column_text(stmt, 5);


        sprintf(sendBuff, "%s|%s|%s|%d|%d|%s",
                numero ? numero : "",
                tipo ? tipo : "",
                estado ? estado : "",
                precio,
                capacidad,
                descripcion ? descripcion : "");
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
    }


    sqlite3_finalize(stmt);


    strcpy(sendBuff, "ROOM_LIST_END");
    send(comm_socket, sendBuff, sizeof(sendBuff), 0);


    cerrarBd();
}


void server_crearReserva(SOCKET comm_socket, char* recvBuff) {
    char sendBuff[512];
    Reserva nuevaReserva;
    Cliente cliente;
    Habitacion habitacion;


    memset(&nuevaReserva, 0, sizeof(Reserva));


    abrirBd();

    //cliente DNI
    recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
    strncpy(nuevaReserva.dni_cliente, recvBuff, sizeof(nuevaReserva.dni_cliente) - 1);

    // Ver si el cliente exite
    int clienteExiste = recuperarClienteBD(nuevaReserva.dni_cliente, &cliente);
    if (!clienteExiste) {
        strcpy(sendBuff, "ERROR|Cliente no encontrado");
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        cerrarBd();
        return;
    }

    // Numero de habitacion
    recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

    nuevaReserva.id_habitacion = atoi(recvBuff);


    // Como espera un string uso recvbuff
    int habitacionExiste = recuperarHabitacionBD(recvBuff, &habitacion);
    if (!habitacionExiste) {
        strcpy(sendBuff, "ERROR|Habitación no existe");
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        cerrarBd();
        return;
    }


    if (strcmp(habitacion.estado, "Disponible") != 0) {
        strcpy(sendBuff, "ERROR|Habitación no disponible");
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        cerrarBd();
        return;
    }

    // mira check in
    recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
    strncpy(nuevaReserva.fecha_entrada, recvBuff, sizeof(nuevaReserva.fecha_entrada) - 1);

    // mira check out
    recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
    strncpy(nuevaReserva.fecha_salida, recvBuff, sizeof(nuevaReserva.fecha_salida) - 1);


    strncpy(nuevaReserva.estado, "Confirmada", sizeof(nuevaReserva.estado) - 1);


    nuevaReserva.monto = habitacion.precio;


    recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
    strncpy(nuevaReserva.observaciones, recvBuff, sizeof(nuevaReserva.observaciones) - 1);

    // Espera al final del mensaje para hacer a continuacion la reserva
    recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
    if (strcmp(recvBuff, "CREATE_RESERVATION_END") != 0) {
        strcpy(sendBuff, "ERROR|Formato de comando incorrecto");
        send(comm_socket, sendBuff, sizeof(sendBuff), 0);
        cerrarBd();
        return;
    }


    crearReservaBD(&nuevaReserva);


    strncpy(habitacion.estado, "Ocupada", sizeof(habitacion.estado) - 1);
    modificarHabitacionBD(&habitacion);


    sprintf(sendBuff, "Reserva creada correctamente|ID: %d", nuevaReserva.id);
    send(comm_socket, sendBuff, sizeof(sendBuff), 0);


    registrarActividad(1, "Reserva creada por servidor remoto", LOG_FILE);


    cerrarBd();
}

int server_main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET conn_socket;
    SOCKET comm_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char sendBuff[512], recvBuff[512];
    int usuario_actual = 1; // Default admin user

    printf("\nInicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return -1;
    }



    // creacion de Socket
    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("No se pudo crear el socket : %d", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    printf("Socket creado.\n");

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    // BIND
    if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        printf("Fallo en el bind: %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }

    printf("Bind done.\n");


    abrirBd();
    cerrarBd();


    if (listen(conn_socket, 1) == SOCKET_ERROR) {
        printf("Fallo en escucha: %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return -1;
    }


    while (1) {

        printf("Esperando conexiones\n");
        int stsize = sizeof(struct sockaddr);
        comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);

        if (comm_socket == INVALID_SOCKET) {
            printf("fallo al aceptar : %d", WSAGetLastError());
            closesocket(conn_socket);
            WSACleanup();
            return -1;
        }

        printf("Conectado con: %s (%d)\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));


        registrarActividad(usuario_actual, "Conexión remota establecida", LOG_FILE);


        printf("Esperando comandos del usuario \n");

        do {
            recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
            printf("Comando recibido: %s \n", recvBuff);


            if (strcmp(recvBuff, "GET_CLIENTS") == 0) {
                mandarListaClientes(comm_socket);
                printf("Mandada lista de clientes\n");
            }
            else if (strcmp(recvBuff, "GET_ROOMS") == 0) {
                mandarListaHabitaciones(comm_socket);
                printf("Mandada la lista de habitaciones\n");
            }
            else if (strcmp(recvBuff, "CREATE_RESERVATION") == 0) {
                server_crearReserva(comm_socket, recvBuff);
                printf("Procesando reserva\n");
            }
            else if (strcmp(recvBuff, "EXIT") == 0) {
                printf("El cliente ha solicitado desconectar\n");
                break;
            }
            else {
                printf("Comando desconocido\n");
                strcpy(sendBuff, "ERROR|Unknown command");
                send(comm_socket, sendBuff, sizeof(sendBuff), 0);
            }

        } while (1);


        registrarActividad(usuario_actual, "Conexión remota cerrada", LOG_FILE);


        closesocket(comm_socket);
        printf("Client disconnected. Waiting for new connections...\n");
    }


    closesocket(conn_socket);
    WSACleanup();

    return 0;
}

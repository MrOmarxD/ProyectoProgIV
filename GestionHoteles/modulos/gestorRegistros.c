#include "gestorRegistros.h"

int registrarActividad(int id_usuario, const char* operacion, const char* detalles, const char* log_file) {
    FILE* archivo;
    RegistroActividad registro;
    time_t t;
    struct tm* tm_info;

    // Obtener fecha y hora actual
    time(&t);
    tm_info = localtime(&t);

    // Formatear fecha como YYYY-MM-DD HH:MM:SS
    strftime(registro.fecha, 20, "%Y-%m-%d %H:%M:%S", tm_info);

    // Obtener el próximo ID
    registro.id = obtenerUltimoIdRegistro(log_file) + 1;
    registro.id_usuario = id_usuario;

    // Copiar operación y detalles
    strncpy(registro.operacion, operacion, 49);
    registro.operacion[49] = '\0'; // Asegurar null-termination

    strncpy(registro.detalles, detalles, 254);
    registro.detalles[254] = '\0'; // Asegurar null-termination

    // Abrir archivo en modo append
    archivo = fopen(log_file, "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de log: %s\n", log_file);
        return 0;
    }

    // Si el archivo está vacío, escribir encabezados
    fseek(archivo, 0, SEEK_END);
    if (ftell(archivo) == 0) {
        fprintf(archivo, "id;id_usuario;fecha;operacion;detalles\n");
    }

    // Escribir el registro
    fprintf(archivo, "%d;%d;%s;%s;%s\n",
            registro.id,
            registro.id_usuario,
            registro.fecha,
            registro.operacion,
            registro.detalles);

    fclose(archivo);
    return 1;
}

int obtenerUltimoIdRegistro(const char* log_file) {
    FILE* archivo;
    char linea[512];
    int ultimo_id = 0;
    int id_leido;

    archivo = fopen(log_file, "r");
    if (archivo == NULL) {
        // Si el archivo no existe, el primer ID será 1
        return 0;
    }

    // Saltar la primera línea (encabezados)
    if (fgets(linea, sizeof(linea), archivo) == NULL) {
        fclose(archivo);
        return 0;
    }

    // Leer cada línea para encontrar el último ID
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (sscanf(linea, "%d;", &id_leido) == 1) {
            if (id_leido > ultimo_id) {
                ultimo_id = id_leido;
            }
        }
    }

    fclose(archivo);
    return ultimo_id;
}

void listarRegistros(SOCKET s, char* recvBuff, char* sendBuff) {
    FILE* archivo;
    char linea[512];
    char respuesta[8192] = ""; // Buffer grande para la respuesta
    char* log_file = "actividad.log"; // Usar la ruta configurada

    // Recibir parámetros adicionales si es necesario
    recv(s, recvBuff, 512, 0);

    archivo = fopen(log_file, "r");
    if (archivo == NULL) {
        strcpy(sendBuff, "ERROR: No se pudo abrir el archivo de registros");
        send(s, sendBuff, strlen(sendBuff), 0);
        return;
    }

    // Leer encabezados
    if (fgets(linea, sizeof(linea), archivo) != NULL) {
        strcat(respuesta, linea);
    }

    // Leer y enviar todos los registros
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        strcat(respuesta, linea);

        // Si la respuesta está llena, enviarla y empezar una nueva
        if (strlen(respuesta) > 7000) {
            strcpy(sendBuff, respuesta);
            send(s, sendBuff, strlen(sendBuff), 0);
            memset(respuesta, 0, sizeof(respuesta));

            // Esperar confirmación antes de continuar
            recv(s, recvBuff, 512, 0);
        }
    }

    // Enviar el resto de la respuesta si hay algo
    if (strlen(respuesta) > 0) {
        strcpy(sendBuff, respuesta);
        send(s, sendBuff, strlen(sendBuff), 0);
    }

    fclose(archivo);
}

void buscarRegistrosPorUsuario(SOCKET s, char* recvBuff, char* sendBuff) {
    FILE* archivo;
    char linea[512];
    char respuesta[8192] = "";
    int id_usuario;
    char* log_file = "actividad.log"; // Usar la ruta configurada

    // Recibir el ID de usuario a buscar
    recv(s, recvBuff, 512, 0);
    id_usuario = atoi(recvBuff);

    archivo = fopen(log_file, "r");
    if (archivo == NULL) {
        strcpy(sendBuff, "ERROR: No se pudo abrir el archivo de registros");
        send(s, sendBuff, strlen(sendBuff), 0);
        return;
    }

    // Leer encabezados
    if (fgets(linea, sizeof(linea), archivo) != NULL) {
        strcat(respuesta, linea);
    }

    // Buscar registros del usuario específico
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        int id_registro, id_usr;
        char fecha[20], operacion[50], detalles[255];

        // Analizar la línea para extraer el ID de usuario
        sscanf(linea, "%d;%d;%19[^;];%49[^;];%254[^\n]",
               &id_registro, &id_usr, fecha, operacion, detalles);

        if (id_usr == id_usuario) {
            strcat(respuesta, linea);

            // Si la respuesta está llena, enviarla y empezar una nueva
            if (strlen(respuesta) > 7000) {
                strcpy(sendBuff, respuesta);
                send(s, sendBuff, strlen(sendBuff), 0);
                memset(respuesta, 0, sizeof(respuesta));

                // Esperar confirmación antes de continuar
                recv(s, recvBuff, 512, 0);
            }
        }
    }

    // Enviar el resto de la respuesta si hay algo
    if (strlen(respuesta) > 0) {
        strcpy(sendBuff, respuesta);
        send(s, sendBuff, strlen(sendBuff), 0);
    } else {
        strcpy(sendBuff, "No se encontraron registros para el usuario especificado");
        send(s, sendBuff, strlen(sendBuff), 0);
    }

    fclose(archivo);
}

void obtenerRegistrosPorFecha(SOCKET s, char* recvBuff, char* sendBuff) {
    FILE* archivo;
    char linea[512];
    char respuesta[8192] = "";
    char fecha_inicio[20], fecha_fin[20];
    char* log_file = "actividad.log"; // Usar la ruta configurada

    // Recibir el formato de fechas: "YYYY-MM-DD YYYY-MM-DD" (inicio y fin)
    recv(s, recvBuff, 512, 0);
    sscanf(recvBuff, "%19s %19s", fecha_inicio, fecha_fin);

    archivo = fopen(log_file, "r");
    if (archivo == NULL) {
        strcpy(sendBuff, "ERROR: No se pudo abrir el archivo de registros");
        send(s, sendBuff, strlen(sendBuff), 0);
        return;
    }

    // Leer encabezados
    if (fgets(linea, sizeof(linea), archivo) != NULL) {
        strcat(respuesta, linea);
    }

    // Buscar registros entre las fechas especificadas
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        int id_registro, id_usr;
        char fecha[20], operacion[50], detalles[255];

        // Analizar la línea para extraer la fecha
        sscanf(linea, "%d;%d;%19[^;];%49[^;];%254[^\n]",
               &id_registro, &id_usr, fecha, operacion, detalles);

        // Comparar fechas (solo la parte de la fecha, sin la hora)
        char fecha_registro[11];
        strncpy(fecha_registro, fecha, 10);
        fecha_registro[10] = '\0';

        if (strcmp(fecha_registro, fecha_inicio) >= 0 &&
            strcmp(fecha_registro, fecha_fin) <= 0) {
            strcat(respuesta, linea);

            // Si la respuesta está llena, enviarla y empezar una nueva
            if (strlen(respuesta) > 7000) {
                strcpy(sendBuff, respuesta);
                send(s, sendBuff, strlen(sendBuff), 0);
                memset(respuesta, 0, sizeof(respuesta));

                // Esperar confirmación antes de continuar
                recv(s, recvBuff, 512, 0);
            }
        }
    }

    // Enviar el resto de la respuesta si hay algo
    if (strlen(respuesta) > 0) {
        strcpy(sendBuff, respuesta);
        send(s, sendBuff, strlen(sendBuff), 0);
    } else {
        strcpy(sendBuff, "No se encontraron registros entre las fechas especificadas");
        send(s, sendBuff, strlen(sendBuff), 0);
    }

    fclose(archivo);
}

#include "gestorRegistros.h"

int registrarActividad(int id_usuario, const char* operacion, const char* detalles, const char* log_file) {
    FILE* archivo;
    RegistroActividad registro;
    time_t t;
    struct tm* tm_info;

    // Verificar parámetros
    if (operacion == NULL || detalles == NULL || log_file == NULL) {
        fprintf(stderr, "Error: Parámetros nulos en registrarActividad\n");
        return 0;
    }

    // Obtener fecha y hora actual
    time(&t);
    tm_info = localtime(&t);
    if (tm_info == NULL) {
        fprintf(stderr, "Error al obtener la hora local\n");
        return 0;
    }

    // Formatear fecha como YYYY-MM-DD HH:MM:SS
    strftime(registro.fecha, sizeof(registro.fecha), "%Y-%m-%d %H:%M:%S", tm_info);

    // Obtener el próximo ID
    registro.id = obtenerUltimoIdRegistro(log_file) + 1;
    registro.id_usuario = id_usuario;

    // Copiar operación y detalles (con límite de seguridad)
    strncpy(registro.operacion, operacion, sizeof(registro.operacion) - 1);
    registro.operacion[sizeof(registro.operacion) - 1] = '\0'; // Asegurar null-termination

    strncpy(registro.detalles, detalles, sizeof(registro.detalles) - 1);
    registro.detalles[sizeof(registro.detalles) - 1] = '\0'; // Asegurar null-termination

    // Intentar crear directorios si no existen (simplificado, debe expandirse según las necesidades)
    char dir_path[256] = {0};
    strncpy(dir_path, log_file, sizeof(dir_path)-1);

    // Encontrar última barra
    char* last_slash = strrchr(dir_path, '/');
    if (last_slash == NULL) last_slash = strrchr(dir_path, '\\');

    if (last_slash != NULL) {
        *last_slash = '\0'; // Truncar en la última barra

        // Crear directorio recursivamente (pseudocódigo, implementar según SO)
        // mkdir_recursive(dir_path);
    }

    // Abrir archivo en modo append
    archivo = fopen(log_file, "a");
    if (archivo == NULL) {
        fprintf(stderr, "Error al abrir el archivo de log: %s\n", log_file);
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

    if (log_file == NULL) {
        fprintf(stderr, "Error: Ruta de archivo nula en obtenerUltimoIdRegistro\n");
        return 0;
    }

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

void listarRegistros(SOCKET comm_socket, char *recvBuff, char *sendBuff) {
    FILE* archivo;
    char linea[512];
    char respuesta[8192] = "";
    const char* log_file = "actividad.log";
    int bytes_enviados;

    // Recibir parámetros adicionales si es necesario
    int bytesRecibidos = recv(comm_socket, recvBuff, 511, 0);
    if (bytesRecibidos > 0) {
        recvBuff[bytesRecibidos] = '\0';
    }

    archivo = fopen(log_file, "r");
    if (archivo == NULL) {
        strcpy(sendBuff, "ERROR|No se pudo abrir el archivo de registros");
        send(comm_socket, sendBuff, strlen(sendBuff), 0);
        return;
    }

    // Leer encabezados
    if (fgets(linea, sizeof(linea), archivo) != NULL) {
        strcat(respuesta, linea);
    }

    // Leer y enviar todos los registros
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // Verificar si añadir esta línea excedería el buffer
        if (strlen(respuesta) + strlen(linea) >= sizeof(respuesta) - 1) {
            // El buffer está casi lleno, enviar lo que tenemos y continuar
            strcpy(sendBuff, respuesta);
            bytes_enviados = send(comm_socket, sendBuff, strlen(sendBuff), 0);
            if (bytes_enviados == SOCKET_ERROR) {
                fprintf(stderr, "Error al enviar datos: %d\n", WSAGetLastError());
                fclose(archivo);
                return;
            }

            // Esperar confirmación antes de continuar
            bytesRecibidos = recv(comm_socket, recvBuff, 511, 0);
            if (bytesRecibidos <= 0) {
                fprintf(stderr, "Error o conexión cerrada al recibir confirmación\n");
                fclose(archivo);
                return;
            }
            recvBuff[bytesRecibidos] = '\0';

            // Reiniciar el buffer de respuesta
            memset(respuesta, 0, sizeof(respuesta));
        }

        // Añadir la línea al buffer de respuesta
        strcat(respuesta, linea);
    }

    // Enviar el resto de la respuesta si hay algo
    if (strlen(respuesta) > 0) {
        strcpy(sendBuff, respuesta);
        bytes_enviados = send(comm_socket, sendBuff, strlen(sendBuff), 0);
        if (bytes_enviados == SOCKET_ERROR) {
            fprintf(stderr, "Error al enviar datos finales: %d\n", WSAGetLastError());
        }
    } else {
        // Si no hay registros, enviar un mensaje adecuado
        strcpy(sendBuff, "INFO|No hay registros disponibles en el archivo de actividad.");
        send(comm_socket, sendBuff, strlen(sendBuff), 0);
    }

    fclose(archivo);
}


void buscarRegistrosPorUsuario(SOCKET s, char* recvBuff, char* sendBuff) {
    FILE* archivo;
    char linea[512];
    char respuesta[8192] = "";
    int id_usuario;
    const char* log_file = "actividad.log"; // Debería usar g_config.log_file
    int bytes_enviados;
    int registros_encontrados = 0;

    // Recibir el ID de usuario a buscar
    int bytes_recibidos = recv(s, recvBuff, 512, 0);
    if (bytes_recibidos <= 0) {
        strcpy(sendBuff, "ERROR: No se recibió el ID de usuario");
        send(s, sendBuff, strlen(sendBuff), 0);
        return;
    }
    recvBuff[bytes_recibidos] = '\0';
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
        if (sscanf(linea, "%d;%d;%19[^;];%49[^;];%254[^\n]",
               &id_registro, &id_usr, fecha, operacion, detalles) != 5) {
            // Si no se pueden extraer todos los valores, saltar esta línea
            continue;
        }

        if (id_usr == id_usuario) {
            registros_encontrados++;

            // Verificar si añadir esta línea excedería el buffer
            if (strlen(respuesta) + strlen(linea) >= sizeof(respuesta) - 1) {
                // El buffer está casi lleno, enviar lo que tenemos y continuar
                strcpy(sendBuff, respuesta);
                bytes_enviados = send(s, sendBuff, strlen(sendBuff), 0);
                if (bytes_enviados == SOCKET_ERROR) {
                    fprintf(stderr, "Error al enviar datos: %d\n", WSAGetLastError());
                    fclose(archivo);
                    return;
                }

                // Recibir confirmación antes de continuar
                bytes_recibidos = recv(s, recvBuff, 512, 0);
                if (bytes_recibidos <= 0) {
                    fprintf(stderr, "Error o conexión cerrada al recibir confirmación\n");
                    fclose(archivo);
                    return;
                }

                // Reiniciar el buffer de respuesta
                memset(respuesta, 0, sizeof(respuesta));
            }

            // Añadir la línea al buffer de respuesta
            strcat(respuesta, linea);
        }
    }

    // Enviar el resto de la respuesta si hay algo
    if (strlen(respuesta) > 0) {
        strcpy(sendBuff, respuesta);
        bytes_enviados = send(s, sendBuff, strlen(sendBuff), 0);
        if (bytes_enviados == SOCKET_ERROR) {
            fprintf(stderr, "Error al enviar datos finales: %d\n", WSAGetLastError());
        }
    } else {
        strcpy(sendBuff, "No se encontraron registros para el usuario especificado");
        send(s, sendBuff, strlen(sendBuff), 0);
    }

    fclose(archivo);
}

// Esta función convierte una cadena de fecha "YYYY-MM-DD" a una estructura tm
// Devuelve 1 si la conversión es exitosa, 0 en caso contrario
int str_to_date(const char* fecha_str, struct tm* fecha) {
    int year, month, day;

    if (sscanf(fecha_str, "%d-%d-%d", &year, &month, &day) != 3) {
        return 0;
    }

    // Inicializar la estructura
    memset(fecha, 0, sizeof(struct tm));
    fecha->tm_year = year - 1900; // tm_year es años desde 1900
    fecha->tm_mon = month - 1;    // tm_mon es 0-11
    fecha->tm_mday = day;

    // Validar fecha
    if (year < 1900 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31) {
        return 0;
    }

    return 1;
}

// Compara dos fechas en formato "YYYY-MM-DD"
// Devuelve: -1 si fecha1 < fecha2, 0 si fecha1 == fecha2, 1 si fecha1 > fecha2
int comparar_fechas(const char* fecha1_str, const char* fecha2_str) {
    struct tm fecha1, fecha2;
    time_t time1, time2;

    // Extraer sólo la parte de la fecha (los primeros 10 caracteres "YYYY-MM-DD")
    char fecha1_solo[11], fecha2_solo[11];
    strncpy(fecha1_solo, fecha1_str, 10);
    strncpy(fecha2_solo, fecha2_str, 10);
    fecha1_solo[10] = '\0';
    fecha2_solo[10] = '\0';

    // Convertir las cadenas a estructuras tm
    if (!str_to_date(fecha1_solo, &fecha1) || !str_to_date(fecha2_solo, &fecha2)) {
        return 0; // Error en conversión
    }

    // Convertir estructuras tm a time_t para comparar
    fecha1.tm_hour = 12; // Establecer mediodía para evitar problemas con cambios horarios
    fecha2.tm_hour = 12;
    time1 = mktime(&fecha1);
    time2 = mktime(&fecha2);

    if (time1 < time2) return -1;
    if (time1 > time2) return 1;
    return 0;
}

void obtenerRegistrosPorFecha(SOCKET s, char* recvBuff, char* sendBuff) {
    FILE* archivo;
    char linea[512];
    char respuesta[8192] = "";
    char fecha_inicio[20], fecha_fin[20];
    const char* log_file = "actividad.log"; // Debería usar g_config.log_file
    int bytes_enviados;
    int registros_encontrados = 0;

    // Recibir el formato de fechas: "YYYY-MM-DD YYYY-MM-DD" (inicio y fin)
    int bytes_recibidos = recv(s, recvBuff, 512, 0);
    if (bytes_recibidos <= 0) {
        strcpy(sendBuff, "ERROR: No se recibieron las fechas");
        send(s, sendBuff, strlen(sendBuff), 0);
        return;
    }

    recvBuff[bytes_recibidos] = '\0';
    if (sscanf(recvBuff, "%19s %19s", fecha_inicio, fecha_fin) != 2) {
        strcpy(sendBuff, "ERROR: Formato de fechas incorrecto. Use 'YYYY-MM-DD YYYY-MM-DD'");
        send(s, sendBuff, strlen(sendBuff), 0);
        return;
    }

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
        if (sscanf(linea, "%d;%d;%19[^;];%49[^;];%254[^\n]",
               &id_registro, &id_usr, fecha, operacion, detalles) != 5) {
            // Si no se pueden extraer todos los valores, saltar esta línea
            continue;
        }

        // Comparar fechas usando nuestra función mejorada
        if (comparar_fechas(fecha, fecha_inicio) >= 0 &&
            comparar_fechas(fecha, fecha_fin) <= 0) {

            registros_encontrados++;

            // Verificar si añadir esta línea excedería el buffer
            if (strlen(respuesta) + strlen(linea) >= sizeof(respuesta) - 1) {
                // El buffer está casi lleno, enviar lo que tenemos y continuar
                strcpy(sendBuff, respuesta);
                bytes_enviados = send(s, sendBuff, strlen(sendBuff), 0);
                if (bytes_enviados == SOCKET_ERROR) {
                    fprintf(stderr, "Error al enviar datos: %d\n", WSAGetLastError());
                    fclose(archivo);
                    return;
                }

                // Recibir confirmación antes de continuar
                bytes_recibidos = recv(s, recvBuff, 512, 0);
                if (bytes_recibidos <= 0) {
                    fprintf(stderr, "Error o conexión cerrada al recibir confirmación\n");
                    fclose(archivo);
                    return;
                }

                // Reiniciar el buffer de respuesta
                memset(respuesta, 0, sizeof(respuesta));
            }

            // Añadir la línea al buffer de respuesta
            strcat(respuesta, linea);
        }
    }

    // Enviar el resto de la respuesta si hay algo
    if (strlen(respuesta) > 0) {
        strcpy(sendBuff, respuesta);
        bytes_enviados = send(s, sendBuff, strlen(sendBuff), 0);
        if (bytes_enviados == SOCKET_ERROR) {
            fprintf(stderr, "Error al enviar datos finales: %d\n", WSAGetLastError());
        }
    } else {
        strcpy(sendBuff, "No se encontraron registros entre las fechas especificadas");
        send(s, sendBuff, strlen(sendBuff), 0);
    }

    fclose(archivo);
}

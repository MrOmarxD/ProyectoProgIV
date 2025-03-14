#include "db_initializer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/stat.h>

static int file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    return 0;
}

static int execute_sql_script(sqlite3 *db, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "No se pudo abrir el archivo de script SQL: %s\n", filename);
        return -1;
    }

    // Leer el contenido del archivo
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *sql = (char *)malloc(file_size + 1);
    if (sql == NULL) {
        fprintf(stderr, "Error de memoria al leer el script SQL\n");
        fclose(file);
        return -1;
    }

    size_t read_size = fread(sql, 1, file_size, file);
    fclose(file);

    if (read_size != file_size) {
        fprintf(stderr, "Error al leer el archivo SQL\n");
        free(sql);
        return -1;
    }

    sql[file_size] = '\0';

    // Ejecutar el script SQL
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
        free(sql);
        return -1;
    }

    free(sql);
    return 0;
}

int initialize_database(const char *db_path, const char *script_path) {
    sqlite3 *db;
    int rc;
    int db_exists = file_exists(db_path);

    // Abrir la conexión a la base de datos (la crea si no existe)
    rc = sqlite3_open(db_path, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Si la base de datos no existía previamente, ejecutar el script de inicialización
    if (!db_exists) {
        printf("La base de datos no existe. Creándola con el script de inicialización...\n");

        if (execute_sql_script(db, script_path) != 0) {
            fprintf(stderr, "Error al ejecutar el script de inicialización.\n");
            sqlite3_close(db);
            return -1;
        }

        printf("Base de datos inicializada correctamente.\n");
    } else {
        printf("La base de datos ya existe.\n");
    }

    // Verificar que la base de datos tiene la estructura correcta
    const char *verify_sql = "SELECT count(*) FROM sqlite_master WHERE type='table' AND name IN ('usuarios', 'clientes', 'empleados', 'habitaciones', 'reservas', 'facturacion', 'registros_actividad');";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, verify_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al preparar la consulta de verificación: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        if (count != 7) {
            fprintf(stderr, "La estructura de la base de datos no es correcta (se encontraron %d de 7 tablas).\n", count);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return -1;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

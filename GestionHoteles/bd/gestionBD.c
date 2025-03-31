#include "gestionBD.h"

int abrirBd(){
	sqlite3 *db;
	int result = sqlite3_open("hotelesBD.sqlite", &db);
    if (result != SQLITE_OK) {
        printf("Error al abrir la BBDD\n");
        fflush(stdout);
        return result;
    }
    printf("BBDD abierta\n");
    fflush(stdout);
}

int cerrarBd(){
	sqlite3 *db;
	int result = sqlite3_close(db);
    if (result != SQLITE_OK) {
        printf("Error al cerrar la BBDD\n");
        printf("%s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return result;
    }

    printf("BBDD cerrada\n");
    fflush(stdout);
}






#include "gestionBD.h"

sqlite3 *db;
sqlite3_stmt *stmt;
int result;

int abrirBd(){

	result = sqlite3_open("hotelesBD.sqlite", &db);
    if (result != SQLITE_OK) {
        printf("Error al abrir la BBDD\n");
        fflush(stdout);
        return result;
    }
    printf("BBDD abierta\n");
    fflush(stdout);
}

int cerrarBd(){
	result = sqlite3_close(db);
    if (result != SQLITE_OK) {
        printf("Error al cerrar la BBDD\n");
        printf("%s\n", sqlite3_errmsg(db));
        fflush(stdout);
        return result;
    }

    printf("BBDD cerrada\n");
    fflush(stdout);
}

void listaUsuarios(){
	char sql2[] = "select nombre from usuarios";

		sqlite3_prepare_v2(db, sql2, strlen(sql2), &stmt, NULL) ;

		printf("\n");
		do {
			result = sqlite3_step(stmt);
			if (result == SQLITE_ROW) {
				printf("%s\n", (char*) sqlite3_column_text(stmt, 0));
			}
		} while (result == SQLITE_ROW);
		printf("\n");

		sqlite3_finalize(stmt);
}






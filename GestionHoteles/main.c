#include <stdio.h>
#include "sqlite3.h"
#include <string.h>

int main() {
	sqlite3 *db;

	int result = sqlite3_open("hoteles.sqlite", &db);
	if (result != SQLITE_OK) {
		printf("Error al abrir la BBDD\n");
		return result;
	}

	printf("BBDD abierta\n");



	result = sqlite3_close(db);
	if (result != SQLITE_OK) {
		printf("Error al abrir la BBDD\n");
		printf("%s\n", sqlite3_errmsg(db));
		return result;
	}

	printf("BBDD cerrada\n") ;
	return 0;
}

#ifndef GESTIONBD_H_
#define GESTIONBD_H_

#include "sqlite3.h"
#include <stdio.h>
#include <string.h>
#include "../modulos/gestorUsuarios.h"

int abrirBd();
int cerrarBd();
void crearUsuario(Usuario *user);




#endif /* GESTIONBD_H_ */

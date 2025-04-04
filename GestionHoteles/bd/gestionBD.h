#ifndef GESTIONBD_H_
#define GESTIONBD_H_

#include "sqlite3.h"
#include <stdio.h>
#include <string.h>
#include "../modulos/gestorUsuarios.h"

int abrirBd();
int cerrarBd();
int comprobarUsuario(const char *usuario);
void crearUsuarioBD(Usuario *user);
void modificarUsuarioBD(Usuario *user);
void listaUsuarios();
void eliminarUsuarioBD();
int comprobarUsuario(const char *usuario);
int recuperarUsuarioBD(const char *nombreUsuario, Usuario *user);

#endif /* GESTIONBD_H_ */

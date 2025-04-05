#ifndef GESTIONBD_H_
#define GESTIONBD_H_

#include "sqlite3.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../modulos/gestorUsuarios.h"

void abrirBd();
void cerrarBd();

int comprobarUsuario(const char *usuario);
void crearUsuarioBD(Usuario *user);
void modificarUsuarioBD(Usuario *user);
void listaUsuarios();
void eliminarUsuarioBD();
int comprobarUsuario(const char *usuario);
int recuperarUsuarioBD(const char *nombreUsuario, Usuario *user);

void crearClienteBD(Cliente *client);
int comprobarCliente(const char *cliente);
void modificarClienteBD(Cliente *client);
int recuperarClienteBD(const char *dniCliente, Cliente *client);
void listaClientes();
void buscarClientesBD(const char *dniCliente);


#endif /* GESTIONBD_H_ */

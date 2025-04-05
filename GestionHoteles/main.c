#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bd/gestionBD.h"
#include "modulos/gestorUsuarios.h"
//#include "modulos/gestorClientes.h"
//#include "modulos/gestorPersonal.h"
#include "modulos/gestorHabitaciones.h"
#include "modulos/gestorReservas.h"
#include "modulos/gestorFacturas.h"
#include "modulos/gestorRegistros.h"
#include "modulos/gestorMenus.h"



/* Funciones para manejo de archivos */




int main() {

	abrirBd();

	eleccionInicial();

    cerrarBd();
    return 0;
}






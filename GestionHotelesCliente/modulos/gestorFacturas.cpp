#include "gestorFacturas.h"
//#include "gestorRegistros.h"
#include "gestorMenus.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

using namespace std;

void gestionFacturacion(SOCKET s) {
    int opcion;
    cout<<"\n--- FACTURACION ---\n";
    cout<<"1. Generar nueva factura\n";
    cout<<"2. Buscar factura\n";
    cout<<"0. Volver al menu principal\n";
    cout<<"Seleccione una opcion: "<<endl;
    cin >> opcion;
    
    switch (opcion) {
    case 1:
        cout<<"Generar nueva factura\n"<<endl;
        //generarNuevaFactura(&usuario_actual);
        break;
    case 2:
        //buscarFactura(&usuario_actual);
        break;
    case 0:
        mostrarMenuPrincipal(s);
        break;
    default:
        cout<<"Opcion no valida. Intente nuevamente.\n"<<endl;
    }

    //registrarActividad(usuario_actual, "Acceso a facturacion", LOG_FILE);
}

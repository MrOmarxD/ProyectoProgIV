#ifndef MGESTORPERSONAL_H_
#define GESTORPERSONAL_H_

typedef struct {
    int id;
    char nombre[50];
    char cargo[30];
    char turno[20];
    float salario;
} Personal;

void gestionPersonal(int usuario_actual, char LOG_FILE);

#endif /* MODULOS_GESTORPERSONAL_H_ */

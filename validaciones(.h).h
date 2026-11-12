#ifndef VALIDACIONES_H
#define VALIDACIONES_H

int leer_entero_rango(const char *mensaje,
                      int minimo,
                      int maximo,
                      int *valor);

int leer_real_rango(const char *mensaje,
                    double minimo,
                    double maximo,
                    double *valor);

int leer_caracter_opciones(const char *mensaje,
                           const char *opciones,
                           char *caracter);

#endif

#include "validaciones.h"
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int leer_linea(char *buffer, size_t tamanio)
{
    size_t longitud;
    int caracter;

    if (fgets(buffer, (int)tamanio, stdin) == NULL) {
        return 0;
    }

    longitud = strlen(buffer);

    if (longitud > 0 && buffer[longitud - 1] == '\n') {
        buffer[longitud - 1] = '\0';

        longitud = strlen(buffer);
        if (longitud > 0 && buffer[longitud - 1] == '\r') {
            buffer[longitud - 1] = '\0';
        }

        return 1;
    }

    if (feof(stdin)) {
        return 1;
    }

    while ((caracter = getchar()) != '\n' && caracter != EOF) {
    }

    return -1;
}

int leer_entero_rango(const char *mensaje,
                      int minimo,
                      int maximo,
                      int *valor)
{
    char linea[128];
    char *fin;
    long numero;
    int estado;

    if (mensaje == NULL || valor == NULL || minimo > maximo) {
        return 0;
    }

    while (1) {
        printf("%s", mensaje);

        estado = leer_linea(linea, sizeof(linea));

        if (estado == 0) {
            return 0;
        }

        if (estado == -1) {
            printf("Entrada demasiado larga. Intente nuevamente.\n");
            continue;
        }

        errno = 0;
        numero = strtol(linea, &fin, 10);

        while (isspace((unsigned char)*fin)) {
            fin++;
        }

        if (linea == fin || errno == ERANGE || *fin != '\0') {
            printf("Entrada invalida. Ingrese un numero entero.\n");
            continue;
        }

        if (numero < minimo || numero > maximo) {
            printf("El valor debe estar entre %d y %d.\n", minimo, maximo);
            continue;
        }

        *valor = (int)numero;
        return 1;
    }
}

int leer_real_rango(const char *mensaje,
                    double minimo,
                    double maximo,
                    double *valor)
{
    char linea[128];
    char *fin;
    double numero;
    int estado;

    if (mensaje == NULL || valor == NULL || minimo > maximo) {
        return 0;
    }

    while (1) {
        printf("%s", mensaje);

        estado = leer_linea(linea, sizeof(linea));

        if (estado == 0) {
            return 0;
        }

        if (estado == -1) {
            printf("Entrada demasiado larga. Intente nuevamente.\n");
            continue;
        }

        errno = 0;
        numero = strtod(linea, &fin);

        while (isspace((unsigned char)*fin)) {
            fin++;
        }

        if (linea == fin || errno == ERANGE ||
            !isfinite(numero) || *fin != '\0') {
            printf("Entrada invalida. Ingrese un numero real. Use punto decimal.\n");
            continue;
        }

        if (numero < minimo || numero > maximo) {
            printf("El valor debe estar entre %.2f y %.2f.\n",
                   minimo,
                   maximo);
            continue;
        }

        *valor = numero;
        return 1;
    }
}


int leer_caracter_opciones(const char *mensaje,
                           const char *opciones,
                           char *caracter)
{
    char linea[128];
    char *inicio;
    char *fin;
    int estado;

    if (mensaje == NULL || opciones == NULL || caracter == NULL) {
        return 0;
    }

    while (1) {
        printf("%s", mensaje);

        estado = leer_linea(linea, sizeof(linea));

        if (estado == 0) {
            return 0;
        }

        if (estado == -1) {
            printf("Entrada demasiado larga. Intente nuevamente.\n");
            continue;
        }

        inicio = linea;
        while (isspace((unsigned char)*inicio)) {
            inicio++;
        }

        fin = inicio + strlen(inicio);
        while (fin > inicio && isspace((unsigned char)fin[-1])) {
            fin--;
        }

        if (fin - inicio != 1) {
            printf("Ingrese solamente un caracter.\n");
            continue;
        }

        if (strchr(opciones, *inicio) == NULL) {
            printf("Opcion no permitida. Valores validos: %s\n", opciones);
            continue;
        }

        *caracter = *inicio;
        return 1;
    }
}

#include <stdio.h>
#include "Artemis.h"
#include "validaciones.h"

static void modificar_parametros(ParametrosAscenso *params)
{
    ParametrosAscenso nuevos = *params;
    EstadoNave prueba;
    char respuesta;

    if (!leer_caracter_opciones("\n¿Restaurar los valores por defecto de la consigna? (s/n): ",
                                "sSnN", &respuesta)) {
        printf("\nError en la lectura. No se modificaron los parametros.\n");
        return;
    }

    if (respuesta == 's' || respuesta == 'S') {
        cargar_parametros_por_defecto(params);
        printf("\nSe restauraron los valores por defecto.\n");
        return;
    }

    printf("\n--Ingreso de parametros (use punto decimal)--\n");

    if (!leer_real_rango("Aceleracion Fase 1 (SRB) [0.1 - 100] m/s²: ",
                         0.1, 100.0, &nuevos.aceleracion_fase1) ||
        !leer_real_rango("Tiempo de desacople SRB [0 - 600] s: ",
                         0.0, 600.0, &nuevos.tiempo_desacople_srb) ||
        !leer_real_rango("Aceleracion Fase 2 (Nucleo) [0.1 - 100] m/s²: ",
                         0.1, 100.0, &nuevos.aceleracion_fase2) ||
        !leer_real_rango("Altura objetivo [1 - 10000000] m: ",
                         1.0, 10000000.0, &nuevos.altura_objetivo) ||
        !leer_real_rango("Gravedad [0.1 - 50] m/s²: ",
                         0.1, 50.0, &nuevos.gravedad)) {
        printf("\nError en la lectura. No se modificaron los parametros.\n");
        return;
    }


    if (!calcular_fase_srb(&nuevos, &prueba)) {
        printf("\nCon esos valores la nave supera la altura objetivo antes del desacople.\n");
        printf("Se mantienen los parametros anteriores.\n");
        return;
    }

    *params = nuevos;
    printf("\nParametros actualizados correctamente.\n");
}

int main() {
    int opcion;
    ParametrosAscenso params;
    ResultadosAscenso resultados;

    cargar_parametros_por_defecto(&params);

    do {
        printf("\n------------------------------------------\n");  
        printf(" SIMULADOR DE MISIONES ARTEMIS - SLS\n");
        printf("1. Consultar constantes \n");
        printf("2. Calcular fase de ascenso y desacople SRB\n");
        printf("3. Calcular velocidad y altitud\n");
        printf("4. Simulación completa (caída libre hipotética)\n");
        printf("5. Modificar parámetros\n");
        printf("6. Salir\n");
        if (!leer_entero_rango("Seleccione una opción: ", 1, 6, &opcion)) {
            printf("\nError en la lectura. Saliendo...\n");
            break;
        }

        switch (opcion) {
            case 1:
                printf("\n--Constantes de trabajo--\n");
                printf("Aceleracion Fase 1 (SRB): %.2f m/s²\n", params.aceleracion_fase1);
                printf("Tiempo Desacople SRB: %.2f s\n", params.tiempo_desacople_srb);
                printf("Aceleracion Fase 2 (Nucleo): %.2f m/s²\n", params.aceleracion_fase2);
                printf("Altura Objetivo: %.2f m\n", params.altura_objetivo);
                printf("Gravedad Terrestre: %.2f m/s²\n", params.gravedad);
                getchar();
                break;

            case 2:
                printf("\nCalculando Fase 1\n");
                if (calcular_fase_srb(&params, &resultados.estado_desacople)) {
                    printf("-> Tiempo de desacople: %.2f s\n", resultados.estado_desacople.tiempo_s);
                    printf("-> Altura alcanzada: %.2f m\n", resultados.estado_desacople.altura_m);
                    printf("-> Velocidad alcanzada: %.2f m/s\n", resultados.estado_desacople.velocidad_m_s);
                } else {
                    printf("Error en el cálculo de la Fase 1.\n");
                }
                getchar();
                break;

            case 3:
                printf("\nCalculando Velocidad y altitud máxima ");
                if (calcular_fase_srb(&params, &resultados.estado_desacople) &&
                    calcular_fase_nucleo(&params, &resultados.estado_desacople, &resultados.estado_apagado)) {

                    printf("\n-> Altura de apagado de motor: %.2f m\n", resultados.estado_apagado.altura_m);
                    printf("-> Velocidad de apagado de motor: %.2f m/s\n", resultados.estado_apagado.velocidad_m_s);

                    if (calcular_altura_maxima(&params, &resultados.estado_apagado, &resultados.altura_maxima)) {
                        printf("-> Altitud máxima total: %.2f m\n", resultados.altura_maxima);
                    }
                } else {
                    printf("\nError en los cálculos de velocidad y altitud.\n");
                }
                getchar();
                break;

            case 4:
                printf("\nRealizando simulación hipotética\n");
                if (calcular_simulacion_completa(&params, &resultados)) {
                    printf("¡Simulación completa exitosa!\n");
                    printf("-> Altura Máxima Final: %.2f m\n", resultados.altura_maxima);
                    printf("-> Aceleración de la tripulación: %.2f g\n", resultados.aceleracion_tripulacion_g);
                } else {
                    printf("Error al ejecutar la simulación hipotética.\n");
                }
                getchar();
                break;

            case 5:
                modificar_parametros(&params);          
                getchar();
                break;

            case 6:
                printf("\nSaliendo de la simulación\n");
                getchar();
                break;

            default:
                printf("\nOpcion no válida, seleccionar una opcion disponible en el menú principal\n");
                getchar();
                break;
        }

    } while (opcion != 6);

    return 0;
}

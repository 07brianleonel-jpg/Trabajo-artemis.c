
#ifndef ARTEMIS_H
#define ARTEMIS_H

#include <stdbool.h>


/* Estado inicial de la nave */
#define VELOCIDAD_INICIAL 0.0       // m/s
#define ALTURA_INICIAL 0.0          // m

/* Primera fase: motores principales + SRB */
#define ACELERACION_FASE1 20.0      // m/s²
#define TIEMPO_DESACOPLE_SRB 120.0  // segundos

/* Segunda fase: motor del núcleo principal */
#define ACELERACION_FASE2 12.5      // m/s²
#define ALTURA_OBJETIVO 160000.0    // metros

/* Gravedad terrestre */
#define GRAVEDAD_TERRESTRE 9.80     // m/s²


/* Representa el estado de movimiento de la nave */
typedef struct {
    double altura_m;
    double velocidad_m_s;
    double tiempo_s;
} EstadoNave;


/* Almacena los parámetros de la simulación */
typedef struct {
    EstadoNave estado_inicial;

    double aceleracion_fase1;
    double tiempo_desacople_srb;

    double aceleracion_fase2;
    double altura_objetivo;

    double gravedad;
} ParametrosAscenso;


/* Almacena los resultados obtenidos */
typedef struct {
    EstadoNave estado_desacople;
    EstadoNave estado_apagado;

    double altura_maxima;
    double aceleracion_tripulacion_g;
} ResultadosAscenso;


/* Inicializa los parámetros con los valores de la consigna */
void cargar_parametros_por_defecto(
    ParametrosAscenso *parametros
);

/* Calcula velocidad y altura al desprenderse los SRB */
bool calcular_fase_srb(
    const ParametrosAscenso *parametros,
    EstadoNave *estado_desacople
);

/* Calcula el movimiento de la segunda fase */
bool calcular_fase_nucleo(
    const ParametrosAscenso *parametros,
    const EstadoNave *estado_desacople,
    EstadoNave *estado_apagado
);

/* Calcula la altura máxima luego del apagado de motores */
bool calcular_altura_maxima(
    const ParametrosAscenso *parametros,
    const EstadoNave *estado_apagado,
    double *altura_maxima
);

/* Calcula la aceleración como múltiplo de la gravedad */
double calcular_aceleracion_en_g(
    double aceleracion,
    double gravedad
);

/* Ejecuta y reúne todos los cálculos de la simulación */
bool calcular_simulacion_completa(
    const ParametrosAscenso *parametros,
    ResultadosAscenso *resultados
);

#endif

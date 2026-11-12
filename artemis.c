#include <stddef.h>
#include <math.h>
#include "artemis.h"

/* Revisa que los parametros sean fisicamente validos.
   Es "static": solo se usa dentro de este archivo. */
static bool parametros_validos(const ParametrosAscenso *p)
{
    if (p == NULL) return false;
    if (p->aceleracion_fase1 <= 0.0) return false;
    if (p->aceleracion_fase2 <= 0.0) return false;
    if (p->tiempo_desacople_srb < 0.0) return false;
    if (p->gravedad <= 0.0) return false;
    if (p->altura_objetivo <= p->estado_inicial.altura_m) return false;
    if (p->estado_inicial.velocidad_m_s < 0.0) return false;
    return true;
}

void cargar_parametros_por_defecto(ParametrosAscenso *parametros)
{
    if (parametros == NULL) return;

    parametros->estado_inicial.altura_m = ALTURA_INICIAL;
    parametros->estado_inicial.velocidad_m_s = VELOCIDAD_INICIAL;
    parametros->estado_inicial.tiempo_s = 0.0;

    parametros->aceleracion_fase1 = ACELERACION_FASE1;
    parametros->tiempo_desacople_srb = TIEMPO_DESACOPLE_SRB;
    parametros->aceleracion_fase2 = ACELERACION_FASE2;
    parametros->altura_objetivo = ALTURA_OBJETIVO;
    parametros->gravedad = GRAVEDAD_TERRESTRE;
}

/* FASE 1 (MRUV): v = v0 + a*t   y = y0 + v0*t + 1/2*a*t^2 */
bool calcular_fase_srb(const ParametrosAscenso *parametros, EstadoNave *estado_desacople)
{
    if (!parametros_validos(parametros) || estado_desacople == NULL) return false;

    double v0 = parametros->estado_inicial.velocidad_m_s;
    double y0 = parametros->estado_inicial.altura_m;
    double a  = parametros->aceleracion_fase1;
    double t  = parametros->tiempo_desacople_srb;

    estado_desacople->velocidad_m_s = v0 + a * t;
    estado_desacople->altura_m = y0 + v0 * t + 0.5 * a * t * t;
    estado_desacople->tiempo_s = parametros->estado_inicial.tiempo_s + t;

    /* Si ya paso la altura objetivo antes del desacople, no tiene sentido seguir */
    if (estado_desacople->altura_m >= parametros->altura_objetivo) return false;

    return true;
}

/* FASE 2: v2^2 = v1^2 + 2*a2*(h - y1)   y   dt = (v2 - v1)/a2 */
bool calcular_fase_nucleo(const ParametrosAscenso *parametros,
                          const EstadoNave *estado_desacople,
                          EstadoNave *estado_apagado)
{
    if (!parametros_validos(parametros) || estado_desacople == NULL || estado_apagado == NULL) return false;
    if (estado_desacople->altura_m >= parametros->altura_objetivo) return false;

    double v1 = estado_desacople->velocidad_m_s;
    double a2 = parametros->aceleracion_fase2;
    double distancia = parametros->altura_objetivo - estado_desacople->altura_m;

    double v2 = sqrt(v1 * v1 + 2.0 * a2 * distancia);

    estado_apagado->velocidad_m_s = v2;
    estado_apagado->altura_m = parametros->altura_objetivo;
    estado_apagado->tiempo_s = estado_desacople->tiempo_s + (v2 - v1) / a2;

    return true;
}

/* FASE 3 (caida libre hacia arriba): h_max = h + v^2 / (2*g) */
bool calcular_altura_maxima(const ParametrosAscenso *parametros,
                            const EstadoNave *estado_apagado,
                            double *altura_maxima)
{
    if (!parametros_validos(parametros) || estado_apagado == NULL || altura_maxima == NULL) return false;

    double v = estado_apagado->velocidad_m_s;
    *altura_maxima = estado_apagado->altura_m + (v * v) / (2.0 * parametros->gravedad);

    return true;
}

/* Devuelve a/g. Si la gravedad es invalida devuelve -1.0 como senal de error. */
double calcular_aceleracion_en_g(double aceleracion, double gravedad)
{
    if (gravedad <= 0.0) return -1.0;
    return aceleracion / gravedad;
}

bool calcular_simulacion_completa(const ParametrosAscenso *parametros, ResultadosAscenso *resultados)
{
    if (parametros == NULL || resultados == NULL) return false;

    if (!calcular_fase_srb(parametros, &resultados->estado_desacople)) return false;
    if (!calcular_fase_nucleo(parametros, &resultados->estado_desacople, &resultados->estado_apagado)) return false;
    if (!calcular_altura_maxima(parametros, &resultados->estado_apagado, &resultados->altura_maxima)) return false;

    /* La aceleracion maxima es la mayor entre las dos fases */
    double a_max = parametros->aceleracion_fase1;
    if (parametros->aceleracion_fase2 > a_max) a_max = parametros->aceleracion_fase2;

    resultados->aceleracion_tripulacion_g = calcular_aceleracion_en_g(a_max, parametros->gravedad);

    return true;
}
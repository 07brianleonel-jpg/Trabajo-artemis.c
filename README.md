# Trabajo-artemis.c

Simulador en C de la fase de ascenso de una misión **Artemis** con el cohete **SLS** (Space Launch System). El programa calcula, mediante ecuaciones de cinemática, cómo evoluciona la altura y la velocidad de la nave durante el despegue, el desacople de los propulsores laterales (SRB) y el vuelo posterior al apagado de motores.

## Funcionalidades

El programa presenta un menú interactivo con las siguientes opciones:

1. **Consultar constantes**: muestra los parámetros de trabajo de la simulación.
2. **Calcular fase de ascenso y desacople SRB**: obtiene el tiempo, la altura y la velocidad en el momento del desacople.
3. **Calcular velocidad y altitud**: calcula la altura y la velocidad al apagarse el motor del núcleo, y la altitud máxima alcanzada.
4. **Simulación completa**: ejecuta todas las fases y calcula además la aceleración máxima que soporta la tripulación, expresada en g.
5. **Salir**

## Modelo físico

| Fase | Descripción | Ecuaciones |
|------|-------------|------------|
| 1 | Motores principales + SRB (MRUV) | `v = v0 + a·t` , `y = y0 + v0·t + ½·a·t²` |
| 2 | Motor del núcleo hasta la altura objetivo | `v2² = v1² + 2·a2·(h − y1)` , `Δt = (v2 − v1) / a2` |
| 3 | Ascenso balístico tras el apagado | `h_max = h + v² / (2·g)` |

## Constantes utilizadas

| Parámetro | Valor |
|-----------|-------|
| Aceleración fase 1 (SRB) | 20,0 m/s² |
| Tiempo de desacople SRB | 120,0 s |
| Aceleración fase 2 (núcleo) | 12,5 m/s² |
| Altura objetivo | 160 000 m |
| Gravedad terrestre | 9,80 m/s² |

Los valores se definen en `Artemis.h` y se pueden modificar desde ahí.

## Estructura del proyecto

```
├── main.c            # Menú principal e interacción con el usuario
├── Artemis.h         # Constantes, estructuras de datos y prototipos
├── artemis.c         # Cálculos físicos de cada fase
├── validaciones.h    # Prototipos de las funciones de validación
├── validaciones.c    # Lectura segura de enteros, reales y caracteres
└── README.md
```

- **Artemis.h / artemis.c**: definen las estructuras `EstadoNave`, `ParametrosAscenso` y `ResultadosAscenso`, e implementan los cálculos de cada fase. Antes de calcular, verifican que los parámetros sean físicamente válidos.
- **validaciones.h / validaciones.c**: leen la entrada del usuario de forma segura, rechazan valores fuera de rango, texto no numérico o entradas demasiado largas, y vuelven a pedir el dato cuando es necesario.

## Compilación y ejecución

Se necesita un compilador de C, por ejemplo GCC.

```bash
gcc main.c artemis.c validaciones.c -o artemis_sim -lm
```

En Windows:

```bash
artemis_sim.exe
```

## Autores
kibish, David Nicolás
Zarate, Mia Evelyn
Espindola, Brian Leonel
Romeli, morales

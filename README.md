# Trabajo-artemis.c

## Autores
kibish, David Nicolás /
Zarate, Mia Evelyn /
Espindola, Brian Leonel /
Morales, Romeli /

## Comisión k1.2 grupo B12

Simulador en C de la fase de ascenso de una misión Artemis con el cohete SLS (Space Launch System). El programa calcula, mediante ecuaciones de cinemática, cómo evoluciona la altura y la velocidad de la nave durante el despegue, el desacople de los propulsores laterales (SRB) y el vuelo posterior al apagado de motores.

## Escenario Artemis asignado

**Problema 1: Ascenso y propulsión con el SLS (MRUV y desprendimiento)**

Durante el despegue desde el Centro Espacial Kennedy, el cohete SLS eleva la nave Orion partiendo del reposo (`v0 = 0,0 m/s`) desde la plataforma de lanzamiento (`y0 = 0,0 m`). El vuelo se divide en tres etapas:

1. **Primera fase (motores principales + SRB):** el cohete asciende en línea recta y en vertical, con una aceleración constante de `a1 = 20,0 m/s²`, impulsado por los motores principales y los dos cohetes de combustible sólido (SRB).
2. **Desacople de los SRB:** a los `t1 = 120,0 s` (2 minutos) de vuelo, los SRB agotados se desprenden limpiamente.
3. **Segunda fase (motor del núcleo):** desde ese momento, el motor del núcleo principal sigue acelerando la nave a `a2 = 12,5 m/s²` hasta una altitud de `h = 160 000 m` (160 km). Ahí los motores se apagan.
4. **Escenario hipotético de falla:** si los motores fallaran al llegar a los 160 km, la nave quedaría en caída libre, frenada solo por la gravedad (`g = 9,80 m/s²`). Seguiría subiendo hasta alcanzar una altitud máxima y después empezaría a caer.


### Resultados esperados con los valores de la consigna

|                Magnitud                |   Resultado    |
|----------------------------------------|----------------|
| Velocidad al desacople `v1`            | 2 400,00 m/s   |
| Altitud al desacople `y1`              | 144 000,00 m   |
| Velocidad al apagado `v2`              | ≈ 2 481,93 m/s |
| Tiempo total hasta el apagado          | ≈ 126,55 s     |
| Altitud máxima `h_max`                 | ≈ 474 285,71 m |
| Aceleración máxima de la tripulación   | ≈ 2,04 g       |

## Reglas del juego

### Hipótesis del modelo

- El movimiento es rectilíneo y vertical en todas las fases.
- Las aceleraciones son constantes dentro de cada fase (MRUV).
- No se tiene en cuenta la resistencia del aire (se supone la alta atmósfera).
- La gravedad se considera constante e igual a 9,80 m/s². En las fases propulsadas, las aceleraciones dadas ya son las aceleraciones netas de la nave.
- El desacople de los SRB es instantáneo: la velocidad y la altura al final de la fase 1 son las condiciones iniciales de la fase 2.

### Uso del programa

- Se navega con un menú numérico. Solo se aceptan números enteros del 1 al 6.
- Al iniciar, el programa usa los valores de la consigna. Con la opción 5 el usuario puede ingresar sus propios valores para analizar otros escenarios, o restaurar los originales respondiendo `s`/`n`.
- Los valores reales se escriben con punto decimal (por ejemplo `12.5`) y deben estar dentro de estos rangos:

|      Parámetro      |  Rango permitido |
|---------------------|------------------|
| Aceleración fase 1  | 0,1 a 100 m/s²   |
| Tiempo de desacople | 0 a 600 s        |
| Aceleración fase 2  | 0,1 a 100 m/s²   |
| Altura objetivo     | 1 a 10 000 000 m |
| Gravedad            | 0,1 a 50 m/s²    |

- Si el usuario escribe letras, varios valores, un número fuera de rango o un texto demasiado largo, el programa muestra un mensaje de error y vuelve a pedir el dato. No se cierra ni se rompe.
- Si los valores ingresados son incoherentes (por ejemplo, la nave supera la altura objetivo antes del desacople), se rechazan y se mantienen los parámetros anteriores.
- Después de mostrar cada resultado, el programa espera que se presione Enter para volver al menú.
- La opción 6 termina la simulación.

### Condiciones de validez de los parámetros

Antes de calcular, el programa comprueba que los parámetros sean físicamente posibles. Si alguno no lo es, muestra un mensaje de error en lugar de un resultado:

- Las aceleraciones `a1` y `a2` deben ser mayores que 0.
- El tiempo de desacople debe ser mayor o igual que 0.
- La gravedad debe ser mayor que 0.
- La velocidad inicial no puede ser negativa.
- La altura objetivo debe ser mayor que la altura inicial.
- La altura alcanzada al desacople de los SRB debe ser menor que la altura objetivo. Si la nave ya superara los 160 km durante la primera fase, la segunda no tendría sentido y el cálculo se rechaza.

## Funcionalidades

El programa presenta un menú interactivo con las siguientes opciones:

1. **Consultar constantes**: muestra los parámetros de trabajo de la simulación.
2. **Calcular fase de ascenso y desacople SRB**: obtiene el tiempo, la altura y la velocidad en el momento del desacople.
3. **Calcular velocidad y altitud**: calcula la altura y la velocidad al apagarse el motor del núcleo, y la altitud máxima alcanzada.
4. **Simulación completa (caída libre hipotética)**: ejecuta todas las fases y calcula además la aceleración máxima que soporta la tripulación, expresada en g.
5. **Modificar parámetros**: permite ingresar valores propios (aceleraciones, tiempo de desacople, altura objetivo y gravedad) o restaurar los valores por defecto de la consigna.
6. **Salir**

## Modelo físico

| Fase |            Descripción                    |                    Ecuaciones                       |
|------|-------------------------------------------|-----------------------------------------------------|
| 1    | Motores principales + SRB (MRUV)          | `v = v0 + a·t` , `y = y0 + v0·t + ½·a·t²`           |
| 2    | Motor del núcleo hasta la altura objetivo | `v2² = v1² + 2·a2·(h − y1)` , `Δt = (v2 − v1) / a2` |
| 3    | Ascenso balístico tras el apagado         | `h_max = h + v² / (2·g)`                            |

## Constantes utilizadas

|         Parámetro           |   Valor   |
|-----------------------------|-----------|
| Aceleración fase 1 (SRB)    | 20,0 m/s² |
| Tiempo de desacople SRB     | 120,0 s   |
| Aceleración fase 2 (núcleo) | 12,5 m/s² |
| Altura objetivo             | 160 000 m |
| Gravedad terrestre          | 9,80 m/s² |

Estos son los valores por defecto, definidos en `Artemis.h`. Durante la ejecución se pueden cambiar con la opción 5 del menú.

## Estructura del proyecto

```
├── main.c            # Menú principal, ingreso de parámetros e interacción con el usuario
├── Artemis.h         # Constantes, estructuras de datos y prototipos
├── artemis.c         # Cálculos físicos de cada fase
├── validaciones.h    # Prototipos de las funciones de validación
├── validaciones.c    # Lectura segura de enteros, reales y caracteres
└── README.md
```

- **Artemis.h / artemis.c**: definen las estructuras `EstadoNave`, `ParametrosAscenso` y `ResultadosAscenso`, e implementan los cálculos de cada fase. Antes de calcular, verifican que los parámetros sean físicamente válidos.
- **validaciones.h / validaciones.c**: leen la entrada del usuario de forma segura, rechazan valores fuera de rango, texto no numérico o entradas demasiado largas, y vuelven a pedir el dato cuando es necesario.

## Compilación y ejecución:

Se necesita un compilador de C, por ejemplo GCC.

compilar: gcc main.c artemis.c validaciones.c -o artemis_sim.exe

Ejecutar el archivo artemis_sim.exe

## Registro de Uso de Inteligencia Artificial

- Se le pidió a la IA que verifique los codigos hechos para verificar que esté bien propuesta la resolucion.
- Se aprendió el lenguaje de C a traves de la IA, comparando pseudocodigo y aplicandolo a C para poder realizar el trabajo.
- Se realizó edicion de video a traves de la IA, sin afectar la voz y apariencia de las personas.

#include <stdio.h>
// Archivos de cabecera 

int main() {
    int opcion; 

    do {
        printf("\n------------------------------------------\n");
        printf(" SIMULADOR DE MISIONES ARTEMIS - SLS\n"); 
        printf("1. Consultar constantes \n");
        printf("2. Calcular fase de ascenso y desacople SRB\n");
        printf("3. Calcular velocidad y altitud\n");
        printf("4. Simulación de caída libre\n");
        printf("5. Salir\n");

        scanf("%d" , &opcion);
         switch (opcion) {
            case 1:
                printf("\n--Costantes de trabajo--\n");
                //Mostramos las constantes con la que vamos a trabajar o las editamos 
                break;
            case 2:
                printf("\nCalculando Fase 1\n");
                //Llamamos a la funcion acargo

                break;
            case 3: 
                printf("\nCalculando Velocidad y altitud máxima ");
                //Llamamos a la función encargada 
                break;
            case 4: 
                printf("\n Realizadno simulacipon hipotética\n");
                //Llamamos a la funncion acargo 
                break; 
            case 5:
                printf("\nSaliendo de la simulación\n");
                break;
            default :
                printf("\nOpcion no válida,seleccionar una opcion dispnible en el menú principal\n");
            
         }
        
    }while (opcion != 5);
        return 0;
    
}
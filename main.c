/**
 *
 * Practica 3
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "red-black-tree.h"
#include "linked-list.h"
#include "utils.h"

#include <pthread.h>

//TODO compilar amb $ gcc programa.c -o programa -lpthread modificar make?

int main(int argc, char **argv){

    char str1[MAXLINE], str2[MAXLINE];
    int opcio;
    rb_tree *tree = NULL;

    if (argc != 1) printf("Opcions de la linia de comandes ignorades\n");

    do {

        opcio = menu();
        printf("\n\n");

        switch (opcio) {

            case 1:
                printf("Introdueix fitxer que conte llistat d'aeroports: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

            	printf("Introdueix fitxer de dades: ");
                fgets(str2, MAXLINE, stdin);
                str2[strlen(str2)-1]=0;
              
                // Opcio per test rapid. no cal introduir noms d'arxius
                strcpy(str1, "aeroports.csv");
                strcpy(str2, "dades.csv");

                // Control per si hi ha un arbre creat, el borra de memoria
                if(tree) free_memory(tree);

                tree = (rb_tree *) malloc(sizeof(rb_tree));
                init_tree(tree);

                // En cas de que es llegeixin be els arxius, es crea l'arbre i s'insertan les dades
                if(start_tree(tree, str1)) break;
                if(insert_data(tree, str2)) break;
                printf("Arbre de dades inicialitzat.\n");
                break;

            case 2:

              if(!tree){
                printf("Arbre no inicialitzat. Crea arbre o llegeix dades de disc.\n");
	          }else{

	            printf("Introdueix el nom de fitxer en el qual es desara l'arbre: ");
	           	fgets(str1, MAXLINE, stdin);
	           	str1[strlen(str1)-1]=0;
	            ////////////  
	            strcpy(str1, "dades.data");

	            write_data(tree, str1);
              }
           	  break;

            case 3:
            
                printf("Introdueix nom del fitxer que conte l'arbre: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                if(tree) free_memory(tree);
                tree = (rb_tree *) malloc(sizeof(rb_tree));
                init_tree(tree);
                
                /////////////
                strcpy(str1, "dades.data");

                read_data(tree, str1);

                break;

            case 4:
	            if(tree == NULL){
	            	printf("Arbre no inicialitzat. Crea arbre o llegeix dades de disc.\n");
	            }else{
	                printf("Introdueix aeroport per cercar retard o polsa enter per saber l'aeroport amb mes destins: ");
	                fgets(str1, MAXLINE, stdin);
	                str1[strlen(str1)-1]=0;

	                if (strlen(str1) == 0){
	                	max_destinations_tree(tree);
	                	
	                }else{
		                estadistics(tree, str1);   
	                }
	            }
	            break;

            case 5:
                printf("Alliberant tota la memòria i sortint...\n");
                if (tree) free_memory(tree);
            
            printf("Memòria alliberada correctament.\n\n\n");

            break;

            default:
                printf("Opcio no valida.\n");
        }
    }
    while (opcio != 5);
    return 0;
}

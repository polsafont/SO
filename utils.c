#include "utils.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "red-black-tree.h"
#include "linked-list.h"




/**
 *
 * Funció per mostrar el menu
 *
 */
int menu(){

    char str[5];
    int opcio;

    printf("\n\nMenu\n\n");
    printf(" 1 - Creacio de l'arbre\n");
    printf(" 2 - Emmagatzemar arbre a disc\n");
    printf(" 3 - Llegir arbre de disc\n");
    printf(" 4 - Consultar informacio de l'arbre\n");
    printf(" 5 - Sortir\n\n");
    printf("   Escull opcio: ");

    fgets(str, 5, stdin);
    opcio = atoi(str);

    return opcio;
}

/**
 *
 * Funció per crear tots els nodes de l'arbre
 * Rep com a parametres un punter al arbre i un punter de char amb el nom del fitxer a llegir
 *
 */
int start_tree(rb_tree *tree, char* arg){
    char str[10];
    node_data *n_data;
    FILE* fp;
    fp= fopen(arg,"r");
    if (fp == NULL){
        printf("Fitxer d'aeroports erroni.");
        return(1);
    }

    fgets(str,10,fp);
    /* --- Inserim els nodes en l'arbre --- */
    while(fgets(str,10,fp) != NULL){
        // Reservem strlen(str) per tindre lloc pel caracter final de linea a memoria
        str[strlen(str) - 1] = '\0';
        n_data = malloc(sizeof(node_data));
        n_data->key = malloc( (strlen(str) +1)*sizeof(char));
        strcpy(n_data->key,str);    

        n_data->l = (list *) malloc (sizeof(list));
        init_list(n_data->l);
        insert_node(tree, n_data);
    }
    fclose(fp);
    return(0);
}
/**
 *
 * Funció per llegir i introduir dades a l'arbre
 * Rep com a parametres un punter al arbre i un punter de char amb el nom del fitxer de dades a llegir
 *
 */
int insert_data(rb_tree *tree, char *arg){
    char str[400];
    node_data *n_data;
    FILE *fp;
    fp = fopen(arg,"r");
    if (fp == NULL){
        printf("Fitxer de dades erroni.");
        return(1);
    }
    /* Ignorem primera linea del fitxer */
    fgets(str,400,fp);

    while (fgets(str, 100, fp) != NULL) {

        int i = 0;
        int col = 1;
        int longitud=0;
        char origin[4];
        char destination[4];
        char delaystring[10];
        memset(origin, '\0', 4);
        memset(destination, '\0', 4);
        memset(delaystring, '\0', 10);
        while(i<strlen(str)){
            if (str[i] == ','){
                col++;
                longitud = 0;
            }else{
                if (col == 15){
                    delaystring[longitud] = str[i];
                    longitud++;
                }
                else if (col == 17){
                    origin[longitud] = str[i];
                    longitud++;
                }
                else if (col == 18){
                    destination[longitud] = str[i];
                    longitud++;
                }
            }
            i++;
        }
        list_data *l_data;
        //search tree node
        n_data = find_node(tree, origin);
        if (n_data) {
            //add elements to tree lists
            l_data = find_list(n_data->l, destination);
            if (l_data != NULL) {
            	l_data->num_times++;
                if (delaystring[0] != 'N'){
                	l_data->delay+= atoi(delaystring);
                }
            } else {
                /* If the key is not in the list, allocate memory for the data and
                * insert it in the list and add 1 to number of destinations of the node*/

                l_data = malloc(sizeof(list_data));

                l_data->key = malloc( (strlen(destination) +1)*sizeof(char));

                strcpy(l_data->key,destination);
                l_data->delay = 0;
                if (delaystring[0] != 'N'){
                    l_data->delay = atoi(delaystring);
                }
                l_data->num_times = 1;
                insert_list(n_data->l, l_data);
            }
        }
        memset(origin,'\0', strlen(origin));
        memset(destination, '\0', strlen(destination));
        memset(delaystring, '\0', strlen(delaystring));
    }
    fclose(fp);
    printf("Fitxer de dades llegit.\n");
    return(0);
}

/**
 * Calcula estadistiques dels destins d'un node del arbre
 *
 */
void estadistics(rb_tree *tree, char*  arg){

	node_data *n_data;

    n_data = find_node(tree,arg);
    if(n_data){
        printf("Aeroport %s trobat.\n", n_data->key);

        list_item *l_item;
        list_data *l_data;
        l_item = n_data->l->first;

        int i = 0;
        while (l_item != NULL) {
            i++;
            float median;
            l_data = l_item->data;
            median = (float)l_data->delay/(float)l_data->num_times;
            printf("    %s -- %.3f minuts .\n", n_data->key, median );
            l_item = l_item->next;
        }
    }else{
    	printf("Aeroport %s no trobat en l'arbre.\n", arg);
    }
}

/**
 * Calcula origen amb major nombre de destins
 *
 */
void max_destinations_tree(rb_tree *tree){
	node_data *n_data;
    n_data = max_tree(tree);
    if (n_data) {
    	printf("Aeroport %s trobat amb %d destins.\n", n_data->key, n_data->l->num_items);
    }else{
    	printf("Error al buscar aeroport amb major nombre d'elements.\n");
    }
}

/**
 *
 * Escriu dades a disc
 * estructura: MagicNumber | number of nodes | (NODE1: origen iata (ABC no zero), n destins, iatadesit, n vols, retard total ) | (NODE2: )
 * magic number 4 bytes IATA tres chars, numbers positive integer 4 bytes
 */
void write_data(rb_tree* tree, char* filename){

    int magic_num = MAGIC_NUMBER;
    FILE* fp;
    
  	fp = fopen(filename,"w");
    fwrite(&magic_num, sizeof(int), 1, fp);
    //funcio recursiva per escriure a arbre 

    write_tree(tree,fp);
    fclose(fp);  
}


/**
 *
 * Llegeix dades del disc i en cas de ser correcte el identificador, 
 * inicialitza un arbre amb les dades.
 *
 */
void read_data(rb_tree* tree, char* filename){
	FILE* fp;
	int magic_num, num_items_tree, num_items_list, i ,j;
	node_data *n_data;
	list *l;
	list_data *l_data;

	fp = fopen(filename,"r");
	fread(&magic_num, sizeof(int), 1, fp);
	if(magic_num != MAGIC_NUMBER){
		printf("Error reading magic number. Wrong number.\n");
		exit(1);
	}
	fread(&num_items_tree, sizeof(int), 1, fp);
	// add tree nodes
    for ( i = 0; i < num_items_tree; i++){
    	n_data = malloc( sizeof(*n_data));
        n_data->key = calloc( 1, 4*sizeof(char));

		l = malloc(sizeof(list));
        init_list(l);

        fread(n_data->key , sizeof(char), 3, fp);
        fread(&num_items_list , sizeof(int), 1, fp);
    	
        // add list nodes
    	for ( j = 0; j < num_items_list; j++ ){
    		l_data = calloc(1,sizeof(*l_data));
    		l_data->key = calloc(1, 4*sizeof(char));

    		fread(l_data->key , sizeof(char), 3, fp);
    		fread(&l_data->num_times, sizeof(int), 1, fp);
    		fread(&l_data->delay, sizeof(int), 1, fp);

    		insert_list(l, l_data);
    	}

    	n_data->l = l;
    	insert_node(tree, n_data);
    }
	fclose(fp);
}


/**
 *
 * Free memory
 *
 */
void free_memory(rb_tree* tree){
    delete_tree(tree);
    free(tree);
}

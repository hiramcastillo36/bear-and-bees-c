#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <math.h>
#include <string.h>


float buffer[1];
int in = 0, out = 0;
sem_t lleno, vacio, sec_crit;

void *produce(void *);
void *consume(void *);

int num_abejas;
int limite_miel;

float num =0 ;

int main(int argc, char *argv[]){
    printf("Dime el numero de abejas productoras \n");
    scanf("%d", &num_abejas);
    printf("Dime el limite de miel \n");
    scanf("%d", &limite_miel);
    pthread_t productor1[num_abejas], consumidor1;
    num = (float)limite_miel / (float)num_abejas;
    round(num);

    sem_init(&lleno,0, 0);
    sem_init(&vacio,0, num_abejas);
	sem_init(&sec_crit, 0, 1);

    buffer[0] = 0.0f;

	pid_t pid = getpid();
	srand((int)pid);
	for(int i=0; i<num_abejas; i++){
		printf("Creando la abeja productora %d...\n", i);
		pthread_create(&productor1[i], NULL,produce, NULL);
	}

	printf("Creando el hilo Consumidor 1 - oso - ...\n");
	pthread_create(&consumidor1, NULL,consume, NULL);

	for(int i=0; i<num_abejas; i++){
		pthread_join(productor1[i], NULL);
	}
	pthread_join(consumidor1, NULL);

	sem_destroy(&lleno);
	sem_destroy(&vacio);
	sem_destroy(&sec_crit);

	exit(0);
}

void mostrar_abeja(int actual, int total){
int limit = ((actual * 100)/total)/10;
	char cadenas[10][50]; //36 34
	printf("porcentaje de miel al momento: %d %% \n", limit*10);
	strcpy(cadenas[0],"        ####################      "); 
	strcpy(cadenas[1],"         ##################       ");
	strcpy(cadenas[2],"         ##################       ");
	strcpy(cadenas[3],"          ###############         ");
	strcpy(cadenas[4],"          ###   #########         ");
	strcpy(cadenas[5],"         #### # ##########        ");
	strcpy(cadenas[6],"         #### # ##########        ");
	strcpy(cadenas[7],"         #### # ##########        ");
	strcpy(cadenas[8],"          ##### #########         ");
	strcpy(cadenas[9],"             ########             ");
	printf("             \\       /            \t\t\t");
	printf("porcentaje de miel al momento: %d %% \n", limit*10);
	printf("          \\    o ^ o    /               \t\t\t");
	if(limit>=9) printf("%s\n", cadenas[0]); else printf("\n");
	printf(" ____________(%%%%%%%%%%%%%%)____________           \t\t\t");
	if(limit>=8) printf("%s\n", cadenas[1]); else printf("\n");
	printf("(     /   /  )%%%%%%%%%%%%%%(  \\   \\     )      \t\t\t");
	if(limit>=7) printf("%s\n", cadenas[2]); else printf("\n");
	printf("(___/___/__/           \\__\\___\\___)       \t\t\t");
	if(limit>=6) printf("%s\n", cadenas[3]); else printf("\n");
	printf("   (     /  /(%%%%%%%%%%%%%%)\\  \\     )               \t\t\t");
	if(limit>=5) printf("%s\n", cadenas[4]); else printf("\n");
	printf("    (__/___/ (%%%%%%%%%%%%%%) \\___\\__)                \t\t\t");
	if(limit>=4) printf("%s\n", cadenas[5]); else printf("\n");
	printf("             /(     )\\                      \t\t\t");
	if(limit>=3) printf("%s\n", cadenas[6]); else printf("\n");
	printf("           /   (%%%%%%)   \\                 \t\t\t");
	if(limit>=2) printf("%s\n", cadenas[7]); else printf("\n");
	printf("                (%%)                         \t\t\t");
	if(limit>=1) printf("%s\n", cadenas[8]); else printf("\n");
	printf("                 !		             \t\t\t");
	if(limit>=0) printf("%s\n", cadenas[9]); else printf("\n");
}

void *produce(void *argument){
	while(1){
		sem_wait(&sec_crit);
		buffer[0] = buffer[0] + num;
		in = (in + 1) % (num_abejas+1);
		// print pthread id
		pthread_t self = pthread_self();
        	printf("La abejaid: %lu acaba de aportar %.2f de un total de %d \n", self, num, limite_miel);
		mostrar_abeja(buffer[0], limite_miel);

		if(buffer[0]>=limite_miel) { // Buffer lleno
			printf("----oso despierto---- Total miel: %d La abejas que lo desperto es Abejaid: %lu\n", limite_miel, self);
			in = 0;
			sem_post(&lleno);
		}
	    sem_post(&sec_crit);
        sleep(1);
	}
	pthread_exit(0);
}

void *consume(void *argument){
	while(1){
		sem_wait(&lleno);
        sem_wait(&sec_crit);

		printf("Oso comiendo - Consumidor: Número %.2lf de miel %d\n", buffer[0], 0);

		printf("(()__(()\n");
		printf("     /       \\ \n");
		printf("    ( /    \\  \\\n");
		printf("     \\ o o    /\n");
		printf("     (_()_)__/ \\\n");
		printf("    / _,==.____ \\\n");
		printf("   (   |--|      )\n");
		printf("   /\\_.|__|'-.__/\\_\n");
		printf("  / (        /     \\ \n");
		printf("  \\  \\      (      /\n");
		printf("   )  '._____)    /    \n");
		printf("(((____.--(((____/\n");
	printf("------- OSO DORMIDO -------\n");
		buffer[0] = 0;
        sem_post(&vacio);
        sem_post(&sec_crit);
		sleep(1);
	}
	pthread_exit(0);
}


#include <stdio.h>
#include "gamelib.h"

int main(void){
int scelta=0;

printf("----COSE STRANE----\n");
printf(" A Occhinz qualcosa sta andando storto: strani portali collegano il Mondo Reale al Soprasotto,\n");
printf("una dimensione oscura popolata da creature ostili.\n");
printf("I giocatori dovranno esplorare le zone, affrontare i nemici e aiutarsi a vicenda per fermare il Demotorzone.\n");
printf("State attenti alle vostre strategie. Vince chi riesce a sconfiggere il Demotorzone.\n");
printf("Se verrete eliminati il mondo e' perduto, ma sopratutto ricordate gli amici non mentono. Buona fortuna.\n");

do{
printf("menu di gioco, scegli una delle seguenti opzioni:\n");
printf(" 1- imposta gioco,\n");
printf(" 2- gioca,\n");
printf(" 3-termina gioco,\n");
printf(" 4-visualizza crediti .\n");
scanf("%d", &scelta);

switch(scelta){
     case 1:
         printf("Hai scelto l'opzione 1- imposta gioco\n");
         imposta_gioco();
         break;
     case 2:
         printf("Hai scelto l'opzione 2- gioca. \n");
         gioca();
         break;
     case 3:
         printf("Hai scelto l'opzione 3- termina gioco. \n");
         termina_gioco();
         break;
     case 4:
         printf("Hai scelto l'opzione 4- visualizza crediti. \n");
         crediti();
         break;
     default:
         printf("attenzione! comando sbagliato reinserire comando 1, 2, 3 o 4: \n");
         }
   }while(scelta!=3);
}



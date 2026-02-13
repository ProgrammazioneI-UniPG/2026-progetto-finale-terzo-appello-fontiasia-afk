#include <stdio.h>
#include <string.h>
#include <stdlib.h> //per utilizzare rand()
#include <time.h>
#include "gamelib.h"

struct Zona_mondoreale *prima_zona_mondoreale = NULL;  //variabili per genera mappa
struct Zona_soprasotto *prima_zona_soprasotto = NULL;
static int mappa_chiusa = 0;

static struct Giocatore *giocatori[4] = {NULL, NULL, NULL, NULL}; // variabile per giocatori

static void genera_mappa(void);
static void inserisci_zona(void);
static void cancella_zona(void);
static void stampa_mappa(void);
static void stampa_zona(void);
static void chiudi_mappa(void);

static char ultimi_vincitori[3][50]={"","",""}; //variabili per funzione crediti
static int partite_concluse = 0;
static int mostri_sconfitti = 0;
static int giocatori_morti = 0;
static int willber_usato=0; // 0=mai usato, 1=usato

static int giocatore_corrente = -1; //variabile per funzione gioca
static int turno_finito=0;

void imposta_gioco(void) {  //FUNZIONE IMPOSATA GIOCO

     srand((unsigned)time(NULL));

     for(int i=0; i < 4; i++){
         if(giocatori[i] != NULL){
            free(giocatori[i]->nome);
            free(giocatori[i]);
            giocatori[i]=NULL;
            }
         }

     while ( prima_zona_mondoreale != NULL){
              struct Zona_mondoreale *tmp = prima_zona_mondoreale;
              prima_zona_mondoreale = prima_zona_mondoreale ->avanti;
              free(tmp);
              }
    while (prima_zona_soprasotto != NULL){
           struct Zona_soprasotto *tmp = prima_zona_soprasotto;
           prima_zona_soprasotto = prima_zona_soprasotto->avanti;
           free(tmp);
           }
    prima_zona_mondoreale = NULL;
    prima_zona_soprasotto = NULL;

     int n;
     printf("Quanti giocatori ci sono? da 1 a 4:\n");  //numero giocatori
     scanf("%d", &n);

     while (n<1 || n>4) {
           printf("Numero non valido. Riprova\n");
           scanf("%d", &n);
           }

     int undici_usata=0;
     for(int i=0; i<n; i++){
        giocatori [i] = malloc(sizeof(struct Giocatore));   //imposto giocatore

        printf("Nome giocatore %d (massimo 49 caratteri altrimenti abbreviero' il tuo nome): ", i+1);
        giocatori[i]->nome = malloc(50); //alloco 50 lettere
        scanf("%49s", giocatori[i]->nome);

        giocatori[i]->attacco_psichico = rand()%20 + 1; //tiro dado
        giocatori[i]->difesa_psichica = rand()%20 + 1;
        giocatori[i]->fortuna = rand()%20 + 1;

        printf("statistica giocatore:\n");
        printf(" attacco psichico: %d\n", giocatori[i]->attacco_psichico);
        printf(" difesa psichica: %d\n", giocatori[i]->difesa_psichica);
        printf(" fortuna: %d\n", giocatori[i]->fortuna);

        printf("Puoi scegliere un bonus:\n"); //scelta bonus del giocatore
        printf("1- +3 attacco, -3 difesa.\n");
        printf("2- +3 difesa, -3 attacco.\n");
        printf("3- niente, mi e' andata bene.\n");

        if (undici_usata==0){
           printf("4-UndiciVirgolaCinque: +4 in attacco, +4 in difesa, -7 fortuna. (puo' esserlo solo un giocatore)\n");
           }

        int bonus;
        scanf("%d", &bonus);

       if(undici_usata==0){
          while(bonus<0 || bonus>4){
               printf("scelta sbagliata. Riprova:\n");
               scanf("%d", &bonus);
               }
      } else{
         while(bonus<0 || bonus>3){
              printf("scelta sbagliata. Riprova:\n");
              scanf("%d", &bonus);
              }
      }

        if(bonus==1){
          giocatori[i]->attacco_psichico+=3;
          giocatori[i]->difesa_psichica-=3;
          }else if(bonus==2){
                  giocatori[i]->difesa_psichica+=3;
                  giocatori[i]->attacco_psichico-=3;
          }else if (bonus==4){
                  undici_usata=1;
                  giocatori[i]->attacco_psichico+=4;
                  giocatori[i]->difesa_psichica+=4;
                  giocatori[i]->fortuna-=7;
                  strcpy(giocatori[i]->nome, "UndiciVirgolaCinque");
                  printf("UndiciVirgolaCinque assegnato al giocatore %d\n", i+1);
                  }

        printf("Ora le statistiche sono: attacco: %d, difesa: %d, fortuna %d\n",
             giocatori[i]->attacco_psichico, giocatori[i]->difesa_psichica, giocatori[i]->fortuna);

       for(int j=0; j<3; j++) { //zaino inizializzato
           giocatori[i]->zaino[j] = nessun_oggetto;
            }
       giocatori[i]->mondo=0; //mondo inizializzato
       giocatori[i]-> pos_mondoreale=NULL;
       giocatori[i]-> pos_soprasotto=NULL;

       printf("Giocatore creato\n");
       }

       for(int i=n; i<4; i++){
          giocatori[i]=NULL;
          }
       int scelta2;
       do{
         printf("MENU MAPPA:\n"); //menu creazione mappa
         printf("1- genera mappa,\n");
         printf("2- inserisci zona,\n");
         printf("3- cancella zona,\n");
         printf("4- stampa mappa,\n");
         printf("5- stampa zona,\n");
         printf("6- chiudi mappa.\n");
         scanf("%d", &scelta2);

         switch (scelta2) {
                case 1: genera_mappa(); break;
                case 2: inserisci_zona(); break;
                case 3: cancella_zona(); break;
                case 4: stampa_mappa(); break;
                case 5: stampa_zona(); break;
                case 6: chiudi_mappa();
                if (mappa_chiusa == 0) scelta2 = 0;
                   break;
                default:
                   printf("Comando sbaglio.\n");
                   break;
                }
         } while (scelta2 != 6);
         for (int i = 0; i<4; i++){ // tutti i giocatori iniziano dalla prima zona del mondo reale
             if (giocatori[i] != NULL){
                 giocatori[i]->mondo = 0;
                 giocatori[i]->pos_mondoreale=prima_zona_mondoreale;
                 giocatori[i]->pos_soprasotto=NULL;
                 }
           }

       printf("Gioco impostato!\n");
       return;
       }

static void genera_mappa(void){  //FUNZIONE GENERA MAPPA()
            while(prima_zona_mondoreale != NULL){
                 struct Zona_mondoreale *tmp = prima_zona_mondoreale;
                 prima_zona_mondoreale = prima_zona_mondoreale->avanti;
                 free(tmp);
                 }
             while(prima_zona_soprasotto != NULL){
                 struct Zona_soprasotto *tmp = prima_zona_soprasotto;
                 prima_zona_soprasotto = prima_zona_soprasotto->avanti;
                 free(tmp);
                 }
               prima_zona_mondoreale = NULL;
               prima_zona_soprasotto = NULL;
               mappa_chiusa = 0;

             int idx_dem =rand()% 15;
             struct Zona_mondoreale *prev_mr = NULL;
             struct Zona_soprasotto *prev_ss = NULL;

             for(int i=0; i<15; i++){
                struct Zona_mondoreale *mr=(struct Zona_mondoreale*)malloc(sizeof(struct Zona_mondoreale));
                struct Zona_soprasotto *ss=(struct Zona_soprasotto*)malloc(sizeof(struct Zona_soprasotto));
                mr->tipo = (Tipo_zona) (rand()%10);
                ss->tipo = mr->tipo;

             int r_mr = rand()%100;  //nessun nemico, democane e billi
             if (r_mr<50) mr->nemico=nessun_nemico;
             else if (r_mr<50) mr->nemico=democane;
             else mr->nemico = billi;

             int r_obj = rand()%100;  //solo nel mondo reale
             if(r_obj<50) mr->oggetto=nessun_oggetto;
             else if(r_obj<65) mr->oggetto=bicicletta;
             else if(r_obj<80) mr->oggetto=maglietta_fuocoinferno;
             else if(r_obj<90) mr->oggetto=bussola;
             else mr->oggetto=schitarrata_metallica;

             if(i==idx_dem) ss->nemico=demotorzone;
             else{
                 int r_ss = rand()%100;
                 if(r_ss<60) ss->nemico = nessun_nemico;
                 else ss->nemico = democane;
                 }

            mr->avanti = NULL; mr->indietro = prev_mr;
            ss->avanti = NULL; ss->indietro = prev_ss;

           mr->link_soprasotto = ss;
           ss->mondoreale = mr;

           if(prev_mr != NULL) prev_mr->avanti = mr;
           else prima_zona_mondoreale = mr;

           if(prev_ss != NULL) prev_ss->avanti = ss;
           else prima_zona_soprasotto = ss;

          prev_mr = mr;
          prev_ss = ss;
          }

      printf("Mappa generata 15+15.\n");
      }

static void inserisci_zona(void){  //FUNZIONE INSERISCI ZONA
            mappa_chiusa = 0;

            int len = 0;
            for(struct Zona_mondoreale *p = prima_zona_mondoreale; p!=NULL; p=p->avanti)len++;

              int i;
              printf("Inserisci posizione (o...%d):\n", len);
              scanf("%d", &i);
              while(i<0 || i>len){
                   printf("Posizione non valida riprova:\n");
                   scanf("%d", &i);
                   }
             int nem_mr, nem_ss, obj;
             printf("Scegli nemico Mondo reale (0-nessun_nemico, 1-billi, 2-democane):\n");
             scanf("%d", &nem_mr);
             while (nem_mr < 0 || nem_mr > 2){
                   printf("Scelta non valida, riprova:\n");
                   scanf("%d", &nem_mr);
                   }
             printf("Scegli oggetto Mondoreale (0-nessun_oggetto, 1-bicicletta, 2-maglietta_fuocoinferno, 3-bussola, 4-schitarrata_metallica\n");
             scanf("%d", &obj);
             while(obj < 0||obj > 4){
                  printf("Scelta non valida, riprova:\n");
                  scanf("%d", &obj);
                  }
            printf("Scegli nemico soprasotto (0-nessun_nemico, 1-democane, 2-demotorzone)\n");
            scanf("%d", &nem_ss);
            while(nem_ss < 0 || nem_ss >2){
                 printf("Scelta non valida, riprova\n");
                 scanf("%d", &nem_ss);
                 }
           struct Zona_mondoreale *mr = (struct Zona_mondoreale*)malloc(sizeof(struct Zona_mondoreale));
           struct Zona_soprasotto *ss = (struct Zona_soprasotto*)malloc(sizeof(struct Zona_soprasotto));

           mr->tipo = (Tipo_zona)(rand()%10);
           ss->tipo = mr->tipo;

           mr->nemico = (nem_mr == 0? nessun_nemico : (nem_mr == 1 ? billi : democane));
           mr->oggetto = (Tipo_oggetto)obj;

           ss->nemico = (nem_ss == 0 ? nessun_nemico : (nem_ss == 1 ? democane : demotorzone ));

           mr->link_soprasotto = ss;
           ss->mondoreale = mr;

           if (i==0) {  //inserimento MR
              mr->indietro = NULL;
              mr->avanti =prima_zona_mondoreale;
              if (prima_zona_mondoreale != NULL) prima_zona_mondoreale->indietro = mr;
              prima_zona_mondoreale=mr;
             } else{
                  struct Zona_mondoreale *p = prima_zona_mondoreale;
                  for(int k=0; k<i-1; k++) p=p->avanti;
                     mr->avanti =p->avanti;
                     mr->indietro=p;
                     if(p->avanti != NULL) p->avanti->indietro = mr;
                     p->avanti=mr;
                  }
           if (i==0){
              ss->indietro = NULL;
              ss->avanti = prima_zona_soprasotto;
              if(prima_zona_soprasotto != NULL) prima_zona_soprasotto->indietro = ss;
              prima_zona_soprasotto = ss;
          }else{
              struct Zona_soprasotto *q = prima_zona_soprasotto;
              for(int k=0; k<i-1; k++) q=q->avanti;
              ss->avanti = q->avanti;
              ss->indietro = q;
              if(q->avanti != NULL) q->avanti->indietro = ss;
              q->avanti = ss;
              }
          printf("Zona inserita in posizione %d mondoreale e soprasotto.\n", i);
          }

static void cancella_zona(void){  //FUNZIONE CANCELLA ZONA
            mappa_chiusa=0;

            int len=0;
            for(struct Zona_mondoreale *p=prima_zona_mondoreale; p != NULL; p=p->avanti)len++;

            if(len==0){
              printf("Mappa vuota.\n");
              return;
              }

           int i;
           printf("Cancella posizione i (0...%d):\n", len-1);
           scanf("%d", &i);
           while(i<0 || i>=len){
                printf("Posizione non valida, riprova:\n");
                scanf("%d", &i);
                }
           struct Zona_mondoreale *mr = prima_zona_mondoreale;
           for(int k=0; k<i; k++) mr =mr ->avanti;

           struct Zona_soprasotto *ss = mr->link_soprasotto;

           if(mr->indietro != NULL) mr->indietro->avanti = mr->avanti;
           else prima_zona_mondoreale = mr->avanti;
           if (mr->avanti != NULL) mr->avanti->indietro = mr->indietro;

           if(ss->indietro != NULL) ss->indietro->avanti = ss->avanti;
           else prima_zona_soprasotto = ss->avanti;
           if(ss->avanti != NULL) ss->avanti->indietro = ss->indietro;

           free(mr);
           free(ss);
           printf("Zona cancellata in posizione %d mondo reale e soprasotto.\n", i);
}

static void stampa_mappa(void){  //FUNZIONE STAMPA MAPPA
            int scelta;
            printf("Stampa quale mappa? 0 mondo reale, 1 soprasotto:\n");
            scanf("%d", &scelta);
            while(scelta != 0 && scelta != 1){
                 printf("Scelta non valida, riprova:\n");
                 scanf("%d", &scelta);
                 }
            if(scelta == 0){
              int idx = 0;
              for (struct Zona_mondoreale *p = prima_zona_mondoreale; p != NULL; p = p->avanti){
                  printf("[%d] tipo=%d nemico=%d oggetto=%d\n", idx, p->tipo, p->nemico, p->oggetto);
                  idx++;
                  }
              }else{
                int idx = 0;
                for(struct Zona_soprasotto *p = prima_zona_soprasotto; p != NULL; p= p->avanti) {
                   printf("[%d] tipo=%d nemico=%d\n", idx, p->tipo, p->nemico);
                   idx++;
                   }
                }
}

static void stampa_zona(void){  //FUNZIONE STAMPA ZONA
            int len=0;
            for(struct Zona_mondoreale *p=prima_zona_mondoreale; p != NULL; p=p->avanti)len++;

            if(len==0){
              printf("Mappa vuota.\n");
              return;
              }

            int i;
            printf("Scegli zona del Mondo Reale in posione i (0...%d):\n", len-1);
            scanf("%d", &i);
            while(i<0 || i>=len){
                 printf("Posizione non valida, riprova:\n");
                 scanf("%d", &i);
                 }
           struct Zona_mondoreale *mr = prima_zona_mondoreale;
           for (int k=0; k<i; k++) mr= mr->avanti;

           struct Zona_soprasotto *ss = mr->link_soprasotto;

           printf("Mondo Reale [%d]: tipo=%d nemico=%d oggetto=%d\n", i, mr->tipo, mr->nemico, mr->oggetto);
           printf("Soprasotto [%d]: tipo=%d nemico=%d\n", i, ss->tipo, ss->nemico);
        }

static void chiudi_mappa(void){   //FUNZIONE CHIUDI MAPPA
            int count_mr = 0;
            for(struct Zona_mondoreale *p = prima_zona_mondoreale; p != NULL; p=p->avanti)count_mr++;

            int count_dem = 0;
            for(struct Zona_soprasotto *p = prima_zona_soprasotto; p != NULL; p=p->avanti){
               if(p->nemico==demotorzone) count_dem++;
              }
            if(count_mr<15){
              printf("Non puoi chiudere: ci sono meno di 15 zone.\n");
              mappa_chiusa=0;
              return;
              }
            if(count_dem != 1){
              printf("Non puoi chiudere: deve esserci un solo demotorzone nel soprasotto.\n");
              mappa_chiusa=0;
              return;
               }
            mappa_chiusa=1;
            printf("Mappa chiusa.\n");
            return;
          }

static void stampa_giocatore(void){ // FUNZIONE STAMPA GIOCATORE
       struct Giocatore *g = giocatori[giocatore_corrente];
       if(g==NULL){
          printf("Nessun giocatore corrente.\n");
          return;
          }

       printf("GIOCATORE:\n");
       printf("-Nome: %s\n", g->nome);
       printf("-Mondo: %s\n", (g->mondo==0) ? "mondo reale":"soprasotto");   //RIGUARDA QUA MONDI !!!!!
       printf("-Attacco: %d\n", g->attacco_psichico);
       printf("-Difesa: %d\n", g->difesa_psichica);
       printf("-Fortuna: %d\n", g->fortuna);
       printf("-Zaino: [%d, %d, %d]\n", g->zaino[0], g->zaino[1], g->zaino[2]);

       if (g->mondo == 0 && g->pos_mondoreale != NULL){
          printf("Posizione mondo reale: tipo=%d, nemico=%d, oggetto=%d\n",
                  g->pos_mondoreale->tipo, g->pos_mondoreale->nemico, g->pos_mondoreale->oggetto);
       }else if(g->mondo == 1 && g->pos_soprasotto != NULL){
               printf("Posizione soprasotto: tipo=%d, nemico=%d\n",
                      g->pos_soprasotto->tipo, g->pos_soprasotto->nemico);
               }
      }

static void combatti(void){ //FUNZIONE COMBATTI
       struct Giocatore *g=giocatori[giocatore_corrente];
       if(g==NULL){
          printf("Nessun giocatore corrente.\n");
          return;
          }
       enum Tipo_nemico nem;

       if (g->mondo == 0&& g->pos_mondoreale != NULL){
            nem=g->pos_mondoreale->nemico;
         } else if (g->mondo == 1 && g->pos_soprasotto != NULL){
             nem=g->pos_soprasotto->nemico;
         } else{ return;
            }

       if(nem == nessun_nemico){
         printf("Nessun nemico in questa zona.\n");
         return;
         }
         printf("COMBATTIMENTO CONTRO NEMICO = %d\n", nem);

        int tiro_g=(rand()%20) + 1;
        int tiro_n=(rand()%20) + 1;

        int punteggio_g = tiro_g + g->attacco_psichico + g->fortuna;
        int punteggio_n = tiro_n + g->difesa_psichica;

        if(nem==demotorzone && willber_usato==0){
           printf("Stai combattendo contro il DEMOTORZONE.\n");
           printf("I tuoi parametri sono: Attacco=%d, Difesa=%d, Fortuna=%d\n", g->attacco_psichico, g->difesa_psichica, g->fortuna);
           printf("Vuoi un aiuto da Willber per infliggere un colpo maggiore? (1=SI, 0=NO): ");
           int aiuto=0;
           if (scanf("%d", &aiuto) !=1){
              aiuto=0;
            }
          if(aiuto==1){
            punteggio_g+=5;
            willber_usato = 1;
            printf("Willber interviene per aiutarti e da +5 al tuo punteggio!\n");
           }
         }

        if (punteggio_g >= punteggio_n){
           printf("Hai sconfitto il nemico!\n");

           //50% il nemico scompare dalla zona
           if(rand() % 2 ==0){
             if(g->mondo == 0) g->pos_mondoreale->nemico=nessun_nemico;
             else g->pos_soprasotto->nemico = nessun_nemico;
             printf("Il nemico scompare dalla zona.\n");
           }else{
             printf("Il nemico rimane nella zona.\n");
            }
       }else{
          printf("Hai perso. SEI MORTO, la prossima volta andra' meglio :(\n");
          free(g->nome);
          free(g);
          giocatori[giocatore_corrente] = NULL;
          turno_finito = 1;
          }
     }

static void avanza(void){ //FUNZIONE AVANZA
            struct Giocatore *g = giocatori[giocatore_corrente];
            if(g == NULL) return;

            //prima di avanzare si controlla se c'e' un cattivo e in caso si combatte
            if(g->mondo == 0 && g->pos_mondoreale != NULL){
              if(g->pos_mondoreale->nemico != nessun_nemico){
                 printf("Prima devi combattere il nemico.\n");
                 return;
              }
              if(g->pos_mondoreale->avanti == NULL){
                printf("Non puoi avanzare: sei all'ultima zona.\n");
                return;
               }
               g->pos_mondoreale = g->pos_mondoreale->avanti;
               printf("Avanzato nel Mondo reale.\n");
             }else if (g->mondo == 1 && g->pos_soprasotto != NULL){
               if (g->pos_soprasotto->nemico != nessun_nemico){
                  printf("Prima devi combattere il nemico.\n");
                  return;
                }
              if (g->pos_soprasotto->avanti == NULL){
                  printf("Non puoi avanzare: sei all'ultima zona.\n");
                  return;
                 }
                 g->pos_soprasotto = g->pos_soprasotto->avanti;
                 printf("Avanzato nel soprasotto.\n");
              }
        }

static void indietreggia(void){ // FUNZIONE INDIETREGGIA
       struct Giocatore *g = giocatori[giocatore_corrente];
       if(g==NULL) return;

       if(g->mondo == 0 && g->pos_mondoreale != NULL){      //prima di indietreggiare se c'e' un nemico bisogna combattere
         if (g->pos_mondoreale->nemico != nessun_nemico){
             printf("Prima devi combattere il nemico.\n");
             return;
            }
         if(g->pos_mondoreale->indietro == NULL){
            printf("Non puoi indietreggiare: sei alla prima zona.\n");
            return;
           }
          g->pos_mondoreale = g->pos_mondoreale->indietro;
          printf("Indietreggiato nel Mondo Reale.\n");
       }else if(g->mondo == 1 && g->pos_soprasotto != NULL){
          if (g->pos_soprasotto->nemico != nessun_nemico){
              printf(" Prima devi combattere il nemico.\n");
              return;
           }
         if (g->pos_soprasotto->indietro == NULL){
             printf("Non puoi indietreggiare: sei alla prima zona.\n");
             return;
           }
           g->pos_soprasotto = g->pos_soprasotto->indietro;
           printf("Indietreggio nel Soprasotto.\n");
        }
    }

static void cambia_mondo(void){ // FUNZIONE CAMBIA MONDO
       struct Giocatore *g= giocatori[giocatore_corrente];
       if(g==NULL) return;

       if(g->mondo == 0){
          if(g->pos_mondoreale == NULL || g->pos_mondoreale->link_soprasotto == NULL) return;

          if(g->pos_mondoreale->nemico != nessun_nemico){
             printf("Non puoi cambiare mondo: devi prima sconfiggere il nemico.\n");
             return;
           }

         g->mondo = 1;
         g->pos_soprasotto = g->pos_mondoreale->link_soprasotto;
         printf("Sei stato catapultato nel Soprasotto.\n");
       }else{
         int tiro = (rand()%20) + 1;
         printf("Tiro d20 = %d deve essere minore della fortuna %d.\n", tiro, g->fortuna);

         if(tiro < g->fortuna){
            if(g->pos_soprasotto == NULL || g->pos_soprasotto->mondoreale == NULL) return;
            g->mondo = 0;
            g->pos_mondoreale = g->pos_soprasotto->mondoreale;
            printf("Sei tornato nel Mondo reale.\n");
         }else{
            printf("Fallito: resti nel Soprasotto.\n");
        }
    }
 }

static void raccogli_oggetto(void){ // FUNZIONI RACCOGLI OGGETTO
       struct Giocatore *g = giocatori[giocatore_corrente];
       if (g==NULL) return;

       if(g->mondo != 0 || g->pos_mondoreale == NULL){
          printf("Non puoi raccogliere oggetti nel soprasotto.\n");
          return;
        }
       if(g->pos_mondoreale->nemico != nessun_nemico){
          printf("Nessun oggetto in questa zona.\n");
          return;
        }

       int slot = -1;
       for (int i=0; i<3; i++){
           if(g->zaino[i] == nessun_oggetto) {slot=i; break;}
          }
      if(slot == -1){
         printf("Zaino pieno.\n");
         return;
       }
      g->zaino[slot] = g->pos_mondoreale->oggetto;
      g->pos_mondoreale->oggetto = nessun_oggetto;
      printf("Oggetto raccolto tasca %d.\n", slot);
   }

static void utilizza_oggetto(void){ //FUNZIONE UTILIZZA OGETTO
       struct Giocatore *g = giocatori[giocatore_corrente];
       if (g==NULL) return;

       printf("Scegli tasca oggetto da usare (0, 1, 2): ");
       int s;
       scanf("%d", &s);
       while(s<0 || s>2){
           printf("Tasca non valida. Riprova: ");
           scanf("%d", &s);
          }

       enum Tipo_oggetto ogg=g->zaino[s];
       if(ogg == nessun_oggetto){
          printf("Tasca vuota.\n");
          return;
        }

      if(ogg == bicicletta){
         g->fortuna += 2;
         printf("Usata bicicletta: fortuna +2\n");
    }else if(ogg == maglietta_fuocoinferno){
          g->attacco_psichico+=3;
          printf("Usata maglietta fuocoinferno: attacco +3\n");
    }else if(ogg == bussola){
          g->difesa_psichica += 2;
          printf("Usata bussola: difesa +2\n");
    }else if(ogg==schitarrata_metallica){
          g->attacco_psichico +=2;
          g->difesa_psichica +=1;
          printf("Usata schitarrata metallica: attacco +2, difesa +1\n");
         }
       g->zaino[s] = nessun_oggetto; //consumato
   }

static void passa(void){ //FUNZIONE PASSA
        turno_finito = 1;
        printf("Turno ceduto al prossimo giocatore.\n");
     }

void gioca(void){ //FUNZIONE GIOCA
     if(mappa_chiusa == 0 || prima_zona_mondoreale == NULL){
       printf("Gioco non impostato: prima crea e chiudi la mappa.\n");
       return;
       }
     int vivi=0;  //controlla che esista almeno un giocatore
     for (int i=0; i<4; i++) if(giocatori[i] != NULL) vivi++;
     if(vivi==0){
       printf("Nessun giocatore è presente, imposta prima il gioco.\n");
       return;
       }

    for(int i=0; i<4; i++){  //tutti i giocatori partono dalla prima zona del mondo reale
       if (giocatori[i] != NULL){
          giocatori[i]->mondo = 0;
          giocatori[i]->pos_mondoreale = prima_zona_mondoreale;
          giocatori[i]->pos_soprasotto = NULL;
          }
      }
   int vincitore = -1;
   while(1) {  //costruisce lista giocatori vivi
        int lista[4];
        int n = 0;
        for(int i=0; i<4; i++){
           if(giocatori[i] != NULL) lista[n++]=i;
           }
        if (n==0){
           printf("Tutti i giocatori sono morti :(\n");
           break;
           }

        for(int t=0; t<n; t++){
           int idx = lista[t];

           if(giocatori[idx] == NULL) continue;
             giocatore_corrente = idx;
             int fine_turno = 0;

             while(!fine_turno){
                  if(giocatori[giocatore_corrente] == NULL){
                    fine_turno = 1;
                    continue;
                    }
             printf("TURNO DI %s:\n", giocatori[giocatore_corrente]->nome);
             printf("1- avanza,\n");
             printf("2- indietreggia,\n");
             printf("3- cambia mondo,\n");
             printf("4- combatti,\n");
             printf("5- stampa giocatore,\n");
             printf("6- stampa zona,\n");
             printf("7- raccogli oggetto,\n");
             printf("8- utilizza oggetto,\n");
             printf("9- passa.\n");

             int scelta;
             scanf("%d", &scelta);

             int nemico_presente = 0;
             if (giocatori[giocatore_corrente]->mondo == 0){
                 if(giocatori[giocatore_corrente]->pos_mondoreale != NULL &&
                 giocatori[giocatore_corrente]->pos_mondoreale->nemico != nessun_nemico)
                 nemico_presente = 1;
                }else{
                   if(giocatori[giocatore_corrente]->pos_soprasotto != NULL &&
                      giocatori[giocatore_corrente]->pos_soprasotto->nemico != nessun_nemico)
                      nemico_presente = 1;
                      }
            switch(scelta){
                  case 1:  // avanzamento
                       if(nemico_presente){
                         printf("Devi prima combattere il nemico.\n");
                         break;
                         }
                       avanza();
                       break;
                 case 2:  //indietreggia
                      if(nemico_presente){
                        printf("Devi prima combattere il nemico.\n");
                        break;
                        }
                        indietreggia();
                        break;
                 case 3:  //cambia mondo
                      if(giocatori[giocatore_corrente]->mondo == 0){
                      if(nemico_presente){
                         printf("Devi sconfiggere prima il nemico.\n");
                         break;
                         }
                         cambia_mondo();
                       }else{
                         int dado=(rand()%20) +1;
                         if(dado<giocatori[giocatore_corrente]->fortuna){
                            cambia_mondo();
                         }else{
                            printf("Cambio mondo fallito (dado=%d, fortuna=%d).\n", dado, giocatori[giocatore_corrente]->fortuna);
                            }
                         }
                         break;

                 case 4:   //combatti
                        {
                         int era_demotorzone = 0;

                         if(giocatori[giocatore_corrente]->mondo == 0){
                           if(giocatori[giocatore_corrente]->pos_mondoreale != NULL &&
                              giocatori[giocatore_corrente]->pos_mondoreale->nemico == demotorzone)
                              era_demotorzone = 1;
                        }else{
                          if(giocatori[giocatore_corrente]->pos_soprasotto != NULL &&
                             giocatori[giocatore_corrente]->pos_soprasotto->nemico == demotorzone)
                             era_demotorzone = 1;
                           }
                         combatti();

                         if(giocatori[giocatore_corrente] == NULL){  //morto durante il combattimento
                            fine_turno=1;
                            break;
                            }
                         if(era_demotorzone){ // se non c'è il demotorzone, si dichiara vittoria
                           int ora_nessuno = 0;
                           if(giocatori[giocatore_corrente]->mondo == 0){
                             if(giocatori[giocatore_corrente]->pos_mondoreale != NULL &&
                                giocatori[giocatore_corrente]->pos_mondoreale->nemico== nessun_nemico)
                                ora_nessuno = 1;
                            }else{
                             if(giocatori[giocatore_corrente]->pos_soprasotto != NULL&&
                                giocatori[giocatore_corrente]->pos_soprasotto->nemico == nessun_nemico)
                                ora_nessuno = 1;
                               }
                             if(ora_nessuno){
                               vincitore = giocatore_corrente;
                               goto FINE_PARTITA;
                               }
                             }
                          }
                          break;
                          case 5:
                               stampa_giocatore();
                               break;
                          case 6:
                               stampa_zona();
                               break;
                          case 7:
                               raccogli_oggetto();
                               break;
                          case 8:
                               utilizza_oggetto();
                               break;
                          case 9:
                               passa();
                               fine_turno =1;
                               break;
                          default:
                               printf("Scelta non valida.\n");
                               break;
                         }
                     }
                  }
              }
FINE_PARTITA:

            if (vincitore != -1 && giocatori[vincitore] != NULL){
                printf("VITTORIA! %s ha sconfitto il demotorzone.\n", giocatori[vincitore]->nome);

                strncpy(ultimi_vincitori[2], ultimi_vincitori[1], 50);
                strncpy(ultimi_vincitori[1], ultimi_vincitori[0], 50);
                strncpy(ultimi_vincitori[0], giocatori[vincitore]->nome, 50);
                ultimi_vincitori[0][49]='\0';
                ultimi_vincitori[1][49]='\0';
                ultimi_vincitori[2][49]='\0';

                partite_concluse++;
            }else{
                printf("Fine partita, nessun vincitore :(\n");
                partite_concluse++;
                }
         }

void termina_gioco(void){ //FUNZIONE TERMINA GIOCO
     printf("Gioco terminato, ci vediamo presto :)\n");

     for(int i=0; i<4; i++){
        if(giocatori[i] != NULL && giocatori[i]->nome != NULL){
          printf("Arrivederci soldato %s, e' stato un onore!\n", giocatori[i]->nome);
          }
       }
    for(int i=0; i<4; i++){
       if(giocatori[i] != NULL){
         if(giocatori[i]->nome != NULL) free(giocatori[i]->nome);
         free(giocatori[i]);
         giocatori[i] = NULL;
         }
       }
    while (prima_zona_mondoreale != NULL){
          struct Zona_mondoreale *tmp = prima_zona_mondoreale;
          prima_zona_mondoreale = prima_zona_mondoreale->avanti;
          free(tmp);
          }
   while(prima_zona_soprasotto != NULL){
        struct Zona_soprasotto *tmp = prima_zona_soprasotto;
        prima_zona_soprasotto = prima_zona_soprasotto->avanti;
        free(tmp);
        }
   prima_zona_mondoreale = NULL;
   prima_zona_soprasotto = NULL;
   mappa_chiusa = 0;

   return;
   }

void crediti(void){   //FUNZIONE CREDITI
     printf("Il creatore, Asia Fonti, porge i suoi complimenti per il coraggio.\n");

     printf("I vincitori delle ultime 3 partite:\n");
     if(partite_concluse == 0 || ultimi_vincitori[0][0] == '\0'){
       printf("Nessuna partita è stata conclusa. \n");
    }else{
      for(int i=0; i<3; i++){
         if (ultimi_vincitori[i][0] != '\0'){
            printf("%d) %s\n", i + 1, ultimi_vincitori[i]);
            }
        }
     }
   printf("Statistiche ultima partita:\n");
   printf("-Partite concluse: %d\n", partite_concluse);
   printf("-Mostri sconfitti: %d\n", mostri_sconfitti);
   printf("-Giocatori morti: %d\n", giocatori_morti);
   }


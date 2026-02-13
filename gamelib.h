//dichiarazioni
typedef enum Tipo_zona {
             bosco,
             scuola,
             laboratorio,
             caverna,
             strada,
             giardino,
             supermercato,
             centrale_elettrica,
             deposito_abbandonato,
             stazione_polizia
 }Tipo_zona;

typedef enum Tipo_nemico {
             nessun_nemico,
             billi,
             democane,
             demotorzone
 }Tipo_nemico;

typedef enum Tipo_oggetto {
             nessun_oggetto,
             bicicletta,
             maglietta_fuocoinferno,
             bussola,
             schitarrata_metallica
 }Tipo_oggetto;

struct Zona_mondoreale;
struct Zona_soprasotto;

struct Giocatore {
       char*nome;
       int mondo;   // 0 mondo reale, 1 soprasotto
       struct Zona_mondoreale *pos_mondoreale; //posizione mondoreale
       struct Zona_soprasotto *pos_soprasotto; //posizione soprasotto
       int attacco_psichico;
       int difesa_psichica;
       int fortuna;
       enum Tipo_oggetto zaino[3];
 };

struct Zona_mondoreale {
       Tipo_zona tipo;
       Tipo_nemico nemico;
       enum Tipo_oggetto oggetto;
       struct Zona_mondoreale *avanti;
       struct Zona_mondoreale *indietro;
       struct Zona_soprasotto *link_soprasotto;
 };

struct Zona_soprasotto {
       Tipo_zona tipo;
       Tipo_nemico nemico;
       struct Zona_soprasotto *avanti;
       struct Zona_soprasotto *indietro;
       struct Zona_mondoreale *mondoreale;
};
void imposta_gioco(void);
void gioca(void);
void termina_gioco(void);
void crediti(void);

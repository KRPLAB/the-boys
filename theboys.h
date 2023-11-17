#ifndef THEBOYS_H
#define THEBOYS_H

#define T_FIM_DO_MUNDO 525600
#define TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 6)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)

#define T_FIM_DO_MUNDO 525600
#define TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 6)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)
/* ------------------------- Definicao das structs padroes
 * ------------------------- */
struct coordenada {
    int x;
    int y;
};

struct heroi {
    int id; /* ID do heroi. */
    struct conjunto
        *habilidades_heroi; /* conjunto de habilidades que o heroi possui. */
    int paciencia;          /* nivel de paciencia do heroi. */
    int velocidade;         /* velocidade do heroi. */
    int experiencia;        /* experiencia adquirida pelo heroi. */
    int id_base;            /* ID da base em que o heroi se encontra. */
};

struct base {
    int id;                    /* ID da base. */
    int lotacao;               /* capacidade MAX da base. */
    struct conjunto *presente; /* conjunto de herois presentes na base. */
    struct fila *espera; /* fila de herois esperando para entrar na base. */
    struct coordenada local_base; /* coordenada da base. */
};

struct missao {
    int id;         /* ID da base. */
    int conclusao;  /* indicador de conclusao de uma missao. */
    int tentativas; /* indicador do numero de tentativas de uma missao. */
    struct conjunto *
        habs_necessarias; /* habilidades requeridas para concluir uma missao. */
    struct coordenada local_missao; /* coordenada da missão no mundo. */
};

struct mundo {
    struct missao missoes[N_MISSOESN_MISSOES]; /* vetor de missoes no mundo. */
    struct heroi herois[N_HEROISN_HEROIS];     /* vetor de herois no mundo. */
    struct base bases[N_MISSOESN_MISSOES];     /* vetor de bases no mundo. */
    struct conjunto *habilidades_mundo; /* conjunto de todas as habilidades
                                           presentes no mundo. */
    int n_herois;                       /* numero de herois no mundo. */
    int n_bases;                        /* numero de bases no mundo. */
    int n_missoes;                      /* numero de missoes no mundo. */
    int n_habilidades; /* numero de habilidades unicas no mundo. */
    int tamanho_mundo; /* tamanho do mundo. */
    int timer_mundo;   /* cronometro indicando o fim do mundo. */
};

/* ===================================== Funcoes auxiliares
 * ==================================== */

/* Calcula a distancia euclidiana entre duas coordenadas. */
int calcula_dist(struct coordenada local_atual,
                 struct coordenada local_destino);

/* Ordena os IDs das bases com base em sua distancia ate uma missao especifica.
 */
void ord_distancia_base(int vetor_indices_bases[], struct mundo *mundo,
                        struct missao *missao_d);

/* Adiciona um heroi a base, verificando se ha espaco disponivel. */
void add_heroi_base(struct base *base, int heroi_id);

/* Retorna um conjunto contendo as habilidades dos herois presentes em uma base.
 */
struct conjunto *habs_base(struct base *b, struct mundo *mundo);

/* ========================== Funcoes que tratam dos herois no mundo
 * =========================== */

/* Cria um novo heroi e retorna um ponteiro para ele. */
struct heroi cria_heroi(int id, struct mundo *m);

/* Libera a memoria alocada para um heroi e retorna NULL. */
struct heroi destroi_heroi(struct heroi *heroi);

/* Incrementa a experiencia dos herois presentes em uma base. */
void incrementa_exp(struct base *b, struct mundo *m);

/* Imprime informacoes sobre todos os herois no mundo. */
void imprime_herois_mundo(struct mundo *m);

/* =========================== Funcoes que tratam das bases no mundo
 * =========================== */

/* Cria uma nova base com o ID especificado e retorna um ponteiro para ela. */
struct base cria_base(int id);

/* Libera a memoria alocada para uma base, incluindo a fila de espera e o
 * conjunto de herois presentes. Retorna NULL. */
struct base destroi_base(struct base *b);

/* Imprime informacoes sobre todas as bases no mundo. */
void imprime_bases_mundo(struct mundo *m);

/* ========================== Funcoes que tratam das missoes no mundo
 * ========================== */

/* Cria uma nova missao com o ID especificado, associando-a ao
 * mundo fornecido, e retorna um ponteiro para ela. */
struct missao cria_missao(int id, struct mundo *mundo);

/* Libera a memoria alocada para uma missao, incluindo o conjunto de
 * habilidades necessarias. Retorna NULL. */
struct missao destroi_missao(struct missao *missao);

/* Imprime informacoes sobre as missoes no mundo, incluindo a quantidade
 * cumprida, a porcentagem cumprida e a media de tentativas por missao. */
void imprime_missoes_mundo(struct mundo *m);

/* ================================ Funcoes que tratam do mundo
 * ================================ */

/* Cria uma nova instância do mundo e retorna um ponteiro para ela. */
struct mundo *cria_mundo();

/* Libera a memória alocada para o mundo, incluindo herois, bases, missoes e
 * conjuntos de habilidades. Retorna NULL. */
struct mundo *destroi_mundo(struct mundo *m);

/* Inicializa o mundo com heróis, bases e missões iniciais, e adiciona eventos
 * correspondentes à chegada de heróis e início de missões na linha do tempo. */
void inicializa_mundo(struct mundo *mundo, struct lef_t *l);

/* ============================== Funcoes para tratar dos eventos
 * ============================== */

/* Processa o evento VIAJA, movendo o heroi para a base de destino
 * e programando o evento CHEGA correspondente. */
int ev_viaja(struct mundo *mundo, struct evento_t *viaja, struct lef_t *l);

/* Processa o evento CHEGA, avaliando se o heroi espera na base ou
 * desiste, programando os eventos correspondentes. */
int ev_chega(struct mundo *mundo, struct evento_t *chega, struct lef_t *l);

/* Processa o evento ESPERA, colocando o heroi na fila de espera da
 * base e programando o evento AVISA. */
int ev_espera(struct mundo *mundo, struct evento_t *espera, struct lef_t *l);

/* Processa o evento DESISTE, movendo o heroi para uma base aleatória
 * e programando o evento VIAJA correspondente. */
int ev_desiste(struct mundo *mundo, struct evento_t *desiste, struct lef_t *l);

/* Processa o evento ENTRA, programando o evento SAI correspondente
 * apos um tempo de permanência na base. */
int ev_entra(struct mundo *mundo, struct evento_t *entra, struct lef_t *l);

/* Processa o evento AVISA, admitindo herois da fila de espera na base
 * ate atingir a lotacao. */
int ev_avisa(struct mundo *mundo, struct evento_t *avisa, struct lef_t *l);

/* Processa o evento SAI, removendo o heroi da base de origem e programando
 * os eventos VIAJA e AVISA correspondentes. */
int ev_sai(struct mundo *mundo, struct evento_t *sai, struct lef_t *l);

/* Processa o evento MISSAO, avaliando se a missao pode ser cumprida
 * em alguma base e programando eventos adicionais. */
int ev_missao(struct mundo *mundo, struct evento_t *missao, struct lef_t *l);

/* Processa o evento FIM, imprime informacoes finais sobre os herois e
 * as missoes, e libera a memoria alocada. */
void ev_fim(struct mundo *mundo, struct evento_t *fim, struct lef_t *l);

/* Processa os eventos na linha do tempo ate que nao haja mais eventos ou
 * o evento de FIM seja alcancado. */
void processa_eventos(struct mundo *m, struct lef_t *l);

#endif

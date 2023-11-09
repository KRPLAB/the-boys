#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "conjunto.h"
#include "fila.h"
#include "lef.h"

#define T_FIM_DO_MUNDO 525600
#define TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 6)
#define N_MISSOES 5

#define E_CHEGA 1
#define E_ESPERA 2
#define E_DESISTE 3
#define E_AVISA 4
#define E_ENTRA 5
#define E_SAI 6
#define E_VIAJA 7
#define E_MISSAO 8

int aleato(int min, int max)
{
    return min + rand() % (max - min + 1);
}

struct coordenada
{
    int x;
    int y;
};

struct heroi
{
    int id;
    struct conjunto *habilidades_heroi;
    int paciencia;
    int velocidade;
    int experiencia;
    int id_base;
};

struct base
{
    int idBase;
    int lotacao;
    struct conjunto *presente;
    struct fila *espera;
    struct coordenada local_base;
};

struct missao
{
    int id;
    struct conjunto *habilidades_necessarias;
    struct coordenada local_missao;
};

struct mundo
{
    struct missao *missoes[N_MISSOES];
    struct heroi *herois[N_HEROIS];
    struct base *bases[N_BASES];
    struct conjunto *habilidades_mundo;
    int n_herois;
    int n_bases;
    int n_missoes;
    int n_habilidades;
    int tamanho_mundo;
    int timer_mundo;
};

/* ---------------- Funcoes que tratam dos herois no mundo ---------------- */

struct heroi *cria_heroi(int id, struct mundo *m)
{
    struct heroi *h;

    if (!(h = malloc(sizeof(struct heroi))))
        return NULL;

    h->id = id;
    h->paciencia = aleato(0, 100);
    h->velocidade = aleato(50, 5000);
    h->experiencia = 0;
    h->habilidades_heroi = cria_subcjt_cjt(m->habilidades_mundo, aleato(1, 3));

    return h;
};

struct heroi *destroi_heroi(struct heroi *heroi)
{
    heroi->habilidades_heroi = destroi_cjt(heroi->habilidades_heroi);
    free(heroi);
    return heroi;
}

void incrementaExperiencia();

void retornaHeroi();

void imprime_herois_mundo(struct mundo *m)
{
    if (m)
    {
        for (int i = 0; i < m->n_herois; i++)
        {
            struct heroi *heroi = m->herois[i];
            if (heroi)
            {
                printf("Herói %d:\n", heroi->id);
                printf("Paciência: %d\n", heroi->paciencia);
                printf("Velocidade: %d\n", heroi->velocidade);
                printf("Experiência: %d\n", heroi->experiencia);
                printf("Habilidades do Herói:\n");
                imprime_cjt(heroi->habilidades_heroi);
                printf("\n");
            }
        }
    }
}

/* ---------------- Funcoes que tratam das bases no mundo ---------------- */

struct base *cria_base(int id)
{
    struct base *b;

    if (!(b = malloc(sizeof(struct base))))
        return NULL;

    b->idBase = id;
    b->lotacao = aleato(3, 10);
    b->presente = cria_cjt(b->lotacao);
    b->espera = fila_cria();
    b->local_base.x = aleato(0, TAMANHO_MUNDO - 1);
    b->local_base.y = aleato(0, TAMANHO_MUNDO - 1);

    return b;
}

/* destruir uma base */
struct base *destroi_base(struct base *b)
{
    fila_destroi(&b->espera);
    destroi_cjt(b->presente);

    free(b);

    return NULL;
}

/* Retorna a habilidade dos heróois presentes na bese.*/
void habilidadeBase();

/* Função auxiliar usada para retornar uma base. */
void retornaBase();

/* ---------------- Funcoes que tratam das missoes no mundo ---------------- */

struct missao *cria_missao(int id, struct mundo *mundo)
{
    struct missao *missao;

    if (!(missao = malloc(sizeof(struct missao))))
        return NULL;

    missao->id = id;
    missao->habilidades_necessarias = cria_subcjt_cjt(mundo->habilidades_mundo, aleato(6, 10));
    missao->local_missao.x = aleato(0, TAMANHO_MUNDO - 1);
    missao->local_missao.y = aleato(0, TAMANHO_MUNDO - 1);

    return missao;
}

struct missao *destroi_missao(struct missao *missao)
{
    if (missao)
    {
        missao->habilidades_necessarias = destroi_cjt(missao->habilidades_necessarias);
        free(missao);
    }
    return NULL;
}

/* ---------------- Funcoes que tratam do mundo ---------------- */
struct mundo *cria_mundo()
{
    struct mundo *m;

    if (!(m = malloc(sizeof(struct mundo))))
        return NULL;

    m->n_herois = N_HEROIS;
    m->n_bases = N_BASES;
    m->n_missoes = N_MISSOES;
    m->tamanho_mundo = TAMANHO_MUNDO;
    m->timer_mundo = T_FIM_DO_MUNDO;
    m->habilidades_mundo = cria_cjt(N_HABILIDADES);

    if (!m->habilidades_mundo)
    {
        free(m);
        return NULL;
    }

    /* cria habilidades do mundo */
    for (int i = 0; i < N_HABILIDADES; i++)
        insere_cjt(m->habilidades_mundo, i);

    /* cria os herois do mundo */
    for (int i = 0; i < N_HEROIS; i++)
        m->herois[i] = cria_heroi(i, m);

    for (int i = 0; i < N_BASES; i++)
        m->bases[i] = cria_base(i);

    for (int i = 0; i < N_MISSOES; i++)
        m->missoes[i] = cria_missao(i, m);

    return m;
}

struct mundo *destroi_mundo(struct mundo *m)
{
    for (int i = 0; i < N_HEROIS; i++)
        destroi_heroi(m->herois[i]);

    for (int i = 0; i < N_BASES; i++)
        destroi_base(m->bases[i]);

    for (int i = 0; i < N_MISSOES; i++)
        destroi_missao(m->missoes[i]);

    m->habilidades_mundo = destroi_cjt(m->habilidades_mundo);

    free(m);

    return NULL;
}

// Função para imprimir as propriedades das bases
void imprime_bases_mundo(struct mundo *m)
{
    if (m)
    {
        for (int i = 0; i < m->n_bases; i++)
        {
            struct base *base = m->bases[i];
            if (base)
            {
                printf("Base %d:\n", base->idBase);
                printf("Lotação: %d\n", base->lotacao);
                printf("Localização: (%d, %d)\n", base->local_base.x, base->local_base.y);
                printf("Heróis Presentes na Base:\n");
                imprime_cjt(base->presente);
                printf("Heróis na Fila de Espera:\n");
                fila_imprime(base->espera);
                printf("\n");
            }
        }
    }
}

// Função para imprimir as propriedades das missões
void imprime_missoes_mundo(struct mundo *m)
{
    if (m)
    {
        for (int i = 0; i < m->n_missoes; i++)
        {
            struct missao *missao = m->missoes[i];
            printf("Missão %d:\n", missao->id);
            printf("Localização da Missão: (%d, %d)\n", missao->local_missao.x, missao->local_missao.y);
            printf("Habilidades Necessárias:\n");
            imprime_cjt(missao->habilidades_necessarias);
            printf("\n");
        }
    }
}

/*-------------------------------------Funcoes para tratar dos eventos-------------------------*/

void adicionaHeroiNaBase(struct base *base, int heroi_id) {
    // Certifique-se de que o conjunto presente ainda tem espaço para adicionar heróis
    if (base->presente->card < base->lotacao) {
        insere_cjt(base->presente, heroi_id);
    }
    // Caso contrário, trate de acordo com a lógica do seu programa, como ignorar ou recusar a adição.
}

int escolheBaseAleatoria(struct mundo *mundo)
{
    // Gere um índice de base aleatório entre 0 e (num_bases - 1)
    int base_aleatoria = aleato(0, N_BASES-1);

    return base_aleatoria;
}

struct evento_t *cria_evchega(int tempo, int heroi_id, int base_id)
{
    struct evento_t *chega;

    if (!(chega = malloc(sizeof(struct evento_t))))
        return NULL;

    chega->tipo = E_CHEGA;
    chega->tempo = tempo;
    chega->dado1 = base_id;
    chega->dado2 = heroi_id;

    return chega;
}

void process_evchega(struct mundo *mundo, struct evento_t *chega)
{
    struct base *base;
    struct heroi *heroi;
    int base_id;
    int heroi_id;
    int espera;

    base_id = chega->dado1;
    heroi_id = chega->dado2;

    base = mundo->bases[base_id];
    heroi = mundo->herois[heroi_id];

    heroi->id_base = base_id;

    if (base->espera->tamanho == 0 && base->presente->card < base->lotacao)
        espera = 1;

    else
        espera = heroi->paciencia > (base->espera->tamanho * 10);

    if (espera)
        printf("Herói %d decidiu ESPERAR na Base %d.\n", heroi_id, base_id);

    else
    {
        // Cria evento DESISTE
        // Supondo que você tenha uma função para adicionar um evento à lista de eventos
        // adicionarEvento(criaEventoDESISTE(evento.instante, heroi_id, base_id));
        printf("Herói %d decidiu DESISTIR na Base %d.\n", heroi_id, base_id);
    }
}

// Função para criar um evento ESPERA
struct evento_t *cria_evespera(int tempo, int heroi_id, int base_id)
{
    struct evento_t *espera;

    if (!(espera = malloc(sizeof(struct evento_t))))
        return NULL;

    espera->tipo = E_ESPERA;
    espera->tempo = tempo;
    espera->dado1 = base_id;
    espera->dado2 = heroi_id;

    return espera;
}

// Função para criar um evento DESISTE
struct evento_t *cria_evdesiste(int tempo, int heroi_id, int base_id)
{
    struct evento_t *desiste;

    if (!(desiste = malloc(sizeof(struct evento_t))))
        return NULL;

    desiste->tipo = E_DESISTE;
    desiste->tempo = tempo;
    desiste->dado1 = base_id;
    desiste->dado2 = heroi_id;

    return desiste;
}

// Função para criar um evento AVISA
struct evento_t *cria_avisa(int tempo, int base_id)
{
    struct evento_t *avisa;

    if (!(avisa = malloc(sizeof(struct evento_t))))
        return NULL;

    avisa->tipo = E_AVISA;
    avisa->tempo = tempo;
    avisa->dado1 = base_id;
    avisa->dado2 = 0;

    return avisa;
}

// Função para processar o evento ESPERA
void process_evespera(struct mundo *mundo, struct evento_t *espera)
{
    int base_id = espera->dado1;
    int heroi_id = espera->dado2;

    struct base *base = mundo->bases[base_id];

    // Adiciona o herói à fila de espera da base
    if (base->espera->tamanho < base->lotacao)
    {
        enqueue(base->espera, heroi_id);
    }
    else
    {
        // A fila de espera está cheia, herói não pode esperar
        // Crie um evento DESISTE ou lide com isso de acordo com a lógica do seu programa.
        printf("A fila de espera da Base %d está cheia. Herói %d desistiu.\n", base_id, heroi_id);
        return;
    }

    // Cria evento AVISA
    struct evento_t *avisa = cria_avisa(espera->tempo, base_id);
    avisa = destroi_evento(avisa);
    // Supondo que você tenha uma função para adicionar um evento à lista de eventos
    // adicionarEvento(avisa);
    printf("Herói %d entrou na fila de espera da Base %d.\n", heroi_id, base_id);
}

// Função para processar o evento DESISTE
void process_evdesiste(struct mundo *mundo, struct evento_t *desiste)
{
    int base_id = desiste->dado1;
    int heroi_id = desiste->dado2;

    // Suponha que você tenha uma função para escolher uma base destino D aleatória
    int destino_id = escolheBaseAleatoria(mundo);

    // Cria evento VIAJA
    // Supondo que você tenha uma função para adicionar um evento à lista de eventos
    // adicionarEvento(criaEventoVIAJA(desiste->tempo, heroi_id, destino_id));
    printf("Herói %d desistiu de entrar na Base %d e escolheu viajar para a Base %d.\n", heroi_id, base_id, destino_id);
}

// Função para processar o evento AVISA
void process_avisa(struct mundo *mundo, struct evento_t *avisa)
{
    int base_id = avisa->dado1;
    struct base *base = mundo->bases[base_id];

    while (base->presente->card < base->lotacao && base->espera->tamanho > 0)
    {
        int heroi_id;
        dequeue(base->espera, &heroi_id);
        // Adiciona o herói ao conjunto de heróis presentes na base
        // Supondo que você tenha funções para adicionar heróis à base
        adicionaHeroiNaBase(base, heroi_id);
        // Cria evento ENTRA
        // Supondo que você tenha uma função para adicionar um evento à lista de eventos
        // adicionarEvento(criaEventoENTRA(avisa->tempo, heroi_id, base_id));
        printf("Herói %d entrou na Base %d.\n", heroi_id, base_id);
    }
}

/*---------------------------------------------------------------------------------------------*/

int main()
{
    /* declarações de variáveis aqui */
    struct mundo *mundo;
    struct evento_t *chegada_heroi;
    srand(0);

    printf("\nIniciando o mundo, bases e heróis\n");
    mundo = cria_mundo();

    // Vendo as habilidades do mundo
    printf("\nHabilidades existentes no mundo:\n");
    imprime_cjt(mundo->habilidades_mundo);

    // Teste das funções de impressão
    printf("\nPropriedades dos Heróis:\n");
    imprime_herois_mundo(mundo);

    printf("\nPropriedades das Bases:\n");
    imprime_bases_mundo(mundo);

    printf("\nPropriedades das Missões:\n");
    imprime_missoes_mundo(mundo);

    // Simulacao evento CHEGA
    printf("\nEvento CHEGA\n");
    chegada_heroi = cria_evchega(0, 0, 0);
    process_evchega(mundo, chegada_heroi);
    chegada_heroi = destroi_evento(chegada_heroi);

    // Simulacao evento ESPERA
    printf("\nEvento ESPERA\n");
    struct evento_t *espera_heroi = cria_evespera(0, 1, 0);
    process_evespera(mundo, espera_heroi);
    espera_heroi = destroi_evento(espera_heroi);

    // Simulação de um evento DESISTE
    printf("\nEvento DESISTE\n");
    struct evento_t *desiste_heroi = cria_evdesiste(10, 2, 0);
    process_evdesiste(mundo, desiste_heroi);
    desiste_heroi = destroi_evento(desiste_heroi);

    // Simulação de um evento AVISA
    printf("\nEvento AVISA\n");
    struct evento_t *avisa_base = cria_avisa(20, 0);
    process_avisa(mundo, avisa_base);
    avisa_base = destroi_evento(avisa_base);

    printf("\ndestruindo o mundo, bases e heróis\n");
    /* Destruir tudo que iniciei dentro da função mundo. */
    mundo = destroi_mundo(mundo);

    return 0;
}

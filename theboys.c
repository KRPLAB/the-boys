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
#define N_MISSOES (T_FIM_DO_MUNDO / 100)

#define E_CHEGA 1
#define E_ESPERA 2
#define E_DESISTE 3
#define E_AVISA 4
#define E_ENTRA 5
#define E_SAI 6
#define E_VIAJA 7
#define E_MISSAO 8
#define E_FIM 9

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

int aleato(int min, int max)
{
    return min + (rand() % (max - min + 1));
}

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

void inicializa_mundo(struct mundo *mundo, struct lef_t *l)
{
    int base, tempo;

    // gerando eventos iniciais para cada heroi
    for (int i = 0; i < N_HEROIS; i++)
    {
        base = aleato(0, N_BASES - 1);
        tempo = aleato(0, 4321);
        insere_lef(l, cria_evento(tempo, E_CHEGA, base, i));
    }

    // gerando eventos iniciais para cada missão
    for (int i = 0; i < N_MISSOES; i++)
    {
        tempo = aleato(0, T_FIM_DO_MUNDO + 1);
        insere_lef(l, cria_evento(tempo, E_MISSAO, i, base));
    }

    // gerando evento fim do mundo
    insere_lef(l, cria_evento(T_FIM_DO_MUNDO, E_FIM, 0, 0));
}

/*-------------------------------------Funcoes para tratar dos eventos-------------------------*/

void adicionaHeroiNaBase(struct base *base, int heroi_id)
{
    if (base->presente->card < base->lotacao)
        insere_cjt(base->presente, heroi_id);
}

int calcularDistancia(int id_base_atual, int id_base_dest, struct mundo *m)
{
    struct coordenada p1, p2;
    int x, y;

    p1 = m->bases[id_base_atual]->local_base;
    p2 = m->bases[id_base_dest]->local_base;
    x = (p2.x - p1.x) * (p2.x - p1.x);
    y = (p2.y - p1.y) * (p2.y - p1.y);

    return sqrt(x + y);
}

/* Processa um evento VIAJA */
int ev_viaja(struct mundo *mundo, struct evento_t *viaja, struct lef_t *l)
{
    struct evento_t *chega;
    int heroi_id, destino_id;
    int distancia, duracao;

    destino_id = viaja->dado1;
    heroi_id = viaja->dado2;

    distancia = calcularDistancia(mundo->herois[heroi_id]->id_base, viaja->dado1, mundo);
    duracao = distancia / mundo->herois[heroi_id]->velocidade;

    if (!(chega = cria_evento(viaja->tempo + duracao, E_CHEGA, destino_id, heroi_id)))
        return 0;

    insere_lef(l, chega);

    return 1;
}

/* Processa um evento CHEGA */
int ev_chega(struct mundo *mundo, struct evento_t *chega, struct lef_t *l)
{
    struct heroi *heroi;
    int base_id, heroi_id;
    int espera;

    base_id = chega->dado1;
    heroi_id = chega->dado2;

    heroi = mundo->herois[heroi_id];
    heroi->id_base = base_id;

    if (mundo->bases[base_id]->espera->tamanho == 0 &&
        mundo->bases[base_id]->presente->card < mundo->bases[base_id]->lotacao)
        espera = 1;

    else
        espera = heroi->paciencia > (mundo->bases[base_id]->espera->tamanho * 10);

    if (espera)
    {
        insere_lef(l, cria_evento(chega->tempo, E_ESPERA, base_id, heroi_id));
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", chega->tempo, 
                heroi_id, base_id, mundo->bases[base_id]->presente->card, 
                mundo->bases[base_id]->lotacao);
    }

    else
    {
        insere_lef(l, cria_evento(chega->tempo, E_DESISTE, base_id, heroi_id));
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", chega->tempo, 
                heroi_id, base_id, mundo->bases[base_id]->presente->card, 
                mundo->bases[base_id]->lotacao);
    }

    return 1;
}

/* Processa um evento ESPERA */
int ev_espera(struct mundo *mundo, struct evento_t *espera, struct lef_t *l)
{
    struct base *base;
    int base_id, heroi_id;

    base_id = espera->dado1;
    heroi_id = espera->dado2;

    base = mundo->bases[base_id];

    enqueue(base->espera, heroi_id);

    insere_lef(l, cria_evento(espera->tempo, E_AVISA, base_id, heroi_id));
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", espera->tempo, heroi_id, base_id,
            base->espera->tamanho);

    return 1;
}

/* Processa um evento DESISTE */
int ev_desiste(struct mundo *mundo, struct evento_t *desiste, struct lef_t *l)
{
    int base_id, heroi_id;
    int destino_id;

    base_id = desiste->dado1;
    heroi_id = desiste->dado2;
    destino_id = aleato(0, N_BASES - 1);

    insere_lef(l, cria_evento(desiste->tempo, E_VIAJA, destino_id, heroi_id));
    printf("%6d: DESIST HEROI %2d BASE %d\n", desiste->tempo, heroi_id, base_id);

    return 1;
}

/* Processa um evento ENTRA */
int ev_entra(struct mundo *mundo, struct evento_t *entra, struct lef_t *l)
{
    int tpb; // tempo de permanencia na base
    int heroi_id;

    heroi_id = entra->dado2;

    tpb = 15 + mundo->herois[heroi_id]->paciencia * aleato(1, 20);

    insere_lef(l, cria_evento(entra->tempo + tpb, E_SAI, entra->dado1, heroi_id));

    return 1;
}

/* Processa um evento AVISA */
int ev_avisa(struct mundo *mundo, struct evento_t *avisa, struct lef_t *l)
{
    struct base *base = mundo->bases[avisa->dado1];

    while (base->presente->card < base->lotacao && base->espera->tamanho > 0)
    {
        int heroi_id;
        dequeue(base->espera, &heroi_id);
        adicionaHeroiNaBase(base, heroi_id);
        insere_lef(l, cria_evento(avisa->tempo, E_ENTRA, base->idBase, heroi_id));
        printf("Herói %d entrou na Base %d.\n", heroi_id, avisa->dado1);
    }

    return 1;
}

/* Processa um evento SAI */
int ev_sai(struct mundo *mundo, struct evento_t *sai, struct lef_t *l)
{
    struct conjunto *herois_base;
    int heroi_id, base_id, destino;

    base_id = mundo->bases[sai->dado1]->idBase;
    heroi_id = mundo->herois[sai->dado2]->id;
    herois_base = mundo->bases[base_id]->presente;

    retira_cjt(herois_base, heroi_id);

    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", sai->tempo, heroi_id, base_id,
            mundo->bases[base_id]->presente->card, 
                mundo->bases[base_id]->lotacao);

    destino = aleato(0, N_BASES - 1);

    insere_lef(l, cria_evento(sai->tempo, E_VIAJA, destino, heroi_id));
    insere_lef(l, cria_evento(sai->tempo, E_AVISA, base_id, 0));

    return 1;
}

/* Processa o evento FIM */
void ev_fim(struct mundo *mundo, struct evento_t *fim, struct lef_t *l)
{
    imprime_herois_mundo(mundo);
    imprime_missoes_mundo(mundo);
    fim = destroi_evento(fim);

    l = destroi_lef(l);

    mundo = destroi_mundo(mundo);
}

void processa_eventos(struct mundo *m, struct lef_t *l)
{
    struct evento_t *evento;

    // Percorre a LEF e processa os eventos
    while ((evento = retira_lef(l)) != NULL)
    {
        switch (evento->tipo)
        {
        case E_CHEGA:
            ev_chega(m, evento, l);
            break;
        case E_ESPERA:
            ev_espera(m, evento, l);
            break;
        case E_DESISTE:
            ev_desiste(m, evento, l);
            break;
        case E_AVISA:
            ev_avisa(m, evento, l);
            break;
        case E_ENTRA:
            ev_entra(m, evento, l);
            break;
        case E_SAI:
            ev_sai(m, evento, l);
            break;
        case E_VIAJA:
            ev_viaja(m, evento, l);
            break;
        }

        if(evento->tipo == E_FIM)
        {
            ev_fim(m, evento, l);
            return;
        }

        evento = destroi_evento(evento);
    }
}

/*---------------------------------------------------------------------------------------------*/

int main()
{
    /* declarações de variáveis aqui */
    struct mundo *mundo;
    struct lef_t *linha_do_tempo;

    srand(0);

    printf("\nIniciando o mundo, bases e heróis\n");
    mundo = cria_mundo();

    printf("\nHabilidades existentes no mundo:\n");
    imprime_cjt(mundo->habilidades_mundo);

    printf("\nCriando a LEF\n");
    linha_do_tempo = cria_lef();

    printf("\nEventos iniciais do mundo na linha do tempo\n");
    inicializa_mundo(mundo, linha_do_tempo);

    printf("\nProcessando os eventos do mundo\n");
    processa_eventos(mundo, linha_do_tempo);

    printf("FIM DO MUNDO");

    return 0;
}

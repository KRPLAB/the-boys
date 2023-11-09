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

/*---------------------------------------------------------------------------------------------*/

int main()
{
    /* declarações de variáveis aqui */
    struct mundo *mundo;
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

    printf("\ndestruindo o mundo, bases e heróis\n");
    /* Destruir tudo que iniciei dentro da função mundo. */
    mundo = destroi_mundo(mundo);

    return 0;
}

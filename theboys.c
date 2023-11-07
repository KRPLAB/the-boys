
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
    struct missao missoes[N_MISSOES];
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


void cria_herois(struct mundo *m)
{
    for(int i = 0; i < N_HEROIS; i++)
    {
        m->herois[i]->id = i;
        m->herois[i]->paciencia = aleat(0, 100);
        m->herois[i]->velocidade = aleat(50, 5000);
        m->herois[i]->experiencia = 0;
        m->herois[i]->habilidades_heroi = cria_subcjt_cjt(m->habilidades_mundo, 3);
    }
}

struct heroi *destroi_heroi(struct heroi *heroi)
{
    heroi->habilidades_heroi = destroi_cjt(heroi->habilidades_heroi);   
    return heroi;
}

/* Incrementa experiência dos herós que cumpriram uma missão. */
void incrementaExperiencia();

/* Função auxiliar usada para retornar um herói. */
void retornaHeroi();

/*---------------------------------------------------------------------------------------------*/

/* cria bases uma base */
struct base *cria_base(int id)
{
    struct base *b;

    if(!(b = malloc(sizeof(struct base))))
        return NULL;

    b->idBase = id;
    b->lotacao = aleat(3, 10);
    b->presente = cria_cjt(b->lotacao);
    b->espera = fila_cria();
    b->local_base.x = aleat(0, TAMANHO_MUNDO-1);
    b->local_base.y = aleat(0, TAMANHO_MUNDO-1);

    return b; 
}


/* loop para criar as bases do mundo */
void cria_bases_mundo(struct mundo *m)
{
    for(int i = 0; i < N_BASES; i++)
        m->bases[i] = cria_base(i);
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


/*---------------------------------------------------------------------------------------------*/


/* Cria o mundo com os parâmetros. */
struct mundo *cria_mundo()
{
    struct mundo *m;

    if(!(m = malloc(sizeof(struct mundo))))
        return NULL;
    
    m->n_herois = N_HEROIS;
    m->n_bases = N_BASES;
    m->n_missoes = N_MISSOES;
    m->tamanho_mundo = TAMANHO_MUNDO;
    m->timer_mundo = T_FIM_DO_MUNDO;
    cria_herois(m);
    cria_bases_mundo(m);
    m->habilidades_mundo = cria_cjt(N_HABILIDADES);

    /* preenche o conjunto de habilidades do mundo.*/
    for(int i = 0; i < N_HABILIDADES; i++)
        insere_cjt(m->habilidades_mundo, i);

    return m;
}
      
struct mundo *destroi_mundo(struct mundo *m)
{
    /* Liberar a memória dos heróis. */
    for(int i = 0; i < N_HEROIS; i++)
        destroi_heroi(m->herois[i]);

    /* Liberar a memória das bases. */
    for(int i = 0; i < N_BASES; i++)
        destroi_base(m->bases[i]);

    m->habilidades_mundo = destroi_cjt(m->habilidades_mundo);

    free(m);

    return NULL;
}   

/*---------------------------------------------------------------------------------------------*/
/* demais includes */
/* funcoes que voce ache necessarias aqui */

int main ()
{
    /* declaracoes de variaveis aqui */
    struct mundo *mundo;
    srand(0);

    printf("\nIniciando o mundo, bases e heróis\n");
    /* coloque seu codigo aqui */
    mundo = cria_mundo();
    
    printf("\ndestruindo o mundo, bases e heróis\n");
    /* Destuir tudo que iniciei destro da função mundo. */
    mundo = destroi_mundo(mundo);

    return 0;
}

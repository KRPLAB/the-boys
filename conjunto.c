#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"

/* ---- Funcoes auxiliares para ordenacao e randomizacao ---- */

/* troca numeros */
void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* particiona o vetor em duas partes e escolhe o pivo */
int partition(int v[], int a, int b)
{
    int x = v[b];
    int i = (a - 1);

    for (int j = a; j <= b - 1; j++)
    {
        if (v[j] <= x)
        {
            i++;
            swap(&v[i], &v[j]);
        }
    }
    swap(&v[i + 1], &v[b]);
    return (i + 1);
}

/* algoritmo de ordenacao quicksort recursivo*/
void quickSort(int v[], int a, int b)
{
    if (a < b)
    {
        int p = partition(v, a, b);
        quickSort(v, a, p - 1);
        quickSort(v, p + 1, b);
    }
}

int aleat(int min, int max)
{
    return min + rand() % (max - min + 1);
}

/* ---------------------------------------------------------------- */

struct conjunto *cria_cjt(int max)
{
    struct conjunto *novo_cjt;

    if (!(novo_cjt = malloc(sizeof(struct conjunto))))
        return NULL;

    if (!(novo_cjt->v = malloc(sizeof(int) * max)))
    {
        free(novo_cjt);
        return NULL;
    }

    novo_cjt->max = max;
    novo_cjt->card = 0;
    inicia_iterador_cjt(novo_cjt);

    return novo_cjt;
}

int vazio_cjt(struct conjunto *c)
{
    return !c->card;
}

struct conjunto *destroi_cjt(struct conjunto *c)
{
    if (c)
    {
        if (c->v)
        {
            free(c->v);
        }
        free(c);
    }

    return NULL;
}

int cardinalidade_cjt(struct conjunto *c)
{
    return c->card;
}

int pertence_cjt(struct conjunto *c, int elemento)
{
    for (int i = 0; i < c->card; i++)
        if (c->v[i] == elemento)
            return 1;

    return 0;
}

int insere_cjt(struct conjunto *c, int elemento)
{
    if (c->card >= c->max)
        return 0;

    for (int i = 0; i < c->card; i++)
        if (c->v[i] == elemento)
            return 1;

    c->v[c->card++] = elemento;
    return 1;
}

int retira_cjt(struct conjunto *c, int elemento)
{
    for (int i = 0; i < c->card; i++)
        if (c->v[i] == elemento)
        {
            for (int j = i; j < c->card - 1; j++)
                c->v[j] = c->v[j + 1];
            c->card--;
            return 1;
        }
    return 0;
}

int contido_cjt(struct conjunto *c1, struct conjunto *c2)
{
    for (int i = 0; i < c1->card; i++)
        if (!pertence_cjt(c2, c1->v[i]))
            return 0;

    return 1;
}

int sao_iguais_cjt(struct conjunto *c1, struct conjunto *c2)
{
    int i;

    if (c1->card != c2->card)
        return 0;

    i = 0;
    while (i < c1->card && c1->v[i] == c2->v[i])
        i++;

    return i <= c1->card;
}

struct conjunto *diferenca_cjt(struct conjunto *c1, struct conjunto *c2)
{
    struct conjunto *diff;
    int i, j, k;

    if (!(diff = cria_cjt(c1->max)))
        return NULL;

    i = 0;
    j = 0;
    k = 0;
    while (i < c1->card && j < c2->card)
    {
        if (c1->v[i] < c2->v[j])
        {
            diff->v[k] = c1->v[i];
            i++;
            k++;
        }

        if (c1->v[i] > c2->v[j])
            j++;
        else
        {
            i++;
            j++;
        }
    }

    /* se ainda tem elementos em c1, copia */
    for (int j = i; j < c1->card; j++)
    {
        k++;
        diff->v[k] = c1->v[j];
    }

    diff->card = k;

    return diff;
}

struct conjunto *interseccao_cjt(struct conjunto *c1, struct conjunto *c2)
{
    struct conjunto *intersec;
    int i, j, k;

    if (!(intersec = cria_cjt(c1->max)))
        return NULL;

    i = 0;
    j = 0;
    k = 0;
    while (i < c1->card && j < c2->card)
    {
        if (c1->v[i] < c2->v[j])
            i++;
        if (c1->v[i] > c2->v[j])
            j++;
        else
        {
            intersec->v[k] = c1->v[i];
            k++;
            i++;
            j++;
        }
    }

    intersec->card = k;

    return intersec;
}

struct conjunto *uniao_cjt(struct conjunto *c1, struct conjunto *c2)
{
    struct conjunto *uni, *diff;
    int tam_max;
    int i = 0, j = 0, k = 0;

    if (!c1 || !c2)
        return NULL;

    if (c1->card == 0)
        return copia_cjt(c2);

    if (c2->card == 0)
        return copia_cjt(c1);

    if (c1->card >= c2->card)
    {
        diff = diferenca_cjt(c1, c2);
        tam_max = diff->card + c1->card;
    }
    else
    {
        diff = diferenca_cjt(c2, c1);
        tam_max = diff->card + c2->card;
    }

    if (!(uni = cria_cjt(tam_max)))
        return NULL;

    while (i < c1->card && j < c2->card)
    {
        if (c1->v[i] < c2->v[j])
        {
            uni->v[k] = c1->v[i];
            i++;
        }
        else if (c1->v[i] > c2->v[j])
        {
            uni->v[k] = c2->v[j];
            j++;
        }
        else
        {
            uni->v[k] = c1->v[i];
            i++;
            j++;
        }
        k++;
    }

    while (i < c1->card)
    {
        uni->v[k] = c1->v[i];
        i++;
        k++;
    }

    while (j < c2->card)
    {
        uni->v[k] = c2->v[j];
        j++;
        k++;
    }

    uni->card = k;

    destroi_cjt(diff);
    return uni;
}

struct conjunto *copia_cjt(struct conjunto *c)
{
    struct conjunto *copia;

    if (!(copia = cria_cjt(c->max)))
        return NULL;

    copia->card = c->card;
    copia->ptr = c->ptr;

    for (int i = 0; i < c->card; i++)
    {
        copia->v[i] = c->v[i];
    }

    return copia;
}

struct conjunto *cria_subcjt_cjt(struct conjunto *c, int n)
{
    struct conjunto *sub;
    int *indices_aleatorios;
    int tam;

    if (c->card == 0 || n >= c->card)
        return copia_cjt(c);

    if (!(sub = cria_cjt(c->card)))
        return NULL;

    if (!(indices_aleatorios = malloc(sizeof(int) * c->card)))
        return NULL;

    tam = c->card;
    for (int i = 0; i < tam; i++)
        indices_aleatorios[i] = i;

    for (int i = 0; i < n; i++)
    {
        int indice_aleatorio = aleat(0, tam - 1);
        sub->card++;
        sub->v[i] = c->v[indices_aleatorios[indice_aleatorio]];
        /* substituindo o indice selecionado pelo ultimo indice valido */
        indices_aleatorios[indice_aleatorio] = indices_aleatorios[tam - 1];
        tam--;
    }

    free(indices_aleatorios);

    return sub;
}

void imprime_cjt(struct conjunto *c)
{
    if (!(c) || !(c->card))
    {
        printf("[ ]\n");
        return;
    }

    /* ordena antes de imprimir*/
    quickSort(c->v, 0, c->card - 1);

    printf("[ ");
    for (int i = 0; i < c->card; i++)
    {
        printf("%d", c->v[i]);
        if (i < c->card - 1)
            printf(" ");
    }

    printf(" ]\n");
    return;
}

void inicia_iterador_cjt(struct conjunto *c)
{
    if (c)
        c->ptr = 0;
}

int incrementa_iterador_cjt(struct conjunto *c, int *ret_iterador)
{
    if (!(c) || c->ptr >= c->card)
        return 0;

    *ret_iterador = c->v[c->ptr];
    c->ptr++;

    return 1;
}

int retira_um_elemento_cjt(struct conjunto *c)
{
    int indice;
    int elemento;

    indice = aleat(0, c->card - 1);
    elemento = c->v[indice];

    c->v[indice] = c->v[c->card - 1];
    c->card--;
    quickSort(c->v, 0, c->card - 1);

    return elemento;
}

#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

struct fila *fila_cria ()
{
	struct fila *f;

	if(!(f = malloc(sizeof(struct fila))))
		return NULL;

	f->ini = NULL;
	f->fim = NULL;
	f->tamanho = 0;

	return f;
}

int fila_vazia (struct fila *fila)
{
	return !fila->tamanho;
}

void fila_destroi (struct fila **fila)
{
	struct nodo *aux;

	while(!fila_vazia((*fila)))
	{
		aux = (*fila)->ini;
		(*fila)->ini = (*fila)->ini->prox;
		free(aux);
		(*fila)->tamanho--;	
	}

	free(*fila);
	(*fila) = NULL;
}

int enqueue (struct fila *fila, int dado)
{
	struct nodo *novo;

	if(!(novo = malloc(sizeof(struct nodo))))
		return 0;
	novo->chave = dado;
	novo->prox = NULL;

	if(fila_vazia(fila))
	{
		fila->ini = novo;

		fila->fim = novo;
		fila->tamanho++;

		return 1;
	}

	fila->fim->prox = novo;
	fila->fim = novo;
	fila->tamanho++;

	return 1;
}

int dequeue (struct fila *fila, int *dado)
{
	struct nodo *aux;

	if(fila_vazia(fila))
		return 0;

	aux = fila->ini;
	*dado = aux->chave;
	fila->ini = fila->ini->prox;
	fila->tamanho--;
	free(aux);

	return 1;
}

int fila_tamanho (struct fila *fila)
{
	return fila->tamanho;
}

void fila_imprime(struct fila *fila) {
    struct nodo *atual = fila->ini;

    printf("[ ");
    while (atual != NULL) {
        printf("%d ", atual->chave);
        atual = atual->prox;
    }
    printf("]\n");
}


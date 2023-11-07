/* 
 * Programa para testar a libfila.
 * feito por Marcos Castilho em 05/04/2022
*/

#include <stdio.h>
#include "fila.h"
#define MAX 5

struct fila* teste_criar_fila ()
{
    struct fila* f;

    if (! (f = fila_cria ()))
        fprintf (stderr, "Falha na alocacao da fila\n");

    printf ("\tFila criada com sucesso\n");
    return f;
}

void teste_fila_vazia (struct fila* f)
{
    if (fila_vazia (f))
        printf ("\tfila esta vazia");
    else
        printf ("\tfila nao esta vazia");
    printf (", seu tamanho eh %d.\n", fila_tamanho (f));
}

void teste_enqueue (struct fila* f, int limite)
{
    int i;

    for (i=1; i <= limite; i++)
    {
        printf ("\tEnfileirando %d\n", i + 10);
        if (! enqueue (f, i + 10)){
            printf ("Fila cheia, nao inseriu.\n");
            return;
        }
    }
    fila_imprime (f);
}

void teste_dequeue (struct fila* f)
{
    int dado;

    /* remove todo mundo */
    while (! fila_vazia (f))
    {
        if (dequeue (f, &dado))
            printf ("\tDesenfileirando %d\n", dado);
    }
    fila_imprime (f);
}

int main (void)
{
    struct fila *f;
    int dado;

    printf ("Teste 1: criar fila e ver se esta vazia:\n");
    printf ("Esperado: tem que dar fila vazia com tamanho zero\n");
    f = teste_criar_fila ();
    teste_fila_vazia (f);
    printf ("\n\n");

    printf ("Teste 2: tenta desenfileirar elemento com fila vazia:\n");
    printf ("Esperado: nao pode ter dado segfault\n");
    if (! dequeue (f, &dado))
        printf ("\tOK: dequeue na fila vazia nao deu segfault.\n");
    printf ("\n\n");

    printf ("Teste 3: inserir 5 elementos:\n");
    printf ("Esperado: tem que mostrar enfileirar de 11 ate 15\n");
    printf ("          o tamanho tem que terminar com valor 5\n");
    printf ("          E a fila NAO PODE estar vazia\n");
    teste_enqueue (f, MAX); 
    teste_fila_vazia (f);
    printf ("\n\n");

    printf ("Teste 4: esvaziar a fila:\n");
    printf ("Esperado: tem que mostrar desenfileirar de 11 ate 15\n");
    printf ("          o tamanho tem que terminar com valor 0\n");
    printf ("          E a fila tem que estar vazia\n");
    teste_dequeue (f); 
    teste_fila_vazia (f);
    printf ("\n\n");

    printf ("Teste 5: destruir uma fila vazia:\n");
    printf ("Esperado: nao pode ter leak (conferir com valdrind)\n");
    printf ("          E nao pode ter segfault\n");
    fila_destroi (&f);
    printf ("\n\n");

    printf ("Teste 6: destruir uma fila com elementos:\n");
    printf ("Esperado: nao pode ter leak (conferir com valdrind)\n");
    printf ("          E nao pode ter segfault\n");
    f = fila_cria ();
    if (enqueue (f, 1) && enqueue (f, 2) && enqueue (f, 3))
        fila_destroi (&f);
    else
        printf ("Falha na alocação dos elementos!!!");
    printf ("\n\n");

    printf ("Teste 7: Teste de aterramento do ponteiro:\n");
    printf ("         Nao pode ter segfault\n");
    if (f != NULL)
        printf ("ERRO: fila nao eh NULL apos fila_destroi\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#define TAM 10

int main(void)
{
    srand(0);
    struct conjunto *a, *b;
    int k, ex;

    printf("---> Teste 1 - Cria cjt\n");
    if (!(a = cria_cjt(TAM)))
    {
        printf("falha na criacao do conjunto\n");
        return 1;
    }

    for (k = 0; k < TAM; k++)
    {
        ex = insere_cjt(a, k);
        if (ex == 0)
        {
            printf("acabou espaco ao tentar inserir %d! Pode aumentar se quiser\n", 2 * k);
        }
    }

    imprime_cjt(a);
    printf("Conjunto A\n");

    /* Criando conjunto para testes de uniao, interseccao e diferenca */
    printf("---> Teste 2\n");
    if (!(b = cria_cjt(TAM)))
    {
        printf("falha na criacao do conjunto\n");
        return 1;
    }
    for (k = 0; k < 10; k++)
    {
        ex = insere_cjt(b, (rand() % 9));
        if (ex == 0)
        {
            printf("acabou espaco ao tentar inserir %d!\n", k);
        }
    }
    imprime_cjt(b);
    printf("Conjunto B\n");

    /* Teste de estar contido e contem */
    printf("---> Teste 8\n");
    if (!contido_cjt(b, a))
    {
        printf("ERRO: o conjunto a esta garantidamente contido no conjunto b e o programa falhou\n");
        return 1;
    }
    printf("OK: conjunto b esta contido no conjunto a\n");

    if (contido_cjt(a, b))
    {
        printf("ERRO: o conjunto a nao esta contido no conjunto b e o programa falhou\n");
        return 1;
    }
    printf("OK: conjunto a nao esta contido no conjunto b\n");

    /* Destroi todos os conjuntos criados */
    destroi_cjt(a);
    destroi_cjt(b);

    return 0;
}

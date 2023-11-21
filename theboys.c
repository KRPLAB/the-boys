#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "conjunto.h"
#include "fila.h"
#include "lef.h"
#include "theboys.h"

#define E_CHEGA 1
#define E_ESPERA 2
#define E_DESISTE 3
#define E_AVISA 4
#define E_ENTRA 5
#define E_SAI 6
#define E_VIAJA 7
#define E_MISSAO 8
#define E_FIM 9

int aleato(int min, int max)
{
    return min + (rand() % (max - min + 1));
}

int calcula_dist(struct coordenada local_atual, struct coordenada local_destino)
{
    int x, y;

    x = (local_destino.x - local_atual.x) * (local_destino.x - local_atual.x);
    y = (local_destino.y - local_atual.y) * (local_destino.y - local_atual.y);

    return sqrt(x + y);
}

int particao(int v[], struct mundo *m, int min, int max, struct coordenada local_destino)
{
    int pivo = v[max];
    int i = min - 1;

    for (int j = min; j <= max - 1; j++)
    {
        int dist_j = calcula_dist(m->bases[v[j]].local_base, local_destino);
        int dist_pivo = calcula_dist(m->bases[pivo].local_base, local_destino);

        if (dist_j < dist_pivo)
        {
            i++;
            int temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }

    /* trocar o indice pivo com o proximo indice */
    int temp = v[i + 1];
    v[i + 1] = v[max];
    v[max] = temp;

    return i + 1;
}

void quicksort(int v[], struct mundo *m, int min, int max, struct coordenada local_destino)
{
    if (min < max)
    {
        int pi = particao(v, m, min, max, local_destino);

        quicksort(v, m, min, pi - 1, local_destino);
        quicksort(v, m, pi + 1, max, local_destino);
    }
}

struct heroi cria_heroi(int id, struct mundo *m)
{
    struct heroi h;

    h.id = id;
    h.paciencia = aleato(0, 100);
    h.velocidade = aleato(50, 5000);
    h.experiencia = 0;
    h.habilidades_heroi = cria_subcjt_cjt(m->habilidades_mundo, aleato(1, 3));

    return h;
}

struct heroi destroi_heroi(struct heroi *heroi)
{
    heroi->habilidades_heroi = destroi_cjt(heroi->habilidades_heroi);
    return (*heroi);
}

void incrementa_exp(struct base *b, struct mundo *m)
{
    for (int i = 0; i < b->presente->card; i++)
        m->herois[b->presente->v[i]].experiencia++;
}

void imprime_herois_mundo(struct mundo *m)
{
    for (int i = 0; i < m->n_herois; i++)
    {
        struct heroi heroi = m->herois[i];

        printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ", heroi.id,
               heroi.paciencia, heroi.velocidade, heroi.experiencia);
        imprime_cjt(heroi.habilidades_heroi);
    }
}

struct base cria_base(int id)
{
    struct base b;

    b.id = id;
    b.lotacao = aleato(3, 10);
    b.presente = cria_cjt(b.lotacao);
    b.espera = fila_cria();
    b.local_base.x = aleato(0, TAMANHO_MUNDO - 1);
    b.local_base.y = aleato(0, TAMANHO_MUNDO - 1);

    return b;
}

struct base destroi_base(struct base *b)
{
    fila_destroi(&b->espera);
    destroi_cjt(b->presente);

    return (*b);
}

void imprime_bases_mundo(struct mundo *m)
{

    for (int i = 0; i < m->n_bases; i++)
    {
        struct base base = m->bases[i];
        printf("Base %d:\n", base.id);
        printf("Lotação: %d\n", base.lotacao);
        printf("Localização: (%d, %d)\n", base.local_base.x, base.local_base.y);
        printf("Heróis Presentes na Base:\n");
        imprime_cjt(base.presente);
        printf("Heróis na Fila de Espera:\n");
        fila_imprime(base.espera);
        printf("\n");
    }
}

struct missao cria_missao(int id, struct mundo *mundo)
{
    struct missao missao;

    missao.id = id;
    missao.habs_necessarias = cria_subcjt_cjt(mundo->habilidades_mundo, aleato(6, 10));
    missao.local_missao.x = aleato(0, TAMANHO_MUNDO - 1);
    missao.local_missao.y = aleato(0, TAMANHO_MUNDO - 1);
    missao.conclusao = 0;
    missao.tentativas = 0;

    return missao;
}

struct missao destroi_missao(struct missao *missao)
{
    missao->habs_necessarias = destroi_cjt(missao->habs_necessarias);

    return (*missao);
}

void imprime_missoes_mundo(struct mundo *m)
{
    int tot_concluidas;
    int tot_tentativas;

    tot_concluidas = 0;
    tot_tentativas = 0;
    for (int i = 0; i < m->n_missoes; i++)
    {
        struct missao missao = m->missoes[i];
        if (missao.conclusao)
            tot_concluidas++;

        tot_tentativas += missao.tentativas;
    }

    printf("%d/%d MISSOES CUMPRIDAS(%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n",
           tot_concluidas, N_MISSOES, ((float)tot_concluidas * 100) / N_MISSOES,
           tot_tentativas / (float)N_MISSOES);
}

struct conjunto *habs_base(struct base *b, struct mundo *mundo)
{
    struct conjunto *habs_totais_base;

    if (!(habs_totais_base = cria_cjt(N_HABILIDADES)))
        return NULL;

    for (int i = 0; i < b->presente->card; i++)
    {
        struct heroi h = mundo->herois[b->presente->v[i]];
        for (int j = 0; j < h.habilidades_heroi->card; j++)
            insere_cjt(habs_totais_base, h.habilidades_heroi->v[j]);
    }

    return habs_totais_base;
}

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

    if (!(m->habilidades_mundo = cria_cjt(N_HABILIDADES)))
    {
        free(m);
        return NULL;
    }

    /* criando habilidades do mundo */
    for (int i = 0; i < N_HABILIDADES; i++)
        insere_cjt(m->habilidades_mundo, i);

    /* criando entidades do mundo */
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
        destroi_heroi(&m->herois[i]);

    for (int i = 0; i < N_BASES; i++)
        destroi_base(&m->bases[i]);

    for (int i = 0; i < N_MISSOES; i++)
        destroi_missao(&m->missoes[i]);

    m->habilidades_mundo = destroi_cjt(m->habilidades_mundo);

    free(m);

    return NULL;
}

void inicializa_mundo(struct mundo *mundo, struct lef_t *l)
{
    int tempo;

    for (int i = 0; i < mundo->n_herois; i++)
    {
        mundo->herois[i].id_base = aleato(0, N_BASES - 1);
        tempo = aleato(0, 4320);
        insere_lef(l, cria_evento(tempo, E_CHEGA, mundo->herois[i].id_base, i));
    }

    /* setando os tempos e criando eventos MISSAO para cada missão */
    for (int i = 0; i < mundo->n_missoes; i++)
    {
        tempo = aleato(0, T_FIM_DO_MUNDO);
        insere_lef(l, cria_evento(tempo, E_MISSAO, i, 0));
    }

    /* gerando evento para o fim do mundo */
    insere_lef(l, cria_evento(T_FIM_DO_MUNDO, E_FIM, 0, 0));
}

void ord_distancia_base(int vetor_indices_bases[], struct mundo *mundo, struct missao *missao_d)
{
    for (int i = 0; i < N_BASES; i++)
        vetor_indices_bases[i] = i;

    quicksort(vetor_indices_bases, mundo, 0, N_BASES - 1, missao_d->local_missao);
}

void add_heroi_base(struct base *base, int heroi_id)
{
    if (base->presente->card < base->lotacao)
        insere_cjt(base->presente, heroi_id);
}

int ev_viaja(struct mundo *mundo, struct evento_t *viaja, struct lef_t *l)
{
    struct evento_t *chega;
    struct base atual, destino;
    struct heroi heroi;
    int heroi_id, distancia, duracao, velocidade;

    heroi_id = viaja->dado2;
    heroi = mundo->herois[heroi_id];
    destino = mundo->bases[viaja->dado1];
    atual = mundo->bases[heroi.id_base];
    velocidade = heroi.velocidade;

    distancia = calcula_dist(atual.local_base, destino.local_base);
    duracao = distancia / velocidade;

    if (!(chega = cria_evento(viaja->tempo + duracao, E_CHEGA, destino.id, heroi_id)))
        return 0;

    insere_lef(l, chega);
    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d \n",
           viaja->tempo, heroi_id, atual.id, viaja->dado1,
           distancia, velocidade, (viaja->tempo + duracao));

    return 1;
}

int ev_chega(struct mundo *mundo, struct evento_t *chega, struct lef_t *l)
{
    struct heroi heroi;
    struct base base;
    int base_id, heroi_id;
    int espera;

    base_id = chega->dado1;
    heroi_id = chega->dado2;

    heroi = mundo->herois[heroi_id];
    base = mundo->bases[base_id];
    heroi.id_base = base_id;

    if (base.espera->tamanho == 0 && base.presente->card < base.lotacao)
        espera = 1;
    else
        espera = heroi.paciencia > (base.espera->tamanho * 10);

    if (espera)
    {
        insere_lef(l, cria_evento(chega->tempo, E_ESPERA, base_id, heroi_id));
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", chega->tempo,
               heroi_id, base_id, base.presente->card, base.lotacao);
    }
    else
    {
        insere_lef(l, cria_evento(chega->tempo, E_DESISTE, base_id, heroi_id));
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", chega->tempo,
               heroi_id, base_id, base.presente->card, base.lotacao);
    }

    return 1;
}

int ev_espera(struct mundo *mundo, struct evento_t *espera, struct lef_t *l)
{
    struct base base;
    int base_id, heroi_id;

    base_id = espera->dado1;
    heroi_id = espera->dado2;

    base = mundo->bases[base_id];

    insere_lef(l, cria_evento(espera->tempo, E_AVISA, base_id, heroi_id));
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", espera->tempo, heroi_id, base_id,
           base.espera->tamanho);
    enqueue(base.espera, heroi_id);

    return 1;
}

int ev_desiste(struct mundo *mundo, struct evento_t *desiste, struct lef_t *l)
{
    int base_id, heroi_id;
    int destino_id;

    base_id = desiste->dado1;
    heroi_id = desiste->dado2;
    destino_id = aleato(0, mundo->n_bases - 1);

    insere_lef(l, cria_evento(desiste->tempo, E_VIAJA, destino_id, heroi_id));
    printf("%6d: DESIST HEROI %2d BASE %d\n", desiste->tempo, heroi_id, base_id);

    return 1;
}

int ev_entra(struct mundo *mundo, struct evento_t *entra, struct lef_t *l)
{
    int tpb;
    int heroi_id;

    heroi_id = entra->dado2;

    tpb = 15 + mundo->herois[heroi_id].paciencia * aleato(1, 20);

    insere_lef(l, cria_evento(entra->tempo + tpb, E_SAI, entra->dado1, heroi_id));

    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d \n", entra->tempo,
           heroi_id, entra->dado1, mundo->bases[entra->dado1].presente->card,
           mundo->bases[entra->dado1].lotacao, entra->tempo + tpb);

    return 1;
}

int ev_avisa(struct mundo *mundo, struct evento_t *avisa, struct lef_t *l)
{
    struct base base = mundo->bases[avisa->dado1];

    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) ", avisa->tempo, base.id,
           base.presente->card, base.lotacao);
    fila_imprime(base.espera);

    while (base.presente->card < base.lotacao && base.espera->tamanho > 0)
    {
        int heroi_id;
        dequeue(base.espera, &heroi_id);
        add_heroi_base(&base, heroi_id);
        insere_lef(l, cria_evento(avisa->tempo, E_ENTRA, base.id, heroi_id));
        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", avisa->tempo,
               base.id, heroi_id);
    }

    return 1;
}

int ev_sai(struct mundo *mundo, struct evento_t *sai, struct lef_t *l)
{
    struct base base;
    struct conjunto *herois_base;
    int heroi_id, base_id, destino;

    base_id = mundo->bases[sai->dado1].id;
    heroi_id = mundo->herois[sai->dado2].id;
    herois_base = mundo->bases[base_id].presente;

    base = mundo->bases[base_id];
    herois_base = base.presente;

    retira_cjt(herois_base, heroi_id);

    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", sai->tempo, heroi_id, base_id,
           base.presente->card,
           base.lotacao);

    destino = aleato(0, N_BASES - 1);

    insere_lef(l, cria_evento(sai->tempo, E_VIAJA, destino, heroi_id));
    insere_lef(l, cria_evento(sai->tempo, E_AVISA, base_id, 0));

    return 1;
}

int ev_missao(struct mundo *mundo, struct evento_t *missao, struct lef_t *l)
{
    struct missao *missao_atual;
    struct conjunto *habs_base_ord[N_BASES];
    int bases_dist_ord[N_BASES];
    int base_cumprida;

    missao_atual = &mundo->missoes[missao->dado1];
    missao_atual->tentativas++;

    /* impressao das informacoes da missao como : ID e habilidades requeridas */
    printf("%6d: MISSAO %d HAB REQ: ", missao->tempo, missao->dado1);
    imprime_cjt(missao_atual->habs_necessarias);

    /* preenchendo o vetor com os id's das bases de acordo com a distancia
     * e em seguida imprimindo as habilidades presentes na base */
    ord_distancia_base(bases_dist_ord, mundo, missao_atual);
    for (int i = 0; i < N_BASES; i++)
    {
        habs_base_ord[i] = habs_base(&mundo->bases[bases_dist_ord[i]], mundo);
        printf("%6d: MISSAO %d HAB BASE %d: ", missao->tempo, missao->dado1,
               mundo->bases[bases_dist_ord[i]].id);
        imprime_cjt(habs_base_ord[i]);
    }

    /* verificando se o conjunto de habilidades necessarias para realizar a missao
     * estah contido no conjunto de habilidades presentes na base*/
    base_cumprida = -1;
    for (int i = 0; i < N_BASES; i++)
    {
        if (contido_cjt(missao_atual->habs_necessarias, habs_base_ord[i]))
        {
            printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS: ", missao->tempo, missao->dado1,
                   mundo->bases[bases_dist_ord[i]].id);
            imprime_cjt(mundo->bases[bases_dist_ord[i]].presente);
            incrementa_exp(&mundo->bases[bases_dist_ord[i]], mundo);
            missao_atual->conclusao = 1;
            base_cumprida = i;
            break;
        }
    }

    for (int i = 0; i < N_BASES; i++)
        destroi_cjt(habs_base_ord[i]);

    if (base_cumprida == -1)
    {
        insere_lef(l, cria_evento(missao->tempo + (24 * 60), E_MISSAO, missao->dado1, 0));
        printf("%6d: MISSAO %d IMPOSSIVEL\n", missao->tempo, missao->dado1);
        return 0;
    }

    return 1;
}

void ev_fim(struct mundo *mundo, struct evento_t *fim, struct lef_t *l)
{
    printf("%6d: FIM\n", fim->tempo);
    imprime_herois_mundo(mundo);
    imprime_missoes_mundo(mundo);
    fim = destroi_evento(fim);

    l = destroi_lef(l);

    mundo = destroi_mundo(mundo);
}

void processa_eventos(struct mundo *m, struct lef_t *l)
{
    struct evento_t *evento;

    /* Percorre a lef enquanto o evento for valido OU
     * enquanto o evento->tempo != T_FIM_DO_MUNDO */
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
        case E_MISSAO:
            ev_missao(m, evento, l);
            break;
        }
        case E_FIM:
            ev_fim(m, evento, l);
            return;
            
        evento = destroi_evento(evento);
    }
}

/*---------------------------------------------------------------------------------------------*/

int main()
{
    struct mundo *mundo;
    struct lef_t *linha_do_tempo;

    srand(0);

    mundo = cria_mundo();

    linha_do_tempo = cria_lef();

    inicializa_mundo(mundo, linha_do_tempo);

    processa_eventos(mundo, linha_do_tempo);

    return 0;
}

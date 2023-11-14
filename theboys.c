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
    int conclusao;
    int tentativas;
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

/* ---------------- Funcoes auxiliares ---------------- */

int aleato(int min, int max)
{
    return min + (rand() % (max - min + 1));
}

int calcularDistancia(struct coordenada local_atual, struct coordenada local_destino)
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
        int dist_j = calcularDistancia(m->bases[v[j]]->local_base, local_destino);
        int dist_pivo = calcularDistancia(m->bases[v[pivo]]->local_base, local_destino);

        if (dist_j < dist_pivo)
        {
            i++;
            // Trocar os índices se a base j está mais próxima que a base pivo
            int temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }

    // Trocar o índice pivo com o próximo índice
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

void incrementa_exp(struct base *b, struct mundo *m)
{
    for (int i = 0; i < b->presente->card; i++)
        m->herois[b->presente->v[i]]->experiencia++;
}

void imprime_herois_mundo(struct mundo *m)
{
    if (m)
    {
        for (int i = 0; i < m->n_herois; i++)
        {
            struct heroi *heroi = m->herois[i];
            if (heroi)
            {
                printf("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ", heroi->id,
                       heroi->paciencia, heroi->velocidade, heroi->experiencia);
                imprime_cjt(heroi->habilidades_heroi);
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
    missao->conclusao = 0;
    missao->tentativas = 0;

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
    int tot_concluidas;
    int tot_tentativas;

    tot_concluidas = 0;
    tot_tentativas = 0;
    for (int i = 0; i < m->n_missoes; i++)
    {
        struct missao *missao = m->missoes[i];
        if (missao->conclusao)
            tot_concluidas++;

        tot_tentativas += missao->tentativas;
    }

    printf("%d/%d MISSOES CUMPRIDAS(%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n",
            tot_concluidas, N_MISSOES, ((float)tot_concluidas * 100) / N_MISSOES,
            tot_tentativas / (float)N_MISSOES);

}

struct conjunto *habs_base(struct base *b, struct mundo *mundo)
{
    struct conjunto *habs_totais_base;
    int habs_base_vet[b->presente->card * 3];
    int tam_final;

    if (!(habs_totais_base = cria_cjt(N_HABILIDADES)))
        return NULL;

    tam_final = 0;

    for (int i = 0; i < b->presente->card; i++)
    {
        struct heroi *h = mundo->herois[b->presente->v[i]];
        for (int j = 0; j < h->habilidades_heroi->card; j++)
        {
            habs_base_vet[tam_final] = h->habilidades_heroi->v[j];
            tam_final++;
        }
    }

    for (int i = 0; i < tam_final; i++)
        insere_cjt(habs_totais_base, habs_base_vet[i]);

    return habs_totais_base;
}

/* Verifica se uma equipe estah apta para cumprir uma missao */
int equipe_apta(struct base *base, struct missao *missao, struct mundo *mundo)
{
    struct conjunto *habs_totais_base;

    habs_totais_base = habs_base(base, mundo);

    if (!(sao_iguais_cjt(habs_totais_base, missao->habilidades_necessarias)))
    {
        destroi_cjt(habs_totais_base);
        return 0;
    }

    destroi_cjt(habs_totais_base);
    return 1;
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
        insere_lef(l, cria_evento(tempo, E_MISSAO, i, 0));
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

/* Processa um evento VIAJA */
int ev_viaja(struct mundo *mundo, struct evento_t *viaja, struct lef_t *l)
{
    struct evento_t *chega;
    struct base *atual, *destino;
    int heroi_id;
    int distancia, duracao;

    heroi_id = viaja->dado2;
    destino = mundo->bases[viaja->dado1];
    atual = mundo->bases[mundo->herois[heroi_id]->id_base];

    distancia = calcularDistancia(atual->local_base, destino->local_base);
    duracao = distancia / mundo->herois[heroi_id]->velocidade;

    if (!(chega = cria_evento(viaja->tempo + duracao, E_CHEGA, destino->idBase, heroi_id)))
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

/* Processa um evento MISSAO */
int ev_missao(struct mundo *mundo, struct evento_t *missao, struct lef_t *l)
{
    struct missao *missao_atual;
    int bases_dist[N_BASES];
    int missao_id;
    int missao_cumprida;

    missao_id = missao->dado1;
    missao_atual = mundo->missoes[missao_id];

    for (int i = 0; i < N_BASES; i++)
        bases_dist[i] = i;

    printf("%6d: MISSAO %d HAB REQ: ", missao->tempo, missao_id);
    imprime_cjt(missao_atual->habilidades_necessarias);

    quicksort(bases_dist, mundo, 0, N_BASES - 1, missao_atual->local_missao);

    for (int i = 0; i < N_BASES; i++)
    {
        struct conjunto *habs_equipe_base;

        habs_equipe_base = habs_base(mundo->bases[bases_dist[i]], mundo);

        printf("%6d: MISSAO %d HAB BASE %d: ", missao->tempo, missao_id,
               mundo->bases[bases_dist[i]]->idBase);
        imprime_cjt(habs_equipe_base);
        destroi_cjt(habs_equipe_base);
    }

    for (int i = 0; i < N_BASES; i++)
    {
        if (equipe_apta(mundo->bases[bases_dist[i]], missao_atual, mundo))
        {
            printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS: ", missao->tempo, missao_id,
                   mundo->bases[bases_dist[i]]->idBase);
            incrementa_exp(mundo->bases[bases_dist[i]], mundo);
            missao_atual->conclusao = 1;
            return missao_cumprida = 1;
        }
        else
            missao_cumprida = 0;
    }

    if (!missao_cumprida)
    {
        insere_lef(l, cria_evento(missao->tempo + (24 * 60), E_MISSAO, missao_id, 0));
        missao_atual->tentativas++;
        printf("%6d: MISSAO %d IMPOSSIVEL\n", missao->tempo, missao_id);
    }

    return missao_cumprida;
}

/* Processa o evento FIM */
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
        case E_MISSAO:
            ev_missao(m, evento, l);
            break;
        }

        if (evento->tipo == E_FIM)
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

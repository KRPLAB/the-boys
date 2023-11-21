    <h1>The Boys</h1>

    <h2>Resumo</h2>
    <p>Trabalho avaliativo realizado para a disciplina de graduação Programação I (CI1001) do curso Bacharelado em
        Ciência da Computação da UFPR</p>

    <h2>O Problema</h2>
    <p>Este projeto consiste em simular um mundo com heróis, bases e missões. Cada herói possui um conjunto de
        habilidades (vôo, invisibilidade, visão de raios X, superaudição, etc.), um nível de experiência e outros
        atributos.</p>

    <p>As bases estão espalhadas pelo mundo e os heróis se deslocam periodicamente entre elas. Cada base possui uma
        lotação máxima e uma fila de espera; ao chegar na base, o herói pode decidir entre esperar na fila ou desistir,
        viajando para outra base. Cada base possui um porteiro que gerencia a entrada/saída dos heróis e a fila de
        espera.</p>

    <p>Esporadicamente surgem missões em locais aleatórios desse mundo; cada missão exige um conjunto de habilidades
        específicas. A missão pode ser atendida por uma equipe de heróis que se encontre em uma base, desde que os
        heróis da equipe tenham, em conjunto, as habilidades requeridas. Ao ser escolhido para participar de uma missão,
        cada herói recebe pontos de experiência.</p>

    <p>Este projeto consiste em simular a dinâmica desse mundo durante um ano (525.600 minutos). Ao final, a simulação
        deve apresentar o número de missões cumpridas (pontos ganhos) por cada herói e outras estatísticas.</p>

    <h2>Simulação a eventos discretos</h2>
    <p>Simular é construir e executar um modelo computacional que imita de forma aproximada a realidade e sua evolução
        ao longo do tempo. A simulação é uma ferramenta muito usada no estudo de fenômenos naturais, como técnica de
        ensino (oferecendo para os alunos um ambiente onde é possível experimentar e errar sem as consequências do
        ambiente real) e também em muitos jogos eletrônicos. A simulação de eventos discretos (SED) é uma forma de
        simulação que modela a dinâmica de um sistema como uma sequência de eventos ocorrendo ao longo do tempo.</p>

    <ul>O modelo de simulação possui uma estrutura estática e uma estrutura dinâmica:
        <li>
            <p>Estrutura estática: descreve as entidades do sistema com seus atributos; o valor desses atributos em um
                dado momento define o estado do sistema.</p>
        </li>

        <li>
            <p>Estrutura dinâmica: descreve como o estado do sistema evolui; geralmente é definida através de uma
                sequência de eventos agendados ao longo do tempo, que alteram os atributos das entidade do sistema e
                podem criar novos eventos.</p>
            <p>Um simulador é um programa que executa os eventos e evolui o modelo de simulação no tempo. Para isso, ele
                mantém um relógio global, representando o tempo atual no modelo, e uma Lista de Eventos Futuros (LEF),
                que é basicamente uma lista de eventos ordenada por datas de ocorrência crescentes, como em uma agenda:
            </p>
            <pre>10:00 ir ao dentista
     : (aqui está no dentista)
    11:45 voltar do dentista
    12:00 almoçar
     : (aqui está almoçando)
    13:00 ir à UFPR
    13:30 ir à aula de Cálculo
     : (aqui está na aula)
    15:20 sair da aula de Cálculo
    15:30 ir à aula de Programação
     : (aqui está na aula)
    17:30 sair da aula de Programação
    18:00 voltar para casa
       : (aqui está voltando para casa)
    ...</pre>

            <p>
                O relógio do simulador geralmente é um número inteiro que representa o tempo no sistema simulado e não
                tem relação direta com o tempo físico. Dependendo do modelo, cada unidade de tempo simulado pode
                representar microssegundos (em uma simulação de circuitos lógicos), minutos (simulação de tráfego) ou
                mesmo milhares de anos (simulação geológica).
            </p>
    </ul>

    <p>
        Executar a simulação consiste basicamente em processar os eventos em sequência, atualizando o estado do sistema
        a cada evento. Entre dois eventos consecutivos o estado do sistema não se altera, por isso o relógio do
        simulador pode saltar diretamente de um evento ao próximo. Dessa forma, o ciclo básico de funcionamento do
        simulador é bem simples:
    </p>

    <ol>
        <li>
            <div> retirar o primeiro evento da lista de eventos</div>
        </li>
        <li>
            <div> atualizar o relógio</div>
        </li>
        <li>
            <div> tratar o evento:</div>
            <ol>
                <li>
                    <div> atualizar o estado do sistema</div>
                </li>
                <li>
                    <div> agendar novos eventos porventura criados</div>
                </li>
            </ol>
        </li>
        <li>
            <div> repetir até concluir a simulação</div>
        </li>
    </ol>

    <p>
        Isso pode ser traduzido no seguinte pseudocódigo:
    </p>

    <pre>iniciar as entidades e atributos do mundo
    relógio = 0
    repetir
        ev = 1º evento da lista de eventos futuros
        relógio = tempo (ev)
        caso tipo (ev) seja:
            EV1: executa evento 1
            EV2: executa evento 2
            EV3: executa evento 3
            ...
        fim
    até o fim da simulação
    apresentar resultados
</pre>

    <h2>Implementação do Projeto</h2>
    
    <p>
        A implementação do projeto consiste na criação da biblioteca theboys.h e na implementação dessa biblioteca no arquivo theboys.c. O projeto foi implementado utilizando as TAD's (Tipo Abstrato de Dados) Conjunto, Fila e LEF (Lista de Eventos Futuros)
    </p>

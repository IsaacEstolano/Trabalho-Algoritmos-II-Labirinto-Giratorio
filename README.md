# Labirinto Giratório

Jogo de labirinto em C++ para terminal Linux, desenvolvido para Algoritmos e Programação II. O jogador percorre cenários em forma de matriz, gira o mapa quando está sobre uma alavanca e usa as mudanças de orientação, portas e gravidade para chegar à saída.

O projeto começou na M1 e evoluiu na M2 para carregar mapas e soluções de arquivos, salvar partidas entre execuções e registrar vitórias em um ranking.

## Ambiente

- **Versão:** Linux, terminal POSIX (`termios.h` e `unistd.h`).
- **Compilador:** `g++`.
- **Dimensões aceitas:** de `11 × 11` a `21 × 21`.
- **Diretório de execução:** raiz do projeto, onde estão `main-Linux.cpp` e `makefile`.

## Como compilar e executar

Na raiz do projeto:

```bash
make
./labirinto
```

Também é possível executar `make run`. O código depende de recursos do terminal Linux e não foi preparado para Windows.

## Regras e controles

O objetivo é alcançar a saída `S`. O jogador pode mover-se pelo mapa; quando está sobre uma alavanca `^`, pode girar todo o cenário em 90 graus. Após a rotação, os blocos soltos sofrem gravidade. Um movimento válido tem uma pausa de um segundo.

| Tecla | Ação |
|:---:|---|
| `W` | Mover para cima |
| `A` | Mover para a esquerda |
| `S` | Mover para baixo |
| `D` | Mover para a direita |
| `Q` | Girar para a esquerda |
| `E` | Girar para a direita |
| `R` | Reiniciar o mapa |
| `ESC` | Salvar a partida e voltar ao menu |
| `K` | Voltar ao menu; o jogo pergunta se deve salvar |

Durante a partida são exibidos os movimentos, as rotações, a orientação e o número do mapa.

## Elementos dos mapas

Os mapas são matrizes quadradas de inteiros. Cada arquivo informa a dimensão e o título antes das linhas da matriz.

| Código | Símbolo | Significado |
|---:|:---:|---|
| `0` | espaço | Espaço vazio |
| `1` | `&` | Parede fixa |
| `2` | `@` | Jogador |
| `3` | `O` | Bloco sujeito à gravidade |
| `4` | `^` | Alavanca |
| `5` | `S` | Saída |
| `6` | `=` ou `:` | Porta fechada ou aberta |
| `8` | `?` | Jogador sobre uma porta |
| `9` | `+` | Jogador sobre uma alavanca |
| `10` | `$` | Jogador na saída |

A porta `=` está fechada e bloqueia o jogador. Com número ímpar de rotações, ela é atravessável e aparece como `:`. A posição do jogador sobre a porta, a alavanca ou a saída é guardada em um único valor da matriz; os códigos `8`, `9` e `10` representam essas combinações.

## Mapas e arquivos de solução

O projeto contém cinco mapas e uma solução para cada um:

| Mapa | Dimensão | Arquivos |
|---:|:---:|---|
| 1 | `14 × 14` | `Mapas/map1.csv`, `Soluções/map1.sol` |
| 2 | `14 × 14` | `Mapas/map2.csv`, `Soluções/map2.sol` |
| 3 | `14 × 14` | `Mapas/map3.csv`, `Soluções/map3.sol` |
| 4 | `17 × 17` | `Mapas/map4.csv`, `Soluções/map4.sol` |
| 5 | `19 × 19` | `Mapas/map5.csv`, `Soluções/map5.sol` |

Os mapas são procurados em ordem numérica. A listagem termina quando encontra o primeiro número sem arquivo. Para acrescentar um mapa, mantenha a sequência sem lacunas e inclua o respectivo `.sol`.

Formato de `Mapas/mapN.csv`:

1. dimensão `n`;
2. título do mapa;
3. `n` linhas com `n` códigos, separados por vírgulas.

O carregamento verifica dimensão, título, quantidade de linhas e colunas, valores, jogador, saída e estabilidade inicial dos blocos. Mapas inválidos são recusados com o motivo.

Cada `Soluções/mapN.sol` contém a sequência de teclas `W`, `A`, `S`, `D`, `Q` e `E` que resolve o mapa. A opção **Ver soluções** reproduz uma solução com o mesmo tratamento de teclas usado durante o jogo. Informar `0` permite verificar todas as soluções sem desenhar os mapas.

## Menu

| Opção | Ação |
|---:|---|
| `1` | Sobre: equipe, disciplina, professor, regras e controles |
| `2` | Novo jogo: escolher um mapa válido ou `0` para sortear um |
| `3` | Continuar: carregar uma partida salva válida |
| `4` | Ver solução de um mapa ou verificar todas com `0` |
| `5` | Ver os cinco melhores recordes de um mapa |
| `6` | Fim |

Se há uma partida salva e o jogador escolhe **Novo jogo**, o programa pede confirmação antes de substituí-la. A opção **Continuar** só fica disponível quando o arquivo salvo pode ser carregado e validado. Ao vencer, a partida salva é removida.

## Formato do arquivo de jogo salvo

O arquivo `jogo_salvo.csv` é criado na raiz do projeto quando a partida é salva. Seu formato é:

```text
numeroMapa,dimensao,orientacao,rotacoes,movimentos,linhaJogador,colunaJogador
titulo do mapa
dimensao linhas da matriz atual, com valores separados por vírgulas
MAPA_ORIGINAL
dimensao linhas da matriz inicial, com valores separados por vírgulas
```

As linhas e colunas do jogador começam em zero. A matriz atual preserva portas, blocos e combinações do jogador com outros elementos no momento do salvamento. A matriz após `MAPA_ORIGINAL` permite reiniciar a fase depois de carregá-la. Ao abrir o jogo, os campos e as duas matrizes são validados antes de continuar.

## Recordes

Ao vencer, o jogo pede um nome de até 15 caracteres e grava uma linha no formato:

```text
nome,mapa,movimentos,rotacoes
```

A opção **Recordes** filtra um mapa, carrega até 200 registros válidos e mostra até cinco resultados: primeiro pelo menor número de movimentos e, em caso de empate, pelo menor número de rotações. A troca durante a ordenação usa a função template `trocar`.

O código atual usa o nome `Recordes.csv`, com `R` maiúsculo. O enunciado da M2 escreve `recordes.csv` em minúsculas; como Linux diferencia maiúsculas de minúsculas, mantenha o nome compatível com o código ao preparar esse arquivo.

## Evolução da M1 para a M2

Na M1, o estado era passado como vários parâmetros e o tamanho dos mapas era fixo em 14. Na M2, o estado da partida foi agrupado em `Game`, e as funções principais recebem essa estrutura por referência.

| Função da M1 | Assinatura na M1 | Assinatura na M2 | Motivo da mudança |
|---|---|---|---|
| `defineCoordinates` | `void defineCoordinates(int&, int&, int)` | `void defineCoordinates(Game&)` | A posição é localizada na matriz carregada, em vez de manter coordenadas fixas por número de mapa. |
| `gravity` | `void gravity(int[][14], int, bool&, bool&)` | `void gravity(Game&)` | Matriz, contadores e estado da partida passam pelo objeto `Game`. |
| `mapRotationNormal` | `void mapRotationNormal(int[][14], int&, int&, int&)` | `void mapRotationNormal(Game&)` | A rotação atualiza o cenário, a posição e o contador juntos. |
| `mapRotationAnti` | `void mapRotationAnti(int[][14], int&, int&, int&)` | `void mapRotationAnti(Game&)` | Mesma mudança da rotação horária; reduz parâmetros soltos. |
| `mapGenerate` | `void mapGenerate(int[][14], int, int, int, int)` | `void mapGenerate(Game&)` | O desenho recebe o estado completo, incluindo dimensão e mapa atual. |
| `win` | `void win(bool&, bool&)` | `void win(Game&)` | O resultado da partida é atualizado no estado único do jogo. |

## Defeitos e limitações da M1 tratados

Comparando o código versionado da M1 com esta versão, foram corrigidos ou tratados:

- mapas e coordenadas iniciais limitados aos três cenários definidos no código;
- dimensão fixa de `14 × 14`, substituída por mapas lidos com dimensão variável entre 11 e 21;
- acesso a células vizinhas sem conferir os limites da matriz antes de mover;
- leitura de arquivos sem validação suficiente de dimensão, conteúdo, linhas, colunas, jogador, saída e estabilidade;
- ausência de reprodução automática das sequências de solução;
- opção **Continuar** que dependia apenas da memória do processo, sem persistência após fechar o programa;
- ausência de registro e consulta de recordes;
- derrota por esmagamento que, no código M1, alterava a variável de vitória e exibia uma mensagem de parabéns;
- leitura de tecla feita com `getchar()` misturada às leituras de `cin`; a versão atual usa `cin.get()` e verifica o modo do terminal antes de alterá-lo.

## Decisões de projeto

- `Position`, `Map` e `Game` agrupam, respectivamente, coordenadas, dados do mapa e estado da partida. `OperationResult` transporta sucesso e motivo de falhas de leitura ou gravação.
- A matriz tem capacidade máxima de `21 × 21`; cada mapa usa somente as primeiras `n × n` posições indicadas no arquivo.
- As regras de movimento ficam em funções que recebem `Game`; a reprodução de `.sol` chama o mesmo tratamento de tecla do jogo para evitar duplicar as regras.
- Ações que mudam a partida recebem `Game&`. A validação da estabilidade trabalha sobre uma cópia, e `readMap` só substitui o mapa atual depois de validar uma cópia candidata.
- O jogo salvo contém a matriz atual e a matriz original: a primeira retoma a partida, e a segunda permite reiniciar o mapa.
- O ranking desempata por rotações depois de comparar os movimentos, conforme o critério do enunciado.
- Os arquivos de mapas e soluções ficam nas pastas `Mapas/` e `Soluções/`; salvamento e recordes ficam na raiz. Os caminhos são relativos ao diretório de execução, então execute o programa a partir da raiz do projeto.

## Desafios extras

Não foram implementados desafios opcionais como dica automática, editor de mapas ou gravação das teclas de uma partida vencida.

## Estrutura do projeto

```text
.
├── main-Linux.cpp
├── makefile
├── labirinto                 # executável gerado por make
├── Mapas/
│   ├── map1.csv ... map5.csv
├── Soluções/
│   ├── map1.sol ... map5.sol
├── jogo_salvo.csv            # criado ao salvar uma partida
└── Recordes.csv              # criado ao registrar a primeira vitória
```

## Referências

- [Enunciado da M2](PDF/Labirinto_Giratorio_M2.pdf).
- [Enunciado da M1](PDF/T1_Labirinto_Giratorio.pdf).
- [One Card Maze](https://www.onecardmaze.com/), inspiração para o conceito de rotação do labirinto.

## Autoria

**Isaac Furtado Estolano da Silveira**\
Professor: **Thiago Felski**\
Disciplina: **Algoritmos e Programação II**\
Versão: **Linux — outubro de 2026**

Projeto acadêmico.

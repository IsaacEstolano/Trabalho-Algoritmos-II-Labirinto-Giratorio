# Labirinto Giratório

Projeto acadêmico desenvolvido em **C++** para a disciplina de **Algoritmos e Programação II**, inspirado no conceito de rotação de labirintos do jogo **One Card Maze**.

O projeto transforma essa ideia em um labirinto 2D jogável no terminal, no qual o cenário inteiro pode ser rotacionado em 90° e os blocos respondem à gravidade após cada rotação.

> **Nota:** este é um projeto acadêmico inspirado no conceito de One Card Maze, não uma cópia do jogo original.
> Site oficial: https://www.onecardmaze.com/

## 🎮 Sobre o jogo

O objetivo é atravessar o labirinto e alcançar a **saída (`S`)**.

O diferencial é que, quando o jogador está sobre uma **alavanca (`^`)**, ele pode girar todo o cenário. A rotação altera a posição dos elementos, a orientação e o comportamento das portas. Em seguida, os blocos soltos sofrem ação da gravidade.

A proposta segue a ideia central do trabalho da M1, em que a gravidade permanece apontando para baixo da tela enquanto o cenário é rotacionado.

## 🧩 Elementos do mapa

O cenário é representado por uma matriz quadrada de inteiros.

| Código | Símbolo | Elemento |
|---:|:---:|---|
| `0` | espaço | Espaço vazio |
| `1` | `&` | Parede fixa |
| `2` | `@` | Jogador |
| `3` | `O` | Bloco solto |
| `4` | `^` | Alavanca |
| `5` | `S` | Saída |
| `6` | `=` / `:` | Porta |
| `8` | `?` | Jogador em porta aberta |
| `9` | `+` | Jogador sobre a alavanca |

## 🔄 Rotação do cenário

O cenário começa na orientação:

```text
0°
```

Cada rotação altera a orientação em 90°.

```text
Q → gira para a esquerda
E → gira para a direita
```

A rotação só pode ser executada quando o jogador estiver sobre uma alavanca.

A rotação transforma o cenário inteiro, incluindo paredes, blocos, alavancas, portas, saída e jogador.

## 🧱 Gravidade

Depois de uma rotação, os blocos `O` são processados pela gravidade.

Enquanto houver espaço livre abaixo, o bloco continua descendo. Ele para quando encontra:

- uma parede;
- outro bloco;
- uma porta fechada;
- a borda do mapa.

Essa mecânica faz com que cada rotação possa modificar completamente a configuração do labirinto.

## 🚪 Portas

A porta continua existindo na matriz, mas sua interpretação muda conforme a orientação do cenário.

No projeto:

```text
=  Porta fechada
:  Porta aberta
```

Uma porta fechada bloqueia a passagem e pode sustentar blocos. Uma porta aberta pode ser atravessada e não sustenta blocos.

## 🗺️ Mapas

O projeto possui **3 mapas**, todos utilizando matrizes `14 x 14`.

Os mapas foram construídos para explorar diferentes situações envolvendo:

- movimentação;
- alavancas;
- rotação;
- portas;
- queda de blocos;
- mudança de orientação.

O trabalho exige que, entre os mapas, existam soluções envolvendo queda de bloco, mudança de estado de porta e pelo menos uma solução com duas ou mais rotações.

## 🎮 Controles

| Tecla | Ação |
|:---:|---|
| `W` | Mover para cima |
| `A` | Mover para a esquerda |
| `S` | Mover para baixo |
| `D` | Mover para a direita |
| `Q` | Girar o cenário para a esquerda |
| `E` | Girar o cenário para a direita |
| `R` | Reiniciar o mapa |
| `K` | Voltar ao menu |

## 📊 Informações durante a partida

Durante o jogo são exibidos:

```text
Movimentos: X
Rotações: Y
Orientação: Z
Mapa: N
```

## 🖥️ Menu

O jogo possui as opções:

```text
1 - Sobre
2 - Novo jogo
3 - Continuar
4 - Fim
```

### Sobre
Exibe as regras, controles e informações do projeto.

### Novo jogo
Permite selecionar um dos três mapas.

### Continuar
Retorna para uma partida que ainda está em andamento, preservando o estado do cenário.

### Fim
Encerra o programa.

## 🧠 Conceitos de programação utilizados

O projeto utiliza principalmente:

- matrizes bidimensionais;
- funções;
- passagem de parâmetros;
- passagem por referência;
- ponteiros para matrizes;
- estruturas de repetição;
- estruturas condicionais;
- manipulação de caracteres;
- rotação de matrizes;
- lógica de movimentação;
- simulação de gravidade;
- controle de estados do jogo.

A atividade destaca especialmente o uso de **sub-rotinas e passagem de parâmetros**.

## 📁 Estrutura

```text
.
├── Pasted code(4).cpp
└── README.md
```

Principais funções:

```cpp
defineCoordinates()
reset()
selection()
win()
smashed()
mapRotationNormal()
mapRotationAnti()
gravity()
mapGenerate()
movi()
getch()
```

## ⚙️ Compilação

O projeto utiliza recursos do terminal Linux através de:

```cpp
#include <termios.h>
#include <unistd.h>
```

Para compilar:

```bash
g++ "Pasted code(4).cpp" -o labirinto
```

Para executar:

```bash
./labirinto
```

A implementação foi desenvolvida para ambientes Unix/Linux.

## 📚 Referências

### Trabalho acadêmico

**Universidade do Vale do Itajaí (UNIVALI)**  
Escola Politécnica  
Curso de Ciência da Computação  
Disciplina: **Algoritmos e Programação II**

**M1 – Labirinto Giratório**

O enunciado define um labirinto 2D cujo cenário pode ser girado 90° para a esquerda ou para a direita, com blocos sujeitos à gravidade, portas dependentes da orientação e rotação condicionada à presença do jogador sobre uma alavanca.

### Inspiração

**One Card Maze**

O One Card Maze é um jogo de labirinto que utiliza rotações e transformações do cartão para alterar a configuração dos caminhos e permitir a resolução dos labirintos.

Site oficial:

https://www.onecardmaze.com/

## 👨‍💻 Autor

**Isaac Furtado Estolano da Silveira**

Projeto desenvolvido para fins acadêmicos.

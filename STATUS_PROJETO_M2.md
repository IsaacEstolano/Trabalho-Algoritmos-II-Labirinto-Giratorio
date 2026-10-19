# Estado do projeto — Labirinto Giratório M2

**Revisado em:** 19/10/2026  
**Referência dos requisitos:** `PDF/Labirinto_Giratorio_M2.pdf`

Este documento registra o que existe hoje no código e o que ainda precisa ser feito para atender à M2. A presença de uma função indica que há uma implementação no código; não significa que todos os casos foram testados ou que o requisito esteja completo.

## Resumo

O projeto já tem a base jogável da M1 e começou a migração do estado para structs e a leitura de mapas em CSV. Ainda faltam várias funcionalidades centrais da M2: validação segura dos mapas, dimensão variável, reprodução de soluções, salvamento persistente e ranking.

## O que já existe

- **Estruturas para organizar o estado:** `Position`, `Map`, `Game` e `Menu` estão em `main-Linux.cpp`. A partida é passada para as funções principais por referência como `Game &game`; a opção do menu fica em `Menu`.
- **Regras básicas do jogo:** há funções para movimentar o jogador, girar o cenário, aplicar gravidade, reiniciar a fase e detectar vitória ou esmagamento.
- **Desenho no terminal:** `mapGenerate` desenha o cenário e mostra movimentos, rotações, orientação e número do mapa.
- **Menu básico:** há opções para Sobre, Novo jogo, Continuar durante a execução atual e Fim.
- **Mapas em CSV:** existem três arquivos, `Mapas/map1.csv`, `Mapas/map2.csv` e `Mapas/map3.csv`, todos com dimensão 14 × 14.
- **Compilação:** o `makefile` compila `main-Linux.cpp` para gerar `labirinto`.

## O que está parcialmente implementado

### Leitura de mapas (`readMap`)

`readMap` abre `Mapas/mapN.csv` e faz a leitura básica da dimensão, do título e dos valores das linhas. Para os arquivos atuais, esse é o caminho esperado quando o programa é executado a partir da raiz do projeto.

Ainda falta:

- devolver sucesso ou falha para quem chamou a função; hoje, se o arquivo não abrir, ela apenas imprime uma mensagem e retorna;
- tratar dimensão ou valores não numéricos sem deixar `stoi` encerrar o programa por exceção;
- verificar dimensão entre 11 e 21, título presente, quantidade de linhas e colunas, valores entre 0 e 7, exatamente um jogador e pelo menos uma saída;
- verificar se o mapa inicial está estável;
- impedir que uma falha de leitura deixe o jogo começar com um mapa vazio ou parcialmente carregado;
- separar acesso a arquivo de mensagens na tela, como recomenda o enunciado.

O enunciado também determina que os arquivos fiquem na pasta de execução, sem subpastas. Hoje eles ficam em `Mapas/` e a função usa esse caminho explicitamente.

### Tamanho do mapa

O CSV informa `mapOrder`, mas o restante do jogo continua usando `order`, `line` e `collum` fixos em 14. A rotação, o desenho e a gravidade também percorrem esse tamanho fixo. Portanto, a leitura de uma dimensão diferente de 14 não torna o mapa jogável nesse tamanho.

### Seleção e continuidade

A seleção ainda oferece apenas os mapas 1, 2 e 3. As coordenadas iniciais do jogador também estão definidas manualmente para esses três mapas. A opção Continuar mantém o estado apenas enquanto o processo está aberto; não grava nem recupera uma partida após fechar o programa.

Atualmente, o número escolhido é guardado em `game.mapReset.numMap` e copiado para `game.currentMap.numMap`. Não existe um campo separado chamado `mapSelection`.

### Separação das responsabilidades

Há funções distintas para desenho e regras, mas algumas ainda misturam responsabilidades. Por exemplo, `readMap` imprime erro, e `mapGenerate` chama `win`, que altera o estado da partida enquanto desenha. O enunciado pede separar regras, tela e acesso a arquivos.

## O que falta para a M2

| Área | Situação atual | Trabalho pendente |
|---|---|---|
| Mapas | 3 mapas CSV de 14 × 14 | Ter pelo menos 5 mapas, incluindo 2 novos autorais, um de 15 × 15 ou maior e um que exija pelo menos 3 rotações |
| Descoberta de mapas | Seleção fixa entre 1, 2 e 3 | Procurar `mapa1.csv`, `mapa2.csv` e assim por diante, listar mapas válidos e indisponíveis e oferecer mapa aleatório |
| Validação | Leitura básica | Validar conteúdo, estabilidade e erros sem travar o programa |
| Dimensão variável | Constantes fixas em 14 | Usar a dimensão carregada, entre 11 e 21, no desenho, movimento, rotação e gravidade |
| Soluções | Não há arquivos `.sol` | Criar uma solução para cada mapa, executar todas com a mesma função de tratamento de teclas usada pelo jogador e informar os resultados |
| Salvamento | Continuar só em memória | Gravar e carregar `jogo_salvo.csv`, validar o arquivo, confirmar substituição e invalidar o salvamento após vitória |
| Recordes | Não implementado | Gravar vitórias em `recordes.csv` e exibir os cinco melhores resultados por mapa, ordenados por movimentos e rotações |
| Menu | Sobre, Novo jogo, Continuar e Fim | Incluir Ver solução e Recordes; atender ao fluxo e às regras do menu da M2 |
| Sobre | Informações básicas e tecla K | Completar os dados pedidos no enunciado e conferir as teclas: o código usa K para voltar ao menu, enquanto o enunciado mantém ESC |
| Robustez | Leitura sem validação completa | Não travar com arquivos ausentes, vazios, inválidos ou incompletos, incluindo os arquivos de salvamento e recordes |
| README | Descreve principalmente a M1 e contém nomes antigos de arquivos | Atualizar versão Linux, formato do salvamento, comparação de funções M1/M2, defeitos corrigidos e decisões de projeto |
| Verificação dos mapas | Não há verificador automático | Executar “Verificar todos os mapas” e comprovar que todas as soluções chegam à saída |

## Ordem sugerida para continuar

1. Fazer `readMap` informar o resultado e o motivo da falha, validar o formato e não iniciar mapas inválidos.
2. Remover o tamanho fixo 14 das operações do jogo e usar a dimensão lida do arquivo.
3. Implementar descoberta e listagem de mapas, depois completar os mapas e suas soluções.
4. Implementar a reprodução/verificação de soluções usando as mesmas regras do jogo.
5. Implementar salvamento persistente e ranking.
6. Atualizar menu, Sobre e README conforme os requisitos da M2.

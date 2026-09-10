#include <iostream>
#include <termios.h>
#include <unistd.h>
#define order 14
#define line 14
#define collum 14
#define hollow 0
#define wall 1
#define player 2
#define box 3
#define lever 4
#define exit 5
#define doorA 6
#define doorB 7
#define playerIn 8
#define playerInLever 9
using namespace std;
void exibirSobre() {
    cout << "==================================================" << endl;
    cout << "                LABIRINTO GIRATORIO               " << endl;
    cout << "==================================================" << endl;
    cout << "Equipe: Isaac Furtado Estolano da Silveira" << endl;
    cout << "Disciplina: Algoritmos e Programacao II -" << endl;
    cout << "OBJETIVO: atravessar o labirinto ate a saida (S)." << endl;
    cout << "O cenario inteiro pode ser girado 90 graus, mas so quando" << endl;
    cout << "voce estiver em cima de uma alavanca (^)." << endl << endl;
    cout << "LEGENDA:" << endl;
    cout << "  @  Jogador" << endl;
    cout << "  &  Parede fixa (solida, nunca cai)" << endl;
    cout << "  O  Bloco solto (cai com a gravidade)" << endl;
    cout << "  ^  Alavanca (habilita girar)" << endl;
    cout << "  S  Saida (vitoria)" << endl;
    cout << "  = / :  Porta tipo A (fechada / aberta)" << endl;
    cout << "  | / ;  Porta tipo B (fechada / aberta)" << endl << endl;
    cout << "CONTROLES:" << endl;
    cout << "  W A S D - mover      Q - girar anti-horario" << endl;
    cout << "  E - girar horario    R - reiniciar a fase" << endl;
    cout << "  K - voltar ao menu (o jogo fica pausado)" << endl << endl;
    cout << "Pressione qualquer tecla para voltar ao menu...";
}

void defineCoordinates(int &pli, int &pco, int mapSelection){
    if (mapSelection == 1) {
        pli = 3;
        pco = 6;
    }
    if (mapSelection == 2) {
        pli = 12;
        pco = 1;
    }
    if (mapSelection == 3) {
        pli = 11;
        pco = 10;
    }
}
void reset(int mapSelection,int mapReset[][order],int map[][order],int &pli,int &pco
){
    if (mapSelection == 1 || mapSelection == 2 || mapSelection == 3) {
        for (int i = 0; i < line; i++) {
            for (int j = 0; j < collum; j++) {
                map[i][j] = mapReset[i][j];
            }
        }
        // Depois de restaurar o mapa, restaur
        // também a posição inicial do jogador.
        defineCoordinates(pli, pco, mapSelection);
    }
}
void selection(int mapSelection,int mapReset[][order],int map[][order],int (*&currentMap)[order],int (*&currentMapReset)[order],int map2[][order],int map2Reset[][order],int mapHard[][order],int mapHardReset[][order]){
    switch(mapSelection){
        case 1:
            currentMap=map;
            currentMapReset=mapReset;
            break;
        case 2:
            currentMap=map2;
            currentMapReset=map2Reset;
            break;
        case 3:
            currentMap=mapHard;
            currentMapReset=mapHardReset;
            break;
    }
}  
void win(bool venceu,bool &fim){
    venceu=true;
            cout << "\n==========================================" << endl;
            cout << "        PARABENS! Voce venceu!           " << endl;
            cout << "==========================================" << endl;
    fim=true;
}
void mapRotationNormal(int map[][collum],int &pli,int &pco,int &contRotates){
   int N = order; // Tamanho da map NxN
for (int i = 0; i < N / 2; i++) {
    for (int j = i; j < N - i - 1; j++) {
        int temp = map[i][j];
        map[i][j] = map[N - 1 - j][i];
        map[N - 1 - j][i] = map[N - 1 - i][N - 1 - j];
        map[N - 1 - i][N - 1 - j] = map[j][N - 1 - i];
        map[j][N - 1 - i] = temp;
    }
}   int newPli=pco;
    int newPco=N-1-pli;

    pli=newPli;
    pco=newPco;
    contRotates ++;
    }
//nova linha = coluna antiga
//nova coluna = N - linha antiga
void mapRotationAnti(int map[][collum],int &pli,int &pco,int &contRotates){
    int N=order;
    for(int i=0;i<N/2;i++){
        for(int j=0;j<(N+1)/2;j++){
            int temp = map[i][j];
                map[i][j] = map[j][N - 1 - i];
                map[j][N - 1 - i] = map[N - 1 - i][N - 1 - j];
                map[N - 1 - i][N - 1 - j] = map[N - 1 - j][i];
                map[N - 1 - j][i] = temp;
        }
    }
    int newPco=pli;
    int newPli=N-1-pco;

    pli=newPli;
    pco=newPco;
    contRotates++;
}
void gravity(int map[][order], int contRotates)
{
    for(int i = order - 2; i >= 0; i--){
        for(int j = 0; j < order; j++){
             if(map[i][j] != box)
                continue;
            int linha = i;
            while(linha + 1 < order){
                bool podeCair = false;

                if(map[linha + 1][j] == hollow){
                    podeCair = true;
                }
                // Porta A aberta
                if(map[linha + 1][j] == doorA && contRotates % 2 != 0){
                    podeCair = true;
                }
                // Porta B aberta
                if(map[linha + 1][j] == doorB && contRotates % 2 == 0){
                    podeCair = true;
                }

                if(podeCair){
                    map[linha + 1][j] = box;
                    map[linha][j] = hollow;
                    linha++;
                }
                else{
                    break;
                }
            }
        }
    }
}
void mapGenerate(int map[][order], int contRotates,int contMovi) {
     cout<<"Movimentos:"<<contMovi <<endl;
    cout<<"Rotações:"<<contRotates << endl;
    for (int i = 0; i < line; i++) {
        for (int j = 0; j < collum; j++) {
            switch (map[i][j]) {
                case hollow:
                    cout << " ";
                    break;
                case wall:
                    cout<< "&";
                    break;
                case player:
                    cout << "@";
                    break;
                case box:
                    cout<< "O";
                    break;
                case lever:
                    cout<< "^";
                    break;
                case exit:
                    cout<< "S";
                    break;
                case playerIn:
                    cout<< "?";
                    break;
                case playerInLever:{}
                    cout<< "+";
                case doorA:
                    if (contRotates % 2 == 0){
                        cout << "=";  // fechada
                    }
                else{
                    cout << ":";  // aberta
                }
                    break;
                case doorB:
                    if (contRotates % 2 == 0){
                        cout << ";";  // aberta
                    }
                    else{
                        cout << "|";  // fechada
                    }
                    break;
                default:
                    cout << " ";
                    break;
            }
        }
        cout << endl;
    }
}
void movi(char x,int map[][order],int mapReset[][order],int &pli,int &pco,int &contRotates,int &contmovi,int mapSelection,bool venceu,bool &voltarMenu,bool fim){
    switch (x) {
            case 'w':
                if (map[pli-1][pco] == wall) {
                    break;
                }
                 if (map[pli-1][pco] == box) {
                    break;
                }
                 if (map[pli-1][pco] == doorA) {
                    break;
                }
                 if (map[pli-1][pco] == doorB) {
                    break;
                }
                if (map[pli-1][pco] == lever) {
                    map[pli-1][pco] = playerIn;
                }
                else {
                    map[pli-1][pco] = player;
                }
                if (map[pli][pco] == playerIn) {
                    map[pli][pco] = lever;
                }
                else {
                    map[pli][pco] = hollow;
                }
                if(map[pli-1][pco] == exit){
                    map[pli-1][pco] = player;
                    map[pli][pco] = hollow;
                    win(venceu,fim);
                }
                pli--;
                contmovi++;
                break;

            case 's':
                if (map[pli+1][pco] == wall) {
                    break;
                }
                 if (map[pli+1][pco] == box) {
                    break;
                }
                if (map[pli+1][pco] == doorA) {
                    break;
                }
                 if (map[pli+1][pco] == doorB) {
                    break;
                }
                if (map[pli+1][pco] == lever) {
                    map[pli+1][pco] = playerIn;
                }
                else {
                    map[pli+1][pco] = player;
                }
                if (map[pli][pco] == playerIn) {
                    map[pli][pco] = lever;
                }
                else {
                    map[pli][pco] = hollow;
                }
                  if(map[pli+1][pco] == exit){
                    map[pli+1][pco] = player;
                    map[pli][pco] = hollow;
                    win(venceu,fim);
                }
                pli++;
                contmovi++;
                break;

            case 'a':
                if (map[pli][pco-1] == wall) {
                    break;
                }
                  if (map[pli][pco-1] == box) {
                    break;
                }
                if (map[pli][pco-1] == doorA) {
                    break;
                }
                 if (map[pli][pco-1] == doorB) {
                    break;
                }
                if (map[pli][pco-1] == lever) {
                    map[pli][pco-1] = playerIn;
                }
                else {
                    map[pli][pco-1] = player;
                }
                if (map[pli][pco] == playerIn) {
                    map[pli][pco] = lever;
                }
                else {
                    map[pli][pco] = hollow;
                }
                  if(map[pli][pco-1] == exit){
                    map[pli][pco-1] = player;
                    map[pli][pco] = hollow;
                    win(venceu,fim);
                }
                pco--;
                contmovi++;
                break;

            case 'd':
                if (map[pli][pco+1] == wall) {
                    break;
                }
                 if (map[pli][pco+1] == box) {
                    break;
                }
                  if (map[pli][pco+1] == doorA) {
                    break;
                }
                 if (map[pli][pco+1] == doorB) {
                    break;
                }
                if (map[pli][pco+1] == lever) {
                    map[pli][pco+1] = playerIn;
                }
                else {
                    map[pli][pco+1] = player;
                }
                if (map[pli][pco] == playerIn) {
                    map[pli][pco] = lever;
                }
                else {
                    map[pli][pco] = hollow;
                }
                if(map[pli][pco+1] == exit){
                    map[pli][pco+1] = player;
                    map[pli][pco] = hollow;
                    win(venceu,fim);
                }
                pco++;
                contmovi++;     
                break;
                case 'q':
                if(map[pli][pco]==playerInLever){
                    mapRotationAnti(map,pli,pco,contRotates);
                    gravity(map,contRotates);
                }
                    break;
                case 'e':
                if(map[pli][pco]==playerInLever){
                    mapRotationNormal(map,pli,pco,contRotates);
                    gravity(map,contRotates);
                }
                    break;
                case 'r':
                     reset(mapSelection,mapReset,map,pli,pco);
                    break;
                case 'k':
                    fim=true;
                    break;
            }
}
int getch(void) {
    int ch;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int main() {
    printf("\033[?25l");

    int pli, pco;
    int options = 0;
    int mapSelection = 0;
    char x;
    bool venceu = false;
    bool voltarMenu= false;
    bool fim = false;
    bool desistiu = false;
    int contRotates=0;
    int contMovi=0;
    int (*currentMap)[order];
    int (*currentMapReset)[order];

    //d s s s s d s e e e d d d d w w a w a
    int map[order][order] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,1,6,0,0,1},
    {1,0,0,0,0,0,2,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,1,0,1,1,0,0,1,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,0,1},
    {1,0,0,1,0,0,3,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,1,4,0,0,0,1,1},
    {1,0,0,0,0,0,0,7,0,0,0,1,0,1},
    {1,0,0,0,1,0,0,1,0,0,1,5,0,1},
    {1,0,0,1,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,3,6,1,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
 int mapReset[order][order] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,1,6,0,0,1},
    {1,0,0,0,0,0,2,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,1,0,1,1,0,0,1,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,0,1},
    {1,0,0,1,0,0,3,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,1,4,0,0,0,1,1},
    {1,0,0,0,0,0,0,7,0,0,0,1,0,1},
    {1,0,0,0,1,0,0,1,0,0,1,5,0,1},
    {1,0,0,1,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,1,0,3,6,1,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
    //w w w w d d d d w d d d e e a a a a
  int map2[order][order] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,7,0,0,0,0,1,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,6,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,4,3,0,0,5,1},
    {1,0,0,0,0,0,0,1,1,1,1,0,0,1},
    {1,0,0,0,3,0,0,0,0,1,0,0,1,1},
    {1,0,0,0,1,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,6,1},
    {1,2,0,0,0,0,0,1,0,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
  int map2Reset[order][order] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,7,0,0,0,0,1,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,6,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,4,3,0,0,5,1},
    {1,0,0,0,0,0,0,1,1,1,1,0,0,1},
    {1,0,0,0,3,0,0,0,0,1,0,0,1,1},
    {1,0,0,0,1,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,6,1},
    {1,2,0,0,0,0,0,1,0,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
//w w w w w a a w a a a a a e e e w w w w d d d d w d d d s
int mapHard[order][order] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,0,0,1,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,4,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,6,0,0,0,0,0,0,0,0,0,6,0,1},
    {1,0,0,0,1,0,7,6,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,1,0,0,2,0,0,1},
    {1,0,0,0,3,0,1,5,3,1,1,0,3,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
int mapHardReset[order][order] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,0,0,1,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,4,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,6,0,0,0,0,0,0,0,0,0,6,0,1},
    {1,0,0,0,1,0,7,6,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,1,0,0,2,0,0,1},
    {1,0,0,0,3,0,1,5,3,1,1,0,3,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
while (options!=4){
    cout << "\n1-Sobre 2-Novo jogo 3-Continuar 4-Fim\n";
    cin >> options;

    if (options == 1) {
        exibirSobre();
        getch();
        cout << "\n1-Sobre 2-Novo jogo 3-Continuar 4-Fim\n";
        cin >> options;
    }

    if (options == 2) {
        cout << "Digite 1 para o mapa map1,2 para map2 e 3 para o mapa difícil: ";
        cin >> mapSelection;
    }

    if (options == 3) {
        fim=false;
    }
    if(options == 4){
        fim=true;
        return 0;
    }
    if(options!=3){
        selection(mapSelection,mapReset,map,currentMap,currentMapReset,map2,map2Reset,mapHard,mapHardReset);    
        defineCoordinates(pli,pco,mapSelection);
    }
    while (fim || venceu) {
        system("clear");  
        mapGenerate(currentMap,contRotates,contMovi);
        x = getch();
        movi(x,currentMap,currentMapReset,pli,pco,contRotates,contMovi,mapSelection,venceu,voltarMenu,fim);
        if(voltarMenu){
            fim=true;
        }
    }


}
    return 0;
}
//Passagem por valor nao precisa de backup das mapes OKK
//Procurar alguma forma de não fazer 3 if para cada mapa OKKK
//Adicionar as alavancas no mapgenerate
//ENTENDER COMPLEXIDADE DA ROTAÇAO
//LOGICA DE MOVIMENTAÇÃO
//CHAMAR TUDO NA FUNÇÃO
//ORIENTAÇÕES NO JOGO

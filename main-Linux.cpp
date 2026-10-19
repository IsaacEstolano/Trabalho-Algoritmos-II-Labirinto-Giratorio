#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
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
#define door 6
#define playerIn 8
#define playerInLever 9
#define playerInExit 10
using namespace std;

struct Position{
    int row;
    int column;
};
struct Map{
    int mapOrder;
    int numMap;
    string MapName;
    int map[21][21];
};
struct Game{
    char keyB;
    bool venceu;
    bool voltarMenu;
    bool fim;
    bool desistiu;
    bool currentGame;
    bool last;
    int orientation;
    int contRotates;
    int contMovi;
    Position position;
    Map currentMap;
    Map mapReset;
};
struct Menu{
    int options;
};
void about() {
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
    cout << "CONTROLES:" << endl;
    cout << "  W A S D - mover      Q - girar anti-horario" << endl;
    cout << "  E - girar horario    R - reiniciar a fase" << endl;
    cout << "  K - voltar ao menu (o jogo fica pausado)" << endl << endl;
}
void readMap(Map &currentMap){
    ifstream file;
    string element,dimension,mapName;
    int i=0,j;
    file.open("Mapas/map"+to_string(currentMap.numMap)+".csv");

    if(!file.is_open()){
        cout<<"Erro ao abrir o arquivo";
        return;
    }
    getline(file,dimension);//pega a dimensao e atribui pra o map order da struct map
    currentMap.mapOrder=stoi(dimension);

    getline(file,mapName);
    currentMap.MapName=mapName;

    while(i<=currentMap.mapOrder-1){
        getline(file,element);
        stringstream ss(element);
        j=0;
            while(getline(ss,element,',')){
                currentMap.map[i][j]=stoi(element);
                j++;
            }
        i++;
    }


}

void defineCoordinates(Game &game){
    if (game.currentMap.numMap == 1) {
        game.position.row = 3;
        game.position.column = 6;
    }
    if (game.currentMap.numMap == 2) {
        game.position.row = 12;
        game.position.column = 1;
    }
    if (game.currentMap.numMap == 3) {
        game.position.row = 11;
        game.position.column = 10;
    }
}
void reset(Game &game){
    game.currentMap = game.mapReset;
    defineCoordinates(game);
    game.contRotates = 0;
    game.contMovi = 0;
    game.orientation = 0;
    game.venceu = false;
    game.fim = false;
    game.last = false;
}
void selection(Game &game){
    game.currentMap.numMap = game.mapReset.numMap;
    readMap(game.currentMap);
    game.mapReset = game.currentMap;
    defineCoordinates(game);
    game.contRotates = 0;
    game.contMovi = 0;
    game.orientation = 0;
    game.venceu = false;
    game.fim = false;
    game.last = false;
}
void win(Game &game){
    game.venceu=true;
            cout << "\n==========================================" << endl;
            cout << "        PARABENS! Voce venceu!           " << endl;
            cout << "==========================================" << endl;
    game.fim=true;
}
void smashed(Game &game){
    game.venceu=true;
            cout << "\n==========================================" << endl;
            cout << "        PARABENS! Voce foi esmagado!           " << endl;
            cout << "==========================================" << endl;
    game.fim=true;
}
void mapRotationNormal(Game &game){
   int N = order; // Tamanho da map NxN
for (int i = 0; i < N / 2; i++) {
    for (int j = i; j < N - i - 1; j++) {
        int temp = game.currentMap.map[i][j];
        game.currentMap.map[i][j] = game.currentMap.map[N - 1 - j][i];
        game.currentMap.map[N - 1 - j][i] = game.currentMap.map[N - 1 - i][N - 1 - j];
        game.currentMap.map[N - 1 - i][N - 1 - j] = game.currentMap.map[j][N - 1 - i];
        game.currentMap.map[j][N - 1 - i] = temp;
    }
}   int newPli=game.position.column;
    int newPco=N-1-game.position.row;

    game.position.row=newPli;
    game.position.column=newPco;
    game.contRotates ++;
    }
//nova linha = coluna antiga
//nova coluna = N - linha antiga
void mapRotationAnti(Game &game){
    int N=order;
    for(int i=0;i<N/2;i++){
        for(int j=0;j<(N+1)/2;j++){
            int temp = game.currentMap.map[i][j];
                game.currentMap.map[i][j] = game.currentMap.map[j][N - 1 - i];
                game.currentMap.map[j][N - 1 - i] = game.currentMap.map[N - 1 - i][N - 1 - j];
                game.currentMap.map[N - 1 - i][N - 1 - j] = game.currentMap.map[N - 1 - j][i];
                game.currentMap.map[N - 1 - j][i] = temp;
        }
    }
    int newPco=game.position.row;
    int newPli=N-1-game.position.column;

    game.position.row=newPli;
    game.position.column=newPco;
    game.contRotates++;
}
void gravity(Game &game)
{
    for(int i = order - 2; i >= 0; i--){
        for(int j = 0; j < order; j++){
             if(game.currentMap.map[i][j] != box)
                continue;
            int linha = i;
            while(linha + 1 < order){
                bool podeCair = false;

                if(game.currentMap.map[linha + 1][j] == hollow){
                    podeCair = true;
                }
                // Porta aberta
                if(game.currentMap.map[linha + 1][j] == door && game.contRotates % 2 != 0){
                    podeCair = true;
                }
                if(game.currentMap.map[linha+1][j] == player || game.currentMap.map[linha+1][j] == playerInLever || game.currentMap.map[linha+1][j]==playerIn){
                   smashed(game);
                    break;
                }
                if(podeCair){
                    game.currentMap.map[linha + 1][j] = box;
                    game.currentMap.map[linha][j] = hollow;
                    linha++;
                }
                else{
                    break;
                }
            }
        }
    }
}
void mapGenerate(Game &game) {
     cout<<"Movimentos:"<<game.contMovi <<endl;
    cout<<"Rotações:"<<game.contRotates << endl;
    cout<<"Orientação:"<<game.orientation <<endl;
    cout<<"Mapa:"<<game.currentMap.numMap << endl;
    for (int i = 0; i < line; i++) {
        for (int j = 0; j < collum; j++) {
            switch (game.currentMap.map[i][j]) {
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
                case playerInLever:
                    cout<< "+";
                    break;
                    case door:
                    if (game.contRotates % 2 == 0){
                        cout << "=";  // fechada
                    }
                    else{
                        cout << ":";  // aberta
                    }
                    break;
                    case playerInExit:
                        cout<<"$";
                        break;
                default:
                    cout << " ";
                    break;
            }
            if(game.last){
                win(game);
                break;
            }
        }
        cout << endl;
    }
}
void movi(Game &game){
    switch (game.keyB) {
            case 'w':
                if (game.currentMap.map[game.position.row-1][game.position.column] == wall) {
                    break;
                }
                 if (game.currentMap.map[game.position.row-1][game.position.column] == box) {
                    break;
                    }
                 if (game.currentMap.map[game.position.row-1][game.position.column] == door && game.contRotates % 2 ==0) {
                    break;
                }
                 if (game.currentMap.map[game.position.row-1][game.position.column] == door && game.contRotates % 2 != 0) {
                     game.currentMap.map[game.position.row-1][game.position.column] = playerIn;
                      game.currentMap.map[game.position.row][game.position.column] = hollow;
                }
                 
                if (game.currentMap.map[game.position.row-1][game.position.column] == lever) {
                    game.currentMap.map[game.position.row-1][game.position.column] = playerInLever;
                    game.currentMap.map[game.position.row][game.position.column]=hollow;

                }
                if (game.currentMap.map[game.position.row][game.position.column] == playerInLever) {
                    game.currentMap.map[game.position.row][game.position.column] = lever;
                    game.currentMap.map[game.position.row-1][game.position.column] = player;

                }
                if(game.currentMap.map[game.position.row][game.position.column]== playerIn && game.currentMap.map[game.position.row-1][game.position.column]==hollow){
                    game.currentMap.map[game.position.row-1][game.position.column]=player;
                    game.currentMap.map[game.position.row][game.position.column]=door;
                }
                if(game.currentMap.map[game.position.row-1][game.position.column] == exit){
                    game.currentMap.map[game.position.row-1][game.position.column] = playerInExit;
                    game.currentMap.map[game.position.row][game.position.column] = hollow;
                    game.last=true;
                }
                 if(game.currentMap.map[game.position.row-1][game.position.column] ==  hollow && game.currentMap.map[game.position.row][game.position.column]==player) {
                    game.currentMap.map[game.position.row-1][game.position.column] = player;
                    game.currentMap.map[game.position.row][game.position.column] = hollow;
                }
              
                game.position.row--;
                game.contMovi++;
                break;

            case 's':
                if (game.currentMap.map[game.position.row+1][game.position.column] == wall) {
                    break;
                }
                 if (game.currentMap.map[game.position.row+1][game.position.column] == box) {
                    break;
                }
                 if (game.currentMap.map[game.position.row+1][game.position.column] == door && game.contRotates % 2 ==0) {
                    break;
                }
                 if (game.currentMap.map[game.position.row+1][game.position.column] == door && game.contRotates % 2 != 0) {
                     game.currentMap.map[game.position.row+1][game.position.column] = playerIn;
                      game.currentMap.map[game.position.row][game.position.column] = hollow;
                }
                if (game.currentMap.map[game.position.row+1][game.position.column] == lever) {
                    game.currentMap.map[game.position.row+1][game.position.column] = playerInLever;
                    game.currentMap.map[game.position.row][game.position.column]=hollow;
                }
                if (game.currentMap.map[game.position.row][game.position.column] == playerInLever) {
                    game.currentMap.map[game.position.row][game.position.column] = lever;
                    game.currentMap.map[game.position.row+1][game.position.column] = player;

                }
                 if(game.currentMap.map[game.position.row][game.position.column]== playerIn && game.currentMap.map[game.position.row+1][game.position.column]==hollow){
                    game.currentMap.map[game.position.row+1][game.position.column]=player;
                    game.currentMap.map[game.position.row][game.position.column]=door;
                }
                  if(game.currentMap.map[game.position.row+1][game.position.column] == exit){
                    game.currentMap.map[game.position.row+1][game.position.column] = playerInExit;
                    game.currentMap.map[game.position.row][game.position.column] = hollow;
                    game.last=true;
                }
                   if(game.currentMap.map[game.position.row+1][game.position.column] ==  hollow && game.currentMap.map[game.position.row][game.position.column]==player) {
                    game.currentMap.map[game.position.row+1][game.position.column] = player;
                    game.currentMap.map[game.position.row][game.position.column] = hollow;
                }
                
                game.position.row++;
                game.contMovi++;
                break;

            case 'a':
                if (game.currentMap.map[game.position.row][game.position.column-1] == wall) {
                    break;
                }
                  if (game.currentMap.map[game.position.row][game.position.column-1] == box) {
                    break;
                }
                 if (game.currentMap.map[game.position.row][game.position.column-1] == door && game.contRotates %2 ==0) {
                    break;
                }
                if (game.currentMap.map[game.position.row][game.position.column-1] == door && game.contRotates % 2 != 0) {
                     game.currentMap.map[game.position.row][game.position.column-1] = playerIn;
                      game.currentMap.map[game.position.row][game.position.column] = hollow;
                }
                if (game.currentMap.map[game.position.row][game.position.column-1] == lever) {
                    game.currentMap.map[game.position.row][game.position.column-1] = playerInLever;
                     game.currentMap.map[game.position.row][game.position.column]=hollow;

                }
                if (game.currentMap.map[game.position.row][game.position.column] == playerInLever) {
                    game.currentMap.map[game.position.row][game.position.column] = lever;
                    game.currentMap.map[game.position.row][game.position.column-1] = player;
                }
                 if(game.currentMap.map[game.position.row][game.position.column]== playerIn && game.currentMap.map[game.position.row][game.position.column-1]==hollow){
                    game.currentMap.map[game.position.row][game.position.column-1]=player;
                    game.currentMap.map[game.position.row][game.position.column]=door;
                }
                if(game.currentMap.map[game.position.row][game.position.column-1] == exit){
                    game.currentMap.map[game.position.row][game.position.column-1] = playerInExit;
                    game.currentMap.map[game.position.row][game.position.column] = hollow;
                    game.last=true;
                }
                 if(game.currentMap.map[game.position.row][game.position.column-1] ==  hollow && game.currentMap.map[game.position.row][game.position.column]==player) {
                    game.currentMap.map[game.position.row][game.position.column-1] = player;
                    game.currentMap.map[game.position.row][game.position.column] = hollow;
                }
                game.position.column--;
                game.contMovi++;
                break;

            case 'd':
                if (game.currentMap.map[game.position.row][game.position.column+1] == wall) {
                    break;
                }
                 if (game.currentMap.map[game.position.row][game.position.column+1] == box) {
                    break;
                }
                if (game.currentMap.map[game.position.row][game.position.column+1] == door && game.contRotates % 2 ==0) {
                    break;
                }
                  if (game.currentMap.map[game.position.row][game.position.column+1] == door && game.contRotates % 2 != 0) {
                     game.currentMap.map[game.position.row][game.position.column+1] = playerIn;
                      game.currentMap.map[game.position.row][game.position.column] = hollow;
                }
                if (game.currentMap.map[game.position.row][game.position.column+1] == lever) {
                    game.currentMap.map[game.position.row][game.position.column+1] = playerInLever;
                    game.currentMap.map[game.position.row][game.position.column]=hollow;

                }
                if (game.currentMap.map[game.position.row][game.position.column] == playerInLever) {
                    game.currentMap.map[game.position.row][game.position.column] = lever;
                    game.currentMap.map[game.position.row][game.position.column+1] = player;

                }
                 if(game.currentMap.map[game.position.row][game.position.column]== playerIn && game.currentMap.map[game.position.row][game.position.column+1]==hollow){
                    game.currentMap.map[game.position.row][game.position.column+1]=player;
                    game.currentMap.map[game.position.row][game.position.column]=door;
                }
                if(game.currentMap.map[game.position.row][game.position.column+1] == exit){
                    game.currentMap.map[game.position.row][game.position.column+1] = playerInExit;
                    game.currentMap.map[game.position.row][game.position.column] = hollow;
                    game.last=true;
                }
                if(game.currentMap.map[game.position.row][game.position.column+1] ==  hollow && game.currentMap.map[game.position.row][game.position.column]==player) {
                    game.currentMap.map[game.position.row][game.position.column+1] = player;
                    game.currentMap.map[game.position.row][game.position.column] = hollow;
                }

                game.position.column++;
                game.contMovi++;
                break;
                case 'q':
                if(game.currentMap.map[game.position.row][game.position.column]==playerInLever){
                    mapRotationAnti(game);
                    gravity(game);
                    if(game.orientation==270 || game.orientation==-270){
                        game.orientation=0;
                        break;
                    }
                    game.orientation-=90;
                }
                    break;
                case 'e':
                if(game.currentMap.map[game.position.row][game.position.column]==playerInLever){
                    mapRotationNormal(game);
                    gravity(game);
                     if(game.orientation==270 || game.orientation==-270){
                        game.orientation=0;
                        break;
                    }
                    game.orientation+=90;
                }
                    break;
                case 'r':
                     reset(game);
                     game.contRotates=0;
                        game.contMovi=0;
                        game.orientation=0;
                        break;
                    case 'k':
                    game.fim=true;
                    game.voltarMenu=true;
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
    Game game = {};
    Menu menu = {};
    game.fim = true;

//Map 1 Solution:
//a a s s d d d d d s s s q q q a a w a a s s d s d 

//Map 2 solution:
 //w w w w d d d d w d d d e e a a a a

//Map hard Solution:
//w w w w w a a w a a a a a e e e w w w w d d d d w d d d s

    while (menu.options != 4){
        cout << "\n1-Sobre 2-Novo jogo 3-Continuar 4-Fim\n";
        cin >> menu.options;

        if (menu.options == 1) {
            about();
            getch();
        }

        if (menu.options == 2) {
            cout << "Digite 1 para o mapa map1,2 para map2 e 3 para o mapa difícil: ";
            cin >> game.mapReset.numMap;
            selection(game);
            game.currentGame = true;
        }

        if (menu.options == 3) {
            if (game.currentGame) {
                game.fim = false;
            }
            else {
                cout << "Nao existe uma partida para continuar." << endl;
            }
        }

        if (menu.options == 4){
            game.fim = true;
            return 0;
        }

        while (!game.fim && !game.venceu) {
            system("clear");
            mapGenerate(game);

            game.keyB = getch();
            movi(game);
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

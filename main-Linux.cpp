#include <iostream>
#include <termios.h>
#include <unistd.h>
#define order 15
#define line 15
#define collum 15
#define line2 9
#define collum2 12
#define hollow 0
#define wall 1
#define rightPlace 2
#define afterWall 3
#define box 4
#define player 5
#define boxIn 6
#define playerIn 7
#define lever 8;
using namespace std;

void defineCoordinates(int pli,int pco){
            pli = 10;
            pco = 2;
}

bool win(bool venceu){
          if (venceu == true) {
            cout << "\n==========================================" << endl;
            cout << "        PARABENS! Voce venceu!           " << endl;
            cout << "==========================================" << endl;
        }
    return false;
}
void mapRotationNormal(int map[][collum],int pli,int pco){
   int N = order; // Tamanho da map NxN
for (int i = 0; i < N / 2; i++) {
    for (int j = i; j < N - i - 1; j++) {
        int temp = map[i][j];
        map[i][j] = map[N - 1 - j][i];
        map[N - 1 - j][i] = map[N - 1 - i][N - 1 - j];
        map[N - 1 - i][N - 1 - j] = map[j][N - 1 - i];
        map[j][N - 1 - i] = temp;
    }
}
    }

void mapRotationAnti(int map[][collum],int pli,int pco){

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
    bool desistiu = false;
    int caixasSoltas = 0;
    int contRotates=0;
    int contMov=0;
    int map[line][collum] = {
        {3,3,1,1,1,1,1,3,3,3,3,3,3,3,3},
        {3,3,1,0,0,0,1,3,3,3,3,3,3,3,3},
        {3,3,1,0,0,0,1,1,1,1,1,1,3,3,3},
        {3,3,1,0,0,0,0,1,0,0,0,1,3,3,3},
        {3,3,1,1,0,0,0,1,0,0,0,1,3,3,3},
        {3,1,1,1,0,0,0,0,0,0,0,1,3,3,3},
        {1,1,1,1,0,0,1,1,1,0,0,1,3,3,3},
        {1,1,1,1,0,0,1,3,1,0,2,1,3,3,3},
        {1,0,0,4,0,0,1,3,1,0,2,1,3,3,3},
        {1,0,0,4,4,0,1,3,1,0,2,1,3,3,3},
        {1,0,5,0,0,0,1,3,1,1,1,1,3,3,3},
        {1,1,1,1,1,1,1,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3}
    };

    int map_reset[line][collum] = {
        {3,3,1,1,1,1,1,3,3,3,3,3,3,3,3},
        {3,3,1,0,0,0,1,3,3,3,3,3,3,3,3},
        {3,3,1,0,0,0,1,1,1,1,1,1,3,3,3},
        {3,3,1,0,0,0,0,1,0,0,0,1,3,3,3},
        {3,3,1,1,0,0,0,1,0,0,0,1,3,3,3},
        {3,1,1,1,0,0,0,0,0,0,0,1,3,3,3},
        {1,1,1,1,0,0,1,1,1,0,0,1,3,3,3},
        {1,1,1,1,0,0,1,3,1,0,2,1,3,3,3},
        {1,0,0,4,0,0,1,3,1,0,2,1,3,3,3},
        {1,0,0,4,4,0,1,3,1,0,2,1,3,3,3},
        {1,0,5,0,0,0,1,3,1,1,1,1,3,3,3},
        {1,1,1,1,1,1,1,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3}
    };

    cout << "\n1-Autores  2-Orientacoes do jogo  3-Jogar\n";
    cin >> options;

    if (options == 1) {
        cout << "Autores: Isaac Estolano\n";
        cout << "\nPressione qualquer tecla para continuar...";
        getch();
        cout << "\n1-Autores  2-Orientacoes do jogo  3-Jogar\n";
        cin >> options;
    }

    if (options == 2) {
        cout << "==================================================" << endl;
        cout << "              ORIENTACOES DO JOGO                " << endl;
        cout << "==================================================" << endl;
        cout << endl;
        cout << "OBJETIVO:" << endl;
        cout << "Posicione todas as caixas ($) sobre os alvos (-)." << endl;
        cout << "Quando uma caixa chega ao alvo ela vira %." << endl;
        cout << "O nivel esta completo quando nao houver mais $ no mapa." << endl;
        cout << endl;
        cout << "CONTROLES:" << endl;
        cout << "  W - Mover para cima" << endl;
        cout << "  S - Mover para baixo" << endl;
        cout << "  A - Mover para esquerda" << endl;
        cout << "  D - Mover para direita" << endl;
        cout << "  R - Reiniciar o mapa atual" << endl;
        cout << "Nao e necessario pressionar Enter." << endl;
        cout << endl;
        cout << "LEGENDA DO MAPA:" << endl;
        cout << "  @  Jogador" << endl;
        cout << "  ?  Jogador sobre um alvo" << endl;
        cout << "  $  Caixa" << endl;
        cout << "  %  Caixa posicionada no alvo (correta)" << endl;
        cout << "  -  Alvo (destino de uma caixa)" << endl;
        cout << "  &  Parede" << endl;
        cout << "     Espaco vazio / area externa" << endl;
        cout << endl;
        cout << "REGRAS FUNDAMENTAIS:" << endl;
        cout << "  - O jogador so consegue empurrar caixas, nunca puxa-las." << endl;
        cout << "  - So e possivel empurrar uma caixa se o espaco atras" << endl;
        cout << "    dela estiver livre (vazio ou alvo)." << endl;
        cout << "  - Nao e possivel empurrar duas caixas ao mesmo tempo." << endl;
        cout << "  - Caixas nao podem atravessar paredes." << endl;
        cout << endl;
        cout << "DICAS DE ESTRATEGIA:" << endl;
        cout << "  - Planeje antes de agir: um movimento errado pode" << endl;
        cout << "    deixar o nivel sem solucao." << endl;
        cout << "  - Caixas encostadas em cantos ou ao longo de paredes" << endl;
        cout << "    sem alvo proximo ficam travadas para sempre." << endl;
        cout << "  - Observe os alvos e trace o caminho inverso: de onde" << endl;
        cout << "    a caixa precisa vir para chegar la." << endl;
        cout << "  - Se travar, reinicie o nivel com R e tente outra abordagem." << endl;
        cout << endl;
        cout << "==================================================" << endl;
        cout << "        Pressione qualquer tecla para jogar...    " << endl;
        cout << "==================================================" << endl;
        getch();
        options = 3;
    }

    if (options == 3) {
        cout << "Digite 1 para o mapa map1 e 2 para map2: ";
        cin >> mapSelection;
        
    }

    while (true) {

        system("clear");

        if (mapSelection == 1) {
            for (int i = 0; i < line; i++) {
                for (int j = 0; j < collum; j++) {
                    switch (map[i][j]) {
                        case hollow:   
                          cout << " "; 
                          break;
                        case wall:     
                          cout << "&"; 
                          break;
                        case rightPlace: 
                        cout << "-";
                         break;
                        case afterWall: 
                         cout << " "; 
                         break;
                        case box:       
                         cout << "$"; 
                         break;
                        case player:    
                         cout << "@";
                          break;
                        case boxIn:     
                         cout << "%";
                          break;
                        case playerIn: 
                          cout << "?";
                           break;
                        default:   
                          cout << " "; 
                          break;
                    }
                }
                cout << endl;
            }
            cout << "\n[R] Reiniciar  [G] Desistir" << endl;
        }

        if (venceu == true) {
            cout << "\n==========================================" << endl;
            cout << "        PARABENS! Voce venceu!           " << endl;
            cout << "==========================================" << endl;
            break;
        }

      
        x = getch();

        // RESET
        if (x == 'r') {
            if (mapSelection == 1) {
                for (int i = 0; i < line; i++) {
                    for (int j = 0; j < collum; j++) {
                        map[i][j] = map_reset[i][j];
                    }
                }
                pli = 10;
                pco = 2;
            }
        }

        if (x == 'g') {
            desistiu = true;
        }
        if (mapSelection == 1) {
            switch (x) {
            case 'w':
                if (map[pli-1][pco] == wall) {
                    break;
                }
                if (map[pli-1][pco] == box || map[pli-1][pco] == boxIn) {
                    if (map[pli-2][pco] == hollow || map[pli-2][pco] == rightPlace) {
                        if (map[pli-2][pco] == rightPlace) {
                            map[pli-2][pco] = boxIn;
                        }
                        else {
                            map[pli-2][pco] = box;
                        }
                        if (map[pli-1][pco] == boxIn) {
                            map[pli-1][pco] = playerIn;
                        }
                        else {
                            map[pli-1][pco] = player;
                        }
                        if (map[pli][pco] == playerIn) {
                            map[pli][pco] = rightPlace;
                        }
                        else {
                            map[pli][pco] = hollow;
                        }
                        pli--;
                    }
                    break;
                }
                if (map[pli-1][pco] == rightPlace) {
                    map[pli-1][pco] = playerIn;
                }
                else {
                    map[pli-1][pco] = player;
                }
                if (map[pli][pco] == playerIn) {
                    map[pli][pco] = rightPlace;
                }
                else {
                    map[pli][pco] = hollow;
                }
                pli--;
                break;

            case 's':
                if (map[pli+1][pco] == wall) {
                    break;
                }
                if (map[pli+1][pco] == box || map[pli+1][pco] == boxIn) {
                    if (map[pli+2][pco] == hollow || map[pli+2][pco] == rightPlace) {
                        if (map[pli+2][pco] == rightPlace) {
                            map[pli+2][pco] = boxIn;
                        }
                        else {
                            map[pli+2][pco] = box;
                        }
                        if (map[pli+1][pco] == boxIn) {
                            map[pli+1][pco] = playerIn;
                        }
                        else {
                            map[pli+1][pco] = player;
                        }
                        if (map[pli][pco] == playerIn) {
                            map[pli][pco] = rightPlace;
                        }
                        else {
                            map[pli][pco] = hollow;
                        }
                        pli++;
                    }
                    break;
                }
                if (map[pli+1][pco] == rightPlace) {
                    map[pli+1][pco] = playerIn;
                }
                else {
                    map[pli+1][pco] = player;
                }
                if (map[pli][pco] == playerIn) {
                    map[pli][pco] = rightPlace;
                }
                else {
                    map[pli][pco] = hollow;
                }
                pli++;
                break;

            case 'a':
                if (map[pli][pco-1] == wall) {
                    break;
                }
                if (map[pli][pco-1] == box || map[pli][pco-1] == boxIn) {
                    if (map[pli][pco-2] == hollow || map[pli][pco-2] == rightPlace) {
                        if (map[pli][pco-2] == rightPlace) {
                            map[pli][pco-2] = boxIn;
                        }
                        else {
                            map[pli][pco-2] = box;
                        }
                        if (map[pli][pco-1] == boxIn) {
                            map[pli][pco-1] = playerIn;
                        }
                        else {
                            map[pli][pco-1] = player;
                        }
                        if (map[pli][pco] == playerIn) {
                            map[pli][pco] = rightPlace;
                        }
                        else {
                            map[pli][pco] = hollow;
                        }
                        pco--;
                    }
                    break;
                }
                if (map[pli][pco-1] == rightPlace) {
                    map[pli][pco-1] = playerIn;
                }
                else {
                    map[pli][pco-1] = player;
                }
                if (map[pli][pco] == playerIn) {
                    map[pli][pco] = rightPlace;
                }
                else {
                    map[pli][pco] = hollow;
                }
                pco--;
                break;

            case 'd':
                if (map[pli][pco+1] == wall) {
                    break;
                }
                if (map[pli][pco+1] == box || map[pli][pco+1] == boxIn) {
                    if (map[pli][pco+2] == hollow || map[pli][pco+2] == rightPlace) {
                        if (map[pli][pco+2] == rightPlace) {
                            map[pli][pco+2] = boxIn;
                        }
                        else {
                            map[pli][pco+2] = box;
                        }
                        if (map[pli][pco+1] == boxIn) {
                            map[pli][pco+1] = playerIn;
                        }
                        else {
                            map[pli][pco+1] = player;
                        }
                        if (map[pli][pco] == playerIn) {
                            map[pli][pco] = rightPlace;
                        }
                        else {
                            map[pli][pco] = hollow;
                        }
                        pco++;
                    }
                    break;
                }
                if (map[pli][pco+1] == rightPlace) {
                    map[pli][pco+1] = playerIn;
                }
                else {
                    map[pli][pco+1] = player;
                }
                if (map[pli][pco] == playerIn) {
                    map[pli][pco] = rightPlace;
                }
                else {
                    map[pli][pco] = hollow;
                }
                pco++;
                break;
                case 'q':
                    mapRotationAnti(map,pli,pco);
                    break;
                case 'e':
                    mapRotationNormal(map,pli,pco);
            }
        }
    }

    if (desistiu == true) {
        system("clear");
        cout << "\n==========================================" << endl;
        cout << "     Voce desistiu. Ate a proxima!       " << endl;
        cout << "==========================================" << endl;
    }
    return 0;
}
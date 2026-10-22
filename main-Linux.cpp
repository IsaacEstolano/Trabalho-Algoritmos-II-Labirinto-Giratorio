#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
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
    int contkeys;
    Position position;
    Map currentMap;
    Map mapOrigin;
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
            cout << "        Voce foi esmagado. Derrota!      " << endl;
            cout << "==========================================" << endl;
    game.fim=true;
}
void gravity(Game &game)
{
    for(int i = game.currentMap.mapOrder - 2; i >= 0; i--){
        for(int j = 0; j < game.currentMap.mapOrder; j++){
             if(game.currentMap.map[i][j] != box)
                continue;
            int linha = i;
            while(linha + 1 < game.currentMap.mapOrder){
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
bool stableMap(const Game &jogoOriginal)
{
    Game jogoTeste = jogoOriginal;
    gravity(jogoTeste);

    /*if (jogoTeste.venceu) {
        return false;
    }*/

    int size = jogoOriginal.currentMap.mapOrder;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (jogoOriginal.currentMap.map[i][j] != jogoTeste.currentMap.map[i][j]) {
                return false;
            }
        }
    }

    return true;
}
bool readMap(Map &currentMap,Game gameCopy,string &reason){
    ifstream file;
    string element,dimension,mapName;
    int playersQuantity=0,exitPresence=0;
    Map loadedMap=currentMap;//copia,para validar,depois atribui para a matriz
    file.open("Mapas/map"+to_string(currentMap.numMap)+".csv");

    if(!file.is_open()){
        reason="Erro ao abrir o arquivo";
        return false;
    }
    if(!getline(file,dimension)){
        reason="Sem dimensão";
        return false;
    }
    if(dimension.empty()){
        reason="Dimensão vazia";
        return false;
    }
    stringstream dimensionStream(dimension);
    int mapOrder;
    string extra;
    if(!(dimensionStream >> mapOrder) || (dimensionStream >> extra)){
        reason="Dimensão não numérica ou inválida";
        return false;
    }
    if(mapOrder<11 || mapOrder>21){
        reason="Dimensão Inválida";
        return false;
    }
    loadedMap.mapOrder=mapOrder;

    if(!getline(file,mapName) || mapName.empty()){
        reason="Titulo ausente";
        return false;
    }
    loadedMap.MapName=mapName;

    for(int i=0;i<mapOrder;i++){
        if(!getline(file,element)){
            reason="Quantidade de linhas invalida";
            return false;
        }
        if(element.empty() || element.back()==','){
            reason="Quantidade de colunas invalida";
            return false;
        }
        stringstream rowStream(element);
        string cell;
        int j=0;
        while(getline(rowStream,cell,',')){
            if(j>=mapOrder){
                reason="Quantidade de colunas invalida";
                return false;
            }
            stringstream cellStream(cell);
            int value;
            string cellExtra;
            if(!(cellStream >> value) || (cellStream >> cellExtra)){
                reason="Valor do mapa não numérico ou inválido";
                return false;
            }
            if(value<0 || value>7){
                reason="O mapa tem valores numéricos inválidos";
                return false;
            }
            loadedMap.map[i][j]=value;
            if(value==2){
                playersQuantity++;
            }
            if(value==5){
                exitPresence++;
            }
            j++;
        }
        if(j!=mapOrder){
            reason="Quantidade de colunas invalida";
            return false;
        }
    }
    if(playersQuantity!=1){
        reason="O mapa deve apresentar exatamente um jogador";
        return false;
    }
    if(exitPresence==0){
        reason="O jogo não apresenta nenhuma saída";
        return false;
    }
    gameCopy.currentMap=loadedMap;
    gameCopy.orientation=0;
    gameCopy.contRotates=0;
    if(!stableMap(gameCopy)){
        reason="O jogo começa com blocos no ar";
        return false;
    }
    currentMap=loadedMap;
    reason.clear();
    return true;
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
    game.currentMap = game.mapOrigin;
    defineCoordinates(game);
    game.contRotates = 0;
    game.contkeys = 0;
    game.orientation = 0;
    game.venceu = false;
    game.fim = false;
    game.last = false;
}
bool selection(Game &game,Game &gameCopy,string reason){
    game.currentMap.numMap = game.mapOrigin.numMap;
    if(!readMap(game.currentMap,gameCopy,reason)){
        cout<<reason<<endl;
        return false;
    }
    game.mapOrigin = game.currentMap;
    defineCoordinates(game);
    game.contRotates = 0;
    game.contkeys = 0;
    game.orientation = 0;
    game.venceu = false;
    game.fim = false;
    game.last = false;
    return true;
}
void mapRotationNormal(Game &game){
   int N = game.currentMap.mapOrder; // size da map NxN
for (int i = 0; i < N / 2; i++) {
    for (int j = i; j < N - i - 1; j++) {
        int temp = game.currentMap.map[i][j];
        game.currentMap.map[i][j] = game.currentMap.map[N - 1 - j][i];
        game.currentMap.map[N - 1 - j][i] = game.currentMap.map[N - 1 - i][N - 1 - j];
        game.currentMap.map[N - 1 - i][N - 1 - j] = game.currentMap.map[j][N - 1 - i];
        game.currentMap.map[j][N - 1 - i] = temp;
    }
}   int newRow=game.position.column;
    int newColumn=N-1-game.position.row;

    game.position.row=newRow;
    game.position.column=newColumn;
    game.contRotates ++;
    }
//nova linha = coluna antiga
//nova coluna = N - linha antiga
void mapRotationAnti(Game &game){
    int N=game.currentMap.mapOrder;
    for(int i=0;i<N/2;i++){
        for(int j=0;j<(N+1)/2;j++){
            int temp = game.currentMap.map[i][j];
                game.currentMap.map[i][j] = game.currentMap.map[j][N - 1 - i];
                game.currentMap.map[j][N - 1 - i] = game.currentMap.map[N - 1 - i][N - 1 - j];
                game.currentMap.map[N - 1 - i][N - 1 - j] = game.currentMap.map[N - 1 - j][i];
                game.currentMap.map[N - 1 - j][i] = temp;
        }
    }
    int newColumn=game.position.row;
    int newRow=N-1-game.position.column;

    game.position.row=newRow;
    game.position.column=newColumn;
    game.contRotates++;
}
void mapGenerate(Game &game) {
    cout<<"Movimentos:"<<game.contkeys <<endl;
    cout<<"Rotações:"<<game.contRotates << endl;
    cout<<"Orientação:"<<game.orientation <<endl;
    cout<<"Mapa:"<<game.currentMap.numMap << endl;
    for (int i = 0; i < game.currentMap.mapOrder; i++) {
        for (int j = 0; j < game.currentMap.mapOrder; j++) {
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
        }
        cout << endl;
    }
    if(game.last && !game.venceu){
        win(game);
    }
}
void movement(Game &game,int displacementRow,int displacementColumn){
    int currentRow=game.position.row;
    int currentColumn=game.position.column;
    int finalPositionRow=currentRow + displacementRow;
    int finalPositionColumn=currentColumn + displacementColumn;

    if(finalPositionRow<0 || finalPositionRow>=game.currentMap.mapOrder ||
       finalPositionColumn<0 || finalPositionColumn>=game.currentMap.mapOrder){
        return;
    }

    int destination=game.currentMap.map[finalPositionRow][finalPositionColumn];
    if(destination==wall || destination==box ||(destination==door && game.contRotates%2==0)){
        return;
    }

    int currentCell=game.currentMap.map[currentRow][currentColumn];
    int cellUnderPlayer=hollow;
    if(currentCell==playerInLever){
        cellUnderPlayer=lever;
    }
    else if(currentCell==playerIn){
        cellUnderPlayer=door;
    }
    else if(currentCell==playerInExit){
        cellUnderPlayer=exit;
    }

    int playerOnDestination=player;
    if(destination==door){
        playerOnDestination=playerIn;
    }
    else if(destination==lever){
        playerOnDestination=playerInLever;
    }
    else if(destination==exit){
        playerOnDestination=playerInExit;
        game.last=true;
    }

    game.currentMap.map[currentRow][currentColumn]=cellUnderPlayer;
    game.currentMap.map[finalPositionRow][finalPositionColumn]=playerOnDestination;
    game.position.row=finalPositionRow;
    game.position.column=finalPositionColumn;
    game.contkeys++;
}
bool verifyPlayerInLever(Game &game){
    if(game.currentMap.map[game.position.row][game.position.column]==playerInLever){
        return true;
    }
    return false;
}
bool verifyGameOrientation(Game &game){
    if(game.orientation==270 || game.orientation==-270){
        game.orientation=0;
        return true;
        }
        return false;
}
void keys(Game &game,char keyB){
    switch (keyB) {
        case 'w':
            movement(game,-1,0);
            break;
        case 's':
            movement(game,1,0);
            break;
        case 'a':
            movement(game,0,-1);
            break;
        case 'd':
            movement(game,0,1);
            break;
                case 'q':
                if(verifyPlayerInLever(game)){
                    mapRotationAnti(game);
                    gravity(game);
                    game.orientation=(game.orientation+270)%360;
                }
                    break;
                case 'e':
                if(verifyPlayerInLever(game)){
                    mapRotationNormal(game);
                    gravity(game);
                    game.orientation=(game.orientation+90)%360;
                }
                    break;
                case 'r':
                     reset(game);
                     game.contRotates=0;
                        game.contkeys=0;
                        game.orientation=0;
                        break;
                    case 'k':
                    game.fim=true;
                    game.voltarMenu=true;
                    break;
            }
}
bool mapSolution(Game &game,string &reason){
    ifstream file;
    string element;
    char keyToMov;
    int i=0;
    file.open("Soluções/map"+to_string(game.currentMap.numMap)+".sol");

    if(!file.is_open()){
        reason="Erro ao abrir o arquivo";
        return false;
        //terminar mapSolution ,fazer mais 2 mapas,recordes e salvar jogo,depois disso finalmente verei um universo agradecido,e arrumar mensagem de vitoria
    }
    stringstream key(element);
    while(getline(file,element,',')){
        keyToMov=element[i];
        keys(game,keyToMov);
        i++;
        system("clear");
        mapGenerate(game);
    }
    return true;
}
bool saveGame(const Game &game,string &reason){
    ofstream file;
    int size=game.currentMap.mapOrder;

    if(size<11 || size>21){
        reason="Dimensão inválida para salvar o jogo";
        return false;
    }
    if(game.mapOrigin.mapOrder!=size){
        reason="Mapa original inválido para salvar o jogo";
        return false;
    }

    file.open("jogo_salvo.csv");

    if(!file.is_open()){
        reason="Erro ao abrir o arquivo do jogo salvo";
        return false;
    }

    file << game.currentMap.numMap << ",";
    file << game.currentMap.mapOrder << ",";
    file << game.orientation << ",";
    file << game.contRotates << ",";
    file << game.contkeys << ",";
    file << game.position.row << ",";
    file << game.position.column << endl;

    file << game.currentMap.MapName << endl;

    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            file << game.currentMap.map[i][j];
            if(j<size-1){
                file << ",";
            }
        }
        file << endl;
    }

    file << "MAPA_ORIGINAL" << endl;

    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            file << game.mapOrigin.map[i][j];
            if(j<size-1){
                file << ",";
            }
        }
        file << endl;
    }

    file.close();

    if(file.fail()){
        reason="Erro ao gravar o jogo salvo";
        return false;
    }

    reason.clear();
    return true;
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
    Game gameCopy = {};
    game.fim = true;
    string reason;
    
//Map 1 Solution:
//a a s s d d d d d s s s q q q a a w a a s s d s d 

//Map 2 solution:
 //c

//Map hard Solution:
//w w w w w a a w a a a a a e e e w w w w d d d d w d d d s

    while (menu.options != 4){
        cout << "\n1-Sobre 2-Novo jogo 3-Continuar 4-Ver soluções 5-Recordes 6-Fim\n";
        cin >> menu.options;

        if (menu.options == 1) {
            about();
            getch();
        }

        if (menu.options == 2) {
            cout << "Digite 1 para o mapa map1,2 para map2 e 3 para o mapa difícil: ";
            cin >> game.mapOrigin.numMap;
            game.currentGame = selection(game,gameCopy,reason);
        }

        if (menu.options == 3) {
          
        }
        if(menu.options == 4){
            cout<<"Digite o número do mapa que deseja ver a solução:";
            cin>>game.mapOrigin.numMap;
            mapSolution(game,reason);
            
        
        }
        if (menu.options == 6){
            game.fim = true;
            return 0;
        }

        while (!game.fim && !game.venceu) {
            system("clear");
            mapGenerate(game);

            game.keyB = getch();
            keys(game,game.keyB);
        }
    }
    return 0;
}
//Passagem por valor nao precisa de backup das mapes OKK
//Procurar alguma forma de não fazer 3 if para cada mapa OKKK
//Adicionar as alavancas no mapgenerate
//ENTENDER COMPLEXIDADE DA ROTAÇAO
//LOGICA DE keysMENTAÇÃO
//CHAMAR TUDO NA FUNÇÃO
//ORIENTAÇÕES NO JOGO

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <cstdio>
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
    bool voltarMenu;
    bool end;
    bool desistiu;
    bool currentGame;
    bool last;
    bool jogoSalvo;
    bool animate;
    int orientation;
    int contRotates;
    int contkeys;
    int escolha;
    Position position;
    Map currentMap;
    Map mapOrigin;
};
struct OperationResult{
    bool success;
    string reason;
};
struct Menu{
    int options;
};
struct Recorde{
    string nome;
    int mapa;
    int movimentos;
    int rotacoes;
};
template <typename T>
void trocar(T &first,T &second){
    T temporary=first;
    first=second;
    second=temporary;
}
void about() {
    cout << "==================================================" << endl;
    cout << "                LABIRINTO GIRATORIO               " << endl;
    cout << "==================================================" << endl;
    cout << "Equipe: Isaac Furtado Estolano da Silveira" << endl;
    cout << "Professor: Thiago Felski" << endl;
    cout << "Disciplina: Algoritmos e Programacao II - Outubro/2026" << endl;
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
    cout << "  ESC - salvar e voltar ao menu" << endl;
    cout << "  K - voltar ao menu com opcao de salvar" << endl;
    cout << "MENU: novo jogo, continuar, ver solucao, recordes, sobre e fim." << endl << endl;
}

void win(Game &game){
    game.end=true;
            cout << "\n==========================================" << endl;
            cout << "        PARABENS! Voce venceu!           " << endl;
            cout << "==========================================" << endl;
}
void smashed(Game &game){
    game.end=true;
            cout << "\n==========================================" << endl;
            cout << "        Voce foi esmagado. Derrota!      " << endl;
            cout << "==========================================" << endl;
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
                if(game.currentMap.map[linha+1][j] == player || game.currentMap.map[linha+1][j] == playerInLever || game.currentMap.map[linha+1][j]==playerIn || game.currentMap.map[linha+1][j]==playerInExit){
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
string mapFileName(int mapNumber){
    string fileName="map"+to_string(mapNumber)+".csv";
    ifstream file(fileName);
    if(file.is_open()){
        return fileName;
    }
    return "Mapas/"+fileName;
}
string solutionFileName(int mapNumber){
    string fileName="map"+to_string(mapNumber)+".sol";
    ifstream file(fileName);
    if(file.is_open()){
        return fileName;
    }
    file.open("Soluções/"+fileName);
    if(file.is_open()){
        return "Soluções/"+fileName;
    }
    return fileName;
}
OperationResult readMap(Map &currentMap,Game gameCopy){
    ifstream file;
    string element,dimension,mapName;
    int playersQuantity=0,exitPresence=0;
    Map loadedMap=currentMap;//copia,para validar,depois atribui para a matriz
    file.open(mapFileName(currentMap.numMap));

    if(!file.is_open()){
        return {false,"Erro ao abrir o arquivo"};
    }
    if(!getline(file,dimension)){
        return {false,"Sem dimensão"};
    }
    if(dimension.empty()){
        return {false,"Dimensão vazia"};
    }
    stringstream dimensionStream(dimension);
    int mapOrder;
    string extra;
    if(!(dimensionStream >> mapOrder) || (dimensionStream >> extra)){
        return {false,"Dimensão não numérica ou inválida"};
    }
    if(mapOrder<11 || mapOrder>21){
        return {false,"Dimensão Inválida"};
    }
    loadedMap.mapOrder=mapOrder;

    if(!getline(file,mapName) || mapName.empty()){
        return {false,"Titulo ausente"};
    }
    loadedMap.MapName=mapName;

    for(int i=0;i<mapOrder;i++){
        if(!getline(file,element)){
            return {false,"Quantidade de linhas invalida"};
        }
        if(element.empty() || element.back()==','){
            return {false,"Quantidade de colunas invalida"};
        }
        stringstream rowStream(element);
        string cell;
        int j=0;
        while(getline(rowStream,cell,',')){
            if(j>=mapOrder){
                return {false,"Quantidade de colunas invalida"};
            }
            stringstream cellStream(cell);
            int value;
            string cellExtra;
            if(!(cellStream >> value) || (cellStream >> cellExtra)){
                return {false,"Valor do mapa não numérico ou inválido"};
            }
            if(value<0 || value>7){
                return {false,"O mapa tem valores numéricos inválidos"};
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
            return {false,"Quantidade de colunas invalida"};
        }
    }
    if(playersQuantity!=1){
        return {false,"O mapa deve apresentar exatamente um jogador"};
    }
    if(exitPresence==0){
        return {false,"O jogo não apresenta nenhuma saída"};
    }
    gameCopy.currentMap=loadedMap;
    gameCopy.orientation=0;
    gameCopy.contRotates=0;
    if(!stableMap(gameCopy)){
        return {false,"O jogo começa com blocos no ar"};
    }
    currentMap=loadedMap;
    return {true,""};
}

void defineCoordinates(Game &game){
    for(int row=0;row<game.currentMap.mapOrder;row++){
        for(int column=0;column<game.currentMap.mapOrder;column++){
            if(game.currentMap.map[row][column]==player){
                game.position.row=row;
                game.position.column=column;
                return;
            }
        }
    }
}
void reset(Game &game){
    game.currentMap = game.mapOrigin;
    defineCoordinates(game);
    game.contRotates = 0;
    game.contkeys = 0;
    game.orientation = 0;
    game.end = false;
    game.last = false;
}
OperationResult selection(Game &game,Game &gameCopy){
    game.currentMap.numMap = game.mapOrigin.numMap;
    OperationResult result=readMap(game.currentMap,gameCopy);
    if(!result.success){
        return result;
    }
    game.mapOrigin = game.currentMap;
    defineCoordinates(game);
    game.contRotates = 0;
    game.contkeys = 0;
    game.orientation = 0;
    game.end = false;
    game.last = false;
    return {true,""};
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
    if(game.last && !game.end){
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
    if(game.animate){
        sleep(1);
    }
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
OperationResult saveGame(const Game &game){
        ofstream file;
        int size=game.currentMap.mapOrder;
    
        if(size<11 || size>21){
            return {false,"Dimensão inválida para salvar o jogo"};
        }
        if(game.mapOrigin.mapOrder!=size){
            return {false,"Mapa original inválido para salvar o jogo"};
        }
    
        file.open("jogo_salvo.csv");
    
        if(!file.is_open()){
            return {false,"Erro ao abrir o arquivo do jogo salvo"};
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
            return {false,"Erro ao gravar o jogo salvo"};
        }
    
        return {true,"Jogo salvo"};
    }
OperationResult verifySaveGame(Game &game){
    int i=0;
    cout<<"Digite 1 se deseja salvar seu jogo antes de voltar ao menu,se não aperte qualquer tecla:";
    cin>>i;
    if(i==1){
       OperationResult result = saveGame(game);
       game.jogoSalvo=result.success;
       return result;
    }
    return {false,""};
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
                    if(verifyGameOrientation(game)){
                        break;
                    };
                    game.orientation-=90;
                }
                    break;
                case 'e':
                if(verifyPlayerInLever(game)){
                    mapRotationNormal(game);
                    gravity(game);
                    if(verifyGameOrientation(game)){
                        break;
                    };
                    game.orientation+=90;
                }
                    break;
                case 'r':
                     reset(game);
                     game.contRotates=0;
                        game.contkeys=0;
                        game.orientation=0;
                        break;
                case 'k':
                {
                    OperationResult result=verifySaveGame(game);
                    cout<<result.reason;
                    if(!result.success && !result.reason.empty()){
                        cout<<result.reason<<endl;
                    }
                    game.end=true;
                    game.voltarMenu=true;
                    break;
                }
                case 27:
                {
                    OperationResult saveResult=saveGame(game);
                    cout<<saveResult.reason<<endl;
                    game.jogoSalvo=saveResult.success;
                    game.end=true;
                    game.voltarMenu=true;
                    break;
                }
                    
            }
}
OperationResult runSolution(Game &game,bool draw){
    int mapNumber=game.currentMap.numMap;
    Game gameCopy=game;
    game.currentMap.numMap=mapNumber;
    OperationResult mapResult=readMap(game.currentMap,gameCopy);
    if(!mapResult.success){
        return {false,"Mapa inválido: "+mapResult.reason};
    }
    game.mapOrigin=game.currentMap;
    game.orientation=0;
    game.contRotates=0;
    game.contkeys=0;
    game.end=false;
    game.last=false;
    game.voltarMenu=false;
    game.animate=draw;

    bool playerFound=false;
    for(int row=0;row<game.currentMap.mapOrder;row++){
        for(int column=0;column<game.currentMap.mapOrder;column++){
            if(game.currentMap.map[row][column]==player){
                game.position.row=row;
                game.position.column=column;
                playerFound=true;
                break;
            }
        }
        if(playerFound){
            break;
        }
    }
    if(!playerFound){
        return {false,"O mapa não contém a posição inicial do jogador"};
    }

    ifstream file(solutionFileName(mapNumber));
    if(!file.is_open()){
        return {false,"Arquivo de solução inexistente"};
    }

    string element;
    int keyNumber=0;
    bool hasKeys=false;
    if(draw){
        system("clear");
        mapGenerate(game);
    }
    while(getline(file,element)){
        int elementLength=element.length();
        for(int i=0;i<elementLength;i++){
            char key=element[i];
            if(key==' ' || key=='\t' || key=='\r'){
                continue;
            }
            hasKeys=true;
            keyNumber++;
            if(key>='A' && key<='Z'){
                key=key-'A'+'a';
            }
            if(key!='w' && key!='a' && key!='s' && key!='d' && key!='q' && key!='e'){
                return {false,"Tecla inválida na posição "+to_string(keyNumber)};
            }
            keys(game,key);
            if(draw){
                system("clear");
                mapGenerate(game);
            }
            if(game.last){
                game.end=true;
                return {true,"Solução válida: "+to_string(keyNumber)+" teclas, "+
                    to_string(game.contkeys)+" movimentos e "+
                    to_string(game.contRotates)+" rotações"};
            }
            if(game.end){
                return {false,"Derrota durante a solução na tecla "+to_string(keyNumber)};
            }
        }
    }
    if(!hasKeys){
        return {false,"Arquivo de solução vazio"};
    }
    return {false,"As teclas acabaram antes de alcançar a saída"};
}
OperationResult mapSolution(Game &game){
    return runSolution(game,true);
}
void verifyAllSolutions(){
    for(int mapNumber=1;mapNumber<=100;mapNumber++){
        ifstream mapFile(mapFileName(mapNumber));
        if(!mapFile.is_open()){
            break;
        }
        Game solutionGame={};
        solutionGame.currentMap.numMap=mapNumber;
        OperationResult result=runSolution(solutionGame,false);
        cout<<"Mapa "<<mapNumber<<" - "<<solutionGame.currentMap.MapName<<": "
            <<result.reason<<endl;
    }
}

OperationResult loadGame(Game &game){
    ifstream file;
    string element,metadata,mapName;
    int savedValues[7];
    int valuesQuantity=0;
    Game loadedGame = {};

    file.open("jogo_salvo.csv");

    if(!file.is_open()){
        return {false,"Não existe um jogo salvo válido"};
    }
    if(!getline(file,metadata) || metadata.empty() || metadata.back()==','){
        return {false,"Arquivo de jogo salvo vazio ou inválido"};
    }

    stringstream metadataStream(metadata);
    while(getline(metadataStream,element,',')){
        if(valuesQuantity>=7){
            return {false,"Dados de identificação do jogo inválidos"};
        }
        stringstream valueStream(element);
        string extra;
        if(!(valueStream >> savedValues[valuesQuantity]) || (valueStream >> extra)){
            return {false,"Dados de identificação do jogo inválidos"};
        }
        valuesQuantity++;
    }
    if(valuesQuantity!=7){
        return {false,"Dados de identificação do jogo incompletos"};
    }

    int mapNumber=savedValues[0];
    int size=savedValues[1];
    int orientation=savedValues[2];
    int rotations=savedValues[3];
    int movements=savedValues[4];
    int playerRow=savedValues[5];
    int playerColumn=savedValues[6];

    if(mapNumber<1){
        return {false,"Número do mapa salvo inválido"};
    }
    if(size<11 || size>21){
        return {false,"Dimensão do jogo salvo inválida"};
    }
    if(orientation<-270 || orientation>270 || orientation%90!=0){
        return {false,"Orientação do jogo salvo inválida"};
    }
    if(rotations<0 || movements<0){
        return {false,"Contadores do jogo salvo inválidos"};
    }
    if(playerRow<0 || playerRow>=size || playerColumn<0 || playerColumn>=size){
        return {false,"Posição do jogador salva inválida"};
    }
    if(!getline(file,mapName) || mapName.empty()){
        return {false,"Título do jogo salvo ausente"};
    }

    loadedGame.currentMap.numMap=mapNumber;
    loadedGame.currentMap.mapOrder=size;
    loadedGame.currentMap.MapName=mapName;
    loadedGame.mapOrigin.numMap=mapNumber;
    loadedGame.mapOrigin.mapOrder=size;
    loadedGame.mapOrigin.MapName=mapName;
    loadedGame.orientation=orientation;
    loadedGame.contRotates=rotations;
    loadedGame.contkeys=movements;
    loadedGame.position.row=playerRow;
    loadedGame.position.column=playerColumn;

    int currentPlayersQuantity=0;
    int currentPlayerRow=-1;
    int currentPlayerColumn=-1;
    int currentExitsQuantity=0;

    for(int i=0;i<size;i++){
        if(!getline(file,element) || element.empty() || element.back()==','){
            return {false,"Matriz atual do jogo salvo incompleta"};
        }
        stringstream rowStream(element);
        string cell;
        int j=0;

        while(getline(rowStream,cell,',')){
            if(j>=size){
                return {false,"Matriz atual do jogo salvo tem colunas extras"};
            }
            stringstream cellStream(cell);
            string cellExtra;
            int value;
            if(!(cellStream >> value) || (cellStream >> cellExtra)){
                return {false,"Valor inválido na matriz atual do jogo salvo"};
            }
            if(value<0 || value>10){
                return {false,"Código inválido na matriz atual do jogo salvo"};
            }

            loadedGame.currentMap.map[i][j]=value;
            if(value==player || value==playerIn || value==playerInLever || value==playerInExit){
                currentPlayersQuantity++;
                currentPlayerRow=i;
                currentPlayerColumn=j;
            }
            if(value==exit || value==playerInExit){
                currentExitsQuantity++;
            }
            j++;
        }
        if(j!=size){
            return {false,"Matriz atual do jogo salvo tem colunas faltando"};
        }
    }
    if(currentPlayersQuantity!=1){
        return {false,"Matriz atual deve conter exatamente um jogador"};
    }
    if(currentExitsQuantity==0){
        return {false,"Matriz atual do jogo salvo não contém saída"};
    }
    if(currentPlayerRow!=playerRow || currentPlayerColumn!=playerColumn){
        return {false,"Posição salva não corresponde à matriz atual"};
    }

    if(!getline(file,element) || element!="MAPA_ORIGINAL"){
        return {false,"Matriz original ausente no jogo salvo"};
    }

    int originalPlayersQuantity=0;
    int exitsQuantity=0;

    for(int i=0;i<size;i++){
        if(!getline(file,element) || element.empty() || element.back()==','){
            return {false,"Matriz original do jogo salvo incompleta"};
        }
        stringstream rowStream(element);
        string cell;
        int j=0;

        while(getline(rowStream,cell,',')){
            if(j>=size){
                return {false,"Matriz original do jogo salvo tem colunas extras"};
            }
            stringstream cellStream(cell);
            string cellExtra;
            int value;
            if(!(cellStream >> value) || (cellStream >> cellExtra)){
                return {false,"Valor inválido na matriz original do jogo salvo"};
            }
            if(value<0 || value>7){
                return {false,"Código inválido na matriz original do jogo salvo"};
            }

            loadedGame.mapOrigin.map[i][j]=value;
            if(value==player){
                originalPlayersQuantity++;
            }
            if(value==exit){
                exitsQuantity++;
            }
            j++;
        }
        if(j!=size){
            return {false,"Matriz original do jogo salvo tem colunas faltando"};
        }
    }
    if(originalPlayersQuantity!=1 || exitsQuantity==0){
        return {false,"Matriz original do jogo salvo não é válida"};
    }

    if(getline(file,element) && !element.empty()){
        return {false,"O arquivo de jogo salvo contém dados extras"};
    }

    loadedGame.keyB='\0';
    loadedGame.voltarMenu=false;
    loadedGame.end=false;
    loadedGame.desistiu=false;
    loadedGame.currentGame=true;
    loadedGame.last=loadedGame.currentMap.map[playerRow][playerColumn]==playerInExit;

    game=loadedGame;
    return {true,"Jogo carregado"};
}
bool parseRecordNumber(const string &text,int &number){
    string extra;
    stringstream valueStream(text);
    if(!(valueStream>>number) || (valueStream>>extra)){
        return false;
    }
    return true;
}
OperationResult loadRecords(Recorde records[],int &recordsQuantity){
    ifstream file("Recordes.csv");
    recordsQuantity=0;
    if(!file.is_open()){
        return {true,""};
    }
    string element;
    while(getline(file,element)){
        string name,mapText,movementsText,rotationsText,extra;
        stringstream rowStream(element);
        Recorde record;
        if(!getline(rowStream,name,',') || !getline(rowStream,mapText,',') ||
           !getline(rowStream,movementsText,',') || !getline(rowStream,rotationsText,',')){
            continue;
        }
        if(getline(rowStream,extra,',')){
            continue;
        }
        if(name.empty() || name.length()>15 ||
           !parseRecordNumber(mapText,record.mapa) ||
           !parseRecordNumber(movementsText,record.movimentos) ||
           !parseRecordNumber(rotationsText,record.rotacoes) ||
           record.mapa<1 || record.movimentos<0 || record.rotacoes<0){
            continue;
        }
        record.nome=name;
        if(recordsQuantity<200){
            records[recordsQuantity]=record;
            recordsQuantity++;
        }
    }
    return {true,""};
}
OperationResult appendRecord(const Game &game,const string &name){
    if(name.empty() || name.length()>15){
        return {false,"Nome inválido para o recorde"};
    }
    int nameLength=name.length();
    for(int i=0;i<nameLength;i++){
        if(name[i]==','){
            return {false,"O nome do recorde não pode conter vírgulas"};
        }
    }
    ofstream file("Recordes.csv",ios::app);
    if(!file.is_open()){
        return {false,"Não foi possível abrir Recordes.csv"};
    }
    file<<name<<","<<game.currentMap.numMap<<","<<game.contkeys<<","<<game.contRotates<<endl;
    file.close();
    if(file.fail()){
        return {false,"Não foi possível gravar o recorde"};
    }
    return {true,"Recorde registrado"};
}
void sortRecordsForMap(Recorde records[],int &recordsQuantity,int mapNumber){
    Recorde mapRecords[200];
    int mapRecordsQuantity=0;
    for(int i=0;i<recordsQuantity;i++){
        if(records[i].mapa==mapNumber){
            mapRecords[mapRecordsQuantity]=records[i];
            mapRecordsQuantity++;
        }
    }
    if(mapRecordsQuantity==0){
        recordsQuantity=0;
        return;
    }
    for(int i=0;i<mapRecordsQuantity-1;i++){
        for(int j=0;j<mapRecordsQuantity-i-1;j++){
            if(mapRecords[j].movimentos>mapRecords[j+1].movimentos ||
               (mapRecords[j].movimentos==mapRecords[j+1].movimentos &&
                mapRecords[j].rotacoes>mapRecords[j+1].rotacoes)){
                trocar(mapRecords[j],mapRecords[j+1]);
            }
        }
    }
    recordsQuantity=mapRecordsQuantity;
    for(int i=0;i<recordsQuantity;i++){
        records[i]=mapRecords[i];
    }
}
void displayRecords(const Recorde records[],int recordsQuantity,int mapNumber){
    if(recordsQuantity==0){
        cout<<"Ainda não há recordes para o mapa "<<mapNumber<<"."<<endl;
        return;
    }
    cout<<"Recordes do mapa "<<mapNumber<<":"<<endl;
    for(int i=0;i<recordsQuantity && i<5;i++){
        cout<<i+1<<". "<<records[i].nome<<" - "
            <<records[i].movimentos<<" movimentos, "
            <<records[i].rotacoes<<" rotações"<<endl;
    }
}

void listMaps(int validMaps[],int &validQuantity){
    validQuantity=0;
    for(int mapNumber=1;mapNumber<=100;mapNumber++){
        ifstream mapFile(mapFileName(mapNumber));
        if(!mapFile.is_open()){
            break;
        }
        Game mapGame={};
        Game gameCopy={};
        mapGame.currentMap.numMap=mapNumber;
        OperationResult result=readMap(mapGame.currentMap,gameCopy);
        if(result.success){
            cout<<mapNumber<<" - "<<mapGame.currentMap.MapName<<endl;
            validMaps[validQuantity]=mapNumber;
            validQuantity++;
        }else{
            cout<<mapNumber<<" - indisponível: "<<result.reason<<endl;
        }
    }
}
void playGame(Game &game){
    game.animate=true;
    game.voltarMenu=false;
    while(!game.end){
        system("clear");
        mapGenerate(game);
        if(game.end){
            if(game.last){
                string playerName;
                cout<<"Nome do jogador (até 15 caracteres): ";
                getline(cin,playerName);
                if(playerName.empty()){
                    playerName="Jogador";
                }
                if(playerName.length()>15){
                    playerName=playerName.substr(0,15);
                }
                int playerNameLength=playerName.length();
                for(int i=0;i<playerNameLength;i++){
                    if(playerName[i]==','){
                        playerName[i]=' ';
                    }
                }
                OperationResult recordResult=appendRecord(game,playerName);
                cout<<recordResult.reason<<endl;
                remove("jogo_salvo.csv");
                game.jogoSalvo=false;
            }
            break;
        }
        game.keyB=getch();
        keys(game,game.keyB);
    }
    game.animate=false;
}
void menu(Game &game,Menu &menuState,Game &gameCopy){
    Game savedGame={};
    OperationResult savedGameResult=loadGame(savedGame);
    game.jogoSalvo=savedGameResult.success;
    
    while(menuState.options!=6){
        cout<<"\n1-Sobre 2-Novo jogo ";
        if(game.jogoSalvo){
            cout<<"3-Continuar ";
        }
        else{
            cout<<"3-Continuar (indisponível) ";
        }
        cout<<"4-Ver soluções 5-Recordes 6-Fim\n";
        cout<<"Escolha: ";
        if(!(cin>>menuState.options)){
            cin.clear();
            cin.ignore(10000,'\n');
            cout<<"Opção inválida."<<endl;
            continue;
        }
        
        if(menuState.options==1){
            about();
            getch();
        }
        else if(menuState.options==2){
            int validMaps[100];
            int validQuantity=0;
            listMaps(validMaps,validQuantity);
            if(validQuantity==0){
                cout<<"Nenhum mapa válido foi encontrado."<<endl;
                continue;
            }
            int selectedMap;
            cout<<"Digite o número do mapa ou 0 para escolher um aleatório: ";
            cin>>selectedMap;
            if(selectedMap==0){
                selectedMap=validMaps[rand()%validQuantity];
            }
            bool selectedMapIsValid=false;
            for(int i=0;i<validQuantity;i++){
                if(validMaps[i]==selectedMap){
                    selectedMapIsValid=true;
                }
            }
            if(!selectedMapIsValid){
                cout<<"Mapa inválido ou indisponível."<<endl;
                continue;
            }
            if(game.jogoSalvo){
                cout<<"Há um jogo salvo. Digite 1 para cancelar ou 2 para substituí-lo: ";
                cin>>game.escolha;
                if(game.escolha!=2){
                    continue;
                }
                remove("jogo_salvo.csv");
                game.jogoSalvo=false;
            }
            game.mapOrigin.numMap=selectedMap;
            OperationResult result=selection(game,gameCopy);
            if(!result.success){
                cout<<result.reason<<endl;
                continue;
            }
            game.currentGame=true;
            game.end=false;
            playGame(game);
        }
        else if(menuState.options==3){
            OperationResult result=loadGame(game);
            if(!result.success){
                game.jogoSalvo=false;
                cout<<"Continuar indisponível: "<<result.reason<<endl;
                continue;
            }
            game.jogoSalvo=true;
            playGame(game);
        }
        else if(menuState.options==4){
            int mapNumber;
            cout<<"Digite o número do mapa ou 0 para verificar todos: ";
            cin>>mapNumber;
            if(mapNumber==0){
                verifyAllSolutions();
                continue;
            }
            game.currentMap.numMap=mapNumber;
            OperationResult result=mapSolution(game);
            cout<<result.reason<<endl;
            game.end=true;
        }
        else if(menuState.options==5){
            int validMaps[100];
            int validQuantity=0;
            listMaps(validMaps,validQuantity);
            int mapNumber;
            cout<<"Digite o número do mapa para ver os recordes: ";
            cin>>mapNumber;
            Recorde records[200];
            int recordsQuantity=0;
            loadRecords(records,recordsQuantity);
            sortRecordsForMap(records,recordsQuantity,mapNumber);
            displayRecords(records,recordsQuantity,mapNumber);
        }else if(menuState.options!=6){
            cout<<"Opção inválida."<<endl;
        }
    }
}
int getch(void) {
    int ch;
    struct termios oldt, newt;
    bool terminal=tcgetattr(STDIN_FILENO,&oldt)==0;
    if(terminal){
        newt=oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO,TCSANOW,&newt);
    }
    ch=cin.get();
    while(ch=='\n' || ch=='\r'){
        ch=cin.get();
    }
    if(terminal){
        tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
    }
    return ch;
}

int main() {
    printf("\033[?25l");
    srand(time(0));
    Game game = {};
    Menu menuState = {};
    Game gameCopy = {};
    game.end = true;

    menu(game,menuState,gameCopy);
    return 0;
}
//Passagem por valor nao precisa de backup das mapes OKK
//Procurar alguma forma de não fazer 3 if para cada mapa OKKK
//Adicionar as alavancas no mapgenerate
//ENTENDER COMPLEXIDADE DA ROTAÇAO
//LOGICA DE keysMENTAÇÃO
//CHAMAR TUDO NA FUNÇÃO
//ORIENTAÇÕES NO JOGO

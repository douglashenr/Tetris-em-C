#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include <locale.h>
#include <conio.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string.h>

#define LARGURA 360
#define ALTURA 690



SDL_Texture * quadrante1;
SDL_Window * SWindow = NULL;
SDL_Renderer * Render = NULL;
Mix_Chunk * musicafundo_abertura = NULL;
Mix_Chunk * Musicafundo_game = NULL;
SDL_Event evento;
TTF_Font * text_font;
SDL_Texture * textTexto;
int channel;
int channel2;

int iniciar_fonte(){
    if (TTF_Init() < 0){
        printf("Erro ao inicializar o sistema de fontes TTF\n");
    }
    text_font = TTF_OpenFont("Letras.ttf", 4);
    if (text_font == NULL){
        printf("Erro ao carregar a font letras.ttf\n");
    }
}

int score = 0;

int escreve(){
    SDL_Color font_color  = {0, 0, 0};
    char texto[255];
	sprintf(texto, "%d", score);
	SDL_Surface * surText =  TTF_RenderText_Solid(text_font,texto,font_color);
	textTexto = SDL_CreateTextureFromSurface( Render, surText );
}

int printa_score(){
    escreve();
    iniciar_fonte();
    pinta_imagem(textTexto, 0, 0, 127, 615, 127, 615, 100, 29);
    SDL_RenderPresent(Render);
}

int console_portugues(){
    setlocale(LC_ALL, "Portuguese");
}

int audio_abertura(){
    int flags = MIX_INIT_OGG|MIX_INIT_MOD;
    int initted=Mix_Init(flags);
    printf(" Initted , %d ", initted );

    if( initted&flags != flags) {
        printf("Mix_Init: Failed to init required ogg and mod support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());
        return -1;
    }

    if(SDL_Init(SDL_INIT_AUDIO)==-1) {
        printf("SDL_Init: %s\n", SDL_GetError());
        return -1;
    }

    if( Mix_OpenAudio( 22050,AUDIO_U16, 2, 8192 ) == -1 ) {
        printf("Erro ao inicializar o sistema de som \n");
        return -1;
    }

    musicafundo_abertura = Mix_LoadWAV("Muse.wav");

    if( musicafundo_abertura == NULL ) {
        printf("Erro ao carregar a musica ... \n");
        return -1;
    }

    Musicafundo_game = Mix_LoadWAV("Tetristheme.ogg");

    if (Musicafundo_game == NULL){
        printf("Erro ao carregar a musica do game ... \n");
        return -1;
    }
}

int inicializa_video(){
    if (SDL_Init(0) == 0){
        SWindow = SDL_CreateWindow("TETRIS",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,LARGURA, ALTURA,
                                  SDL_WINDOW_SHOWN);
        if (SWindow != NULL){
            Render = SDL_CreateRenderer(SWindow, -1, 0);
            if (Render != NULL) {
                IMG_Init(IMG_INIT_PNG);
                return 1;
            }
        }
    }
    return 0;
}

SDL_Texture* carrega_imagem(char file_name[]){
    SDL_Surface * imagem = IMG_Load(file_name);
    SDL_Texture * tex = SDL_CreateTextureFromSurface(Render,imagem);
}

int pinta_imagem(SDL_Texture * tex,
                 int xs, int ys, int ws, int hs,
                 int xt, int yt, int wt, int ht){
    SDL_Rect rOrig = {xs, ys, ws, hs};
    SDL_Rect rDest = {xt, yt, wt, ht};
    SDL_Point ponto = {xt + (wt/2), yt + (ht / 2)};
    SDL_RenderCopyEx(Render, tex, &rOrig, &rDest, 0, &ponto, 0);
}

int sair_do_jogo(){
    SDL_DestroyWindow(SWindow);
    exit(0);
}

int menu_regras(){
    SDL_RenderClear(Render);
    SDL_Texture * Fundo_regras = carrega_imagem("./Regras.png");
    pinta_imagem(Fundo_regras, 0, 0, 360, 690,
                            0, 0, LARGURA, ALTURA);
    SDL_RenderPresent(Render);
    SDL_Delay(1000);
}

int menu_creditos(){
    SDL_RenderClear(Render);
    SDL_Texture * Fundo_creditos = carrega_imagem("./Creditos.png");
    pinta_imagem(Fundo_creditos, 0, 0, 360, 690,
                            0, 0, LARGURA, ALTURA);
    SDL_RenderPresent(Render);
    SDL_Delay(1000);
}

int menu(){
    Mix_Pause(channel2);
    Mix_Resume(channel);


    printf("No Menu \n");
    SDL_RenderClear(Render);
    SDL_Texture * MENU = carrega_imagem("./menu.png");
    pinta_imagem(MENU, 0, 0, 360, 690,
                            0, 0, LARGURA, ALTURA);
    SDL_RenderPresent(Render);
    int apertarmenu = 1;
    while (apertarmenu==1){
        while(SDL_PollEvent(&evento) !=0){
        if(evento.type == SDL_QUIT){
                apertarmenu = 0;
        }
        else if(evento.type == SDL_KEYDOWN){
            switch(evento.key.keysym.sym){
                printf("TECLA DETECTADA menu, pode fazer sua escolha \n");
                case SDLK_1: no_game();

                case SDLK_2: menu_regras();

                int regrasvoltar = 1;
                while (regrasvoltar == 1){
                    while(SDL_PollEvent(&evento) !=0){
                        if(evento.type == SDL_QUIT){
                        regrasvoltar = 0;
                        }
                    else if(evento.type == SDL_KEYDOWN){
                        if(evento.key.keysym.sym == SDLK_SPACE){
                        printf("TECLA DETECTADA \n");
                        menu();
                        break;
                        }
                    }
                    }
                continue;
                break;
                }
                case SDLK_3:
                case SDLK_4: menu_creditos();
                    printf("DETECTADO ESCOLHA: CREDITOS \n");

                int creditosvoltar = 1;
                    while (creditosvoltar == 1){
                        while(SDL_PollEvent(&evento) !=0){
                            if(evento.type == SDL_QUIT){
                            creditosvoltar = 0;
                            }
                        else if(evento.type == SDL_KEYDOWN){
                            if(evento.key.keysym.sym == SDLK_SPACE){
                            printf("TECLA DETECTADA \n");
                            menu();
                            break;
                            }
                        }
                        }
                    continue;
                    break;
                    }

                case SDLK_5:
                    sair_do_jogo();
                    break;
                }
            }
        }
        continue;
        break;
    }
}

int x = 150;
int y = 0;
int x2 = 180;
int y2 = 30;
int x21 , x22;

int peca_quadrado2(){

    SDL_Surface * imagem = IMG_Load("./pec2.png");
    SDL_Texture * peca_vermelha = SDL_CreateTextureFromSurface(
                                        Render, imagem);
    pinta_imagem(peca_vermelha, 0, 0, 30, 30,
                            x, y, 30, 30);
    SDL_RenderPresent(Render);


    SDL_Texture * peca_vermelha2 = carrega_imagem("./pec2.png");
    pinta_imagem(peca_vermelha2, 0, 0, 30, 30,
                            x2, y, 30, 30);
    SDL_RenderPresent(Render);

    SDL_Texture * peca_vermelha3 = carrega_imagem("./pec2.png");
    pinta_imagem(peca_vermelha3, 0, 0, 30, 30,
                            x, y2, 30, 30);
    SDL_RenderPresent(Render);

    SDL_Texture * peca_vermelha4 = carrega_imagem("./pec2.png");
    pinta_imagem(peca_vermelha4, 0, 0, 30, 30,
                            x2, y2, 30, 30);
    SDL_RenderPresent(Render);
}

int raio(){


    SDL_Texture * peca_azul1 = carrega_imagem("./pec1.png");
    pinta_imagem(peca_azul1, 0, 0, 30, 30,
                            x, y, 30, 30);
    SDL_RenderPresent(Render);


    SDL_Texture * peca_azul2 = carrega_imagem("./pec1.png");
    pinta_imagem(peca_azul2, 0, 0, 30, 30,
                            x2, y, 30, 30);
    SDL_RenderPresent(Render);


    SDL_Texture * peca_azul3 = carrega_imagem("./pec1.png");
    pinta_imagem(peca_azul3, 0, 0, 30, 30,
                            x2, y2, 30, 30);
    SDL_RenderPresent(Render);


    SDL_Texture * peca_azul4 = carrega_imagem("./pec1.png");
    pinta_imagem(peca_azul4, 0, 0, 30, 30,
                            x21, y2, 30, 30);
    SDL_RenderPresent(Render);
}

int regua(){

    SDL_Texture * peca_laranja = carrega_imagem("./pec4.png");
    pinta_imagem(peca_laranja, 0, 0, 30, 30,
                            x, y2, 30, 30);
    SDL_RenderPresent(Render);

    SDL_Texture * peca_laranja2 = carrega_imagem("./pec4.png");
    pinta_imagem(peca_laranja2, 0, 0, 30, 30,
                            x2, y2, 30, 30);
    SDL_RenderPresent(Render);

    SDL_Texture * peca_laranja3 = carrega_imagem("./pec4.png");
    pinta_imagem(peca_laranja3, 0, 0, 30, 30,
                            x21, y2, 30, 30);
    SDL_RenderPresent(Render);

    SDL_Texture * peca_laranja4 = carrega_imagem("./pec4.png");
    pinta_imagem(peca_laranja4, 0, 0, 30, 30,
                            x22, y2, 30, 30);
    SDL_RenderPresent(Render);
}


int calculador[20][10] = {{0,0,0,1,1,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0},
                          {0,0,0,0,0,0,0,0,0,0}};

int y3;

int l_invertido(){
    SDL_Texture * peca_roxa = carrega_imagem("./pec6.png");
    pinta_imagem(peca_roxa, 0, 0, 30, 30,
                            x, y2, 30, 30);
    SDL_RenderPresent(Render);

    SDL_Texture * peca_roxa2 = carrega_imagem("./pec6.png");
    pinta_imagem(peca_roxa2, 0, 0, 30, 30,
                            x2, y2, 30, 30);
    SDL_RenderPresent(Render);

    SDL_Texture * peca_roxa3 = carrega_imagem("./pec6.png");
    pinta_imagem(peca_roxa3, 0, 0, 30, 30,
                            x2, y, 30, 30);
    SDL_RenderPresent(Render);

    SDL_Texture * peca_roxa4 = carrega_imagem("./pec6.png");
    pinta_imagem(peca_roxa4, 0, 0, 30, 30,
                            x2, y3, 30, 30);
    SDL_RenderPresent(Render);
}

int l_invertidocima(){
    SDL_Texture * peca_roxa = carrega_imagem("./pec6.png");
    pinta_imagem(peca_roxa, 0, 0, 30, 30,
                            x, y2, 30, 30);
    SDL_RenderPresent(Render);

    SDL_Texture * peca_roxa2 = carrega_imagem("./pec6.png");
    pinta_imagem(peca_roxa2, 0, 0, 30, 30,
                            x, y3, 30, 30);
    SDL_RenderPresent(Render);

    SDL_Texture * peca_roxa3 = carrega_imagem("./pec6.png");
    pinta_imagem(peca_roxa3, 0, 0, 30, 30,
                            x, y, 30, 30);
    SDL_RenderPresent(Render);

    SDL_Texture * peca_roxa4 = carrega_imagem("./pec6.png");
    pinta_imagem(peca_roxa4, 0, 0, 30, 30,
                            x2, y, 30, 30);
    SDL_RenderPresent(Render);
}


int calcular1(){
    int posy=30;
    int posx=30;
    int b,a;

    for(a=0;a<20;a++){
        for(b=0; b<10;b++){

            if(calculador[a][b] == 4){
                SDL_Texture * peca_vermelha4 = carrega_imagem("./pec1.png");
                pinta_imagem(peca_vermelha4, 0, 0, 30, 30,
                            ((b+1)*posx), a*posy, 30, 30);
                SDL_RenderPresent(Render);
            }
            if(calculador[a][b] == 1){
                SDL_Texture * peca_vermelha4 = carrega_imagem("./pec2.png");
                pinta_imagem(peca_vermelha4, 0, 0, 30, 30,
                            ((b+1)*posx), a*posy, 30, 30);
                SDL_RenderPresent(Render);
            }
            if(calculador[a][b] == 3){
                SDL_Texture * peca_vermelha4 = carrega_imagem("./pec3.png");
                pinta_imagem(peca_vermelha4, 0, 0, 30, 30,
                            ((b+1)*posx), a*posy, 30, 30);
            SDL_RenderPresent(Render);
            }
            if(calculador[a][b] == 2){
                SDL_Texture * peca_vermelha4 = carrega_imagem("./pec4.png");
                pinta_imagem(peca_vermelha4, 0, 0, 30, 30,
                            ((b+1)*posx), a*posy, 30, 30);
                SDL_RenderPresent(Render);
            }
            if(calculador[a][b] == 5){
                SDL_Texture * peca_vermelha4 = carrega_imagem("./pec5.png");
                pinta_imagem(peca_vermelha4, 0, 0, 30, 30,
                            ((b+1)*posx), a*posy, 30, 30);
                SDL_RenderPresent(Render);
            }
            if(calculador[a][b] == 6)     {
                SDL_Texture * peca_vermelha4 = carrega_imagem("./pec6.png");
                pinta_imagem(peca_vermelha4, 0, 0, 30, 30,
                            ((b+1)*posx), a*posy, 30, 30);
                SDL_RenderPresent(Render);
            }
        }
    }
}

int mvbloqueado = 0;


int calculador_de_impacto(){
    int b,a;
    int parar = 0;
    int xt = x, xt2= x2, xt21 = x21, xt22 = x22;
    for(a=19;a>=0;a--){
        b = 0;
        if(parar == 1){
            break;
        }
        for(b=0; b<10;b++){
            if(calculador[a][b] != 0){
                if(xt == (b+1)*30 || xt2 == (b+1)*30 || xt21 == (b+1)*30 || xt22 == (b+1)*30){
                    if(y2 == (a-1)*30){
                        mvbloqueado = 1;
                        parar = 1;
                        break;
                    }
                    else{
                        continue;
                    }
                }
            }
        }
    }
}






int quadrado(){
    int parar = 1;
    while(parar == 1){

        SDL_RenderClear(Render);
        SDL_Texture * jogo = carrega_imagem("./jogo.png");

        pinta_imagem(jogo, 0, 0, 360, 690,
                            0, 0, LARGURA, ALTURA);

        printa_score();
        calcular1();
        peca_quadrado2();

        SDL_RenderPresent(Render);
        SDL_Delay(1000);

        if(y2< 570){

            calculador_de_impacto();

            if(mvbloqueado == 1){
                break;
            }
            int mv = 1;
            while (mv==1){
                while(SDL_PollEvent(&evento) !=0){
                    if(evento.type == SDL_QUIT){
                        mv = 0;
                    }
                    else if(evento.type == SDL_KEYDOWN){
                        if(evento.key.keysym.sym == SDLK_RIGHT){
                            x = x+30;
                            x2 = x2 +30;
                            if(x >  270){
                                x = x-30;
                                x2 = x2-30;
                            }
                        break;
                        }
                        if(evento.key.keysym.sym == SDLK_LEFT){
                            x = x-30;
                            x2 = x2 -30;
                            if(x <  30){
                                x = x+30;
                                x2 = x2+30;
                            }
                        break;
                        }
                        if(evento.key.keysym.sym == SDLK_DOWN){
                            y = y+30;
                            y2 = y2 +30;
                            if(y2 >  570){
                                y = y-30;
                                y2 = y2-30;
                            }
                        break;
                        }
                    }
                }
            break;
            }
            x = x+0;
            y = y+30;
            x2 = x2+0;
            y2 = y2+30;
        }
        else{
            break;
        }
    }
}




int printregua(){
    int parar = 1;
    while(parar == 1){

        SDL_RenderClear(Render);
        SDL_Texture * jogo = carrega_imagem("./jogo.png");

        pinta_imagem(jogo, 0, 0, 360, 690,
                            0, 0, LARGURA, ALTURA);

        printa_score();
        calcular1();
        regua();

        SDL_RenderPresent(Render);
        SDL_Delay(1000);

        if(y2< 570){
            calculador_de_impacto();
                if(mvbloqueado == 1){
                    break;
                }
            int mv = 1;
            while (mv==1){
                while(SDL_PollEvent(&evento) !=0){
                    if(evento.type == SDL_QUIT){
                        mv = 0;
                    }
                else if(evento.type == SDL_KEYDOWN){
                    if(evento.key.keysym.sym == SDLK_RIGHT){
                        x = x+30;
                        x2 = x2 +30;
                        x21 = x21 + 30;
                        x22 = x22+30;
                        if(x21 >  270){
                            x = x-30;
                            x2 = x2-30;
                            x21 = x21-30;
                            x22 = x22-30;
                        }
                    break;
                    }
                    if(evento.key.keysym.sym == SDLK_LEFT){
                        x = x-30;
                        x2 = x2 -30;
                        x21 = x21-30;
                        x22 = x22-30;
                        if(x <  30){
                            x = x+30;
                            x2 = x2+30;
                            x21 = x21+30;
                            x22 = x22+30;
                        }
                    break;
                    }
                    if(evento.key.keysym.sym == SDLK_DOWN){
                        y2 = y2+30;
                        if(y2 >  570){
                            y2 = y2-30;
                        }
                    break;
                    }
                }
            }
        break;
        }
        y2 = y2+30;
        }
    else{
        break;
    }
    }
}


int print_raio(){
    int parar = 1;
    while(parar == 1){

        SDL_RenderClear(Render);
        SDL_Texture * jogo = carrega_imagem("./jogo.png");

        pinta_imagem(jogo, 0, 0, 360, 690,
                            0, 0, LARGURA, ALTURA);

        printa_score();
        calcular1();
        raio();

        SDL_RenderPresent(Render);
        SDL_Delay(1000);

        if(y2< 570){
            calculador_de_impacto();
                if(mvbloqueado == 1){
                    mvbloqueado = 0;
                    break;
                }
            int mv = 1;
            while (mv==1){
                while(SDL_PollEvent(&evento) !=0){
                    if(evento.type == SDL_QUIT){
                        mv = 0;
                    }
                    else if(evento.type == SDL_KEYDOWN){
                        if(evento.key.keysym.sym == SDLK_RIGHT){
                            x = x+30;
                            x2 = x2 +30;
                            x21 = x21 +30;
                            if(x21 >  300){
                                x = x-30;
                                x2 = x2-30;
                                x21 = x21-30;
                            }
                        break;
                        }
                        if(evento.key.keysym.sym == SDLK_LEFT){
                            x = x-30;
                            x2 = x2 -30;
                            x21 = x21 - 30;
                            if(x <  30){
                                x = x+30;
                                x2 = x2+30;
                                x21 = x21 + 30;
                            }
                        break;
                        }
                        if(evento.key.keysym.sym == SDLK_DOWN){
                            y2 = y2+30;
                            y = y+30;
                            if(y2 >  570){
                                y2 = y2-30;
                                y = y-30;
                            }
                        break;
                        }
                    }
                }
            break;
            }
            y2 = y2+30;
            y = y+30;
            printf("X=%d  X2=%d X21=%d Y=%d Y2=%d \n", x, x2, x21, y, y2);
        }
        else{
            break;
        }
    }
}


int printl_invertido(){
    int parar = 1;
    while(parar == 1){
        SDL_RenderClear(Render);
        SDL_Texture * jogo = carrega_imagem("./jogo.png");
        pinta_imagem(jogo, 0, 0, 360, 690,
                            0, 0, LARGURA, ALTURA);

        printa_score();
        calcular1();
        l_invertido();

        SDL_RenderPresent(Render);
        SDL_Delay(1000);

        if(y2< 570){
            calculador_de_impacto();
            if(mvbloqueado == 1){
                mvbloqueado = 0;
                break;
            }
            int mv = 1;
            while (mv==1){
                while(SDL_PollEvent(&evento) !=0){
                    if(evento.type == SDL_QUIT){
                        mv = 0;
                    }
                    else if(evento.type == SDL_KEYDOWN){
                        if(evento.key.keysym.sym == SDLK_RIGHT){
                            x = x+30;
                            x2 = x2 +30;

                            if(x2 >  300){
                                x = x-30;
                                x2 = x2-30;
                            }
                        break;
                        }
                        if(evento.key.keysym.sym == SDLK_LEFT){
                            x = x-30;
                            x2 = x2 -30;
                            if(x <  30){
                                x = x+30;
                                x2 = x2+30;
                            }
                        break;
                        }
                        if(evento.key.keysym.sym == SDLK_DOWN){
                            y2 = y2+30;
                            y = y+30;
                            y3 = y3+30;
                            if(y2 >  570){
                                y2 = y2-30;
                                y = y-30;
                                y3 = y3-30;
                            }
                        break;
                        }
                    }
                }
            break;
            }
        y2 = y2+30;
        y = y+30;
        y3 = y3+30;
        printf("X=%d  X2=%d Y=%d Y2=%d Y3=%d \n", x, x2, y, y2, y3);
    }
        else{
            break;
        }
    }
}




int printl_invertidoacima(){
    int parar = 1;
    while(parar == 1){
        SDL_RenderClear(Render);
        SDL_Texture * jogo = carrega_imagem("./jogo.png");
        pinta_imagem(jogo, 0, 0, 360, 690,
                            0, 0, LARGURA, ALTURA);

        printa_score();
        calcular1();
        l_invertidocima();

        SDL_RenderPresent(Render);
        SDL_Delay(1000);

        if(y2< 570){
            calculador_de_impacto();
            if(mvbloqueado == 1){
                mvbloqueado = 0;
                break;
            }
            int mv = 1;
            while (mv==1){
                while(SDL_PollEvent(&evento) !=0){
                    if(evento.type == SDL_QUIT){
                    mv = 0;
                    }
                    else if(evento.type == SDL_KEYDOWN){
                        if(evento.key.keysym.sym == SDLK_RIGHT){
                            x = x+30;
                            x2 = x2 +30;

                            if(x2 >  300){
                                x = x-30;
                                x2 = x2-30;
                            }
                        break;
                        }
                        if(evento.key.keysym.sym == SDLK_LEFT){
                            x = x-30;
                            x2 = x2 -30;

                            if(x <  30){
                                x = x+30;
                                x2 = x2+30;
                            }
                        break;
                        }

                        if(evento.key.keysym.sym == SDLK_DOWN){
                            y2 = y2+30;
                            y = y+30;
                            y3 = y3+30;
                            if(y2 >  570){
                                y2 = y2-30;
                                y = y-30;
                                y3 = y3-30;
                            }
                        break;
                        }
                    }
                }
            break;
            }
        y2 = y2+30;
        y = y+30;
        y3 = y3+30;

                 printf("X=%d  X2=%d Y=%d Y2=%d, Y3=%d \n", x, x2, x2, y2);

        }
        else{
            break;
        }
    }
}



int addquadrado(){
    int count1, count2;
    int x3 = x,x4 = x2,y3 = y,y4 =y2;
    int calculando = 30;
    int calculando2 = 60;
    int calculando3 = 0;
    int calculando4 = 30;
    int impedir = 0;
    for(count1=0;count1<19;count1++){
        if(impedir == 1){
            break;
        }
        if(y3 == calculando3 && y4 == calculando4){
            for(count2=0;count2<9;count2++){
                if(x3 == calculando && x4 == calculando2){
                    calculador[count1][count2] = 1;
                    calculador[count1][count2 + 1] = 1;
                    calculador[count1 + 1][count2] = 1;
                    calculador[count1 + 1][ count2 +1] = 1;
                    impedir = 1;
                    break;
                }
                else{
                    calculando= calculando+30;
                    calculando2 = calculando2+30;
                }
            }
        }

        else{
            calculando4 = calculando4+30;
            calculando3 = calculando3+30;
        }
    }
}

int addregua(){
    int count1 ,count2 ;
    int calculando1 = 0;
    int calculando2 = 30;
    int impedir = 0;
    printf(" \n ADD REGUA CARREGOU \n");
    for(count1=0; count1<20; count1++){
        if (impedir == 1){
            break;
        }
        if(y2 == calculando1){
            for(count2=0; count2<9; count2++){
                if(x == calculando2){
                    calculador[count1][count2] = 2;
                    calculador[count1][count2+1] = 2;
                    calculador[count1][count2+2] = 2;
                    calculador[count1][count2+3] = 2;
                    impedir = 1;
                    break;
                }
                else{
                    calculando2 = calculando2 +30;
                }
            }
        }
    calculando1 = calculando1 +30;
    }
}

int addl_invertido(){
    x21 = 0;
    x22 = 0;
    int count1, count2;
    int calculandox1 = 30;
    int calculandox2 = 60;
    int calculandoy = 30;
    int calculandoy2 = 60;
    int calculandoy3 = 0;
    int impedir = 0;
    for(count1=0;count1<19;count1++){
        if(impedir == 1){
            break;
        }
        if(y == calculandoy && y2 == calculandoy2 && y3 == calculandoy3 ){
            for(count2=0;count2<9;count2++){
                if(x == calculandox1 && x2 == calculandox2){
                    calculador[count1][count2 +1] = 6;
                    calculador[count1 + 1][count2 + 1] = 6;
                    calculador[count1 + 2][count2] = 6;
                    calculador[count1 + 2][ count2 +1] = 6;
                    impedir = 1;
                    break;
                }
            else{
                calculandox1 = calculandox1+30;
                calculandox2 = calculandox2+30;
            }
            }
        }

    else{
        calculandoy = calculandoy+30;
        calculandoy2 = calculandoy2+30;
        calculandoy3 = calculandoy3+30;
    }
    }
}

int addl_invertidocima(){
    x21 = 0;
    x22 = 0;
    int count1, count2;
    int calculandox1 = 30;
    int calculandox2 = 60;
    int calculandoy = 0;
    int calculandoy2 = 60;
    int calculandoy3 = 30;
    int impedir = 0;
    for(count1=0;count1<19;count1++){
        if(impedir == 1){
            break;
        }
        if(y == calculandoy && y2 == calculandoy2 && y3 == calculandoy3 ){
            for(count2=0;count2<9;count2++){
                if(x == calculandox1 && x2 == calculandox2){
                    calculador[count1][count2 +1] = 6;
                    calculador[count1][count2] = 6;
                    calculador[count1+1][count2] = 6;
                    calculador[count1+2][ count2] = 6;
                    impedir = 1;
                    break;
                }
            else{
                calculandox1 = calculandox1+30;
                calculandox2 = calculandox2+30;
            }
            }
        }

    else{
        calculandoy = calculandoy+30;
        calculandoy2 = calculandoy2+30;
        calculandoy3 = calculandoy3+30;
    }
    }
}


int add_raio(){
    x22 = 0;
    y3 = 0;
    int count1, count2;
    int calculandox1 = 30;
    int calculandox2 = 60;
    int calculandox21 = 90;
    int calculandoy = 0;
    int calculandoy2 = 30;
    int impedir = 0;
    for(count1=0;count1<19;count1++){
        if(impedir == 1){
            break;
        }
        if(y == calculandoy && y2 == calculandoy2 ){
            for(count2=0;count2<9;count2++){
                if(x == calculandox1 && x2 == calculandox2 && x21 == calculandox21){
                    calculador[count1][count2] = 4;
                    calculador[count1][count2 +1] = 4;
                    calculador[count1+1][count2+1] = 4;
                    calculador[count1+1][ count2+2] = 4;
                    impedir = 1;
                    break;
                }
            else{
                calculandox1 = calculandox1+30;
                calculandox2 = calculandox2+30;
                calculandox21 = calculandox21+30;
            }
            }
        }

    else{
        calculandoy = calculandoy+30;
        calculandoy2 = calculandoy2+30;
    }
    }
}




int eliminador_de_linha (){
    int count1 = 0, count2 = 0;
    int passar;
    int n;
    printf("\n %d,%d,%d,%d,%d,%d,%d,%d,%d,%d \n",
           calculador[19][0],
           calculador[19][1],
           calculador[19][2],
           calculador[19][3],
           calculador[19][4],
           calculador[19][5],
           calculador[19][6],
           calculador[19][7],
           calculador[19][8],
           calculador[19][9]);


    for(n=19;n>0;n--){
        if (calculador[n][0] != 0 &&
            calculador[n][1] != 0 &&
            calculador[n][2] != 0 &&
            calculador[n][3] != 0 &&
            calculador[n][4] != 0 &&
            calculador[n][5] != 0 &&
            calculador[n][6] != 0 &&
            calculador[n][7] != 0 &&
            calculador[n][8] != 0 &&
            calculador[n][9] != 0){
                printf("entrou \n");
                score = score +400;
                for(count1=n;count1>0;count1--){
                    for(count2=0;count2<10;count2++){
                        passar = calculador[count1-1][count2];
                        calculador[count1][count2] = 0;
                        calculador[count1][count2] = passar;
                        calculador[count1-1][count2] = 0;
                    }
                }
        count2=0;
        for(count1=0;count1<9;count1++){
            calculador[count2][count1] = 0;
        }
        }
    }
}

int over = 0;
int game_over(){
    if(calculador[0][4] != 0 ||
       calculador[0][5] != 0 ||
       calculador[1][4] != 0 ||
       calculador[1][5] != 0){
        SDL_Texture * gameover = carrega_imagem("./GameOver.png");
        pinta_imagem(gameover, 0, 0, 240, 60,
                            60, 150, 240, 60);
        SDL_RenderPresent(Render);
        over = 1;
        SDL_Delay(10000);
        menu();
    }
}




int no_game(){
    Mix_Pause(channel);
    channel2 = Mix_PlayChannel(0,Musicafundo_game,-1);
    Mix_Volume(channel2, 30);

    printf("continua no jogo \n");
    int count1, hh  ohd9uWGIYN  IECYDQUbejhcount2qgheojx7qwwtbxhuyiqhe8hwet87ex6hh1t86xt    q7rx    qhf48hqw48rtrgw37yjj4i284w  qyfeuwguyGAsusawcuyydgweyudfAYTFSYaczfg dqhsg12uwdfqwefdhqwef;
    for(count1=0;count1<19;count1++){
        for(count2=0;count2<9;count2++){
            calculador[count1][count2] = 0;
        }
    }



    int c;
    while(1){
        for(c=0;c<100000000;c=c+4){
            int t = time(NULL);
            srand(t);
            int numero_aleatorio = rand() %5;


            switch(numero_aleatorio){
                case 1 :
                    score = score + 40;
                    x21 = 0;
                    x22 = 0;
                    y3 = 0;
                    x = 150;
                    y = 0;
                    x2 = 180;
                    y2 = 30;
                    quadrado();
                    addquadrado();
                    eliminador_de_linha();
                    game_over();
                    if(over == 1){
                        break;
                    }
                    mvbloqueado = 0;

                case 2:
                    score = score + 40;
                    y = 0;
                    y3 = 0;
                    x = 150;
                    y2 = 0;
                    x2 = 180;
                    x21 = 210;
                    x22 = 240;
                    printregua();
                    addregua();
                    eliminador_de_linha();
                    game_over();
                    if(over == 1){
                        break;
                    }
                    mvbloqueado = 0;

                case 3:
                    x21 = 0;
                    x22 = 0;
                    score = score +40;
                    x = 150;
                    x2 = 180;
                    y2 = 60;
                    y3 = 0;
                    y = 30;
                    printl_invertido();
                    addl_invertido();
                    eliminador_de_linha();
                    game_over();
                    if(over == 1){
                        break;
                    }
                    mvbloqueado = 0;

                case 4:
                    score = score + 40;
                    x21 = 0;
                    x22 = 0;
                    x = 0;
                    y3 = 0;
                    y = 30;
                    y2=60;
                    y3=30;
                    y=0;
                    x=150;
                    x2=180;
                    printl_invertidoacima();
                    addl_invertidocima();
                    eliminador_de_linha();
                    game_over();
                    if(over == 1){
                        break;
                    }
                    mvbloqueado = 0;

                case 5:
                    x22 = 0;
                    y3 = 0;
                    x = 150;
                    x2 = 180;
                    x21 = 210;
                    y = 0;
                    y2 = 30;
                    score = score +40;
                    print_raio();
                    add_raio();
                    eliminador_de_linha();
                    if(over == 1){
                        break;
                    }
                    mvbloqueado = 0;



            };
        }
    }
}






int main(int argc, char * argv[])
{
    iniciar_fonte();
    escreve();

    console_portugues();

    audio_abertura();
    channel = Mix_PlayChannel(0, musicafundo_abertura,-1);
    Mix_Volume(channel, 30);


    inicializa_video();


    SDL_Texture * Fundo = carrega_imagem("./Abertura.png");
    pinta_imagem(Fundo, 0, 0, 360, 690,
                            0, 0, LARGURA, ALTURA);
    SDL_RenderPresent(Render);
    SDL_Delay(1000);



    int apertarespaco = 1;
        while (apertarespaco==1){
            while(SDL_PollEvent(&evento) !=0){
                if(evento.type == SDL_QUIT){
                apertarespaco = 0;
                }
                else if(evento.type == SDL_KEYDOWN){
                    if(evento.key.keysym.sym == SDLK_SPACE){
                        printf("\n TECLA DETECTADA \n");
                        menu();
                        apertarespaco = 0;
                        break;
                    }
                }
            }
        continue;
        break;
        }



    getch();
    return 0;
}

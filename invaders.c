#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <allegro5/allegro_image.h>

#define RECORD_FILE "files/record.txt"

// VARIAVEIS GLOBAIS
const int SCREEN_W = 960;
const int SCREEN_H = 700;

const int GRASS_H = 60;

const int NAVE_W = 100;
const int NAVE_H = 50;

const int N_ALIEN_ROWS = 4;
const int N_ALIEN_COLS = 5;

const int ALIEN_W = 40;
const int ALIEN_H = 40;
float ALIEN_X_VEL = 0.8;

const int MAX_TIROS_ALIEN = 5;

const float FPS = 100;

ALLEGRO_COLOR ALIEN_COR;
ALLEGRO_COLOR NAVE_COR;
ALLEGRO_COLOR FUNDO_COR;
ALLEGRO_COLOR GRAMA_COR;

// TIPOS ESPECIAIS
typedef struct Alien {
	float x, y;
	ALLEGRO_COLOR cor;
	int ativo;
} Alien;

typedef struct Nave {
	float x;
	float vel;
	int dir, esq;
	ALLEGRO_COLOR cor;
} Nave;

typedef struct Tiro {
	float x, y;
	float vel;
	int ativo;
} Tiro;

typedef struct TiroAlien{
    float x, y;
    float vel;
    int ativo;
} TiroAlien;

// FUNCOES RECORDE
int readRecord() {
    FILE *file = fopen(RECORD_FILE, "r");
    int recorde = 0;

    if (file != NULL) {
        fscanf(file, "%d", &recorde);
        fclose(file);
    }

    return recorde;
}

void saveRecord(int newR) {
    FILE *file = fopen(RECORD_FILE, "w");

    if (file != NULL) {
        fprintf(file, "%d", newR);
        fclose(file);
    }
}

// FUNCOES CORES
ALLEGRO_COLOR getRandomColor() {
	return al_map_rgb(
		rand() % 256,
		rand() % 256,
		rand() % 256
	);
}

void changeColors() {
	ALIEN_COR = getRandomColor();
	NAVE_COR = getRandomColor();
	FUNDO_COR = getRandomColor();
	GRAMA_COR = getRandomColor();
}

// FUNCOES CENARIO
void draw_cenario(int* pontos,  ALLEGRO_FONT* font, int* recorde) {
	al_clear_to_color(FUNDO_COR);

	al_draw_filled_rectangle(
		0, SCREEN_H - GRASS_H,
		SCREEN_W, SCREEN_H,
		GRAMA_COR
	);

	al_draw_textf(font, al_map_rgb(255, 255, 255), 10, SCREEN_H - 40, 0, "Pontos: %d", *pontos);
	al_draw_textf(font, al_map_rgb(255, 255, 255), SCREEN_W-160, SCREEN_H - 40, 0, "Recorde: %d", *recorde);
}

// FUNCOES NAVE
void initNave(Nave* nave) {
	nave->x = SCREEN_W/2;
	nave->vel = 3;
	nave->dir = 0;
	nave->esq = 0;
	nave->cor = NAVE_COR;
}
	
void draw_nave(Nave nave, ALLEGRO_BITMAP* nave_sprite) {
	float y_base = SCREEN_H - GRASS_H/2 - 60;
	/*al_draw_filled_triangle(
		nave.x, y_base - NAVE_H,
		nave.x-NAVE_W/2,y_base,
		nave.x+NAVE_W/2,y_base,
		NAVE_COR
	);*/
	al_draw_tinted_bitmap(nave_sprite, NAVE_COR, nave.x - NAVE_W / 2, y_base, 0);
}
	
void update_nave(Nave* nave) {
	if(nave->dir && nave->x+nave->vel <= SCREEN_W) {
		nave->x += nave->vel;
	}
	if(nave->esq && nave->x-nave->vel >= 0) {
		nave->x -= nave->vel;
	}
}

// FUNCOES ENTIDADE ALIEN
void draw_alien(Alien alien, ALLEGRO_BITMAP* alien_sprite) {
	/*al_draw_filled_rectangle(
		alien.x, alien.y + 10,
		alien.x + ALIEN_W, alien.y + ALIEN_H + 10,
		ALIEN_COR); */
	al_draw_tinted_bitmap(alien_sprite, ALIEN_COR, alien.x, alien.y, 0);
}

// FUNCOES CONJUNTO ALIENS
void initAliens(Alien aliens[N_ALIEN_ROWS][N_ALIEN_COLS]) {
	for (int i = 0; i < N_ALIEN_ROWS; i++) {
		for (int j = 0; j < N_ALIEN_COLS; j++) {
			aliens[i][j].x = j * (ALIEN_W + 30);
			aliens[i][j].y = i * (ALIEN_H + 30);
			aliens[i][j].cor = ALIEN_COR;
			aliens[i][j].ativo = 1;
		}
	}
}

void draw_aliens(Alien aliens[N_ALIEN_ROWS][N_ALIEN_COLS], ALLEGRO_BITMAP* alien_sprite) {
	for (int i = 0; i < N_ALIEN_ROWS; i++) {
		for (int j = 0; j < N_ALIEN_COLS; j++) {
			if(aliens[i][j].ativo) { draw_alien(aliens[i][j], alien_sprite); }
		}
	}
}

void update_aliens(Alien aliens[N_ALIEN_ROWS][N_ALIEN_COLS]) {
	int getDown = 0;
	for (int i = 0; i < N_ALIEN_ROWS; i++) {
		for (int j = 0; j < N_ALIEN_COLS; j++) {
			Alien a = aliens[i][j];

			if (!a.ativo) continue;

			if ((a.x + ALIEN_W + ALIEN_X_VEL > SCREEN_W) ||
			    (a.x + ALIEN_X_VEL < 0)) {
				getDown = 1;
				break;
			}
		}
		if (getDown) {
 			ALIEN_X_VEL *= -1;
			break;
		}
	}

	for (int i = 0; i < N_ALIEN_ROWS; i++) {
		for (int j = 0; j < N_ALIEN_COLS; j++) {
			if (getDown) {
				aliens[i][j].y += ALIEN_H;
			}
			aliens[i][j].x += ALIEN_X_VEL;
		}
	}
}

int colisao_aliens_solo(Alien aliens[N_ALIEN_ROWS][N_ALIEN_COLS]) {
	for (int i = 0; i < N_ALIEN_ROWS; i++) {
		for (int j = 0; j < N_ALIEN_COLS; j++) {
			if (aliens[i][j].y + ALIEN_H >  SCREEN_H - GRASS_H && aliens[i][j].ativo) {
				return 1;
			}
		}
	}
	return 0;
}

// FUNCOES TIRO NAVE
void initTiro(Tiro* tiro) {
	tiro->ativo = 0;
	tiro->vel = 2;
}

void atirar(Tiro* tiro, float nave_x) {
	if (!tiro->ativo) {
		tiro->ativo = 1;
		tiro->x = nave_x;
		tiro->y = SCREEN_H - GRASS_H - NAVE_H;
	}
}

void update_tiro(Tiro* tiro) {
	if (tiro->ativo) {
		tiro->y -= tiro->vel;
		if (tiro->y < 0)
			tiro->ativo = 0;
	}
}

void draw_tiro(Tiro tiro) {
	if (tiro.ativo) {
		al_draw_filled_circle(tiro.x, tiro.y, 5, al_map_rgb(255, 255, 0));
	}
}

void colisao_tiro_alien(Tiro* tiro, Alien aliens[N_ALIEN_ROWS][N_ALIEN_COLS], int* pontos, ALLEGRO_SAMPLE* audio) {
    if (!tiro->ativo)
        return;

    for (int i = 0; i < N_ALIEN_ROWS; i++) {
        for (int j = 0; j < N_ALIEN_COLS; j++) {
            Alien* a = &aliens[i][j];
            if (a->ativo) {
                if (tiro->x >= a->x && tiro->x <= a->x + ALIEN_W &&
                    tiro->y >= a->y && tiro->y <= a->y + ALIEN_H) {
                    
                    a->ativo = 0;
                    tiro->ativo = 0;
					(*pontos)++;
					al_play_sample(audio, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					changeColors();
                    return;
                }
            }
        }
    }
}

// FUNCOES TIROS ALIENS
void initTirosAlien(TiroAlien tiros[]) {
    for (int i = 0; i < MAX_TIROS_ALIEN; i++) {
        tiros[i].ativo = 0;
        tiros[i].vel = 1.0;
    }
}

void alien_atira(Alien aliens[N_ALIEN_ROWS][N_ALIEN_COLS], TiroAlien tiros[]) {
    int j = rand() % N_ALIEN_COLS;

    // Encontra o alien ativo mais abaixo da coluna escolhida
    for (int i = N_ALIEN_ROWS - 1; i >= 0; i--) {
        if (aliens[i][j].ativo) {
            // Procura um tiro disponível
            for (int k = 0; k < MAX_TIROS_ALIEN; k++) {
                if (!tiros[k].ativo) {
                    tiros[k].ativo = 1;
                    tiros[k].x = aliens[i][j].x + ALIEN_W / 2;
                    tiros[k].y = aliens[i][j].y + ALIEN_H;
                    break;
                }
            }
            break;
        }
    }
}

void update_tiros_alien(TiroAlien tiros[]) {
    for (int i = 0; i < MAX_TIROS_ALIEN; i++) {
        if (tiros[i].ativo) {
            tiros[i].y += tiros[i].vel;
            if (tiros[i].y > SCREEN_H) {
                tiros[i].ativo = 0;
            }
        }
    }
}

void draw_tiros_alien(TiroAlien tiros[]) {
    for (int i = 0; i < MAX_TIROS_ALIEN; i++) {
        if (tiros[i].ativo) {
            al_draw_filled_circle(tiros[i].x, tiros[i].y, 5, al_map_rgb(255, 255, 255));
        }
    }
}

int colisao_tiro_nave(TiroAlien tiros[], Nave nave) {
    float y_base = SCREEN_H - GRASS_H/2;

    for (int i = 0; i < MAX_TIROS_ALIEN; i++) {
        if (tiros[i].ativo) {
            if (tiros[i].y >= y_base - NAVE_H && tiros[i].y <= y_base &&
                tiros[i].x >= nave.x - NAVE_W/2 && tiros[i].x <= nave.x + NAVE_W/2) {
                return 1;
            }
        }
    }
    return 0;
}

// FUNCOES VERIFICACOES FIM DE JOGO
int isOver(int* pontos) {
    if (*pontos < N_ALIEN_ROWS * N_ALIEN_COLS) {
        return 0;
    }
    return 1;
}

int colisao_nave_alien(Alien aliens[N_ALIEN_ROWS][N_ALIEN_COLS], Nave nave) {
    float y_nave_topo = SCREEN_H - GRASS_H - NAVE_H;

    for (int i = 0; i < N_ALIEN_ROWS; i++) {
        for (int j = 0; j < N_ALIEN_COLS; j++) {
            if (aliens[i][j].ativo) {
                Alien alien = aliens[i][j];

                // Verifica se o alien atingiu ou ultrapassou a parte de cima da nave
                if (alien.y + ALIEN_H >= y_nave_topo &&
                    alien.x + ALIEN_W >= nave.x - NAVE_W / 2 &&
                    alien.x <= nave.x + NAVE_W / 2) {
                    return 1; // Colisão detectada
                }
            }
        }
    }
    return 0; // Sem colisão
}

		
int main() {
	int pontos = 0;
	int recorde = readRecord();

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT* font = NULL;
	
	srand(time(NULL));
			
    //inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	//inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }

	// Inicializa e carrega a fonte
	al_init_font_addon();
	al_init_ttf_addon();

	font = al_load_ttf_font("bitter.ttf", 24, 0);
	if (!font) {
		fprintf(stderr, "Falha ao carregar fonte TTF!\n");
		return -1;
	}

	// Inicializa e carrega os audios
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(3);

	ALLEGRO_SAMPLE *explosao = al_load_sample("files/audio/explosao.wav");
	ALLEGRO_SAMPLE *win = al_load_sample("files/audio/win.wav");
	ALLEGRO_SAMPLE *loss = al_load_sample("files/audio/loss.wav");
	if (!explosao || !win || !loss) {
		fprintf(stderr, "Erro ao carregar os efeitos sonoros\n");
		return -1;
	}

	ALLEGRO_AUDIO_STREAM* bg_music = NULL;
	bg_music = al_load_audio_stream("files/audio/bg.ogg", 4, 2048);
    if (!bg_music) {
        fprintf(stderr, "Erro ao carregar a musica de fundo!\n");
        return -1;
    }

	al_set_audio_stream_playmode(bg_music, ALLEGRO_PLAYMODE_LOOP);
    al_attach_audio_stream_to_mixer(bg_music, al_get_default_mixer());

	// Inicializa o addon de imagens
	al_init_image_addon();
	
	ALLEGRO_BITMAP* alien_sprite = NULL;
	ALLEGRO_BITMAP* nave_sprite = NULL;
	alien_sprite = al_load_bitmap("files/images/alien.png");
	nave_sprite = al_load_bitmap("files/images/nave.png");
	if(!alien_sprite || !nave_sprite) {
		fprintf(stderr, "Erro ao carregar sprites!\n");
		return -1;
	}

    //cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
    
    //instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}

    //cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

    //registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	changeColors();

	Nave nave;
	initNave(&nave);

	Alien aliens[N_ALIEN_ROWS][N_ALIEN_COLS];
	initAliens(aliens);

	Tiro tiro;
	initTiro(&tiro);

	TiroAlien tiros[MAX_TIROS_ALIEN];
	initTirosAlien(tiros);
    
	//inicia o temporizador
	al_start_timer(timer);
	
    int playing = 1;
    while(playing) {

		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {

			draw_cenario(&pontos, font, &recorde);

			update_nave(&nave);
			draw_nave(nave, nave_sprite);

			update_aliens(aliens);
			draw_aliens(aliens, alien_sprite);

			update_tiro(&tiro);
			draw_tiro(tiro);

			update_tiros_alien(tiros);
			draw_tiros_alien(tiros);

			al_set_audio_stream_playing(bg_music, true);

			if (rand() % 100 < 2) { // ~2% de chance por frame
    			alien_atira(aliens, tiros);
			}

			colisao_tiro_alien(&tiro, aliens, &pontos, explosao);

			if(pontos > recorde) {saveRecord(pontos); recorde = pontos;}

			playing = 	!colisao_aliens_solo(aliens) 
						&& !isOver(&pontos) 
						&& !colisao_tiro_nave(tiros, nave) 
						&& !colisao_nave_alien(aliens, nave);

			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();
			
			if(al_get_timer_count(timer)%(int)FPS == 0)
				printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/FPS));
		}
        //se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			//imprime qual tecla foi
			printf("\ncodigo tecla pressionada: %d", ev.keyboard.keycode);

			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_A:
					nave.esq = 1;
				break;

				case ALLEGRO_KEY_D:
					nave.dir = 1;
				break;

				case ALLEGRO_KEY_LEFT:
					nave.esq = 1;
				break;

				case ALLEGRO_KEY_RIGHT:
					nave.dir = 1;
				break;

				case ALLEGRO_KEY_SPACE:
					atirar(&tiro, nave.x);
			}
		}
		//se o tipo de evento for um soltar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			//imprime qual tecla foi
			printf("\ncodigo tecla pressionada: %d", ev.keyboard.keycode);

			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_A:
					nave.esq = 0;
				break;

				case ALLEGRO_KEY_D:
					nave.dir = 0;
				break;

				case ALLEGRO_KEY_LEFT:
					nave.esq = 0;
				break;

				case ALLEGRO_KEY_RIGHT:
					nave.dir = 0;
				break;
			}
		}
    }

	al_destroy_audio_stream(bg_music);

	if (isOver(&pontos)) {
		// Limpa a tela
		al_clear_to_color(al_map_rgb(0, 0, 0));

		// Desenha a mensagem de "Vitória"
		al_draw_text(font, al_map_rgb(0, 255, 0),
					SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTER,
					"Parabéns! Você venceu!");

		al_flip_display();
		al_play_sample(win, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

		// Espera 5 segundos
		al_rest(2.0);

		al_destroy_font(font);

	} else {
		// Limpa a tela
		al_clear_to_color(al_map_rgb(0, 0, 0));

		// Desenha a mensagem de "Derrota"
		al_draw_text(font, al_map_rgb(255, 0, 0),
					SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTER,
					"Que pena! Você perdeu!");

		al_flip_display();
		al_play_sample(loss, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

		// Espera 5 segundos
		al_rest(2.0);
	}

	al_destroy_bitmap(alien_sprite);
	al_destroy_sample(explosao);
	al_destroy_sample(win);
	al_destroy_sample(loss);
	al_destroy_font(font);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

    return 0;
}
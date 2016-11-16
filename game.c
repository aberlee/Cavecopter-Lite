/*
<< Cavecopter Lite >>
Classic helicopter game written in C (Allegro5)
https://github.com/coregameHD/Cavecopter-Lite/
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

/* Global Variables */
const float FPS = 60;
const int WINWIDTH = 800;
const int WINHEIGHT = 600;

unsigned int score = 0;
unsigned int highscore = 0;

/* Function Prototypes */
void LoadScore(void);
void OverwriteScore(void);

/* Structure */
enum STATE { TITLE, PLAYING, GAMEOVER };
enum MYKEYS { KEY_UP, KEY_DOWN };

struct HELICOPTER {
	int x;
	int y;
	int width;
	int height;
	ALLEGRO_BITMAP *image;
};

struct OBSTACLES {
	int x;
	int y;
	int width;
	int height;
};

int main(int argc, char **argv) {
	/* Declaring Allegro Variables */
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *title = NULL, *lost = NULL;
	ALLEGRO_FONT *font = NULL;

	/* Declaring Game Variables */
	int magicnumber;
	char score_string[9], highscore_string[9];

	struct HELICOPTER helicopter;
	helicopter.x = 100;
	helicopter.y = 300;
	helicopter.width = 100;
	helicopter.height = 80;

	struct OBSTACLES obs_top;
	obs_top.x = 400;
	obs_top.y = 100;
	obs_top.width = 100;
	obs_top.height = 200;

	struct OBSTACLES obs_buttom;
	obs_buttom.x = 700;
	obs_buttom.y = 400;
	obs_buttom.width = 100;
	obs_buttom.height = 100;

	int state = -1;
	bool key[2] = { false, false };
	bool redraw = true;
	bool doexit = false;

	/* Allegro __initialization__ */
	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	if (!al_init_image_addon()) {
		fprintf(stderr, "failed to initialize image addon!\n");
		return -1;
	}

	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize primitives addon!\n");
		return -1;
	}

	if (!al_init_font_addon()) {
		fprintf(stderr, "failed to create font addon!\n");
		return -1;
	}

	if (!al_init_ttf_addon()) {
		fprintf(stderr, "failed to create ttf addon!\n");
		return -1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	display = al_create_display(WINWIDTH, WINHEIGHT);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	font = al_load_ttf_font("rd_chulajaruek.ttf", 26, 0);
	if (!font) {
		fprintf(stderr, "failed to load font!\n");
		al_destroy_timer(display);
		al_destroy_timer(timer);
		return -1;
	}

	helicopter.image = al_load_bitmap("helicopter.png");
	al_convert_mask_to_alpha(helicopter.image, al_map_rgb(255, 0, 255));
	if (!helicopter.image) {
		fprintf(stderr, "failed to create helicopter.image bitmap!\n");
		al_destroy_display(font);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	title = al_load_bitmap("title.png");
	al_convert_mask_to_alpha(title, al_map_rgb(255, 0, 255));
	if (!title) {
		fprintf(stderr, "failed to create title bitmap!\n");
		al_destroy_display(helicopter.image);
		al_destroy_display(font);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	lost = al_load_bitmap("gameover.png");
	al_convert_mask_to_alpha(title, al_map_rgb(255, 0, 255));
	if (!title) {
		fprintf(stderr, "failed to create lost bitmap!\n");
		al_destroy_display(title);
		al_destroy_display(helicopter.image);
		al_destroy_display(font);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_bitmap(lost);
		al_destroy_display(title);
		al_destroy_display(helicopter.image);
		al_destroy_display(font);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_set_target_bitmap(al_get_backbuffer(display));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_start_timer(timer);
	srand(time(NULL));
	state = TITLE;

	/* Main Event Game Loop */
	while (!doexit) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
			if (key[KEY_UP] && helicopter.y >= 3.0) {// Release a button
				helicopter.y -= 3.0;
			}
			if (key[KEY_DOWN] && helicopter.y <= WINHEIGHT - helicopter.height - 3.0) {// Press a button
				helicopter.y += 3.0;
			}
			if (state == TITLE){}
			else if (state == PLAYING) {
				if (score > 2000) {
					obs_top.x -= 6;
					obs_buttom.x -= 6;
					score += 2;
				}
				else if (score > 4000) {
					obs_top.x -= 9;
					obs_buttom.x -= 9;
					score += 3;
				}
				else {
					obs_top.x -= 3;
					obs_buttom.x -= 3;
					score += 1;
				}
				// Helicopter-walls collision detection
				if (helicopter.y <= 100 || helicopter.y >= 500 - helicopter.height) {
					state = GAMEOVER;
				}
				// Helicopter-obs_top collision detection
				if (helicopter.x < obs_top.x + obs_top.width && helicopter.x + helicopter.width > obs_top.x &&
					helicopter.y < obs_top.y + obs_top.height && helicopter.height + helicopter.y > obs_top.y) {
					state = GAMEOVER;
				}
				// Helicopter-obs_buttom collision detection
				if (helicopter.x < obs_buttom.x + obs_buttom.width && helicopter.x + helicopter.width > obs_buttom.x &&
					helicopter.y < obs_buttom.y + obs_buttom.height && helicopter.height + helicopter.y > obs_buttom.y) {
					state = GAMEOVER;
				}

				/* OBSTACLE __initialization__ */
				if (obs_top.x + obs_top.width <= 0) {//if obs_top is out of the screen
					obs_top.x = 800;
					obs_top.y = 100;
					obs_top.width = 100;

					magicnumber = rand() % 4 + 1;
					switch (magicnumber) {
					case 1:	obs_top.height = 100;	break;
					case 2:	obs_top.height = 150;	break;
					case 3:	obs_top.height = 200;	break;
					case 4:	obs_top.height = 250;	break;
					}
				}
				if (obs_buttom.x + obs_buttom.width <= 0) {//if obs_buttom is out of the screen		
					obs_buttom.x = 800;
					obs_buttom.y = 400;
					obs_buttom.width = 100;
					obs_buttom.height = 200;
				}
			}
			else if (state == GAMEOVER) {
				if (score > highscore) {
					OverwriteScore();
				}
				// Reset positions
				obs_top.x = 400;
				obs_top.y = 100;
				obs_buttom.x = 700;
				obs_buttom.y = 400;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			doexit = true;
			break;
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) { //Press a button
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_A://Button 'A'
				if (state == TITLE)
					state = PLAYING;
					helicopter.y = 300;
				if (state == GAMEOVER)
					state = PLAYING;
					helicopter.y = 300;
					score = 0;
			case ALLEGRO_KEY_SPACE://Button 'Space Bar'
				key[KEY_UP] = true;
				key[KEY_DOWN] = false;
				break;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP) { //Release a button
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_SPACE:
				key[KEY_UP] = false;
				key[KEY_DOWN] = true;
				break;
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}

		/* Draw objects into the screen */
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			if (state == TITLE) {
				// Draw title screen
				al_draw_bitmap(title, 0, 0, 0);
			}

			else if (state == PLAYING) {
				// Draw background
				al_clear_to_color(al_map_rgb(0, 0, 0));
				// Draw Helicopter
				al_draw_bitmap(helicopter.image, helicopter.x, helicopter.y, 0);
				// Draw walls
				al_draw_filled_rectangle(0, 0, 800, 100, al_map_rgb(105, 240, 174));
				al_draw_filled_rectangle(0, 500, 800, 600, al_map_rgb(105, 240, 174));
				// Draw texts (Score/High Score)
				al_draw_text(font, al_map_rgb(0, 0, 0), 600, 0, ALLEGRO_ALIGN_LEFT, "SCORE: ");
				al_draw_text(font, al_map_rgb(0, 0, 0), 700, 0, ALLEGRO_ALIGN_LEFT, _itoa(score, score_string, 10));
				LoadScore();
				al_draw_text(font, al_map_rgb(0, 0, 0), 50, 0, ALLEGRO_ALIGN_LEFT, "HIGH SCORE: ");
				if (score > highscore) //Show score and high score simultaneously
					al_draw_text(font, al_map_rgb(0, 0, 0), 200, 0, ALLEGRO_ALIGN_LEFT, _itoa(score, score_string, 10));
				else
					al_draw_text(font, al_map_rgb(0, 0, 0), 200, 0, ALLEGRO_ALIGN_LEFT, _itoa(highscore, highscore_string, 10));
				// Draw obstacles
				al_draw_filled_rectangle(obs_top.x, obs_top.y, obs_top.x + obs_top.width, obs_top.y + obs_top.height, al_map_rgb(105, 240, 174));
				al_draw_filled_rectangle(obs_buttom.x, obs_buttom.y, obs_buttom.x + obs_buttom.width, obs_buttom.y + obs_buttom.height, al_map_rgb(105, 240, 174));
			}

			else if (state == GAMEOVER) {
				al_draw_bitmap(lost, 0, 0, 0);
				al_draw_text(font, al_map_rgb(255, 255, 255), 250, 200, ALLEGRO_ALIGN_LEFT, "YOUR SCORE: ");
				al_draw_text(font, al_map_rgb(255, 255, 255), 450, 200, ALLEGRO_ALIGN_LEFT, _itoa(score, score_string, 10));
			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_destroy_timer(timer);
	return 0;
}

void LoadScore() {
	FILE *filescore;
	int tempscore;
	filescore = fopen("highscore.dat", "r");
	fscanf(filescore, "%d", &tempscore);
	if (filescore == NULL)
		highscore = 0;
	else
		highscore = tempscore;
	fclose(filescore);
	return;
}

void OverwriteScore() {
	FILE *filescore;
	filescore = fopen("highscore.dat", "w");
	if (score > highscore)
		fprintf(filescore, "%d", score);
	else
		fprintf(filescore, "%d", highscore);
	fclose(filescore);
	return;
}
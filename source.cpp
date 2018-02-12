// Remove warnings on Visual Studio
#define _CRT_SECURE_NO_WARNINGS

// Solve Winmm library
#pragma comment(lib, "winmm.lib")

// Include libraries
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <WinBase.h>
#include <mmsystem.h>

// Declare constants
#define HEIGHT 20
#define WIDTH 40
#define CHR_FILE_INTRO 49
#define CHR_INTRO 177
#define CHR_SPACE 32
#define CHR_BORDER_HORIZIONAL 205
#define CHR_BORDER_VERTICAL 186
#define CHR_BORDER_CORNER_TOP_RIGHT 187
#define CHR_BORDER_CORNER_TOP_LEFT 201
#define CHR_BORDER_CORNER_BOTTOM_RIGHT 188
#define CHR_BORDER_CORNER_BOTTOM_LEFT 200
#define CHR_INVADER 177
#define CHR_PLAYER 254
#define CHR_BULLET 111
#define CHR_BULLET_TRACE 39
#define COLOR_INTRO 10
#define COLOR_DEFAULT 7
#define COLOR_INVADER 10
#define COLOR_PLAYER 5
#define COLOR_BULLET 4

// Declare variables
HANDLE console;
FILE *logs;
clock_t key_first, key_second;
char scena[HEIGHT][WIDTH];
char last_key;
int bullet_x[100], bullet_y[100], bullet_direction[100];
int invaders_x[50], invaders_y[50], invaders_life[50];
int invaders_left, invaders_right, invaders_bottom;
int nr_bullets = 0;
int nr_invaders = 0;
int score = 0;
int game_speed = 1;
int contor_down;
int player_position = WIDTH / 2;
int movement_direction = 1;
int movement_count = 0;
int movement_unit = 1;

// Declare functions
bool have_no_bullets();
bool win_game();
bool kill_player();
void hide_cursor();
void init_game();
void init_scena();
void print_intro();
void print_scena();
void print_infos();
void update_scena();
void clear_scena();
void move_block();
void refresh_limits();
void verify_keyboard();
void pause_game();
void exit_game();

// Define MAIN function: implement game logic
void main(){
	init_game();
	print_intro();
	init_scena();
	while (!win_game() && !have_no_bullets() && !kill_player()) {
		clear_scena();
		update_scena();
		print_scena();
		print_infos();
		for (int i = 0; i < 10000 / game_speed; i++) {
			verify_keyboard();
			Sleep(0.1);
		}
		move_block();
	}
	exit_game();
}

// Define HAVE_NO_BULLETS: verify if player have bullets; in case he doesn't, wait before no chance to win
bool have_no_bullets() {

	// Declare variables
	int count_alive_invaders = nr_invaders - score;
	int count_alive_bullets = 0;

	for (int i = 0; i < nr_bullets; i++) {
		if (bullet_direction[i] != 2) {
			count_alive_bullets++;
		}
	}
	if (nr_bullets == 100 && (count_alive_bullets == 0 || count_alive_bullets < count_alive_invaders)) {
		PlaySound(TEXT("Music/playerkilled.wav"), NULL, SND_FILENAME | SND_ASYNC);
		fprintf(logs, "Joc pierdut datorita lipsei de munitie\r\n");
		print_scena();
		printf("\n Din pacate, galaxia nu a putut fi salvata. Munitia ta a fost consumata.");
		printf("\n\n Apasa orice tasta pentru a continua!");
		_getch();
		return true;
	}
	return false;

}

// Define WIN_GAME: verify if game is winned, no invaders left
bool win_game() {
	if (score == nr_invaders) {
		fprintf(logs, "Joc castigat cu scor de: %d puncte\r\n", score);
		print_scena();
		printf("\n Felicitari! Toti invaders au fost omorati.");
		printf("\n\n Apasa orice tasta pentru a continua!");
		_getch();
		return true;
	}
	return false;
}

// Define KILL_PLAYER: verify if player is killed by invaders
bool kill_player() {
	if (invaders_bottom == HEIGHT - 2) {
		PlaySound(TEXT("Music/playerkilled.wav"), NULL, SND_FILENAME | SND_ASYNC);
		fprintf(logs, "Joc pierdut datorita omorarii de catre invaders\r\n");
		print_scena();
		printf("\n Din pacate, galaxia nu a putut fi salvata. Ai fost omorat de invaders.");
		printf("\n\n Apasa orice tasta pentru a continua!");
		_getch();
		return true;
	}
	return false;
}

// Define HIDE_CURSOR: hide cursor on console
void hide_cursor() {
	console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(console, &info);
}

// Define INIT_GAME: initialize game resources
void init_game() {

	// Declare variables
	time_t now;
	char file_name[20];

	// Init random
	srand(time(NULL));
	hide_cursor();

	// Init time
	key_first = clock();
	now = time(NULL);

	// Init log file
	sprintf(file_name, "Logs/%d.txt", now);
	logs = fopen(file_name, "w+");

}

// Define INIT_SCENA: initialize game matrix
void init_scena() {

	// Define variables
	int random;

	// Generate borders
	for (int i = 0; i < HEIGHT; i++) {
		scena[i][0] = CHR_BORDER_VERTICAL;
		scena[i][WIDTH - 1] = CHR_BORDER_VERTICAL;
	}
	for (int j = 0; j < WIDTH; j++) {
		scena[0][j] = CHR_BORDER_HORIZIONAL;
		scena[HEIGHT - 1][j] = CHR_BORDER_HORIZIONAL;
	}
	scena[0][0] = CHR_BORDER_CORNER_TOP_LEFT;
	scena[0][WIDTH - 1] = CHR_BORDER_CORNER_TOP_RIGHT;
	scena[HEIGHT - 1][0] = CHR_BORDER_CORNER_BOTTOM_LEFT;
	scena[HEIGHT - 1][WIDTH - 1] = CHR_BORDER_CORNER_BOTTOM_RIGHT;

	// Generate invaders
	fprintf(logs, "Invaders generated:\r\n");
	for (int i = 1; i < 4; i++) {
		random = rand() % 3 + 2;
		for (int j = 0; j < WIDTH - 10; j++) {
			if (j % random == 0) {
				invaders_x[nr_invaders] = i;
				invaders_y[nr_invaders] = j + 5;
				fprintf(logs, "(%d, %d)\r\n", invaders_x[nr_invaders], invaders_y[nr_invaders]);
				invaders_life[nr_invaders] = 1;
				nr_invaders++;
			}
		}
	}

	// Init bullet coords
	for (int i = 0; i < 100; i++) {
		bullet_x[i] = INT_MAX;
		bullet_y[i] = INT_MAX;
		bullet_direction[i] = 2;
	}
	refresh_limits();
	fprintf(logs, "\r\nActions:\r\n");

}

// Define PRINT_INTRO: print intro scene with "SPACE INVADERS" text
void print_intro() {

	// Declare variables
	FILE *intro_file;
	char intro_char;
	char ch = 'i';

	do {
		system("cls");
		intro_file = fopen("Intro/image.txt", "r");
		while ((intro_char = getc(intro_file)) != EOF) {
			if (intro_char == CHR_FILE_INTRO) {
				SetConsoleTextAttribute(console, COLOR_INTRO);
				printf("%c", CHR_INTRO);
			}
			else {
				SetConsoleTextAttribute(console, COLOR_DEFAULT);
				printf("%c", intro_char);
			}
		}
		SetConsoleTextAttribute(console, COLOR_DEFAULT);
		if (ch == 'a') {
			printf("\n\n");
			intro_file = fopen("Menu/about.txt", "r");
			while ((intro_char = getc(intro_file)) != EOF) {
				printf("%c", intro_char);
				Sleep(10);
			}
		}
		if (ch == 'h') {
			printf("\n\n");
			intro_file = fopen("Menu/controls.txt", "r");
			while ((intro_char = getc(intro_file)) != EOF) {
				printf("%c", intro_char);
			}
		}
		printf("\n\n Apasa tasta 'h' pentru ajutor, 'a' pentru istoricul jocului si orice alta tasta pentru a incepe jocul");
		ch = _getch();
	} while (ch == 'h' || ch == 'a');

}

// Define PRINT_SCENA: print game matrix
void print_scena() {
	system("cls");
	printf("\n");
	for (int i = 0; i < HEIGHT; i++) {
		printf(" ");
		for (int j = 0; j < WIDTH; j++) {
			if (scena[i][j] == CHR_PLAYER || scena[i][j] == CHR_PLAYER - 256) {
				SetConsoleTextAttribute(console, COLOR_PLAYER);
			}
			else if (scena[i][j] == CHR_INVADER || scena[i][j] == CHR_INVADER - 256) {
				SetConsoleTextAttribute(console, COLOR_INVADER);
			}
			else if (scena[i][j] == CHR_BULLET || scena[i][j] == CHR_BULLET - 256 || scena[i][j] == CHR_BULLET_TRACE || scena[i][j] == CHR_BULLET_TRACE - 256) {
				SetConsoleTextAttribute(console, COLOR_BULLET);
			}
			else {
				SetConsoleTextAttribute(console, COLOR_DEFAULT);
			}
			printf("%c", scena[i][j]);
		}
		printf("\n");
	}
}

// Define PRINT_INFOS: print game infos
void print_infos() {
	printf("\n Score: %d puncte", score);
	printf("\n Munitie trasa: %d/100", nr_bullets);
	printf("\n Viteza invaders: %d", movement_unit);
}

// Define UPDATE_SCENA: update game matrix
void update_scena() {

	// Update player position
	scena[HEIGHT - 2][player_position] = CHR_PLAYER;

	// Update invaders
	for (int i = 0; i < nr_invaders; i++) {
		if (invaders_life[i] == 1) {
			scena[invaders_x[i]][invaders_y[i]] = CHR_INVADER;
		}
	}

	// Update bullets
	for (int i = 0; i < nr_bullets; i++) {
		if (bullet_direction[i] != 2) {

			// verificare intersectare cu invader
			for (int j = 0; j < nr_invaders; j++) {
				if (bullet_x[i] == invaders_x[j] && bullet_y[i] == invaders_y[j] && invaders_life[j] == 1) {
					PlaySound(TEXT("Music/invaderkilled.wav"), NULL, SND_FILENAME | SND_ASYNC);
					invaders_life[j] = 0;
					bullet_direction[i] = 2;
					bullet_x[i] = INT_MAX;
					bullet_y[i] = INT_MAX;
					score++;
					refresh_limits();
				}
			}

			// deplsaare dreapta
			if (bullet_direction[i] == 0) {
				scena[bullet_x[i]][bullet_y[i]] = CHR_BULLET;
				if (bullet_x[i] < HEIGHT - 3) {
					scena[bullet_x[i] + 1][bullet_y[i]] = CHR_BULLET_TRACE;
				}
				bullet_x[i]--;
				if (bullet_x[i] == 0) {
					bullet_x[i] = INT_MAX;
					bullet_y[i] = INT_MAX;
					bullet_direction[i] = 2;
				}
			}

			// deplasare oblica la stanga
			if (bullet_direction[i] == -1) {
				scena[bullet_x[i]][bullet_y[i]] = CHR_BULLET;
				if (bullet_x[i] < HEIGHT - 3) {
					scena[bullet_x[i] + 1][bullet_y[i] + 1] = CHR_BULLET_TRACE;
				}
				bullet_x[i]--;
				bullet_y[i]--;
				if (bullet_x[i] == 0 || bullet_y[i] == 0) {
					bullet_x[i] = INT_MAX;
					bullet_y[i] = INT_MAX;
					bullet_direction[i] = 2;
				}
			}

			// deplasare oblica la dreapta
			if (bullet_direction[i] == 1) {
				scena[bullet_x[i]][bullet_y[i]] = CHR_BULLET;
				if (bullet_x[i] < HEIGHT - 3) {
					scena[bullet_x[i] + 1][bullet_y[i] - 1] = CHR_BULLET_TRACE;
				}
				bullet_x[i]--;
				bullet_y[i]++;
				if (bullet_x[i] == 0 || bullet_y[i] == WIDTH - 2) {
					bullet_x[i] = INT_MAX;
					bullet_y[i] = INT_MAX;
					bullet_direction[i] = 2;
				}
			}
		}
	}

}

// Define CLEAR_SCENA: fill game matrix with empty characters
void clear_scena() {
	for (int i = 1; i < HEIGHT - 1; i++) {
		for (int j = 1; j < WIDTH - 1; j++) {
			scena[i][j] = CHR_SPACE;
		}
	}
}

// Define MOVE_BLOCK: move invaders block
void move_block() {
	if (movement_count == movement_unit) {
		movement_count = 0;
		PlaySound(TEXT("Music/invadermoved.wav"), NULL, SND_FILENAME | SND_ASYNC);
		if ((invaders_left == 1 || invaders_right == WIDTH - 2) && contor_down == 0) {
			movement_direction++;
			invaders_bottom++;
			for (int i = 0; i <= nr_invaders; i++) {
				invaders_x[i]++;
			}
			contor_down = 1;
		}
		else {
			if (movement_direction % 2 == 0) {
				invaders_left--;
				invaders_right--;
				for (int i = 0; i <= nr_invaders; i++) {
					invaders_y[i]--;
				}
				contor_down = 0;
			}
			else if (movement_direction % 2 == 1) {
				invaders_left++;
				invaders_right++;
				for (int i = 0; i <= nr_invaders; i++) {
					invaders_y[i]++;
				}
				contor_down = 0;
			}
		}
	}
	else {
		movement_count++;
	}
}

// Define REFRESH_LIMITS: reinitialize invaders block limits if one of it is killed
void refresh_limits() {

	// Declare variables
	int min_left = WIDTH;
	int max_right = 0;
	int max_bottom = 0;

	for (int i = 0; i < nr_invaders; i++) {
		if (invaders_x[i] > max_bottom && invaders_life[i] == 1) {
			max_bottom = invaders_x[i];
		}
		if (invaders_y[i] > max_right && invaders_life[i] == 1) {
			max_right = invaders_y[i];
		}
		if (invaders_y[i] < min_left && invaders_life[i] == 1) {
			min_left = invaders_y[i];
		}
	}
	invaders_left = min_left;
	invaders_right = max_right;
	invaders_bottom = max_bottom;

}

// Define VERIFY_KEYBOARD: verify if user pressed a key
void verify_keyboard() {

	// Define variables
	char ch;
	int key_spent;

	if (_kbhit()) {
		ch = _getch();
		key_second = clock();
		key_spent = (key_second - key_first)/100;
		switch (ch) {
			case 'a':
				if (player_position > 1) {
					fprintf(logs, "Left movement detected\r\n");
					scena[HEIGHT - 2][player_position] = CHR_SPACE;
					player_position--;
					scena[HEIGHT - 2][player_position] = CHR_PLAYER;
				}
				break;
			case 'd':
				if (player_position < WIDTH - 2) {
					fprintf(logs, "Right movement detecteds\r\n");
					scena[HEIGHT - 2][player_position] = CHR_SPACE;
					player_position++;
					scena[HEIGHT - 2][player_position] = CHR_PLAYER;
				}
				break;
			case ' ':
				if (nr_bullets < 100) {
					PlaySound(TEXT("Music/shoot.wav"), NULL, SND_FILENAME | SND_ASYNC);
					if (last_key == 'a' && key_spent < 2) {
						bullet_direction[nr_bullets] = 1;
					}
					else if (last_key == 'd' && key_spent < 2) {
						bullet_direction[nr_bullets] = -1;
					}
					else {
						bullet_direction[nr_bullets] = 0;
					}
					bullet_x[nr_bullets] = HEIGHT - 3;
					bullet_y[nr_bullets] = player_position;
					fprintf(logs, "Shoot detected. Bullet at coords: %d, %d with direction: %d\r\n", bullet_x[nr_bullets], bullet_y[nr_bullets], bullet_direction[nr_bullets]);
					nr_bullets++;
				}
				break;
			case 'n':
				if (game_speed > 1) {
					game_speed--;
				}
				fprintf(logs, "Decreasing speed: %d\r\n", game_speed);
				break;
			case 'm':
				game_speed++;
				fprintf(logs, "Increasing speed: %d\r\n", game_speed);
				break;
			case 'k':
				if (movement_unit > 1) {
					movement_unit--;
				}
				fprintf(logs, "Decreasing invaders speed: %d\r\n", movement_unit);
				break;
			case 'l':
				movement_unit++;
				fprintf(logs, "Increasing invaders speed: %d\r\n", movement_unit);
				break;
			case 'e':
				exit_game();
				break;
			case 'p':
				pause_game();
				break;
		}
		last_key = ch;
		key_first = key_second;
	}

}

// Define PAUSE_GAME: pause game
void pause_game() {

	// Declare variables
	char did_you_know[3][300] = {
		"Tomohiro Nishikado, inventatorul acestui joc, s-a inspirat din 'The world of the worlds', 'Brakeout' si 'Star wars'",
		"La momentul crearii jocului, nu existau prea multe calculatoare destul de puternice pentru a-l rula. Asa ca inventatorul a creeat o platforma hardware dedicata 'Space invaders'",
		"'Space invaders'a fost primul joc care a introdus conceptul de high score"
	};
	int random;

	PlaySound(TEXT("Music/pause.wav"), NULL, SND_FILENAME | SND_ASYNC);
	fprintf(logs, "Pauza joc\r\n");
	print_scena();
	random = rand() % 3;
	printf("\n STIATI CA: ");
	printf("%s?\n", did_you_know[random]);
	printf("\n Apasa orice tasta pentru a reveni in joc!");
	_getch();

}

// Define EXIT_GAME: exit game
void exit_game() {
	PlaySound(TEXT("Music/exit.wav"), NULL, SND_FILENAME | SND_ASYNC);
	fprintf(logs, "Iesire joc\r\n");
	print_scena();
	printf("\n Apasa orice tasta pentru a iesi din joc!");
	_getch();
	fclose(logs);
	exit(0);
}
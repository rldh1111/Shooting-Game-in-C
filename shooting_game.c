#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#define SPACE 64
#define ENTER 13
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define PAUSE 112
#define HEIGHT 40
#define WIDTH 80
#define TEXT_HEIGHT HEIGHT / 2 -14
#define TEXT_WIDTH WIDTH / 2 - 25
#define HP 3
#define MAX_BULLET 3
#define MAX_ENEMY 20
#define ENEMY_SIZE 2
#define PLAYER_SIZE 1
void draw_main(void);
typedef struct bullet {
	double x;
	double y;
	boolean exist;
}Bullet;
typedef struct player {
	double x;
	double y;
	int hp;
	boolean invin;
	Bullet bullet[MAX_BULLET];
} Player;
typedef struct enemy {
	int x;
	int y;
	int speed;
	int hp;
	boolean exist;
	Bullet bullet[MAX_BULLET];
}Enemy;



void playerMove();
void makePlayerBullet();
void makeEnemyBullet();
void gotoxy(int x, int y);
void makeEnemy();
void playerShoot();
void enemyMove();
void enemyShoot();
void cursorRemove();
void playerBulletControll();
void enemyBulletControll();
void playerInvincibility();  //hp 1다운과 무적상태 playerHP() 구현 보류
void playerHp();
void playerHpPrint();
void scorePrint();
void GameInitial();
void pauseKey();
void end();
int score = 0;
int cnt = 0;
int f_cnt;
int num;
int enemy1 = 0;
int enemy2 = 0;
Player player = { WIDTH / 2, HEIGHT / 2 + 10,HP,FALSE };
Enemy enemy[MAX_ENEMY];
boolean pause = TRUE;

int main(void) {
	system("mode con:cols=80 lines=40");
	cursorRemove();
	draw_main();
	for (f_cnt = 0;; f_cnt++) {
		pauseKey();
		playerHpPrint();
		scorePrint();
		playerMove();
		playerShoot();
		makeEnemy();
		makeEnemyBullet();
		enemyMove();
		enemyShoot();
		playerBulletControll();
		enemyBulletControll();
		Sleep(5);
		if (pause == TRUE) {
			system("cls");
		}

	}
	return 0;
}
void cursorRemove() {
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	srand((unsigned)time(NULL));
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
void pauseKey() {
	int ch;
	if (_kbhit()) {
		ch = _getch();
		if (ch == PAUSE) {
			if (pause == FALSE) {
				pause = TRUE;
			}
			else {
				pause = FALSE;
			}
		}
	}
}
void playerHpPrint() {
	gotoxy(60, 35);
	printf("HP: ");
	for (int i = 0; i < player.hp; i++) {
		printf("♥ ");
	}
}
void scorePrint() {
	gotoxy(60, 37);
	printf("score: %d", score);
}
void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void GameInitial() {
	score = 0;
	cnt = 0;
	num = 0;
	enemy1 = 0;
	enemy2 = 0;
	player.x = WIDTH / 2;
	player.y = HEIGHT / 2 + 10;
	player.invin = FALSE;
	player.hp = 3;
	f_cnt = 0;
	CloseHandle(playerInvincibility);
	for (int i = 0; i < MAX_ENEMY; i++) {
		for (int j = 0; j < MAX_BULLET; j++) {

			enemy[i].exist = FALSE;
			enemy[i].bullet[j].exist = FALSE;
		}
	}
}
void draw_main(void) {
	GameInitial();
	int ch;
	int text_shoot[5][23] = {
		1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,
		1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1,0,
		1,1,1,0,1,1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1,0,
		0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1,0,
		1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,0,1,0
	};
	int gameText[5][21] = {
		1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,
		1,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,
		1,0,1,1,0,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,
		1,0,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,
		1,1,1,1,0,1,0,0,1,0,1,0,1,0,1,0,1,1,1,1,1,
	};
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 23; j++) {
			if (text_shoot[i][j] == 1) {
				gotoxy(TEXT_WIDTH + j * 2 - 10, TEXT_HEIGHT + i - 3); printf("▣");
			}
			else if (text_shoot[i][j] == 0) {
				gotoxy(TEXT_WIDTH + j * 2 - 10, TEXT_HEIGHT + i - 3); printf(" ");
			}
		}
		gotoxy(TEXT_WIDTH, TEXT_HEIGHT + i); printf("\n");
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 21; j++) {
			if (gameText[i][j] == 1) {
				gotoxy(TEXT_WIDTH + j * 2 + 10, TEXT_HEIGHT + i + 5); printf("▣");
			}
			else if (gameText[i][j] == 0) {
				gotoxy(TEXT_WIDTH + j * 2 + 10, TEXT_HEIGHT + i + 5); printf(" ");
			}
		}
		gotoxy(TEXT_WIDTH, TEXT_HEIGHT + i); printf("\n");
	}
	gotoxy(WIDTH / 2 - 35, TEXT_HEIGHT + 13); printf("┌ ─ ─ ─  GAME CONTROL ─ ── ─┐");
	gotoxy(WIDTH / 2 - 35, TEXT_HEIGHT + 14); printf("│		    ↑           │");
	gotoxy(WIDTH / 2 - 35, TEXT_HEIGHT + 15); printf("│  MOVE:	 ← ↓ →        │");
	gotoxy(WIDTH / 2 - 35, TEXT_HEIGHT + 16); printf("│			         │");
	gotoxy(WIDTH / 2 - 35, TEXT_HEIGHT + 17); printf("│  SHOOT: SPACE             │");
	gotoxy(WIDTH / 2 - 35, TEXT_HEIGHT + 18); printf("│		                 │");
	gotoxy(WIDTH / 2 - 35, TEXT_HEIGHT + 19); printf("│  PAUSE: P                 │");
	gotoxy(WIDTH / 2 - 35, TEXT_HEIGHT + 20); printf("└ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┘");

	gotoxy(WIDTH / 2, TEXT_HEIGHT + 13); printf("┌ ─ ─ ─ ─  ENEMY  ─ ─ ─ ─ ─┐");
	gotoxy(WIDTH / 2, TEXT_HEIGHT + 14); printf("│                          │  ");
	gotoxy(WIDTH / 2, TEXT_HEIGHT + 15); printf("│  [-|-] : 100점           │  ");
	gotoxy(WIDTH / 2, TEXT_HEIGHT + 16); printf("│                          │  ");
	gotoxy(WIDTH / 2, TEXT_HEIGHT + 17); printf("│                          │  ");
	gotoxy(WIDTH / 2, TEXT_HEIGHT + 18); printf("│  [*|*] : 300점           │  ");
	gotoxy(WIDTH / 2, TEXT_HEIGHT + 19); printf("│                          │  ");
	gotoxy(WIDTH / 2, TEXT_HEIGHT + 20); printf("└ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ── ─ ┘");
	while (1) {
		gotoxy(WIDTH / 2 - 15, TEXT_HEIGHT + 25); printf("<Press Enter Key To Start>");
		if ((ch = _getch()) == ENTER) {
			system("cls");
			break;
		}
	}
}
void end() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	int ch;
	int gameText[5][21] = {
	1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,
	1,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,
	1,0,1,1,0,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,
	1,0,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,
	1,1,1,1,0,1,0,0,1,0,1,0,1,0,1,0,1,1,1,1,1,
	};
	int overText[5][23] = {
	1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,
	1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,
	1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,
	1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,
	1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,1,
	};
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 21; j++) {
			if (gameText[i][j] == 1) {
				gotoxy(TEXT_WIDTH + j * 2 - 10, TEXT_HEIGHT + i); printf("■");
			}
			else if (gameText[i][j] == 0) {
				gotoxy(TEXT_WIDTH + j * 2 - 10, TEXT_HEIGHT + i); printf(" ");
			}
		}
		gotoxy(TEXT_WIDTH, TEXT_HEIGHT + i); printf("\n");
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 23; j++) {
			if (overText[i][j] == 1) {
				gotoxy(TEXT_WIDTH + j * 2 + 10, TEXT_HEIGHT + i + 7); printf("■");
			}
			else if (overText[i][j] == 0) {
				gotoxy(TEXT_WIDTH + j * 2 + 10, TEXT_HEIGHT + i + 7); printf(" ");
			}
		}
		gotoxy(TEXT_WIDTH, TEXT_HEIGHT + i + 10); printf("\n");
	}

	gotoxy(TEXT_WIDTH + 10, TEXT_HEIGHT + 17); printf("[---]: %d", enemy1);
	gotoxy(TEXT_WIDTH + 30, TEXT_HEIGHT + 17); printf("[=|=]: %d", enemy2);
	gotoxy(TEXT_WIDTH + 20, TEXT_HEIGHT + 20); printf("score : %d", score);
	while (1) {
		gotoxy(WIDTH / 2 - 15, TEXT_HEIGHT + 30); printf("<Press Enter Key To Start>");
		if ((ch = _getch()) == ENTER) {
			system("cls");
			draw_main();
			break;
		}
	}
}
void playerMove() {

	gotoxy(player.x, player.y); printf("<--->");
	if (pause) {
		if (GetAsyncKeyState(VK_RIGHT) && player.x < WIDTH - PLAYER_SIZE - 3) {
			player.x += 1.5;
		}
		else if (GetAsyncKeyState(VK_LEFT) && player.x > 0) {
			player.x -= 1.5;
		}
		else if (GetAsyncKeyState(VK_UP) && player.y > 0) {
			player.y -= 0.75;
		}
		else if (GetAsyncKeyState(VK_DOWN) && player.y < HEIGHT - 0.5) {
			player.y += 0.75;
		}
	}
}

void makePlayerBullet() {
	if (pause) {
		if (player.bullet[cnt].exist == FALSE) {
			player.bullet[cnt].exist = TRUE;
			player.bullet[cnt].x = player.x;
			player.bullet[cnt].y = player.y - 1;
			cnt++;
			if (cnt == 3) {
				cnt = 0;
			}
		}
	}
}
void playerShoot() {
	if (GetAsyncKeyState(VK_SPACE)) {
		makePlayerBullet();
	}
	for (int i = 0; i < MAX_BULLET; i++) {
		if (player.bullet[i].exist == TRUE) {
			gotoxy(player.bullet[i].x + 1.9, player.bullet[i].y); printf("º");
			if (pause) {
				player.bullet[i].y -= 1.2;
			}
		}
	}
}
void playerBulletControll() {
	if (pause) {
		for (int i = 0; i < MAX_BULLET; i++) {
			if (player.bullet[i].y < 0 && player.bullet[i].exist == TRUE) {
				player.bullet[i].exist = FALSE;
			}
			else {
				for (int j = 0; j < MAX_ENEMY; j++) {
					if (player.bullet[i].exist == TRUE && enemy[j].exist == TRUE) {
						if (player.bullet[i].y >= (double)enemy[j].y - ENEMY_SIZE && player.bullet[i].y <= (double)enemy[j].y + ENEMY_SIZE
							&& player.bullet[i].x >= (double)enemy[j].x - ENEMY_SIZE && player.bullet[i].x <= (double)enemy[j].x + ENEMY_SIZE + 1) {
							player.bullet[i].exist = FALSE;
							enemy[j].hp--;
							if (enemy[j].hp == 0) {
								if (j % 3 == 0) {
									score += 300;
									enemy2++;
								}
								else {
									score += 100;
									enemy1++;
								}
								enemy[j].exist = FALSE;
							}
						}
					}
				}
			}
		}
	}
}
void playerInvincibility() {
	if (pause) {
		if (player.invin) {
			Sleep(3000);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			player.invin = FALSE;
		}
	}
}
void playerHp() {
	player.hp -= 1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
	if (player.hp == 0) {
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		end();
	}
}
void makeEnemy() {
	if (pause) {
		if (f_cnt % 40 == 0 || f_cnt % 100 == 0 && enemy[num].exist == FALSE) {
			enemy[num].x = rand() % 70 + 3;
			enemy[num].y = rand() % 5 + 1;
			enemy[num].speed = 1;
			enemy[num].hp = 2;
			enemy[num].exist = TRUE;
			num++;
			if (num == MAX_ENEMY) {
				num = 0;
			}
		}
	}
}
void enemyMove() {

	for (int i = 0; i < MAX_ENEMY; i++) {
		if (enemy[i].exist == TRUE) {
			if (i % 3 == 0) {
				gotoxy(enemy[i].x, enemy[i].y); printf("[=|=]");
			}
			else {
				gotoxy(enemy[i].x, enemy[i].y); printf("[---]");
			}
			if (pause) {
				enemy[i].x += enemy[i].speed;
				if (enemy[i].x >= WIDTH - ENEMY_SIZE - 3 && enemy[i].exist == TRUE) {
					enemy[i].speed *= (-1);
				}
				else if (enemy[i].x <= 0 && enemy[i].exist == TRUE) {
					enemy[i].speed *= (-1);
				}
			}
		}
	}
}

void makeEnemyBullet() {
	if (pause) {
		for (int i = 0; i < MAX_ENEMY; i++) {
			for (int j = 0; j < MAX_BULLET; j++) {
				if (enemy[i].exist == TRUE && enemy[i].bullet[j].exist == FALSE) {
					enemy[i].bullet[j].exist = TRUE;
					enemy[i].bullet[j].x = enemy[i].x;
					enemy[i].bullet[j].y = enemy[i].y;
				}
			}
		}
	}
}
void enemyShoot() {

	for (int i = 0; i < MAX_ENEMY; i++) {
		for (int j = 0; j < MAX_BULLET; j++) {
			if (enemy[i].bullet[j].exist == TRUE && enemy[i].exist == TRUE) {
				if (i % 3 == 0) {
					gotoxy(enemy[i].bullet[j].x, enemy[i].bullet[j].y); printf("●");
					if (pause) {
						enemy[i].bullet[j].y += 0.8;
						if (enemy[i].bullet[j].x < player.x + 2) {
							enemy[i].bullet[j].x += 0.3;
						}
						else {
							enemy[i].bullet[j].x -= 0.3;
						}
					}
				}
				else {
					gotoxy(enemy[i].bullet[j].x, enemy[i].bullet[j].y); printf("l");
					if (pause) {
						enemy[i].bullet[j].y += 0.8;
					}
				}
			}
			if (enemy[i].bullet[j].y > HEIGHT - 1 && enemy[i].bullet[j].exist == TRUE) {
				enemy[i].bullet[j].exist = FALSE;
			}
		}
	}
}
void enemyBulletControll() {
	if (pause) {
		for (int i = 0; i < MAX_ENEMY; i++) {
			for (int j = 0; j < MAX_BULLET; j++) {
				if (enemy[i].exist == TRUE && enemy[i].bullet[j].exist == TRUE) {
					if (enemy[i].bullet[j].y >= player.y - PLAYER_SIZE && enemy[i].bullet[j].y <= player.y + PLAYER_SIZE
						&& enemy[i].bullet[j].x >= player.x - PLAYER_SIZE && enemy[i].bullet[j].x <= player.x + PLAYER_SIZE + 2) {
						if (player.invin == FALSE) {
							player.invin = TRUE;
							(HANDLE)_beginthreadex(NULL, 0, (int(__stdcall*)(void*))playerInvincibility, 0, 0, 0);
							playerHp();
						}
						enemy[i].bullet[j].exist = FALSE;
					}
				}
			}
		}
	}
}




#include "config.h"

#ifdef GAME_OBSTACLE

#define JOYSTICK_DEAD_ZONE 10
#define MAX_OBSTACLES 5
#define GOAL_X 30
#define GOAL_Y 100
#define OBSTACLE_RADIUS 2
#define PLAYER_RADIUS 3
#define GOAL_RADIUS 4

unsigned int x_position = 31;
unsigned int y_position = 63;
unsigned int score = 0;
unsigned char lives = 3;

typedef struct {
	unsigned int x;
	unsigned int y;
} Point;

Point obstacles[MAX_OBSTACLES];
Point goal = {GOAL_X, GOAL_Y};
	
void show_score(void) {
	char score_text[20];
	sprintf(score_text, "Score: %d", score); // Convert the score into a string
	lcd_string(1, 0, score_text); // Display the score on the second line of the LCD
}	

void place_obstacles(void) {
	for (int i = 0; i < MAX_OBSTACLES; i++) {
		obstacles[i].x = rand() % 64;
		obstacles[i].y = rand() % 128;
	}
}

void check_collision(void) {
	for (int i = 0; i < MAX_OBSTACLES; i++) {
		if (abs(x_position - obstacles[i].x) < (OBSTACLE_RADIUS + PLAYER_RADIUS) &&
		abs(y_position - obstacles[i].y) < (OBSTACLE_RADIUS + PLAYER_RADIUS)) {
			lives--;
			if (lives == 0) {
				// Game over logic
				lcd_clear();
				lcd_string(0, 0, "Game Over!");
				lcd_string(0, 1, "Final Score: "); // Display final score
				char final_score_text[12];
				sprintf(final_score_text, "%d", score);
				lcd_string(12, 1, final_score_text);
				while(1); // Stop the game loop
			}
			x_position = 31; // Reset position to start
			y_position = 63;
			show_score(); // Update score display after losing a life
		}
	}

	if (abs(x_position - goal.x) < (GOAL_RADIUS + PLAYER_RADIUS) &&
	abs(y_position - goal.y) < (GOAL_RADIUS + PLAYER_RADIUS)) {
		score++;
		//buzzer ������ �������� "����"
		//1 ������
		//2 ���� ����
		//3 ������ ����
		//4 ��� 1 ��������
		//5 ��� 2 ���������
		//6 ���� �������������
		//7
		S_S1();
		
		show_score(); // Update score display after reaching the goal
		goal.x = rand() % 64;
		goal.y = rand() % 128;
		place_obstacles(); // Place new obstacles
	}
}


void draw_game_elements(void) {
	// Draw the player
	GLCD_Circle(x_position, y_position, PLAYER_RADIUS);
	

	// Draw the goal
	GLCD_Circle(goal.x, goal.y, GOAL_RADIUS);

	// Draw the obstacles
	for (int i = 0; i < MAX_OBSTACLES; i++) {
		GLCD_Circle(obstacles[i].x, obstacles[i].y, OBSTACLE_RADIUS);
	}

	// Show the score on the screen
	show_score();
}



void game_obstacle_init(void) {
	lcd_clear();
	ScreenBuffer_clear();
	lcd_string(0, 0, "Use joystick to move");
	show_score(); // Show the initial score
	_delay_ms(1000);

	place_obstacles(); // Initialize obstacles at random positions
}


void game_obstacle_update(void) {
	unsigned int joystick_x = Read_Adc_Data(3) / 16;
	unsigned int joystick_y = 127 - Read_Adc_Data(4) / 8;

	if(abs(joystick_x - 31) > JOYSTICK_DEAD_ZONE) {
		x_position += (joystick_x > 31) ? 1 : -1;
	}
	if(abs(joystick_y - 63) > JOYSTICK_DEAD_ZONE) {
		y_position += (joystick_y > 63) ? 1 : -1;
	}

	x_position = (x_position > 63) ? 63 : (x_position < 0) ? 0 : x_position;
	y_position = (y_position >127) ? 127 : (y_position < 0) ? 0 : y_position;

	check_collision();
	
	lcd_clear();
	ScreenBuffer_clear();
	draw_game_elements();
}

void main_game_obstacle(void) {
	init_devices();
	game_obstacle_init();
	while(1) {
		game_obstacle_update();
		_delay_ms(50);
	}
}
#endif
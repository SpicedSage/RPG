#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//structures
struct Map 
{ //map structure
    char tiles[10][50];
    int x;
    int y;
};

struct Attack 
{ //attack structure
	char name[50];
	int accuracy;
	int damageHit;
	int damageMiss;
};

struct Entity 
{ //entity structure
	char name[50];
    char species[50];

	int hp;
	int maxHp;
	int regen;
	int regenTime;
	int control;

	struct Attack attacks[4];

	char mapChar;

	int x;
	int y;
};

//map bottom layer
struct Map g_main_map;

//enemy types
struct Entity g_goblin;
struct Entity g_troll;
struct Entity g_orc;
struct Entity g_kobold;

//map enemys
struct Entity* g_map_enemies;
int g_enemy_num;

struct Entity* g_map_entities[100];
int g_entity_num;

//player entity
struct Entity g_player;

//continue or exit game
int g_run;

//output text to screen
char* g_text;

//map tiles
char* g_tiles;
int g_tiles_len;

//entemy tiles
char* g_enemy_tiles;
int g_enemy_tiles_len;

//battle info
struct Entity g_enemy;
int g_battle_status;
int g_battle_win;

//constructors
struct Entity entity_constructor(char* name, char* species, int health, int regen, int regen_time, int control, struct Attack attacks[4], char map_value)
{ //entity constructor

	//create entity
	struct Entity entity;

	//asign values
	strcpy(entity.name, name);
	strcpy(entity.species, species);
	entity.hp = health;
	entity.maxHp = health;
	entity.regen = regen;
	entity.regenTime = regen_time;
	entity.control = control;
	entity.mapChar = map_value;

	for (int x = 0; x < 4; x++) 
		{ entity.attacks[x] = attacks[x]; }

	//return entity
	return entity;
}

struct Attack attack_constructor(char* name, int accuracy, int damage_hit, int damage_miss)
{ // attack constructor
	
	//create attack
	struct Attack attack;

	//asign values
	strcpy(attack.name, name);
	attack.accuracy = accuracy;
	attack.damageHit = damage_hit;
	attack.damageMiss = damage_miss;

	//return attack
	return attack;
}

//add map entity
struct Entity map_entity(struct Entity entity, int x, int y)
{ //add x and y values to entity
	
	entity.x = x;
	entity.y = y;

	return entity;
}

//other
int clear()
{ //clears screen
	
	printf("\n");		//add next line to remove "> " character on refresh
    system("clear");	//clear

	return 0;
}

char input()
{ //gets single char input
	
    char key;			//char vaiable
    scanf("%c", &key);	//get input to var

    return key;			//return variable
}

int match(char value, char* array)
{ //compare value to an array of values

	//array length
	int array_len = strlen(array);

	//loop through array
	for (int x = 0; x < array_len; x++)
	{
		//if array value is same as value return true 
		if ( value == array[x] )
			{ return 1; }
	}

	//else return false
	return 0;
}

//initiate elements
int init_entities()
{ //Initialize player and enemy values

	//create attack array
	struct Attack attacks[4];
	
	//player
	attacks[0] = attack_constructor( "Swing", 40, 2, 0 );
	attacks[1] = attack_constructor( "Throw Rocks", 70, 3, 1 );
	attacks[2] = attack_constructor( "Punt", 90, 5, 1 );
	attacks[3] = attack_constructor( "Trip", 20, 7, 0 );
	
	g_player = entity_constructor("Sage", "Skeleton", 20, 0, 0, 40, attacks, '@');
	
	//goblin
	attacks[0] = attack_constructor( "Slash", 80, 3, 0 );
	attacks[1] = attack_constructor( "Bite", 40, 2, 0 );
	attacks[2] = attack_constructor( "Screech", 90, 2, 1 );
	attacks[3] = attack_constructor( "Spit", 20, 1, 0 );
	
	g_goblin = entity_constructor("Goblin", "Goblin", 5, 1, 3, 40, attacks, 'g');
	
	//troll
	attacks[0] = attack_constructor( "Bash", 70, 8, 0 );
	attacks[1] = attack_constructor( "Charge", 50, 10, 2 );
	attacks[2] = attack_constructor( "Swat", 60, 5, 0 );
	attacks[3] = attack_constructor( "Roar", 80, 4, 2 );
	
	g_troll = entity_constructor("Troll", "Troll", 18, 3, 3, 30, attacks, 't');
	
	//orc
	attacks[0] = attack_constructor( "Clap", 20, 18, 5 );
	attacks[1] = attack_constructor( "Charge", 40, 14, 4 );
	attacks[2] = attack_constructor( "Grab", 30, 11, 0 );
	attacks[3] = attack_constructor( "Throw", 25, 21, 6 );
	
	g_orc = entity_constructor("Orc", "Orc", 22, 4, 3, 10, attacks, 'o');
	
	//kobold
	attacks[0] = attack_constructor( "Stab", 70, 3, 0 );
	attacks[1] = attack_constructor( "Trap", 40, 12, 2 );
	attacks[2] = attack_constructor( "Bite", 30, 5, 0 );
	attacks[3] = attack_constructor( "Throw Rocks", 80, 2, 1 );
	
	g_kobold = entity_constructor("Kobold", "Kobold", 9, 1, 1, 60, attacks, 'k');

	
	return 0;
}

int init_map()
{ //initiate map values

	//map enemies
	struct Entity enemy[6] = {
		map_entity(g_goblin, 14, 1),
		map_entity(g_troll,  26, 6),
		map_entity(g_orc,    38, 8),
		map_entity(g_kobold, 31, 1),
		map_entity(g_kobold, 31, 2),
		map_entity(g_kobold, 31, 3),
	};
	
	g_map_enemies = enemy;

	//player
	g_player = map_entity(g_player, 4, 2);
	
	//map entities
	struct Entity* entity[7];

	g_enemy_num = (sizeof(enemy) / sizeof(enemy[0]));
	g_entity_num = (sizeof(entity) / sizeof(entity[0]));
	
	
	for (int i = 0; i < g_enemy_num ; i++)
		{ entity[i] = &enemy[i]; }

	entity[6] = &g_player;

	for (int i = 0; i < g_entity_num; i++)
		{ g_map_entities[i] = entity[i]; }
	
	//map sizes
    g_main_map.x = sizeof(g_main_map.tiles[0]) / sizeof(g_main_map.tiles[0][0]);
    g_main_map.y = sizeof(g_main_map.tiles) / sizeof(g_main_map.tiles[0]);

	//map textures
    char map[10][50] = {
        "#################################################",
        "##                  ##                          #",
        "##             ##   ##   C                      #",
        "##             ##   ##                          #",
        "##             ##   #################   ##///####",
        "##      C      ##                       ##   ####",
        "##             ##                       ##   ####",
        "########################## ###############   ####",
        "#################          ######                |",
        "#################---#############---#############",
	
    };

	//assign to global map
    for(int place = 0; place < g_main_map.y; place++)
		{ strcpy(g_main_map.tiles[place], map[place]); }

    return 0;
} 

int init_variables()
{ //initiates global variable values
	
	g_run = 1; 						//say to continue running game
	g_text = "";					//output text is none
	
	g_battle_status = 0; 			//tells game to show battle screen
	g_battle_win = 0;				//tells game if you won battle
	
	g_tiles = "# @gtkoC/?-|";		//usable map tiles
	g_tiles_len = strlen(g_tiles);

	g_enemy_tiles = "gtko";			//enemy map tiles
	g_enemy_tiles_len = strlen(g_enemy_tiles);
	
	return 0;
}

//graphics
struct Map entity_overlay()
{ //return new map with player character 

	//create new map
    struct Map map;

	//copy global map to local
    for(int y = 0; y < g_main_map.y; y++)
		{ strcpy(map.tiles[y], g_main_map.tiles[y]); }

	//add global entitys to new map
	for (int i = 0; i < g_entity_num; i++)
	{
		struct Entity entity = *g_map_entities[i];
		map.tiles[entity.y][entity.x] = entity.mapChar;
	}
	
	//return new map
    return map;
}

int printf_map(struct Map map)
{ //print out map

	//Map seperator
    printf("===================== M A P ======================\n");

	//loop through y plane
    for(int y = 0; y < g_main_map.y; y++)
	{
		//loop through x plan
        for (int x = 0; x < g_main_map.x; x++)
		{
			//if tile is in global tile list print that tile else print default
			if ( match(g_main_map.tiles[y][x], g_tiles) ) 
				{ printf("%c", map.tiles[y][x]); }
			else 
				{ printf("#"); }
        }
		//print next line
        printf("\n");
    }

	//print spacer for next function
	printf("\n"); 
	
    return 0;
}

int printf_player(struct Entity player)
{ //prints entity as player information

	//print player spacer
    printf("================== P L A Y E R ===================\n");

	//print entity info
    printf( "Name     %s\n", player.name );
    printf( "Body     %s\n", player.species );
    printf( "Health   %i/%i\n", player.hp, player.maxHp );
    printf( "Control  %i\n", player.control );
    
    printf("Attacks  ");
    for ( int x = 0; x < 4; x++ )
    	{ printf( "\'%s\' ", player.attacks[x].name ); }

	//print new line for next funtiom
    printf("\n\n");

	return 0;
}

int printf_enemy(struct Entity enemy)
{ //prints entity as enemy information

	//print enemy spacer
    printf("=================== E N E M Y ====================\n");

	//print entity info
    printf( "Species  %s\n", enemy.species );
    printf( "Health   %i/%i\n", enemy.hp, enemy.maxHp );

	//prepare next function
    printf("\n");

	return 0;
}

int printf_input()
{ //formatting for better inpur
	
	//input spacer
    printf("=================== I N P U T ====================\n");

	//input blinker
	printf("> ");

	return 0;
}

int printf_text(char* text)
{ //print text to screen in game format

	//print text spacer
	printf("==================== T E X T =====================\n");
	
	//print text
	printf("%s\n", text);

	//prepare next function
	printf("\n");

	return 0;
};

//battle fucntions
struct Entity get_enemy(char sprite)
{ // copys attacking enemy's information into global enemy variable

	// create enemy variable
	struct Entity enemy;

	// add enemy based on character in map
	switch ( sprite )
	{
	case 'g':				//goblin
		enemy = g_goblin;
		break;
	case 'o':				//orc
		enemy = g_orc;
		break;
	case 't':				//troll
		enemy = g_troll;
		break;
	case 'k':				//kobold
		enemy = g_kobold;
		break;
	}

	//return enemy
	return enemy;
}

int battle_turn()
{
	int enemy_atk_value = rand() % 4;
	struct Attack enemy_atk = g_enemy.attacks[enemy_atk_value];

	char hit_status[30];
	int damage;
	
	int accuracy = rand() % 100;
	if ( accuracy < enemy_atk.accuracy ) 
	{ 
		strcpy(hit_status, "but it missed");
		damage = enemy_atk.damageHit; 
	}
	else
	{ 
		strcpy(hit_status, "and it hit");
		damage = enemy_atk.damageMiss; 
	}
	
	char text[100];
  	snprintf(text, sizeof(text), "The attacking %s used %s.", g_enemy.species, enemy_atk.name);

	g_text = text;
	
	input();
	return 0;
}

int battle_check(){

	return 0;
}

//movement
int collision(int x_move, int y_move) 
{ //checks for empty space or enemy

	//gets future x and y positions
    int new_pos_x = g_player.x + x_move;
    int new_pos_y = g_player.y + y_move;

	//combined movement char
	char combined = g_main_map.tiles[new_pos_y][new_pos_x];

	//seperated x y position chars
	char seperate[2] = {
		g_main_map.tiles[g_player.y][new_pos_x],
		g_main_map.tiles[new_pos_y][g_player.x]
	};

	//check if next space is an enemy
	int enemy_tile = 0;

	for ( int i = 0; i < g_enemy_num; i++ )
		{
			int check_x = g_map_enemies[i].x == new_pos_x;
			int check_y = g_map_enemies[i].y == new_pos_y;
			
			if (check_x && check_y) 
			{
				enemy_tile = 1;
				g_enemy = g_map_enemies[i];
				break;
			}
		}
	
	//if path exsist true
	int path = match(' ', seperate);

	//if path exsists and final location empty move to final location
    if ( combined == ' ' && path && !enemy_tile )
    {
        g_player.x = new_pos_x;
        g_player.y = new_pos_y;
    }
	//if path open and final location is enemy flip battle flags
	else if ( enemy_tile && path )
	{
		g_text = "An enemy is attacking!";
		g_battle_status = 1;
	}

    return 0;
}

int map_end()
{ //check if player is on map

	//x and y checks seperate to account for diagonal movement
	
	//if player x pos less than 0 set to 0
    if ( g_player.x < 0 ) 
		{ g_player.x = 0; }
	//if player x pos greater than map maximum x pos set to max
	else if ( g_player.x > (g_main_map.x - 1) )  
		{ g_player.x = g_main_map.x - 1; }
	
	//if player y pos less than 0 set to 0
    if ( g_player.y < 0 ) 
		{ g_player.y = 0; }
	//if player y pos greater than map maximum y pos set to max
	else if ( g_player.y > (g_main_map.y - 1) ) 
		{ g_player.y = g_main_map.y - 1; }

    return 0;
}

int move(char key)
{ //8 directional movement wasd and numpad
	
    switch (key)
    {
    case 'w': 				//up 
    case '8':
        collision(0, -1);
        break;
    case '9': 				//up + right
        collision(1, -1);
        break;
    case 'd': 				//right
    case '6':
        collision(1, 0);
        break;
    case '3': 				//down + right
        collision(1, 1);
        break;
    case 's': 				//down
    case '2':
        collision(0, 1);
        break;
    case '1': 				//down + left
        collision(-1, 1);
        break;
    case 'a': 				//left
    case '4':
        collision(-1, 0);
        break;
    case '7': 				//up + left
        collision(-1, -1);
        break;
    default:
        break;
    }

	//check player position is on map
    map_end();

    return 0;
}

//controls
int controls_map()
{ //map screen input, controls and logic
	
	printf_input();			//input text
    char key = input();		//get input
	
    move(key);				//logic

    return 0;
}

int controls_battle()
{ //battle screen input, controls and logic
	
	printf_input();			//input text
    char key = input();		//get input

	battle_turn();

    return 0;
}

//frames
int frame_map()
{ //map display information

	//create map with player
	struct Map visible_map;

	// visible_map = entity_overlay();
	visible_map = entity_overlay();
	
	//print functions
    printf_player(g_player);
    printf_map(visible_map);
	printf_text(g_text);

    return 0;
}

int frame_battle()
{ //battle display info
	
	//display functions
	printf_player(g_player);
	printf_enemy(g_enemy);
	printf_text(g_text);
	
	return 0;
}

//laod
int run_map()
{ //run all functions for map

	//display
	frame_map();
	//logic
	controls_map();

	return 0;
}

int run_battle()
{ //run all functions for map
	
	//display
	frame_battle();
	
	//logic
	controls_battle();

	return 0;
}

int start()
{ // start game

	//initiate all esential ellements
	init_variables();
	init_entities();
    init_map();

	//init random
	srand(time(NULL));

	//game loop
    while(g_run)
	{
		//clear screen
		clear();

		//next frame
		if ( g_battle_status )	//if battle status run battle 
			{ run_battle(); }
		else
			{ run_map(); }		//else run map
    }

    return 0;
}

int main()
{ //main entry point
	
    start();		//start game

    return 0;
}
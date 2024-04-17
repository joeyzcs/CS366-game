/*
 * File: 	game.c
 * Author: 	Denis Alisa, Joseph Stiehm
 * Description: Adventure game by Joseph Stiehm and Denis Alisa
 * Date:	3/19/2024
 ****************************************************************/


/* Preprocessor Directives */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 50
#define ROOMS_MAX 10
#define NEARBY_ROOMS_MAX 12
#define INV_SIZE_MAX 5

struct Player {
	char name[MAX_INPUT_SIZE];
	char* inventory[INV_SIZE_MAX];
};

/* Structure of a ROOM, What one room contains */
struct Room {
	char* name;
	char* description;
	char* altDescription;
	struct Room* nearbyRooms[NEARBY_ROOMS_MAX];
	char* items[INV_SIZE_MAX];
};

/* Structure of the map. Contains all rooms in the game */
struct Map {
	struct Room* currentRoom;

	struct Room roomsList[ROOMS_MAX];

	struct Player player;
};

/* Function Prototypes */
void printIntro();
void helpMenu();
void printNearby(struct Map*);
void gotoroom(struct Map*, char*);
void whereami(struct Map*);
void pickup(struct Map*);
void inventory(struct Map*);
void run(struct Map*);
void altDescCheck(struct Map*);

/* This function creates the definitions of all the rooms and returns the map pointer */
struct Map* initMap() {
	struct Map* map = malloc(sizeof(struct Map));

	/* DEFINE MAP ROOMS HERE 
		Room 0:		Start
		Room 1:		Room_1
					*/
	map->roomsList[0].name = "Cemetery_Entrance";
	map->roomsList[1].name = "Ancient_Ruins";
	map->roomsList[2].name = "Lost_Burials";
	map->roomsList[3].name = "Soul_Forest";
	map->roomsList[4].name = "Morgue";
	map->roomsList[5].name = "Morgue_Storage";
	map->roomsList[6].name = "Old_Chapel";

	
	/* Description of all rooms */

	map->roomsList[0].description = "\nYou are currently located at the start area of the game. The Cemetery_Entrance. Type 'headstone' to view the entire map.\n\n";

	map->roomsList[1].description = "\nYou are currently in the Ancient_Ruins. Centuries old graves of those who died many years ago lay to rest here. You notice a Damascus Steel Sword rested upon a tombstone. It could prove to be useful, you should pick it up.\n\n";

	map->roomsList[2].description = "\nYou are currently in the Lost_Burials. Those forgotten in history roam through these graves angry and undead. The angry undead have noticed you. They begin to swarm you. Do you run or fight?.\n\n";

	map->roomsList[3].description = "\nYou are currently in Soul_Forest. A heavily wooded area seperating the cemetery from the rest of the graveyard. Those who enter these woods rarely come out alive. You are surrouned by ghosts who crave a body to inhabit. Do you run or fight?.\n\n";

	map->roomsList[4].description = "\nYou are currently at the Morgue. An overgrown house that is home to those yet to be buried. You notice a padlock on the gate. There must be a key somewhere. Use the key to enter.\n\n";

	map->roomsList[5].description = "\nYou are inside the morgue storage room. It is filled witht the undead waiting for their time to be buried. You notice these undead look much stronger than the ones before. Do you run or fight?\n\n";

	map->roomsList[6].description = "\nYou are currently at the Old_Chapel. An old structure that appears it could collapse any second. You sense victory is near and a strong urge to fight overwhelms you. You enter the chapel and at the alter you see him. The final opponent. The skeleton. Do you run or fight?\n\n";


	/* Alternate descriptions of rooms [1], [3], and [4] */
	
	map->roomsList[1].altDescription = "You are currently in the Ancient_Ruins. Centuries old graves of those who died many years ago lay to rest here.\n\n";

	map->roomsList[3].altDescription = "You are currently in Soul_Forest. A heavily wooded area seperating the cemetery from the rest of the graveyard. Those who enter these woods rarely come out alive. Good luck.\n\n";

	map->roomsList[4].altDescription = "You are currently at the Morgue. An overgrown house that is home to those yet to be buried.\n\n";



	/* Set the items in each room */
	map->roomsList[1].items[0] = "Damascus Steel Sword";
	map->roomsList[3].items[1] = "Key";

	/* Set the nearby rooms of each room */
	map->roomsList[0].nearbyRooms[0] = &(map->roomsList[1]);
	map->roomsList[0].nearbyRooms[1] = &(map->roomsList[2]);

	map->roomsList[1].nearbyRooms[0] = &(map->roomsList[0]); 

	map->roomsList[2].nearbyRooms[0] = &(map->roomsList[0]);
	map->roomsList[2].nearbyRooms[1] = &(map->roomsList[1]);
	map->roomsList[2].nearbyRooms[2] = &(map->roomsList[3]);

	map->roomsList[3].nearbyRooms[0] = &(map->roomsList[2]);
	map->roomsList[3].nearbyRooms[1] = &(map->roomsList[4]);

	map->roomsList[4].nearbyRooms[0] = &(map->roomsList[2]);
	map->roomsList[4].nearbyRooms[1] = &(map->roomsList[3]);
	map->roomsList[4].nearbyRooms[2] = &(map->roomsList[5]);

	map->roomsList[5].nearbyRooms[0] = &(map->roomsList[4]);
	map->roomsList[5].nearbyRooms[1] = &(map->roomsList[6]);

	map->roomsList[6].nearbyRooms[0] = &(map->roomsList[5]);


	return map;
}

/* Start of MAIN function */
int main() {
	printIntro();

	struct Map* gameMap = initMap();
	char userInput[MAX_INPUT_SIZE];
	char* token;

	printf("Welcome to Graveyard Escape! You must fight your way through the undead to defeat the skeleton.\n\n");

	printf("\nEnter your character's name: ");
	if (fgets(gameMap->player.name, MAX_INPUT_SIZE, stdin) == NULL) {
		printf("Input error!\n");
		return 0;
	}
	gameMap->player.name[strlen(gameMap->player.name)-1] = '\0';

	printf("\nGood luck %s. Your adventure starts now.\n", gameMap->player.name);
	

	gameMap->currentRoom = &gameMap->roomsList[0];
	printf("%s", gameMap->currentRoom->description);

	while(1) {
		printf("> ");
		if(fgets(userInput, MAX_INPUT_SIZE, stdin) == NULL) {
			printf("Input error!\n");
			continue;
		}
		userInput[strlen(userInput)-1] = '\0';

		if(strcmp(userInput, "quit") == 0) {
			break;
		}

		if(strcmp(userInput, "help") == 0) {
			helpMenu();
		}

		if(strcmp(userInput, "nearby") == 0) {
			printNearby(gameMap);
		}

		if(strcmp(userInput, "whereami") == 0) {
			whereami(gameMap);
		}

		token = strtok(userInput, " ");
		if(strcmp(token, "goto") == 0) {
			token = strtok(NULL, " ");
			if(token != NULL) {
				gotoroom(gameMap, token);
			}
		}
		if(strcmp(token, "pickup") == 0) {
			pickup(gameMap);
		}
		if(strcmp(userInput, "inventory") == 0) {
			inventory(gameMap);
		}

		if(strcmp(userInput, "run") == 0) {
			run(gameMap);
		}
	}

	free(gameMap);
	return 0;
}

void pickup(struct Map* map) {
	int i, j;
	for(i = 0; i < INV_SIZE_MAX; i++) {
		if(map->currentRoom->items[i] != NULL) {
			for(j = 0; j < INV_SIZE_MAX; j++) {
				if(map->player.inventory[j] != map->currentRoom->items[i] && map->player.inventory[j] == NULL) {
					map->player.inventory[j] = map->currentRoom->items[i];
					break;
				}
			}
			printf("\nYou picked up a %s\n", map->currentRoom->items[i]);

		}
	}
	printf("\n");
}

void gotoroom(struct Map* map, char* room) {
	int i, j, k;
	for(i = 0; i < NEARBY_ROOMS_MAX; i++) {
		if(map->currentRoom->nearbyRooms[i] != NULL) {
			if(strcmp(map->currentRoom->nearbyRooms[i]->name, room) == 0)  {
				map->currentRoom = map->currentRoom->nearbyRooms[i];
			
				/* check for picked up items for alternate description */
				for(k = 0; k < INV_SIZE_MAX; k++) {
					if(map->currentRoom->items[k] != NULL) {
						for(j = 0; j < INV_SIZE_MAX; j++) {
							if(map->player.inventory[j] == map->currentRoom->items[k] && map->player.inventory[j] != NULL) {
								printf("\n%s\n", map->currentRoom->altDescription);
								return;
							}
						}
					}
				}
				printf("%s\n", map->currentRoom->description);
				return;
			}
		}
	}
	printf("\nThat area is not nearby!\n\n");
}

void printIntro() {
	printf("                           ___\n");
	printf("                          ( ((\n");
	printf("                           ) ))\n");
	printf("  .::.                    / /(\n");
	printf("'M .-;-.-.-.-.-.-.-.-.-/| ((::::::::::::::::::::::::::::::::::::::::::::::.._\n");
	printf("(J ( ( ( ( ( ( ( ( ( ( ( |  ))   -====================================-      _.>\n");
	printf(" `P `-;-`-`-`-`-`-`-`-`-\\| ((::::::::::::::::::::::::::::::::::::::::::::::''\n");
	printf("  `::'                    \\ \\(	\tWelcome to Graveyard Escape\n");
	printf("                           ) ))		Type 'help' to learn the commands\n");
	printf("                          (_((		By Joey and Denis\n");
	printf("##############################################################################\n\n");

}

void helpMenu() {
	printf("\nHelp Menu:\n"
			"\tnearby:	\tshows nearby locations\n"
			"\tgoto <room>:	enters a room\n"
			"\tpickup <item>:	picks up item\n"
			"\thelp:	\topens this menu\n"
			"\tquit:	\texits the game\n"
			"\twhereami:   \tdescribes your current location\n"
			"\tinventory:	displays contents of your inventory\n");
}

void printNearby(struct Map* map) {
	printf("\nNearby areas: ");
	int i;
	for(i = 0; i < NEARBY_ROOMS_MAX; i++) {
		if(map->currentRoom->nearbyRooms[i] != NULL) {
			printf("%s ", map->currentRoom->nearbyRooms[i]->name);
		}
	
	}
	printf("\n\n");
}

void whereami(struct Map* map) {

	altDescCheck(map);
}

void inventory(struct Map* map) {

	int i;
	if(map->player.inventory[0] == NULL) {
		printf("\nInventory is empty.\n");
	}
	for(i = 0; i < INV_SIZE_MAX; i++) {
		if(map->player.inventory[i] != NULL) {
			printf("\n%s ", map->player.inventory[i]);
		}
	}
	printf("\n\n");
}

void run(struct Map* map) {
	if(strcmp(map->currentRoom->name, map->roomsList[3].name) == 0 ) {
		map->currentRoom = &map->roomsList[2];
		altDescCheck(map);
		return;
	}


	if(strcmp(map->currentRoom->name, map->roomsList[2].name) == 0 ) {
		map->currentRoom = &map->roomsList[0];
		altDescCheck(map);
		return;
	}

	if(strcmp(map->currentRoom->name, map->roomsList[5].name) == 0 ) {
		map->currentRoom = &map->roomsList[4];
		altDescCheck(map);
		return;
	}

	if(strcmp(map->currentRoom->name, map->roomsList[6].name) == 0 ) {
		printf("\nYou make a dash for the chapel doors but they are shut by some mysterious force. You are trapped. You are dead.\n");
		map->currentRoom = &map->roomsList[0];
		altDescCheck(map);
		return;
	}


		
}

	/* REUSABLE CODE */
void altDescCheck(struct Map* map) {
	int j, k;

	/* check for picked up items for alternate description */
	for(k = 0; k < INV_SIZE_MAX; k++) {
		if(map->currentRoom->items[k] != NULL) {
			for(j = 0; j < INV_SIZE_MAX; j++) {
				if(map->player.inventory[j] == map->currentRoom->items[k] && map->player.inventory[j] != NULL) {
					printf("\n%s\n", map->currentRoom->altDescription);
					return;
				}
			}
		}
	}
	printf("%s\n", map->currentRoom->description);
	return;
}

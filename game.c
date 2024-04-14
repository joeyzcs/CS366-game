/****************************************************************
 * File: 	game.c
 * Author: 	Denis Alisa, Joseph Stiehm
 * Description: Adventure game by Joseph Stiehm and Denis Alisa
 * Date:	3/19/2024
 ****************************************************************/


/* Preprocessor Directives */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 20
#define MAX_NEARBY_ROOMS 4


/* Structure of a ROOM, What one room contains */
struct Room {
	char* name;
	char* description;
	char* nearbyRooms[MAX_NEARBY_ROOMS];
	char* items[4];
};

/* Structure of the map. Contains all rooms in the game */
struct Map {
	struct Room* currentRoom;

	struct Room roomsList[2];
};

/* Function Prototypes */
void printIntro();
void helpMenu();
void printNearby(struct Map*);

/* This function creates the definitions of all the rooms and returns the map pointer */
struct Map* initMap() {
	struct Map* map = malloc(sizeof(struct Map));

	map->roomsList[0].name = "Start";
	map->roomsList[0].description = "You are currently located in the start area of the game.\n";
	map->roomsList[0].nearbyRooms[0] = "Room 1";

	map->currentRoom = &map->roomsList[0];
	
	map->roomsList[1].name = "Room 1";
	map->roomsList[1].description = "This is the first room in the game.\n";

	return map;
}

/* Start of MAIN function */
int main() {
	printIntro();

	struct Map* gameMap = initMap();
	char userInput[MAX_INPUT_SIZE];

	printf("%s", gameMap->currentRoom->description);

	while(1) {
		printf("> ");
		if(fgets(userInput, MAX_INPUT_SIZE, stdin) == NULL) {
			printf("Input error!\n");
			continue;
		}
		userInput[strlen(userInput)-1] = '\0';

		if(!strcmp(userInput, "quit")) {
			break;
		}

		if(!strcmp(userInput, "help")) {
			helpMenu();
		}

		if(!strcmp(userInput, "nearby")) {
			printNearby(gameMap);
		}
	}

	free(gameMap);
	return 0;
}

void printIntro() {
	printf("                           ___\n");
	printf("                          ( ((\n");
	printf("                           ) ))\n");
	printf("  .::.                    / /(\n");
	printf("'M .-;-.-.-.-.-.-.-.-.-/| ((::::::::::::::::::::::::::::::::::::::::::::::.._\n");
	printf("(J ( ( ( ( ( ( ( ( ( ( ( |  ))   -====================================-      _.>\n");
	printf(" `P `-;-`-`-`-`-`-`-`-`-\\| ((::::::::::::::::::::::::::::::::::::::::::::::''\n");
	printf("  `::'                    \\ \\(\n");
	printf("                           ) ))\n");
	printf("                          (_((\n");
	printf("#	Welcome to Graveyard Escape\n"
		"#	Type 'help' to learn the commands\n"
		"##############################################################################\n\n");

}

void helpMenu() {
	printf("\nHelp Menu:\n"
			"\tnearby :	shows nearby locations\n"
			"\thelp :	\topens this menu\n"
			"\tquit :	\texits the game\n"
			"\t...\n\n");
}

void printNearby(struct Map* map) {
	printf("\nNearby rooms: ");
	int i;
	for(i = 0; i < MAX_NEARBY_ROOMS; i++) {
		if(map->currentRoom->nearbyRooms[i] != NULL) {
			printf("%s ", map->currentRoom->nearbyRooms[i]);
		}
	
	}
	printf("\n\n");
}

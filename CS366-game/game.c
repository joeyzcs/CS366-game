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
#define ROOMS_MAX 10
#define NEARBY_ROOMS_MAX 4
#define INV_SIZE_MAX 5


/* Structure of a ROOM, What one room contains */
struct Room {
	char* name;
	char* description;
	struct Room* nearbyRooms[NEARBY_ROOMS_MAX];
	char* items[INV_SIZE_MAX];
};

/* Structure of the map. Contains all rooms in the game */
struct Map {
	struct Room* currentRoom;

	struct Room roomsList[ROOMS_MAX];
};

/* Function Prototypes */
void printIntro();
void helpMenu();
void printNearby(struct Map*);

/* This function creates the definitions of all the rooms and returns the map pointer */
struct Map* initMap() {
	struct Map* map = malloc(sizeof(struct Map));

	/* DEFINE MAP ROOMS HERE 
		Room 0:		Start
		Room 1:		Room_1
					*/
	map->roomsList[0].name = "Start";
	map->roomsList[1].name = "Room_1";


	map->roomsList[0].description = "You are currently located in the start area of the game.\n";
	map->roomsList[1].description = "This is the first room in te game.\n";


	map->roomsList[0].nearbyRooms[0] = &(map->roomsList[1]);

	return map;
}

/* Start of MAIN function */
int main() {
	printIntro();

	struct Map* gameMap = initMap();
	char userInput[MAX_INPUT_SIZE];

	gameMap->currentRoom = &gameMap->roomsList[0];
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
	printf("  `::'                    \\ \\(	\tWelcome to Graveyard Escape\n");
	printf("                           ) ))		Type 'help' to learn the commands\n");
	printf("                          (_((		By Joey and Denis\n");
	printf("##############################################################################\n\n");

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
	for(i = 0; i < NEARBY_ROOMS_MAX; i++) {
		if(map->currentRoom->nearbyRooms[i] != NULL) {
			printf("%s ", map->currentRoom->nearbyRooms[i]->name);
		}
	
	}
	printf("\n\n");
}

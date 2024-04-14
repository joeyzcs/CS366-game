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

/* Structure of a ROOM, What one room contains */
struct Room {
	char* name;
	char* description;
	char* nearbyRooms[4];
	char* items[4];
};

/* Structure of the map. Contains all rooms in the game */
struct Map {
	struct Room currentRoom;

	struct Room start;
	struct Room test;
};

/* This function creates the definitions of all the rooms and returns the map pointer */
struct Map* initMap() {
	struct Map* map = malloc(sizeof(struct Map));

	map->start.name = "testing";
	map->start.description = "Description of the start map. Game begins here.\n";

	map->currentRoom = map->start;
	
	return map;
}

/* Start of MAIN function */
int main() {

	struct Map* gameMap = initMap();
	char userInput[MAX_INPUT_SIZE];

	printf("%s", gameMap->currentRoom.description);

	while(1) {
		if(fgets(userInput, MAX_INPUT_SIZE, stdin) == NULL) {
			printf("Input error!\n");
			continue;
		}
		userInput[strlen(userInput)-1] = '\0';

		if(!strcmp(userInput, "quit")) {
			break;
		}

		printf("%s\n", userInput);
	}

	free(gameMap);
	return 0;
}

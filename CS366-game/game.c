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
	int itemCount;
};

	/* Structure of a ROOM, What one room contains */
struct Room {
	char* name;
	char* description;
	char* altDescription;
	struct Room* nearbyRooms[NEARBY_ROOMS_MAX];
	char* items[INV_SIZE_MAX];
	int hasEnemy;
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
void headstoneMap();
void printNearby(struct Map*);
void gotoroom(struct Map*, char*, char* userInput);
void pickup(struct Map*, char*);
void inventory(struct Map*);
void run(struct Map*);
void fight(struct Map*, char* userInput);
void altDescCheck(struct Map*);
void undeadSkeleton();
void scaryGhost();
void strongSkeleton();
void flameSkeleton();
void defeatedSkeleton();
void credits();

	/* Creates the definitions of all the rooms and returns the map pointer */
struct Map* initMap() {
	struct Map* map = malloc(sizeof(struct Map));

	map->player.itemCount = 0;
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

	map->roomsList[2].hasEnemy = 1;
	map->roomsList[3].hasEnemy = 1;
	map->roomsList[5].hasEnemy = 1;
	map->roomsList[6].hasEnemy = 1;	


	
		/* Description of all rooms */

	map->roomsList[0].description = "\nYou are currently located at the start area of the game. The Cemetery_Entrance. Type 'headstone' to view the entire map or 'nearby' to view adjacent areas.\n\n";

	map->roomsList[1].description = "\nYou are currently in the Ancient_Ruins. Centuries old graves of those who died many years ago lay to rest here. You notice a sword rested upon a tombstone. It could prove to be useful, you should pick it up.\n\n";
	
	map->roomsList[2].description = "\nYou are currently at the Lost_Burials. Those forgotten in history roam this area angry and undead. They notice you and begin to swarm you. Do you run or fight?\n\n";

	map->roomsList[3].description = "\nYou are currently in the Soul_Forest. A heavily wooded area seperating the cemetery from the rest of the graveyard. Those who enter these woods rarely come out alive. You are surrouned by ghosts who crave a body to inhabit. Do you run or fight?.\n\n";

	map->roomsList[4].description = "\nYou are currently at the Morgue. An overgrown house that is home to those yet to be buried. You notice a padlock on the gate. There must be a key somewhere. Use the key to enter the Morgue_Storage.\n\n";

	map->roomsList[5].description = "\nYou are currently inside the morgue storage room. It is filled with the undead waiting for their time to be buried. You notice these undead look much stronger than the ones before. Do you run or fight?\n\n";

	map->roomsList[6].description = "\nYou are currently at the Old_Chapel. An old structure that appears it could collapse any second. You sense victory is near and a strong urge to fight overwhelms you. You enter the chapel and at the alter you see him. The final opponent. The Skeleton. Do you run or fight?\n\n";


		/* Alternate descriptions of specific rooms */
	
	map->roomsList[1].altDescription = "\nYou are currently in the Ancient_Ruins. Centuries old graves of those who died many years ago lay to rest here.\n\n";

	map->roomsList[2].altDescription = "\nYou are currently at the Lost_Burials. Those forgotten in history roam this area angry and undead.\n\n"; 
 
	map->roomsList[3].altDescription = "\nYou are currently in Soul_Forest. A heavily wooded area seperating the cemetery from the rest of the graveyard. Those who enter these woods rarely come out alive. Good luck.\n\n";

	map->roomsList[4].altDescription = "\nYou are currently at the Morgue. An overgrown house that is home to those yet to be buried.\n\n";

	map->roomsList[5].altDescription = "\nYou are currently inside the morgue storage room. It is filled with the undead waiting for their time to be buried.\n\n"; 
	map->roomsList[6].altDescription = "\nYou are currently at the Old_Chapel. An old structure that appears it could collapse any second.\n\n"; 


		/* Set the items in each room */
	map->roomsList[1].items[0] = "sword";
	map->roomsList[3].items[1] = "key";

		/* Set the nearby rooms of each room */
	map->roomsList[0].nearbyRooms[0] = &(map->roomsList[1]);
	map->roomsList[0].nearbyRooms[1] = &(map->roomsList[2]);

	map->roomsList[1].nearbyRooms[0] = &(map->roomsList[0]); 
	map->roomsList[1].nearbyRooms[1] = &(map->roomsList[2]); 

	map->roomsList[2].nearbyRooms[0] = &(map->roomsList[0]);
	map->roomsList[2].nearbyRooms[1] = &(map->roomsList[1]);
	map->roomsList[2].nearbyRooms[2] = &(map->roomsList[3]);

	map->roomsList[3].nearbyRooms[0] = &(map->roomsList[2]);
	map->roomsList[3].nearbyRooms[1] = &(map->roomsList[4]);

	map->roomsList[4].nearbyRooms[0] = &(map->roomsList[3]);
	map->roomsList[4].nearbyRooms[1] = &(map->roomsList[5]);

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

	printf("Welcome to Graveyard Escape! You must fight your way through the undead to defeat the Skeleton.\n\n");

	printf("\nEnter your character's name: ");
	if (fgets(gameMap->player.name, MAX_INPUT_SIZE, stdin) == NULL) {
		printf("Input error!\n");
		return 0;
	}

	gameMap->player.name[strlen(gameMap->player.name)-1] = '\0';

		/* Check if player input is valid for their name */
	while(strlen(gameMap->player.name) == 0) {
		printf("\nEnter a valid character name!\n\n");

		printf("\nEnter your character's name: ");
		if (fgets(gameMap->player.name, MAX_INPUT_SIZE, stdin) == NULL) {
			printf("\nInput error!\n\n");
			return 0;
		}

		gameMap->player.name[strlen(gameMap->player.name)-1] = '\0';
	}


	printf("\nGood luck %s. Your adventure starts now.\n", gameMap->player.name);
	

	gameMap->currentRoom = &gameMap->roomsList[0];
	printf("%s", gameMap->currentRoom->description);

	while(1) {
		printf("> ");
		if(fgets(userInput, MAX_INPUT_SIZE, stdin) == NULL) {
			printf("\nInput error!\n\n");
			return 0;
		}
		userInput[strlen(userInput)-1] = '\0';

			/* Check if player input is valid for commands */
		while(strlen(userInput) == 0) {
			printf("\nEnter a valid command!\n\n");

			printf("> ");
			if (fgets(userInput, MAX_INPUT_SIZE, stdin) == NULL) {
				printf("\nInput error!\n\n");
				return 0;
			}

			userInput[strlen(userInput)-1] = '\0';
		}


		if(strcmp(userInput, "quit") == 0) {
			break;
		}

		if(strcmp(userInput, "help") == 0) {
			helpMenu();
		}

		if(gameMap->currentRoom->hasEnemy == 1) {
			if((strcmp(userInput, "fight") != 0) && (strcmp(userInput, "run") != 0)) {
				printf("Must fight the enemy or run!\n\n");
				continue;
			}
		}		

		if(strcmp(userInput, "headstone") == 0) {
			headstoneMap();
		}

		if(strcmp(userInput, "nearby") == 0) {
			printNearby(gameMap);
		}

		if(strcmp(userInput, "credits") == 0) {
			credits();
		}

		token = strtok(userInput, " ");
		if(strcmp(token, "goto") == 0) {
			token = strtok(NULL, " ");

			if(token != NULL) {
				gotoroom(gameMap, token, userInput);
			}
		}

		if(strcmp(token, "pickup") == 0) {
			token = strtok(NULL, " ");
			if(token != NULL) {
				pickup(gameMap, token);
			}
			
		}

		if(strcmp(userInput, "inventory") == 0) {
			inventory(gameMap);
		}

		if(strcmp(userInput, "run") == 0) {
			run(gameMap);
		}

		if(strcmp(userInput, "fight") == 0) {
			fight(gameMap, userInput);
		}

	}

	free(gameMap);
	return 0;
}

	/* Pick up items throughout the game */
void pickup(struct Map* map, char* item) {

	if(map->player.itemCount >= INV_SIZE_MAX) {
		printf("\nYour inventory is full!\n\n");
		return;
	}
	int i;
	for(i = 0; i < INV_SIZE_MAX; i++) {
		if(map->currentRoom->items[i] != NULL) {
			if(strcmp(item, map->currentRoom->items[i]) == 0) {
				map->player.inventory[map->player.itemCount] = map->currentRoom->items[i];
				map->player.itemCount++;
				printf("\nYou picked up the: %s!\n\n", map->currentRoom->items[i]);
				map->currentRoom->items[i] = NULL;
				return;
			}
		}
	}
	printf("\nItem not found: %s!\n\n", item);
}

	/* Traverse between areas of the game */
void gotoroom(struct Map* map, char* room, char* userInput) {
	int i, j;
	for(i = 0; i < NEARBY_ROOMS_MAX; i++) {
		if(map->currentRoom->nearbyRooms[i] != NULL) {
			if(strcmp(map->currentRoom->nearbyRooms[i]->name, room) == 0)  {


					/* Check if player has 'key' in their inventory */
				int hasKey = 0;

				for(j = 0; j < INV_SIZE_MAX; j++) {
					if(map->player.inventory[j] != NULL) {
						if((strcmp(map->player.inventory[j], "key") == 0) || (strcmp(map->player.inventory[j], "key") == 0)) {
							hasKey = 1;
						}
					}
				}


					/* Check if player has the key to enter Morgue_Storage area */
				if(strcmp(room, map->roomsList[5].name) == 0) {	
						
					if(hasKey != 1) {
						printf("\nYou need a key!\n\n");
						return;
					}	
				}


				if(strcmp(room, map->roomsList[2].name) == 0) {	
					
					map->currentRoom = map->currentRoom->nearbyRooms[i];

					undeadSkeleton();

					printf("%s\n", map->currentRoom->description);
					return;

				}

				if(strcmp(room, map->roomsList[3].name) == 0) {	
	
					map->currentRoom = map->currentRoom->nearbyRooms[i];

					if(map->currentRoom->hasEnemy == 0 && hasKey == 0) {

						printf("\nYou are currently in Soul_Forest. A heavily wooded area seperating the cemetery from the rest of the graveyard. \n\nDon't forget the ghost dropped a key!\n\n");
						return;
					}

					else if(map->currentRoom->hasEnemy == 0 && hasKey == 1) {

						printf("%s\n", map->currentRoom->altDescription);
						return;
					}

					
					else {

						scaryGhost();

						printf("%s\n", map->currentRoom->description);
						return;
					}
				}

				if(strcmp(room, map->roomsList[4].name) == 0) {	
					
					map->currentRoom = map->currentRoom->nearbyRooms[i];

					if(hasKey == 0) {

						printf("%s\n", map->currentRoom->description);
						return;
					}
	
					else {
						
						printf("%s\n", map->currentRoom->altDescription);
						return;
					}

				} 	

				if(strcmp(room, map->roomsList[5].name) == 0) {	
					
					map->currentRoom = map->currentRoom->nearbyRooms[i];

					strongSkeleton();

					printf("%s\n", map->currentRoom->description);
					return;

				}     

				if(strcmp(room, map->roomsList[6].name) == 0) {	
					
					map->currentRoom = map->currentRoom->nearbyRooms[i];

					flameSkeleton();

					printf("%s\n", map->currentRoom->description);
					return;

				} 	

				map->currentRoom = map->currentRoom->nearbyRooms[i];


				if(strcmp(room, map->roomsList[0].name) == 0) {

					printf("%s\n", map->currentRoom->description);
					return;
				}

			
					/* Gives alternate description if sword has been picked up */
				if(map->currentRoom->items[0] == NULL) {


					printf("\n%s\n", map->currentRoom->altDescription);
					return;
				}

				else {
			
					printf("%s\n", map->currentRoom->description);
					return;
				}
	
			
					/* check if enemy has been fought for alternate description */
				if(map->currentRoom->hasEnemy == 1) { 
					printf("\n%s\n", map->currentRoom->description);
					return;
				}
			
				else {
			
					printf("%s\n", map->currentRoom->altDescription);
					return;
				}
			}
		}
	}
	printf("\nThat area is not nearby!\n\n");
}

	/* Prints introduction text/art */
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

	/* Prints help menu */
void helpMenu() {
	printf("\nHelp Menu:\n"
			"\tnearby:	\tshows nearby locations\n"
			"\tgoto <room>:	enters a room\n"
			"\tpickup <item>:	picks up item\n"
			"\thelp:	\topens this menu\n"
			"\tquit:	\texits the game\n"
			"\tinventory:	displays contents of your inventory\n"
			"\tcredits:	show credits\n\n");
}

	/* Print nearby areas of the game */
void printNearby(struct Map* map) {
	printf("\nNearby areas: ");
	int i;
	for(i = 0; i < NEARBY_ROOMS_MAX; i++) {
		if(map->currentRoom->nearbyRooms[i] != NULL) {
			printf("\n\n%s ", map->currentRoom->nearbyRooms[i]->name);
		}
	
	}
	printf("\n\n");
}

	/* Prints items in players inventory */
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

	/* Returns player to another area depending on current room if they choose not to fight */
void run(struct Map* map) {
	if(strcmp(map->currentRoom->name, map->roomsList[2].name) == 0 ) {
		map->currentRoom = &map->roomsList[0];
		altDescCheck(map);
		return;
	}


	if(strcmp(map->currentRoom->name, map->roomsList[3].name) == 0 ) {
		map->currentRoom = &map->roomsList[2];
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

	/* Prints description of the fight depending on area if player chooses to battle */
void fight(struct Map* map, char* userInput) {
	
	int hasSword = 0;
	int i;
	for(i = 0; i < INV_SIZE_MAX; i++) {
		if(map->player.inventory[i] != NULL) {
			if(strcmp(map->player.inventory[i], "sword") == 0) {
				hasSword = 1;
			}
		}
	}

	if((strcmp(map->currentRoom->name, map->roomsList[2].name) == 0) && (map->currentRoom->hasEnemy == 1) ) {
		if(hasSword == 0) {
			printf("\nHIT!\n");
			printf("HIT!\n");
			printf("\nYou were defeated by the undead... find a weapon to increase your odds!\n");
			printf("You were sent back to the start.\n\n");
			map->currentRoom = &map->roomsList[0];
			printf("%s\n", map->currentRoom->description);
			return;
		}

		else {
			printf("\nHIT!\n");
			printf("HIT!\n");
			printf("HIT!\n");
			printf("HIT!\n");
			printf("HIT!\n");
			printf("HIT!\n\n");
			printf("You have come out victorious against the undead. Your sword cut through them like butter. Good job!\n\n");
			map->currentRoom->hasEnemy = 0;
			return;
		}
	}

	if(strcmp(map->currentRoom->name, map->roomsList[3].name) == 0 ) {
		printf("\nHIT!\n");
		printf("HIT!\n");
		printf("HIT!\n");
		printf("HIT!\n");
		printf("MISS!\n");
		printf("MISS!\n");
		printf("HIT!\n\n");
		printf("You have elmininated the ghosts who swarmed you. One of them drops a key. Could be useful, you should pick it up.\n\n");	
		map->currentRoom->hasEnemy = 0;
		return;
	}

	if(strcmp(map->currentRoom->name, map->roomsList[5].name) == 0) {
		printf("\nHIT!\n");
		printf("HIT!\n");
		printf("MISS!\n");
		printf("MISS!\n");
		printf("MISS!\n");
		printf("MISS!\n\n");
		printf("You have been injured by the stronger undead. Do you run or fight?\n\n");

		printf("> ");
		
		if(fgets(userInput, MAX_INPUT_SIZE, stdin) == NULL) {
			printf("Input error!\n");
			return;
		}
		userInput[strlen(userInput)-1] = '\0';

		if(strcmp(userInput, "fight") == 0) {
			
			printf("\nMISS!\n");
			printf("HIT!\n");
			printf("HIT!\n");
			printf("HIT!\n");
			printf("HIT!\n");
			printf("HIT!\n\n");
			printf("You have killed the already dead. Nice. You notice your sword is glowing now. The stronger undead seemed to have strenghtened your sword.");
		
			map->player.inventory[0] = "Enchanted sword\n";
			printf("\n\nCheck your inventory!\n\n");
			map->currentRoom->hasEnemy = 0;
			return;
		}

		if(strcmp(userInput, "run") == 0) {
			run(map);
		}

	}

	if(strcmp(map->currentRoom->name, map->roomsList[6].name) == 0) {
		printf("\nHIT!\n");
		printf("HIT!\n");
		printf("HIT!\n");
		printf("MISS!\n");
		printf("MISS!\n");
		printf("MISS!\n");
		printf("MISS!\n");
		printf("MISS!\n\n");
		printf("The Skeleton is stronger and faster than you. He seems to know your every move. Do you run or keep fighting?\n\n");

		printf("> ");
		
		if(fgets(userInput, MAX_INPUT_SIZE, stdin) == NULL) {
			printf("Input error!\n");
			return;
		}
		userInput[strlen(userInput)-1] = '\0';

		if(strcmp(userInput, "fight") == 0) {

			printf("\nHIT!\n");
			printf("MISS!\n");
			printf("MISS!\n");
			printf("MISS!\n");
			printf("MISS!\n");
			printf("MISS!\n");
			printf("HIT!\n");
			printf("MISS!\n\n");
			printf("The Skeleton is eating your hits like its breakfast, lunch, and dinner! You miss and his attack throws you back. Your sword is flung to the other side of the chapel. You've gotten this far already. Do you run or fight to the death?\n\n");
		}

		printf("> ");
		
		if(fgets(userInput, MAX_INPUT_SIZE, stdin) == NULL) {
			printf("Input error!\n");
			return;
		}
		userInput[strlen(userInput)-1] = '\0';


		if(strcmp(userInput, "fight") == 0) {

			printf("\nYou stand up, ready to fight again. The Skeleton makes a lunge at you. Just as your life flashes before your eyes, your enchanted sword levitates behind the Skeleton. It rips through the air at lightening speed and rips apart the Skeleton bone by bone. You have been saved by some mysterious force. You have won.\n\n\n\n");
	
			defeatedSkeleton();
	
			printf("CONGRATULATIONS! %s you have reached the end of the game. Thank you for playing.\n\n", map->player.name);
			map->currentRoom->hasEnemy = 0;

		}
		
		if(strcmp(userInput, "run") == 0) {
			run(map);
		}
	}
}

	/* Prints a map of all the areas */
void headstoneMap() {
	printf("\n\n\n");

	printf("       Cemtery_Entrance\n");
    	printf("             / \\\n");
    	printf("            /   \\\n");
    	printf("Ancient_Ruins <--> Lost_Burials\n");
    	printf("                     / \n");
    	printf("                    /   \n");
    	printf("             Soul_Forest <--> Morgue\n");
    	printf("                               |\n");
    	printf("                         Morgue_Storage\n");
    	printf("                               |\n");
    	printf("                           Old_Chapel\n\n\n");
}

	/* REUSABLE CODE for the alternate desciption of areas*/
void altDescCheck(struct Map* map) {
	
	if(strcmp(map->currentRoom->name, map->roomsList[0].name) == 0 ) {
		printf("\n%s\n", map->currentRoom->description); 
		return;
	}

	if(strcmp(map->currentRoom->name, map->roomsList[2].name) == 0 ) {
		printf("\n%s\n", map->currentRoom->altDescription); 
		return;
	}

 	if(strcmp(map->currentRoom->name, map->roomsList[4].name) == 0 ) {
		printf("\n%s\n", map->currentRoom->altDescription); 
		return;
	}

}

void credits() {
			printf("\n\n************************************************************************************\n");
			printf("******************                                          ************************\n");
			printf("****************** CREDIT GOES TO asciiart.eu FOR ASCII ART ************************\n");
			printf("******************                                          ************************\n");
			printf("************************************************************************************\n\n");

}

	/* ASCII art of undead skeleton */
void undeadSkeleton() {

        printf("\n\n");
        printf("                              _.--\"\"-._\n");
        printf("  .                         .\"         \".\n");
        printf(" / \\    ,^.         /(     Y             |      )\\\n");
        printf("/   `---. |--'\\    (  \\__..'--   -   -- -'\"\"-.-'  )\n");
        printf("|        :|    `>   '.     l_..-------.._l      .'\n");
        printf("|      __l;__ .'      \"-.__.||_.-'v'-._||`\"----\"\n");
        printf(" \\  .-' | |  `              l._       _.'\n");
        printf("  \\/    | |                   l`^^'^^'j\n");
        printf("        | |                _   \\_____/     _\n");
        printf("        j |               l `--__)-'(__.--' |\n");
        printf("        | |               | /`---``-----'\"1 |  ,-----.\n");
        printf("        | |               )/  `--' '---'   \\'-'  ___  `-.                                             Skeletons Appear!\n");
        printf("      _ L |_            //  `-.-.'`-----'  /  /  |   |  `. \\                                          'run' or 'fight'!\n");
        printf("     '._' / \\         _/(   `/   )- ---' ;  /__.J   L.__.\\ :\n");
        printf("      `._;/7(-.......'  /        ) (     |  |            | |\n");
        printf("      `._;l _'--------_/        )-'/     :  |___.    _._./ ;\n");
        printf("        | |                 .__ )-'\\  __  \\  \\  I   1   / /\n");
        printf("        `-'                /   `-\\-(-'   \\ \\  `.|   | ,' /\n");
        printf("                           \\__  `-'    __/  `-. `---'',-'\n");
        printf("                              )-._.-- (        `-----'\n");
        printf("                             )(  l\\ o ('..-.\n");
        printf("                       _..--' _'-' '--'.-. |\n");
        printf("                __,,-'' _,,-''            \\ \\\n");
        printf("               f'. _,,-'                   \\ \\\n");
        printf("              ()--  |                       \\ \\\n");
        printf("                \\   |                       /  \\\n");
        printf("                  \\ \\                      |._  |\n");
        printf("                   \\ \\                     |  ()|\n");
        printf("                    \\ \\                     \\  /\n");
        printf("                     ) `-.                   | |\n");
        printf("                    // .__)                  | |\n");
        printf("                 _.//7'                      | |\n");
        printf("               '---'                         j_| `\n");
        printf("                                            (| |\n");
        printf("                                             |  \\\n");
        printf("                                             |lllj\n");
        printf("                                             |||||\n\n\n");

}

        /* ASCII art of ghost */
void scaryGhost() {

        printf("\n\n");
        printf("     .-.\n");
        printf("   .'   `.\n");
        printf("   :g g   :\n");
        printf("   : o    `.\n");
        printf("  :         ``.                                            The Ghost Approaches!\n");
        printf(" :             `.                                            'run' or 'fight'!\n");
        printf(":  :         .   `.\n");
        printf(":   :          ` . `.\n");
        printf(" `.. :            `. ``;\n");
        printf("    `:;             `:'\n");
        printf("       :              `.\n");
        printf("        `.              `.     .\n");
        printf("          `'`'`'`---..,___`;.-'\n\n");

}

       /* ASCII art of strong skeleton */
void strongSkeleton() {

        printf("\n\n");
        printf("             .7\n");
        printf("            .'/\n");
        printf("           / /\n");
        printf("          / /\n");
        printf("         / /\n");
        printf("        / /\n");
        printf("       / /\n");
        printf("      / /\n");
        printf("     / /\n");
        printf("    / /\n");
        printf("  __|/\n");
        printf(",-\\__\\\n");
        printf("|f-\"Y\\|\n");
        printf("\\()7L/\n");
        printf(" cgD                            __ _\n");
        printf(" |\\(                          .'  Y '>,\n");
        printf("  \\ \\                        / _   _   \\                                            Skeleton Boss Appears!\n");
        printf("   \\\\                       )(_) (_)(|}                                               'run' or 'fight'!\n");
        printf("    \\\\                      {  4A   } /\n");
        printf("     \\\\                      \\uLuJJ/\\l\n");
        printf("      \\\\                     |3    p)/\n");
        printf("       \\\\___ __________      /nnm_n//\n");
        printf("       c7___-__,__-),__)('.  \\_>-<_/D\n");
        printf("                  //V     \\_\"-._.__G G_c__.-__<\"/ ( \\\n");
        printf("                         <\"-._>__-,G_.___)\\   \\7\\\n");
        printf("                        (\"-.__.| \"<.__.-\" )   \\ \\\n");
        printf("                        |\"-.__\"\\  |\"-.__.-\".\\   \\ \\\n");
        printf("                        (\"-.__\"\". \\\"-.__.-\".|    \\_\\\n");
        printf("                         \"-.__\"\"\\_|\"-.__.-\"./      \\ l\n");
        printf("                          \".__\"\">G>-.__.-\">       .--,_\n\n\n");

}

	/*ASCII art of Skeleton */
void flameSkeleton() {

        printf("\n\n");
        printf("                 .            )        )\n");
        printf("                  (  (|              .\n");
        printf("              )   )\\/ ( ( (          \n");
        printf("      *  (   ((  /     ))\\))  (  )    )\n");
        printf("    (     \\   )\\(          |  ))( )  (|\n");
        printf("    >)     ))/   |          )/  \\((  ) \"\n");
        printf("    (     (      .        -.     V )/   )(    (                                            Flame Skeleton Appears!\n");
        printf("     \\   /     .   \\            .       \\))   ))                                             'run' or 'fight'!\n");
        printf("       )(      (  | |   )            .    (  /\n");
        printf("      )(    ,'))     \\ /          \\( `.    )\n");
        printf("      (\\>  ,'/__      ))            __`.  /\n");
        printf("     ( \\   | /  ___   ( \\/     ___   \\ | ( (\n");
        printf("      \\.)  |/  /   \\__      __/   \\  \\ \\|  )))\n");
        printf("     .  \\. |>  \\      | __ |      /   <|  /\n");
        printf("          )/    \\____/ :..: \\____/     \\ <\n");
        printf("   )   \\ (|__  .      / ;: \\          __| )  (\n");
        printf("  ((    )\\)  ~--_     --  --      _--~    /  ))\n");
        printf("   \\    (    |  ||               ||  |   (  /\n");
        printf("         \\.  |  ||_             _||  |  /\n");
        printf("           > :  |  ~V+-I_I_I-+V~  |  : (.\n");
        printf("          (  \\:  T\\   _     _   /T  : ./\n");
        printf("           \\  :    T^T T-+-T T^T    ;<\n");
        printf("            \\..`_       -+-       _'  )\n");
        printf("  )            . `--=.._____..=--'. ./  \n\n");

}

        /* ASCII art of defeated skeleton */
void defeatedSkeleton() {

        printf("\n\n");
        printf("                          ,--.\n");
        printf("                          {    }\n");
        printf("                          K,   }\n");
        printf("                         /  `Y`\n");
        printf("                    _   /   /\n");
        printf("                   {_'-K.__/\n");
        printf("                     `/-.__L._\n");
        printf("                     /  ' /`\\_}\n");
        printf("                    /  ' /                  \n");
        printf("            ____   /  ' /\n");
        printf("     ,-'~~~~    ~~/  ' /_\n");
        printf("   ,'             ``~~~%%',\n");
        printf("  (                     %%  Y\n");
        printf(" {                      %% I\n");
        printf("{      -                 %%  `.\n");
        printf("|       ',                %%  )\n");
        printf("|        |   ,..__      __. Y\n");
        printf("|    .,_./  Y ' / ^Y   J   )|\n");
        printf("\\           |' /   |   |   ||\n");
        printf(" \\          L_/    . _ (_,.'(\n");
        printf("  \\,   ,      ^^\"\"' / |      )\n");
        printf("    \\_  \\          /,L]     /\n");
        printf("      '-_`-,       ` `   ./'  \n");
        printf("         `-(_            )\n");
        printf("             ^^\\..___,.--`\n\n\n");

}

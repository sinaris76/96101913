#include <stdio.h>
#include <stdbool.h>

#include "input.h"
#include "models.h"

int listener() {
    int direction = DIR_NONE;
    SDL_Event event;
    while (SDL_PollEvent(&event));
        SDL_Keycode key = event.key.keysym.sym;
        switch (key) {
            case SDLK_UP:
                direction = DIR_UP;
                break;
            case SDLK_RIGHT:
                direction = DIR_RIGHT;
                break;
            case SDLK_DOWN:
                direction = DIR_DOWN;
                break;
            case SDLK_LEFT:
                direction = DIR_LEFT;
                break;
            default:
                break;
        }
    return direction;
}

void initialize(Game *game) {
    FILE *map_txt;
    map_txt = fopen("map.txt", "r");

    if (map_txt == NULL) {
        printf("Damn The map file does\'t exists!\n");
        return;
    }
    fscanf(map_txt, "%d %d\n", &game->stage.n, &game->stage.m);
    game->stage.numberOfFoods = 0;
    for (int i = 0; i < game->stage.n; ++i) {
        for (int j = 0; j < game->stage.m; ++j) {
            fscanf(map_txt, "%c", &game->stage.tiles[i][j]);
            if (game->stage.tiles[i][j] == CHEESE || game->stage.tiles[i][j] == PINEAPPLE)
                game->stage.numberOfFoods++;
        }
        fscanf(map_txt, "\n");
    }
    game->stage.roomNumber = 1;
    pacmanInit(&game->pacman, map_txt);
    ghostInit(&game->blinky, map_txt, CHARACTER_BLINKY);
    ghostInit(&game->pinky, map_txt, CHARACTER_PINKY);
    ghostInit(&game->clyde, map_txt, CHARACTER_CLYDE);
    ghostInit(&game->inky, map_txt, CHARACTER_INKY);
    fclose(map_txt);
}

void pacmanInit(Pacman *pacman, FILE *map_txt) {
    fscanf(map_txt, "%d %d",
           &pacman->coordinates.startPosition.x,
           &pacman->coordinates.startPosition.y);
    pacman->hearts = 3;
    pacman->coordinates.currentPosition.x = pacman->coordinates.startPosition.x;
    pacman->coordinates.currentPosition.y = pacman->coordinates.startPosition.y;
    pacman->coordinates.direction = DIR_NONE;
    pacman->coordinates.cyclesPerMove = PACMAN_CYCLES_PER_MOVE;
    pacman->coordinates.isDefensive = false;
    pacman->coordinates.waitedCycles = 0;
    pacman->coordinates.characterType = CHARACTER_PACMAN;
    characterCoordinatesToPixels(&pacman->coordinates);
}

void ghostInit(Ghost *ghost, FILE *map_txt, int ghostType) {
    fscanf(map_txt, "%d %d",
           &ghost->coordinates.startPosition.x,
           &ghost->coordinates.startPosition.y);
    ghost->coordinates.currentPosition.x = ghost->coordinates.startPosition.x;
    ghost->coordinates.currentPosition.y = ghost->coordinates.startPosition.y;
    ghost->defensiveCyclesLeft = 0;
    ghost->coordinates.direction = DIR_NONE;
    ghost->coordinates.cyclesPerMove = GHOST_AGGRESSIVE_CYCLES_PER_MOVE;
    ghost->coordinates.isDefensive = false;
    ghost->coordinates.waitedCycles = 0;
    ghost->coordinates.characterType = ghostType;
    characterCoordinatesToPixels(&ghost->coordinates);
}

void characterCoordinatesToPixels(Coordinates *coordinates) {
    coordinates->currentPosition.x *= TILE;
    coordinates->currentPosition.y *= TILE;
    coordinates->startPosition.x *= TILE;
    coordinates->startPosition.y *= TILE;
}
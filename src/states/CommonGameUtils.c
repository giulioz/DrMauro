/*
 *  CommonGameUtils.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "CommonGameUtils.h"

bool isVirusPresent(GameBoard *board, GameBoardElementColor virusColor) {
	size_t i;
	Vector_foreach(board->board, i) {
		GameBoardElement *element = VT(board->board)->base.get((const struct Vector *) &board->board, i);
		if (element->color == virusColor && element->type == GameBoardElement_Virus)
			return true;
	}
	return false;
}

void drawVirus(Screen* screen, uint32_t row, uint32_t col, GameBoardElementColor color) {
    Graphics *graphics = VTP(screen)->getGraphics(screen);
    uint32_t x, y, frame;
    Sprite virusSprite = { 0 };

    x = (uint32_t) (95 + col * (Asset_VirusRed.spriteWidth - 1));
    y = (uint32_t) (199 - row * (Asset_VirusRed.spriteHeight - 1));
    frame = (uint32_t) ((int)(VTP(screen)->getCurrentTime(screen) * 0.005) % 2);

    switch (color) {
        case GameBoardElement_Red:
            Sprite_init(&virusSprite, screen, &Asset_VirusRed, x, y, 0);
            frame += 4;
            break;
        case GameBoardElement_Blue:
            Sprite_init(&virusSprite, screen, &Asset_VirusBlue, x, y, 0);
            break;
        case GameBoardElement_Yellow:
            Sprite_init(&virusSprite, screen, &Asset_VirusYellow, x, y, 0);
            frame += 2;
            break;
        default: break;
    }

    VT(virusSprite)->drawFrame(&virusSprite, screen, graphics, frame);
}

Texture* getPillTexture(GameBoardElementColor color, PillType type) {
    switch (color) {
        case GameBoardElement_Red:
            switch (type) {
                case Left: return &Asset_PillRed_A;
                case Right: return &Asset_PillRed_B;
                case Top: return &Asset_PillRed_AR;
                case Bottom: return &Asset_PillRed_BR;
                default: return &Asset_PillRed;
            }
        case GameBoardElement_Blue:
            switch (type) {
                case Left: return &Asset_PillBlue_A;
                case Right: return &Asset_PillBlue_B;
                case Top: return &Asset_PillBlue_AR;
                case Bottom: return &Asset_PillBlue_BR;
                default: return &Asset_PillBlue;
            }
        case GameBoardElement_Yellow:
            switch (type) {
                case Left: return &Asset_PillYellow_A;
                case Right: return &Asset_PillYellow_B;
                case Top: return &Asset_PillYellow_AR;
                case Bottom: return &Asset_PillYellow_BR;
                default: return &Asset_PillYellow;
            }
        default:
            return Asset_Mario.texture; /* lol wtf */
    };
}

void drawPill(Screen* screen, uint32_t row, uint32_t col, GameBoardElementColor color, PillType type) {
    Graphics *graphics = VTP(screen)->getGraphics(screen);
    uint32_t x, y;
    Texture *pillTexture = getPillTexture(color, type);

    x = (uint32_t) (96 + col * (Asset_PillRed.width + 1));
    y = (uint32_t) (200 - row * (Asset_PillRed.height + 1));

    VTP(graphics)->drawTexture(graphics, pillTexture,
                               x, y, x + pillTexture->width, y + pillTexture->height,
                               0, 0, pillTexture->width, pillTexture->height);
}

PillType checkPillNeighborhoods(Vector2D *board, uint32_t x, uint32_t y) {
    GameBoardElement *celem = VTP(board)->get2D(board, y, x);
    GameBoardElement *pelem = NULL, *nelem = NULL;
    GameBoardElement *telem = NULL, *belem = NULL;

    if (x > 0) {
        pelem = VTP(board)->get2D(board, y, x - 1);
    }
    if (x < board->width - 1) {
        nelem = VTP(board)->get2D(board, y, x + 1);
    }
    if (y > 0) {
        belem = VTP(board)->get2D(board, y - 1, x);
    }
    if (y < board->height - 1) {
        telem = VTP(board)->get2D(board, y + 1, x);
    }

    if (pelem && celem->id == pelem->id && pelem->type == celem->type) {
        return Right;
    } else if (nelem && celem->id == nelem->id && nelem->type == celem->type) {
        return Left;
    } else if (telem && celem->id == telem->id && telem->type == celem->type) {
        return Bottom;
    } else if (belem && celem->id == belem->id && belem->type == celem->type) {
        return Top;
    } else {
        return Normal;
    }
}

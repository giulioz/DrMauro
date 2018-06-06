/*
 *  Assets.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef ASSETS_H
#define ASSETS_H

#include "Texture.h"
#include "Object.h"
#include "Font.h"
#include "FragmentedPanel.h"
#include "Box.h"
#include "Palette.h"
#include "Sprite.h"
#include "Bool.h"

extern Font Asset_DefaultFont;

extern FragmentedPanel Asset_PanelSmall, Asset_PanelLarge, Asset_PanelBottle;
extern Box Asset_MarioBox, Asset_EndBox, Asset_MainMenuBox;
extern Texture Asset_MagVirus, Asset_Logo, Asset_BigPill;
extern SpriteClass Asset_Heart, Asset_BigTitle;
extern SpriteClass Asset_Mario, Asset_VirusLargeBlue, Asset_VirusLargeYellow, Asset_VirusLargeRed;

extern SpriteClass Asset_VirusBlue, Asset_VirusRed, Asset_VirusYellow;
extern Texture Asset_PillBlue, Asset_PillRed, Asset_PillYellow;
extern Texture Asset_PillBlue_A, Asset_PillRed_A, Asset_PillYellow_A;
extern Texture Asset_PillBlue_B, Asset_PillRed_B, Asset_PillYellow_B;
extern Texture Asset_PillBlue_AR, Asset_PillRed_AR, Asset_PillYellow_AR;
extern Texture Asset_PillBlue_BR, Asset_PillRed_BR, Asset_PillYellow_BR;

extern Palette Asset_MedPalette, Asset_MenuPalette;

#endif

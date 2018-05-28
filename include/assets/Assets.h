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
extern Box Asset_MarioBox, Asset_EndBox;
extern Texture Asset_Bottle, Asset_MagVirus, Asset_Logo;
extern SpriteClass Asset_Mario, Asset_VirusLargeBlue, Asset_VirusLargeYellow, Asset_VirusLargeRed;

extern Palette Asset_DefaultPalette;

#endif

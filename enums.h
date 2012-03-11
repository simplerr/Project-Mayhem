#ifndef ENUMS_H
#define ENUMS_H

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// Enum for different type of slots
enum SlotId {WEAPON, SHIELD, HEAD, CHEST, LEGS, MISC1, MISC2, BAG, SKILL};

// Tile types
enum TileType
{
	BACKGROUND_TILE,
	GRASS_TILE,
	WALL_TILE
};

// Identifies each window component
enum wId
{
	WID_STANDARD,
	WID_TESTBUTTON1,
	WID_TESTBUTTON2,
	WID_PANEL1,
	WID_OBJECTTYPE,
	WID_BUTTONCONTAINER,
	WID_TILE_BUTTON,
	WID_OBJECT_BUTTON
};

#endif
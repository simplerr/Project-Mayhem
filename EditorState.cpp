#include "EditorState.h"
#include "Graphics.h"
#include "Level.h"
#include "Input.h"
#include "Game.h"
#include "MenuState.h"
#include "Editor.h"
#include "enums.h"

EditorState EditorState::mEditorState;

void EditorState::init(Game *game)
{
	useCamera(true);
	// Load the background texture, temporary
	mBgkd = gGraphics->loadTexture("Data\\imgs\\dust_bkgd.bmp");

	mEditor = new Editor();

	// Important when we later need to switch state
	setGame(game);
}

void EditorState::cleanup()
{
	// Delete and release everything
	delete mEditor;
	ReleaseCOM(mBgkd);
}

void EditorState::pause()
{

}
	
void EditorState::resume()
{

}

void EditorState::update(double dt)
{
	// Update all objects in the level
	mEditor->update(dt);

	if(gInput->keyPressed(VK_ESCAPE))
		changeState(MenuState::Instance());
}
	
void EditorState::draw()
{
	// Draw the background
	gGraphics->drawTexture(mBgkd, 400, 300, 800, 600);

	// Draw all objects in the level
	mEditor->draw();
}

void EditorState::handleEvents(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// For windows messages
	// Pass to what needs it
}
#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include "GameState.h"
#include "enums.h"

class Editor;

class EditorState : public GameState
{
public:
	void init(Game* game);
	void cleanup();

	void pause();
	void resume();

	void update(double dt);
	void draw();
	void handleEvents(UINT msg, WPARAM wParam, LPARAM lParam);

	static EditorState* Instance() {
		return &mEditorState;
	}
protected:
	EditorState() {};
private:
	static EditorState mEditorState;
	IDirect3DTexture9* mBgkd;
	Editor* mEditor;
};	// Class

#endif
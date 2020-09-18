#pragma once
#include "XPLMDisplay.h"
class MouseHandler
{
public:
	static int DummyMouseClickHandler(XPLMWindowID in_window_id, int x, int y, int is_down, void* inRefcon) { return 1; }
	static int DummyWheelHandler(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void* inRefcon) { return 1; }
	static int MouseClickHandler(XPLMWindowID in_window_id, int x, int y, int is_down, void* inRefcon);
	static XPLMCursorStatus DummyCursorStatus(XPLMWindowID in_window_id, int x, int y, void* inRefcon) { return xplm_CursorDefault; }
};

class KeyHandler
{

};
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include <string>

#if IBM
#include <windows.h>
#endif
#if LIN
#include <GL/gl.h>
#elif __GNUC__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifndef XPLM300
#error This is made to be compiled against the XPLM300 SDK
#endif

//set up of a basic window before the required callbacks

static XPLMWindowID my_window; //set the window ID to the name

/*
callbacks that will be registered when the window is created in the XPLUGINSTART. function names are user specific.
any created function must be defined later in the code to handle the inputs, there is no hidden functionality
the created callback function inputs have to match the corresponding API function you want to use, or bad things happen
*/

void	draw_hello_world(XPLMWindowID in_window_id, void* inRefcon);
//XPLMDrawWindow_f draw_hello_world(XPLMWindowID in_window_id, void* inRefcon) equivalent, in this case we ignore the refcon

int		dummy_mouse_handler(XPLMWindowID in_window_id, int x, int y, int is_down, void* inRefcon) { return 0; }

//XPLMHandleMouseClick_f dummy_mouse_handler. we aren't handling mouse clicks in this window so we wont define the function later
//return 0 to pass the click through

XPLMCursorStatus dummy_cursor_status_handler(XPLMWindowID in_window_id, int x, int y, void* inRefcon) { return xplm_CursorDefault; }
/* we have to use XPLMCursor status because it's own type. we return xplm_cursordefault so xplane handles the cursor on it's own
without additional handling */

int dummy_wheel_handler(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void* inRefcon) { return 1; }
//XPLMHandleMouseWheel_f equivalent, returns 1 to comsume the mouse wheel rather than 0 to pass onto a lower window

void dummy_key_handler(XPLMWindowID in_window_id, char key, XPLMKeyFlags, char virtual_key, void* inRefcon, int losing_focus) { }
//XPLMHandleKey_f equivalent

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) //points to 3 human readable buffers addresses
{
	strcpy(outName, "Hello World Project 1");
	strcpy(outSig, "mv.project1.helloworld");
	strcpy(outDesc, "My first plugin to make a hello world window in xplane.");

	XPLMCreateWindow_t params; //local variable params struct created
	params.structSize = sizeof(params);
	params.visible = 1;
	params.drawWindowFunc = draw_hello_world; //we tell XPLMCreateWindow_t to use the draw function callback we defined earlier
	params.handleMouseClickFunc = dummy_mouse_handler;
	params.handleRightClickFunc = dummy_mouse_handler;
	params.handleMouseWheelFunc = dummy_wheel_handler;
	params.handleKeyFunc = dummy_key_handler;
	params.handleCursorFunc = dummy_cursor_status_handler;
	params.refcon = NULL;
	params.layer = xplm_WindowLayerFloatingWindows; //tell the sim to make the window float over the sim like other native xplane windows
	params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle; //decorate the window like native xplane windows

	int left, bottom, right, top; //create the screen bounds variables
	XPLMGetScreenBoundsGlobal(&left, &top, &right, &bottom); //reference the values of the screenbounds function output to the ints created earlier
	params.left = left + 50;
	params.bottom = bottom + 150;
	params.right = params.left + 200;
	params.top = params.bottom + 200;

	my_window = XPLMCreateWindowEx(&params); //pass in the param struct 
	
	XPLMSetWindowPositioningMode(my_window, xplm_WindowPositionFree, -1); //sets my_window to be movable by user and on monitor idx -1 which is the xpl window
	XPLMSetWindowResizingLimits(my_window, 200, 200, 300, 300);
	XPLMSetWindowTitle(my_window, "Hello World Window");

	return my_window != NULL; //returns a value for the start call back
	
}

PLUGIN_API void XPluginStop(void)
{
	XPLMDestroyWindow(my_window); //when the stop call back is reached, the window is destroyed and the window values are null
	my_window = NULL;
}

PLUGIN_API void XPluginDisable(void) {} //we would use this to deallocate resources not used when the plugin is disabled

PLUGIN_API int XpluginEnable(void) { return 1; }

PLUGIN_API void XpluginRecieveMessage(XPLMPluginID inFrom, int inMSG, void* inParam) {}

void draw_hello_world(XPLMWindowID in_window_id, void* inRefcon)
{
	// Mandatory: We *must* set the OpenGL state before drawing
	// (we can't make any assumptions about it)
	XPLMSetGraphicsState(
		0 /* no fog */,
		0 /* 0 texture units */,
		0 /* no lighting */,
		0 /* no alpha testing */,
		1 /* do alpha blend */,
		1 /* do depth testing */,
		0 /* no depth writing */
	);

	int l, t, r, b;
	XPLMGetWindowGeometry(in_window_id, &l, &t, &r, &b); //this gets the size and position of the window and stores them in the variables

	char my_str[]{ "Hello World" };
	float color_white[]{ 1.0,1.0,1.0 }; //RGB
	XPLMDrawString(color_white, l + 10, t - 20, my_str, NULL, xplmFont_Proportional); //here we draw the hello world statement
}

#include "Input.h"

Input * GInput = NULL;

bool ZATON_INPUT_Init() {
	GInput = new Input();
	
	bool ret = GInput->init();
	if(!ret) {
		delete GInput;
		GInput = NULL;
	}

	return ret;
}

void ZATON_INPUT_Release() {
	delete GInput;
	GInput = NULL;
}


Input::Input()
    : mousePos(0.0f), mouseDelta(0.0f), mouseCaptured(false), invertMouse(false),
     mouseSensibility(.003f)
{
    memset(keyStates, 0, sizeof(keyStates));
    memset(mouseStates, 0, sizeof(mouseStates));
}

Input::~Input()
{

}
bool Input :: init () {
    memset(keyStates, 0, sizeof(keyStates));
    mouseCaptured = false;
    mousePos = vec3(0.0f);
    mouseDelta = vec3(0.0f);
    mouseSensibility = .3f;
    #if defined(_WIN32)
    invertMouse = false;
    #elif defined(__linux__)
    invertMouse = true;
    #endif
    return true;
}


#ifndef INPUT_H_
#define INPUT_H_

#include <string.h>

#include "Mouse.h"
#include "Keyboard.h"
#include "util/utils.h"

class Input {

public:
    Input();
    ~Input();
    
    bool init();
	
    bool mouseCaptured;
    bool invertMouse;
    float mouseSensibility;
    vec3 mousePos;
    vec3 mouseDelta;
    bool keyStates[Keyboard::KeyCount];
    bool mouseStates[NUM_MOUSE_BUTTONS];

    void getMousePos()
    {
	printf("%f %f\n", mouseDelta.x, mouseDelta.y);	
    }
	
};

extern Input * GInput;

bool ZATON_INPUT_Init();
void ZATON_INPUT_Release();

#endif // INPUT_H_

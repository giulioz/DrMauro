/*
 *  SDL_Screen.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SDL_Screen.h"

#define scaling 2
#define joystick_axisTollerance 20


/*****************************************************************/
/* Init                                                          */
/*****************************************************************/

static void init(this_p(Screen)) {
    SDL_Screen *screen = (SDL_Screen*)this;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        ThrowError("SDL_INIT failed!");
    }

	if (SDL_NumJoysticks() < 1) {
		ThrowWarning("No joysticks connected!");
	} else {
		screen->controller = SDL_JoystickOpen(0);
		if (!screen->controller)
			ThrowWarning("Cannot open joystick");
	}

    screen->window = SDL_CreateWindow(
            screen->windowTitle,                                    /* title */
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,       /* position */
            this->width*scaling, this->height*scaling,              /* size */
            SDL_WINDOW_SHOWN                                        /* flags */
    );

    if (!screen->window) ThrowError("SDL_CreateWindow failed!");
    screen->tempSurface = SDL_GetWindowSurface(screen->window);
    screen->screenSurface = SDL_CreateRGBSurface(0, this->width, this->height, 32, 0, 0, 0, 0);
}


/*****************************************************************/
/* Input                                                         */
/*****************************************************************/

static void setKey(this_p(SDL_Screen), SDL_Event *e) {
	if (e->key.type == SDL_KEYUP) {
		/* Player 1 */
		if(e->key.keysym.sym == SDLK_LEFT
			|| e->key.keysym.sym == SDLK_RIGHT
			|| e->key.keysym.sym == SDLK_UP
			|| e->key.keysym.sym == SDLK_DOWN
			|| e->key.keysym.sym == SDLK_m
			|| e->key.keysym.sym == SDLK_n)
			this->inputDevice->currentState.direction1 = PillDirection_Nothing;

		/* Player 2 */
		if (e->key.keysym.sym == SDLK_a
			|| e->key.keysym.sym == SDLK_s
			|| e->key.keysym.sym == SDLK_d
			|| e->key.keysym.sym == SDLK_w
			|| e->key.keysym.sym == SDLK_1
			|| e->key.keysym.sym == SDLK_2)
			this->inputDevice->currentState.direction2 = PillDirection_Nothing;

		if (e->key.keysym.sym == SDLK_RETURN)
			this->inputDevice->currentState.enterButton = false;
	}
	else {
		switch (e->key.keysym.sym) {
			/* Player 1 */
			case SDLK_LEFT:
				this->inputDevice->currentState.direction1 = PillDirection_Left;
				break;
			case SDLK_RIGHT:
				this->inputDevice->currentState.direction1 = PillDirection_Right;
				break;
			case SDLK_UP:
				this->inputDevice->currentState.direction1 = PillDirection_Up;
				break;
			case SDLK_DOWN:
				this->inputDevice->currentState.direction1 = PillDirection_Down;
				break;
			case SDLK_m:
				this->inputDevice->currentState.direction1 = PillDirection_RotateRight;
				break;
			case SDLK_n:
				this->inputDevice->currentState.direction1 = PillDirection_RotateLeft;
				break;

			/* Player 2 */
			case SDLK_a:
				this->inputDevice->currentState.direction2 = PillDirection_Left;
				break;
			case SDLK_d:
				this->inputDevice->currentState.direction2 = PillDirection_Right;
				break;
			case SDLK_w:
				this->inputDevice->currentState.direction2 = PillDirection_Up;
				break;
			case SDLK_s:
				this->inputDevice->currentState.direction2 = PillDirection_Down;
				break;
			case SDLK_1:
				this->inputDevice->currentState.direction2 = PillDirection_RotateRight;
				break;
			case SDLK_2:
				this->inputDevice->currentState.direction2 = PillDirection_RotateLeft;
				break;


			case SDLK_RETURN:
				this->inputDevice->currentState.enterButton = true;
				break;
			default:
				break;
		}
	}
}

static void setJoy(this_p(SDL_Screen), SDL_Event *e, PillDirection *dest) {
	switch (e->jbutton.button) {
	    case 0:
            this->inputDevice->currentState.direction1 = PillDirection_RotateRight;
            break;
        case 3:
            this->inputDevice->currentState.direction1 = PillDirection_RotateLeft;
            break;
        case 11:
            this->inputDevice->currentState.enterButton = true;
            break;
        default:
            break;
	}
}

static void setJoyAxis(this_p(SDL_Screen), SDL_Event *e, PillDirection *destination) {
	int xAxis = SDL_JoystickGetAxis(this->controller, 0);
	int yAxis = SDL_JoystickGetAxis(this->controller, 1);

	if (abs(xAxis) <= joystick_axisTollerance && abs(yAxis) <= joystick_axisTollerance)
		*destination = PillDirection_Nothing;
	else if (xAxis > joystick_axisTollerance && abs(yAxis) <= joystick_axisTollerance)
		*destination = PillDirection_Right;
	else if (xAxis < -joystick_axisTollerance && abs(yAxis) <= joystick_axisTollerance)
		*destination = PillDirection_Left;
	else if (abs(xAxis) <= joystick_axisTollerance && yAxis > joystick_axisTollerance)
		*destination = PillDirection_Down;
	else if (abs(xAxis) <= joystick_axisTollerance && yAxis < -joystick_axisTollerance)
		*destination = PillDirection_Up;
}


/*****************************************************************/
/* Run                                                           */
/*****************************************************************/

static void run(this_p(Screen), GameState* callbacks) {
    SDL_Screen *screen = (SDL_Screen *) this;

    SDL_Rect dstRect = { 0, 0, 0, 0 };
    dstRect.w = screen->tempSurface->w;
    dstRect.h = screen->tempSurface->h;
    this->running = true;

    /* event loop */
    while (this->running) {

        /* pool Events */
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    this->running = false;
                    break;
                case SDL_KEYUP:
                    setKey(screen, &event, false);
                    break;
                case SDL_KEYDOWN:
                    setKey(screen, &event, true);
                    break;
				case SDL_JOYBUTTONUP:
					setJoy(screen, &event, false);
					break;
				case SDL_JOYBUTTONDOWN:
					setJoy(screen, &event, true);
					break;
				case SDL_JOYAXISMOTION:
					setJoyAxis(screen, &event, &(screen->inputDevice->currentState.direction1));
					break;
                default:
                    break;
            }
        }

        /* callbacks */
        if (!VTP(callbacks)->update(callbacks))
            break;
        VTP(callbacks)->draw(callbacks);
        SDL_BlitScaled(screen->screenSurface, NULL, screen->tempSurface, &dstRect);
        SDL_UpdateWindowSurface(screen->window);

        /* HACK: frame limiter */
        SDL_Delay(10);
    }
}


/*****************************************************************/
/* Close                                                         */
/*****************************************************************/

static void close(this_p(Screen)) {
    SDL_Screen *screen = (SDL_Screen*)this;

    this->running = false;
    SDL_FreeSurface(screen->screenSurface);
    SDL_FreeSurface(screen->tempSurface);
    SDL_DestroyWindow(screen->window);
	SDL_JoystickClose(screen->controller);
    SDL_Quit();
}

static Graphics* getGraphics(this_p(Screen)) {
    SDL_Screen *screen = ((SDL_Screen*)this);
    return (Graphics *) &(screen->graphics);
}

static uint32_t getCurrentTime(this_p(Screen)) {
    return SDL_GetTicks();
}

static struct Screen_VTABLE _vtable = {
        init, run, close, getGraphics, getCurrentTime
};

/* ********************************************************* */
/* Constructor                                               */
void SDL_Screen_init(this_p(SDL_Screen), uint16_t width, uint16_t height, char* windowTitle, SDL_InputDevice *inputDevice) {
    VT(this->base) = &_vtable;

    this->base.width = width;
    this->base.height = height;
    this->base.running = false;
    this->windowTitle = windowTitle;
    this->inputDevice = inputDevice;
	this->controller = NULL;
    SDL_Graphics_init(&this->graphics, this);
}


/* This program is a derivative of a test program also shipped within SDL.
   It uses a happy face sprite and draws multiple instances of them, bouncing them around the screen.
   I (Eric Wing) have modified it to help test bringing up SDL on other platforms and to do both performance benchmarking.
   
   Please see the SDL documentation for details on using the SDL API:
   /Developer/Documentation/SDL/docs.html
*/
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SDL/SDL.h"
#include "Lua/lua.hpp"

#include "../../../../Source/Input/InputWrapper.h"

#include <stdlib.h>

/*
 Produces a random int x, min <= x <= max
 following a uniform distribution
 */
int
randomInt(int min, int max)
{
    return min + rand() % (max - min + 1);
}

/*
 Produces a random float x, min <= x <= max
 following a uniform distribution
 */
float
randomFloat(float min, float max)
{
    return rand() / (float) RAND_MAX *(max - min) + min;
}

void
fatalError(const char *string)
{
    SDL_Log("%s: %s\n", string, SDL_GetError());
    exit(1);
}



static int NUM_HAPPY_FACES = 4000;    /* number of faces to draw */
#define MILLESECONDS_PER_FRAME 16 * 1       /* about 60 frames per second */
#define HAPPY_FACE_SIZE 32      /* width and height of happyface (pixels) */

#define SCREEN_WIDTH 480*2
#define SCREEN_HEIGHT 320*2

// How many frames time values to keep
// The higher the value the smoother the result is...
// Don't make it 0 or less :)
#define FRAME_VALUES 10

// An array to store frame times:
Uint32 frametimes[FRAME_VALUES];

// Last calculated SDL_GetTicks
Uint32 frametimelast;

// total frames rendered
Uint32 framecount;

// the value you want
float framespersecond;

// This function gets called once on startup.
void fpsinit() {

	// Set all frame times to 0ms.
	memset(frametimes, 0, sizeof(frametimes));
	framecount = 0;
	framespersecond = 0;
	frametimelast = SDL_GetTicks();

}

void fpsthink() {

	Uint32 frametimesindex;
	Uint32 getticks;
	Uint32 count;
	Uint32 i;

	// frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
	// This value rotates back to 0 after it hits FRAME_VALUES.
	frametimesindex = framecount % FRAME_VALUES;

	// store the current time
	getticks = SDL_GetTicks();

	// save the frame time value
	frametimes[frametimesindex] = getticks - frametimelast;

	// save the last frame time for the next fpsthink
	frametimelast = getticks;

	// increment the frame count
	framecount++;

	// Work out the current framerate

	// The code below could be moved into another function if you don't need the value every frame.

	// I've included a test to see if the whole array has been written to or not. This will stop
	// strange values on the first few (FRAME_VALUES) frames.
	if (framecount < FRAME_VALUES) {

		count = framecount;

	} else {

		count = FRAME_VALUES;

	}

	// add up all the values and divide to get the average frame time.
	framespersecond = 0;
	for (i = 0; i < count; i++) {

		framespersecond += frametimes[i];

	}

	framespersecond /= count;

	// now to make it an actual frames per second value...
	framespersecond = 1000.f / framespersecond;

}



static SDL_Texture *texture = 0;    /* reference to texture holding happyface */

struct HappyFaceData
{
    float x, y;                 /* position of happyface */
    float xvel, yvel;           /* velocity of happyface */
};

static struct HappyFaceData* faces = NULL;

/*
 Sets initial positions and velocities of happyfaces
 units of velocity are pixels per millesecond
 */
void
initializeHappyFaces()
{
	int i = 0;
#define MAX_FILE_STRING_LENGTH 2048
	char resource_file_path[MAX_FILE_STRING_LENGTH];
	char* base_path;
	FILE* resource_handle;
	SDL_RWops* rw_ops;

#if 0
	base_path = SDL_GetBasePath();

	strncpy(resource_file_path, base_path, MAX_FILE_STRING_LENGTH-1);
	strncat(resource_file_path, "/config.txt", MAX_FILE_STRING_LENGTH-1);

	resource_handle = fopen(resource_file_path, "r");
	if(NULL != resource_handle)
	{
		/* Please be nice. */
		fscanf(resource_handle, "%d", &i);
		NUM_HAPPY_FACES = i;
	}
	fclose(resource_handle);
#else

	/* Please be nice. Assumes the file contains a single ASCII number representing the number of sprites. */
	/* Must use RWops because Android will bundle this inside the .apk which makes it inaccessible to functions like fopen(). */
	/* No error checking is done for malicious input. */
	rw_ops = SDL_RWFromFile("config.txt", "r");
	if(NULL != rw_ops)
	{
		int result = 0;
		Uint8 character;
	   	do
		{
			character = SDL_ReadU8(rw_ops);
			if(character >=48 && character <= 57)
			{
				/* convert ascii value to real number */
				int value = character - 48;
//				SDL_Log("value: %d", value);
				/* update value */
				result = 10 * result + value;
			}
		}
		while(character >=48 && character <= 57);

		SDL_Log("config.txt specified %d sprites", result);
		SDL_RWclose(rw_ops);
		NUM_HAPPY_FACES = result;
	}

#endif
	faces = (struct HappyFaceData*)SDL_malloc(NUM_HAPPY_FACES*sizeof(struct HappyFaceData));

	for (i = 0; i < NUM_HAPPY_FACES; i++)
	{
		faces[i].x = randomFloat(0.0f, SCREEN_WIDTH - HAPPY_FACE_SIZE);
		faces[i].y = randomFloat(0.0f, SCREEN_HEIGHT - HAPPY_FACE_SIZE);
		faces[i].xvel = randomFloat(-0.1f, 0.1f);
		faces[i].yvel = randomFloat(-0.1f, 0.1f);
	}
}

void
render(SDL_Renderer *renderer)
{

    int i;
    SDL_Rect srcRect;
    SDL_Rect dstRect;

    /* setup boundaries for happyface bouncing */
    Uint16 maxx = SCREEN_WIDTH - HAPPY_FACE_SIZE;
    Uint16 maxy = SCREEN_HEIGHT - HAPPY_FACE_SIZE;
    Uint16 minx = 0;
    Uint16 miny = 0;

    /* setup rects for drawing */
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = HAPPY_FACE_SIZE;
    srcRect.h = HAPPY_FACE_SIZE;
    dstRect.w = HAPPY_FACE_SIZE;
    dstRect.h = HAPPY_FACE_SIZE;

    /* fill background in with black */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    /*
	 loop through all the happy faces:
	 - update position
	 - update velocity (if boundary is hit)
	 - draw
     */
    for (i = 0; i < NUM_HAPPY_FACES; i++) {
        faces[i].x += faces[i].xvel * MILLESECONDS_PER_FRAME;
        faces[i].y += faces[i].yvel * MILLESECONDS_PER_FRAME;
        if (faces[i].x > maxx) {
            faces[i].x = maxx;
            faces[i].xvel = -faces[i].xvel;
        } else if (faces[i].y > maxy) {
            faces[i].y = maxy;
            faces[i].yvel = -faces[i].yvel;
        }
        if (faces[i].x < minx) {
            faces[i].x = minx;
            faces[i].xvel = -faces[i].xvel;
        } else if (faces[i].y < miny) {
            faces[i].y = miny;
            faces[i].yvel = -faces[i].yvel;
        }
        dstRect.x = faces[i].x;
        dstRect.y = faces[i].y;
        SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
    }
    /* update screen */
    SDL_RenderPresent(renderer);

}
/*void ShowFrame(void* renderer)
{
	Uint32 startFrame;
    Uint32 endFrame;
	static Uint32 my_fps_print_timer = 0;

	startFrame = SDL_GetTicks();

	render(renderer);
	endFrame = SDL_GetTicks();

	fpsthink();
	if( (my_fps_print_timer +  3000) < (endFrame))
	{
		SDL_Log("%f\n", framespersecond);
		my_fps_print_timer = endFrame;
	}
}*/

/*
 loads the happyface graphic into a texture
 */
void
initializeTexture(SDL_Renderer *renderer)
{
    SDL_Surface *bmp_surface;
    /* load the bmp */
    bmp_surface = SDL_LoadBMP("icon.bmp");
    if (bmp_surface == NULL) {
        fatalError("could not load bmp");
    }
    /* set white to transparent on the happyface */
    SDL_SetColorKey(bmp_surface, 1,
                    SDL_MapRGB(bmp_surface->format, 255, 255, 255));

    /* convert RGBA surface to texture */
    texture = SDL_CreateTextureFromSurface(renderer, bmp_surface);
    if (texture == 0) {
        fatalError("could not create texture");
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    /* free up allocated memory */
    SDL_FreeSurface(bmp_surface);
}




#if 0
static int report (lua_State *L, int status) {
	const char *msg;
	if (status) {
		msg = lua_tostring(L, -1);
		if (msg == NULL) msg = "(error with no message)";
		SDL_Log("status=%d, %s\n", status, msg);
		lua_pop(L, 1);
	}
	return status;
}
#endif

int main(int argc, char *argv[])
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_close(L);

    SDL_Window *window;
    SDL_Renderer *renderer;
    Uint32 startFrame;
    Uint32 endFrame;
    Sint32 delay;
    int done;

	Uint32 my_fps_print_timer = 0;


    /* initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Could not initialize SDL");
    }
#if 1
        SDL_Log("SDL_CreateWindow");

    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    //window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
//							  SDL_WINDOW_RESIZABLE);
	if(NULL == window)
	{
        SDL_Log("SDL_CreateWindow failed");
        SDL_Log("SDL_CreateWindow failed %s", SDL_GetError());
	}
	else
	{
        SDL_Log("SDL_CreateWindow passed");
	}
    renderer = SDL_CreateRenderer(window, -1, 0);
	if(NULL == renderer)
	{
        SDL_Log("SDL_CreateRenderer failed");
        SDL_Log("SDL_CreateRenderer failed %s", SDL_GetError());

	}
	else
	{
        SDL_Log("SDL_CreateRenderer passed");
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);


	initializeTexture(renderer);
        SDL_Log("initializeTexture passed");
    initializeHappyFaces();
        SDL_Log("initializeHappyFaces passed");
#endif




	done = 0;
	while ( !done ) {
		SDL_Event event;
		int the_result;

        startFrame = SDL_GetTicks();


#if 1

		/* Check for events */
		do
		{
			the_result = SDL_PollEvent(&event);
			if(the_result > 0)
			{
				switch (event.type)
				{

					case SDL_MOUSEMOTION:
					//	SDL_Log("SDL_MOUSEMOTION, %d", the_result);
						break;
					case SDL_MOUSEBUTTONDOWN:
						SDL_Log("SDL_MOUSEBUTTONDOWN, %d", the_result);
						break;
					case SDL_KEYDOWN:
						SDL_Log("SDL_KEYDOWN, %d", the_result);
						if(event.key.keysym.sym == SDLK_AC_BACK)
						{
							SDL_Log("Android back button pressed, going to quit, %d", the_result);

							done = 1;
						}
						break;

					case SDL_QUIT:
					case SDL_APP_TERMINATING:
						SDL_Log("SDL_QUIT, %d", the_result);
						done = 1;
						break;
					default:
						break;
				}
			}
		} while(the_result > 0);

#endif
		render(renderer);

		endFrame = SDL_GetTicks();
		fpsthink();
		if( (my_fps_print_timer +  3000) < (endFrame))
		{
			SDL_Log("%f\n", framespersecond);
			my_fps_print_timer = endFrame;
		}
        /* figure out how much time we have left, and then sleep */
        delay = MILLESECONDS_PER_FRAME - (endFrame - startFrame);
        if (delay <= 0) {
            delay = 0;
        } else if (delay > MILLESECONDS_PER_FRAME) {
            delay = MILLESECONDS_PER_FRAME;
        }

		SDL_Delay(delay);
	}


	SDL_DestroyTexture(texture);

	SDL_free(faces);

	/* Clean up the SDL library */
	SDL_Quit();
	return(0);
}

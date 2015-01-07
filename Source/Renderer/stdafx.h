#ifndef STDAFX_RENDERER_H
#define STDAFX_RENDERER_H

#ifdef WIN32
	#include <VLD/vld.h>
#else
#endif

#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include <SDL/SDL.h>
#ifndef __ANDROID__
#include <GLEW/glew.h>
#include <SDL/SDL_opengl.h>
#else
#include <GLES2/gl2.h>
#include <SDL/SDL_opengles2.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#define ERRORMSG(x) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "RENDERER ERROR", x, m_window); }

#endif
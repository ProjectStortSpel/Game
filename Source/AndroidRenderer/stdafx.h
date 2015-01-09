#ifndef STDAFX_RENDERER_H
#define STDAFX_RENDERER_H

#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include <SDL/SDL.h>
#ifdef __ANDROID__
#include <GLES2/gl2.h>
#else
#include <GLEW/glew.h>
#endif
#include <SDL/SDL_opengles.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#define ERRORMSG(x) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "RENDERER ERROR", x, m_window); }

#endif
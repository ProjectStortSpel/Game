#ifndef STDAFX_H
#define STDAFX_H

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include <SDL/SDL.h>
#include <GLEW/glew.h>
#include <SDL/SDL_opengl.h>
#include <glm/glm.hpp>

using namespace glm;

#define ERRORMSG(x) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "RENDERER ERROR", x, m_window); }

#endif
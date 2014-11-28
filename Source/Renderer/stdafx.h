#ifndef STDAFX_RENDERER_H
#define STDAFX_RENDERER_H

#include <map>
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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#define ERRORMSG(x) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "RENDERER ERROR", x, m_window); }

#endif
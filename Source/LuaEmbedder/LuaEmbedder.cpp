#include "LuaEmbedder.h"

#include <iostream>

LuaEmbedder& LuaEmbedder::GetInstance()
{
  static LuaEmbedder instance;
  return instance;
}

LuaEmbedder::LuaEmbedder()
{
  m_L = luaL_newstate();
  luaL_openlibs(m_L);
}

LuaEmbedder::~LuaEmbedder()
{
  lua_gc(m_L, LUA_GCCOLLECT, 0);
  lua_close(m_L);
}

void LuaEmbedder::Run(const std::string& filepath)
{
  luaL_dofile(m_L, filepath.c_str());
}

#define PARAMETER_ERROR_CHECK(condition, errorMessage) \
  if (condition) \
  { \
    std::cerr << errorMessage << std::endl; \
    return 0; \
  }

int LuaEmbedder::GetParameterInt(int index)
{
  PARAMETER_ERROR_CHECK(index <= 0,
    "LuaEmbedder::GetParameterInt : \"Invalid parameter index: " << index << "\"");
  int isValid;
  int parameter = lua_tointegerx(m_L, index, &isValid);
  PARAMETER_ERROR_CHECK(isValid == 0,
    "LuaEmbedder::GetParameterInt : \"Parameter is not an integer\"");
  return parameter;
}

void LuaEmbedder::SetParameterInt(int parameter)
{
  lua_pushinteger(m_L, parameter);
}
#ifndef LUAMATRIX_H
#define LUAMATRIX_H
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <LuaEmbedder/LuaEmbedder.h>

namespace LuaBridge
{
  class LuaMatrix
  {
  public:
    LuaMatrix(lua_State* L);
    
    static void Embed(lua_State* L);
    
    glm::mat4* GetGlmMatrix();
    
  private:
    int Translate(lua_State* L);
    int RotateX(lua_State* L);
    int RotateY(lua_State* L);
    int RotateZ(lua_State* L);
    int Scale(lua_State* L);
    
  private:
    glm::mat4 m_matrix;
  };
}

#endif
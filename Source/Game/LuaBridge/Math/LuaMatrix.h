#ifndef LUAMATRIX_H
#define LUAMATRIX_H
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace LuaBridge
{
  class LuaMatrix
  {
  public:
    LuaMatrix();
    
    static void Embed();
    
    glm::mat4* GetGlmMatrix();
    
  private:
    int Translate();
    int RotateX();
    int RotateY();
    int RotateZ();
    int Scale();
    
  private:
    glm::mat4 m_matrix;
  };
}

#endif
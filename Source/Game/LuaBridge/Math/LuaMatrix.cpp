#include "LuaMatrix.h"
#include "LuaEmbedder/LuaEmbedder.h"

#include <glm/ext.hpp>

namespace LuaBridge
{
  LuaMatrix::LuaMatrix(lua_State* L)
  {
    m_matrix = glm::mat4(1.0f);
  }
  
  void LuaMatrix::Embed(lua_State* L)
  {
    LuaEmbedder::EmbedClass<LuaMatrix>(L, "Matrix");
    LuaEmbedder::EmbedClassFunction<LuaMatrix>(L, "Matrix", "Translate", &LuaMatrix::Translate);
    LuaEmbedder::EmbedClassFunction<LuaMatrix>(L, "Matrix", "RotateX", &LuaMatrix::RotateX);
    LuaEmbedder::EmbedClassFunction<LuaMatrix>(L, "Matrix", "RotateY", &LuaMatrix::RotateY);
    LuaEmbedder::EmbedClassFunction<LuaMatrix>(L, "Matrix", "RotateZ", &LuaMatrix::RotateZ);
    LuaEmbedder::EmbedClassFunction<LuaMatrix>(L, "Matrix", "Scale", &LuaMatrix::Scale);
  }
  
  glm::mat4* LuaMatrix::GetGlmMatrix()
  {
    return &m_matrix;
  }
  
  int LuaMatrix::Translate(lua_State* L)
  {
    float x = LuaEmbedder::PullFloat(L, 1);
    float y = LuaEmbedder::PullFloat(L, 2);
    float z = LuaEmbedder::PullFloat(L, 3);
    m_matrix = glm::translate(m_matrix, glm::vec3(x, y, z));
    return 0;
  }
  int LuaMatrix::RotateX(lua_State* L)
  {
    float angle = LuaEmbedder::PullFloat(L, 1);
    m_matrix = glm::rotate(m_matrix, angle, glm::vec3(1.0f, 0.0f, 0.0f));
    return 0;
  }
  int LuaMatrix::RotateY(lua_State* L)
  {
    float angle = LuaEmbedder::PullFloat(L, 1);
    m_matrix = glm::rotate(m_matrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    return 0;
  }
  int LuaMatrix::RotateZ(lua_State* L)
  {
    float angle = LuaEmbedder::PullFloat(L, 1);
    m_matrix = glm::rotate(m_matrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    return 0;
  }
  int LuaMatrix::Scale(lua_State* L)
  {
    float x = LuaEmbedder::PullFloat(L, 1);
    float y = LuaEmbedder::PullFloat(L, 2);
    float z = LuaEmbedder::PullFloat(L, 3);
    m_matrix = glm::scale(m_matrix, glm::vec3(x, y, z));
    return 0;
  }
}
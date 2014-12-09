#include "LuaMatrix.h"
#include "LuaEmbedder/LuaEmbedder.h"

#include <glm/ext.hpp>

namespace LuaBridge
{
  LuaMatrix::LuaMatrix()
  {
    m_matrix = glm::mat4(1.0f);
  }
  
  void LuaMatrix::Embed()
  {
    LuaEmbedder::EmbedClass<LuaMatrix>("Matrix");
    LuaEmbedder::EmbedClassFunction<LuaMatrix>("Matrix", "Translate", &LuaMatrix::Translate);
    LuaEmbedder::EmbedClassFunction<LuaMatrix>("Matrix", "RotateX", &LuaMatrix::RotateX);
    LuaEmbedder::EmbedClassFunction<LuaMatrix>("Matrix", "RotateY", &LuaMatrix::RotateY);
    LuaEmbedder::EmbedClassFunction<LuaMatrix>("Matrix", "RotateZ", &LuaMatrix::RotateZ);
    LuaEmbedder::EmbedClassFunction<LuaMatrix>("Matrix", "Scale", &LuaMatrix::Scale);
  }
  
  glm::mat4* LuaMatrix::GetGlmMatrix()
  {
    return &m_matrix;
  }
  
  int LuaMatrix::Translate()
  {
    float x = LuaEmbedder::PullFloat(1);
    float y = LuaEmbedder::PullFloat(2);
    float z = LuaEmbedder::PullFloat(3);
    m_matrix = glm::translate(m_matrix, glm::vec3(x, y, z));
    return 0;
  }
  int LuaMatrix::RotateX()
  {
    float angle = LuaEmbedder::PullFloat(1);
    m_matrix = glm::rotate(m_matrix, angle, glm::vec3(1.0f, 0.0f, 0.0f));
    return 0;
  }
  int LuaMatrix::RotateY()
  {
    float angle = LuaEmbedder::PullFloat(1);
    m_matrix = glm::rotate(m_matrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    return 0;
  }
  int LuaMatrix::RotateZ()
  {
    float angle = LuaEmbedder::PullFloat(1);
    m_matrix = glm::rotate(m_matrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    return 0;
  }
  int LuaMatrix::Scale()
  {
    float x = LuaEmbedder::PullFloat(1);
    float y = LuaEmbedder::PullFloat(2);
    float z = LuaEmbedder::PullFloat(3);
    m_matrix = glm::scale(m_matrix, glm::vec3(x, y, z));
    return 0;
  }
}
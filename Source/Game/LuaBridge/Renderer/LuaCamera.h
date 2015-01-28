#ifndef LUACAMERA_H
#define LUACAMERA_H

#ifdef __ANDROID__
#include "AndroidRenderer/Camera.h"
#else
#include "Renderer/Camera.h"
#endif

#include <LuaEmbedder/LuaEmbedder.h>

namespace LuaBridge
{
	class LuaCamera : public Camera
	{
	public:
		LuaCamera(lua_State* L);

		static void Embed(lua_State* L);

	private:
		int MoveForward(lua_State* L);
		int MoveBackward(lua_State* L);
		int MoveLeft(lua_State* L);
		int MoveRight(lua_State* L);

		int UpdateMouse(lua_State* L);

		int MoveToAndLookAt(lua_State* L);

		int SetRight(lua_State* L);
		int SetUp(lua_State* L);
		int SetLook(lua_State* L);
		int SetPosition(lua_State* L);

		int GetRight(lua_State* L);
		int GetUp(lua_State* L);
		int GetLook(lua_State* L);
		int GetPosition(lua_State* L);
	};
}

#endif
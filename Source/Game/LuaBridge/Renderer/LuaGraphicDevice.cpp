#include "LuaGraphicDevice.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaCamera.h"
#include "../Math/LuaMatrix.h"

namespace LuaBridge
{
	LuaGraphicDevice::LuaGraphicDevice(lua_State* L) { }

	void LuaGraphicDevice::Embed(lua_State* L)
	{
		LuaEmbedder::EmbedClass<LuaGraphicDevice>(L, "GraphicDevice");

		LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>(L, "GraphicDevice", "GetTouchPosition", &LuaGraphicDevice::GetTouchPosition);
		LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>(L, "GraphicDevice", "GetAspectRatio", &LuaGraphicDevice::GetAspectRatio);

		LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>(L, "GraphicDevice", "ResizeWindow", &LuaGraphicDevice::ResizeWindow);
		LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>(L, "GraphicDevice", "SetTitle", &LuaGraphicDevice::SetTitle);
		LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>(L, "GraphicDevice", "RenderSimpleText", &LuaGraphicDevice::RenderSimpleText);
		LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>(L, "GraphicDevice", "SetSimpleTextColor", &LuaGraphicDevice::SetSimpleTextColor);
		LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>(L, "GraphicDevice", "GetCamera", &LuaGraphicDevice::GetCamera);
		LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>(L, "GraphicDevice", "GetWindowSize", &LuaGraphicDevice::GetWindowSize);
		LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>(L, "GraphicDevice", "LoadModel", &LuaGraphicDevice::LoadModel);
		LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>(L, "GraphicDevice", "ChangeModelTexture", &LuaGraphicDevice::ChangeModelTexture);
		LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>(L, "GraphicDevice", "ChangeModelNormalMap", &LuaGraphicDevice::ChangeModelNormalMap);
		LuaEmbedder::EmbedClassFunction<LuaGraphicDevice>(L, "GraphicDevice", "ChangeModelSpecularMap", &LuaGraphicDevice::ChangeModelSpecularMap);

		LuaEmbedder::EmbedClassProperty<LuaGraphicDevice>(L, "GraphicDevice", "Camera", &LuaGraphicDevice::GetCamera, &LuaGraphicDevice::SetCamera);
	}

	int LuaGraphicDevice::GetAspectRatio(lua_State* L)
	{
		int x, y;
		GraphicDevice::GetWindowSize(x, y);
		float ratioX = (float)(x) / (float)(y);
		LuaEmbedder::PushFloat(L, ratioX * 0.5625);
		LuaEmbedder::PushFloat(L, 1 * 0.5625);
		return 2;
	}

	int LuaGraphicDevice::GetTouchPosition(lua_State* L)
	{
		/*Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		SDL_FingerID finger = (SDL_FingerID)LuaEmbedder::PullInt(1);
		LuaEmbedder::PushFloat(inputWrapper.GetTouch()->GetX(finger)-0.5f);
		LuaEmbedder::PushFloat(-inputWrapper.GetTouch()->GetY(finger)+0.5f);
		return 2;*/
		Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		int wX, wY;
		GraphicDevice::GetWindowSize(wX, wY);
		int mX = inputWrapper.GetMouse()->GetX();
		int mY = inputWrapper.GetMouse()->GetY();

		LuaEmbedder::PushFloat(L, (float)(mX) / (float)(wX)-0.5f);
		LuaEmbedder::PushFloat(L, (float)(-mY) / (float)(wY)+0.5f);
		return 2;
	}

	int LuaGraphicDevice::ResizeWindow(lua_State* L)
	{
		int width = LuaEmbedder::PullInt(L, 1);
		int height = LuaEmbedder::PullInt(L, 2);
		GraphicDevice::ResizeWindow(width, height);
		return 0;
	}

	int LuaGraphicDevice::SetTitle(lua_State* L)
	{
		std::string title = LuaEmbedder::PullString(L, 1);
		//GraphicDevice::SetTitle(title);
		return 0;
	}

	int LuaGraphicDevice::RenderSimpleText(lua_State* L)
	{
		std::string text = LuaEmbedder::PullString(L, 1);
		int x = LuaEmbedder::PullInt(L, 2);
		int y = LuaEmbedder::PullInt(L, 3);
		GraphicDevice::RenderSimpleText(text, x, y);
		return 0;
	}

	int LuaGraphicDevice::SetSimpleTextColor(lua_State* L)
	{
		float r = LuaEmbedder::PullFloat(L, 1);
		float g = LuaEmbedder::PullFloat(L, 2);
		float b = LuaEmbedder::PullFloat(L, 3);
		float a = LuaEmbedder::IsFloat(L, 4) ? LuaEmbedder::PullFloat(L, 4) : 1.0f;
		GraphicDevice::SetSimpleTextColor(r, g, b, a);
		return 0;
	}

	int LuaGraphicDevice::GetCamera(lua_State* L)
	{
		Camera* camera = GraphicDevice::GetCamera();
		LuaEmbedder::PushObject<Camera>(L, "Camera", camera);
		return 1;
	}

	int LuaGraphicDevice::SetCamera(lua_State* L)
	{
		return 0;
	}

	int LuaGraphicDevice::GetWindowSize(lua_State* L)
	{
		int x, y;
		GraphicDevice::GetWindowSize(x, y);
		LuaEmbedder::PushInt(L, x);
		LuaEmbedder::PushInt(L, y);
		return 2;
	}

	int LuaGraphicDevice::LoadModel(lua_State* L)
	{
		std::string dir = LuaEmbedder::PullString(L, 1);
		std::string file = LuaEmbedder::PullString(L, 2);
		LuaMatrix* matrix = LuaEmbedder::PullObject<LuaMatrix>(L, "Matrix", 3);
		int model = GraphicDevice::LoadModel(dir, file, matrix->GetGlmMatrix());
		LuaEmbedder::PushInt(L, model);
		return 1;
	}

	int LuaGraphicDevice::ChangeModelTexture(lua_State* L)
	{
		int id = LuaEmbedder::PullInt(L, 1);
		std::string fileDir = LuaEmbedder::PullString(L, 2);
		bool result = GraphicDevice::ChangeModelTexture(id, fileDir);
		LuaEmbedder::PushBool(L, result);
		return 1;
	}

	int LuaGraphicDevice::ChangeModelNormalMap(lua_State* L)
	{
		int id = LuaEmbedder::PullInt(L, 1);
		std::string fileDir = LuaEmbedder::PullString(L, 2);
		bool result = GraphicDevice::ChangeModelNormalMap(id, fileDir);
		LuaEmbedder::PushBool(L, result);
		return 1;
	}

	int LuaGraphicDevice::ChangeModelSpecularMap(lua_State* L)
	{
		int id = LuaEmbedder::PullInt(L, 1);
		std::string fileDir = LuaEmbedder::PullString(L, 2);
		bool result = GraphicDevice::ChangeModelSpecularMap(id, fileDir);
		LuaEmbedder::PushBool(L, result);
		return 1;
	}

	int LuaGraphicDevice::AddFont(lua_State* L)
	{
		std::string filepath = LuaEmbedder::PullString(L, 1);
		int size = LuaEmbedder::PullInt(L, 2);
		int fontIndex = GraphicDevice::AddFont(filepath, size);
		LuaEmbedder::PushInt(L, fontIndex);
		return 1;
	}

	int LuaGraphicDevice::CreateTextTexture(lua_State* L)
	{
		std::string textureName = LuaEmbedder::PullString(L, 1);
		std::string textString = LuaEmbedder::PullString(L, 2);
		int fontIndex = LuaEmbedder::PullInt(L, 3);
		Uint8 r = (Uint8)LuaEmbedder::PullInt(L, 4);
		Uint8 g = (Uint8)LuaEmbedder::PullInt(L, 5);
		Uint8 b = (Uint8)LuaEmbedder::PullInt(L, 6);
		SDL_Color color = { 255, 255, 255, 0 };
		//int w = LuaEmbedder::PullInt(7);
		//int h = LuaEmbedder::PullInt(8);
		//glm::ivec2 size = glm::ivec2(w, h);
		float ratio = GraphicDevice::CreateTextTexture(textureName, textString, fontIndex, color);
		LuaEmbedder::PushFloat(L, ratio);
		return 1;
	}

	int LuaGraphicDevice::CreateWrappedTextTexture(lua_State* L)
	{
		std::string textureName = LuaEmbedder::PullString(L, 1);
		std::string textString = LuaEmbedder::PullString(L, 2);
		int fontIndex = LuaEmbedder::PullInt(L, 3);
		Uint8 r = (Uint8)LuaEmbedder::PullInt(L, 4);
		Uint8 g = (Uint8)LuaEmbedder::PullInt(L, 5);
		Uint8 b = (Uint8)LuaEmbedder::PullInt(L, 6);
		SDL_Color color = { r, g, b, 0 };
		int wrapLength = LuaEmbedder::PullInt(L, 7);
		//int w = LuaEmbedder::PullInt(8);
		//int h = LuaEmbedder::PullInt(9);
		//glm::ivec2 size = glm::ivec2(w, h);
		GraphicDevice::CreateWrappedTextTexture(textureName, textString, fontIndex, color, wrapLength);
		return 0;
	}
}
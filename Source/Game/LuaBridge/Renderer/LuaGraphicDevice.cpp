#include "LuaGraphicDevice.h"
#include "LuaEmbedder/LuaEmbedder.h"
#include "LuaCamera.h"
#include "../Math/LuaMatrix.h"
#include "Game/LuaBridge/LuaBridge.h"

namespace LuaBridge
{
	namespace LuaGraphicDevice
	{
		Renderer::GraphicDevice* g_graphicDevice = NULL;
		
		int GetAspectRatio(lua_State* L);
		int GetTouchPosition(lua_State* L);

		int ResizeWindow(lua_State* L);
		int SetTitle(lua_State* L);
		
		int RenderSimpleText(lua_State* L);
		int SetSimpleTextColor(lua_State* L);
		
		int GetCamera(lua_State* L);
		int SetCamera(lua_State* L);
		
		int GetWindowSize(lua_State* L);
		
		int SetAnimation(lua_State* L);

		int LoadModel(lua_State* L);
		int ChangeModelTexture(lua_State* L);
		int ChangeModelNormalMap(lua_State* L);
		int ChangeModelSpecularMap(lua_State* L);

		int AddFont(lua_State* L);
		int CreateTextTexture(lua_State* L);
		int CreateWrappedTextTexture(lua_State* L);
		
		void Embed(lua_State* L)
		{
			LuaEmbedder::AddFunction(L, "GetTouchPosition", &GetTouchPosition, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "GetAspectRatio", &GetAspectRatio, "GraphicDevice");

			LuaEmbedder::AddFunction(L, "ResizeWindow", &ResizeWindow, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "SetTitle", &SetTitle, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "RenderSimpleText", &RenderSimpleText, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "SetSimpleTextColor", &SetSimpleTextColor, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "GetCamera", &GetCamera, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "GetWindowSize", &GetWindowSize, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "SetAnimation", &SetAnimation, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "LoadModel", &LoadModel, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "ChangeModelTexture", &ChangeModelTexture, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "ChangeModelNormalMap", &ChangeModelNormalMap, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "ChangeModelSpecularMap", &ChangeModelSpecularMap, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "AddFont", &AddFont, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "CreateTextTexture", &CreateTextTexture, "GraphicDevice");
			LuaEmbedder::AddFunction(L, "CreateWrappedTextTexture", &CreateWrappedTextTexture, "GraphicDevice");
		}
		
		void SetGraphicDevice(Renderer::GraphicDevice* graphicDevice)
		{
			g_graphicDevice = graphicDevice;
		}

		Renderer::GraphicDevice* GetGraphicDevice()
		{
			return g_graphicDevice;
		}
        
		int GetAspectRatio(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
            {
                LuaEmbedder::PushFloat(L, 0.5625f);
                LuaEmbedder::PushFloat(L, 0.5625f);
                return 2;
            }
			int x, y;
			g_graphicDevice->GetWindowSize(x, y);
			float ratioX = (float)(x) / (float)(y);
			LuaEmbedder::PushFloat(L, ratioX * 0.5625);
			LuaEmbedder::PushFloat(L, 1 * 0.5625);
			return 2;
		}

		int GetTouchPosition(lua_State* L)
		{
			/*Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
			SDL_FingerID finger = (SDL_FingerID)LuaEmbedder::PullInt(1);
			LuaEmbedder::PushFloat(inputWrapper.GetTouch()->GetX(finger)-0.5f);
			LuaEmbedder::PushFloat(-inputWrapper.GetTouch()->GetY(finger)+0.5f);
			return 2;*/
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
            {
                LuaEmbedder::PushFloat(L, 0.0f);
                LuaEmbedder::PushFloat(L, 0.0f);
                return 2;
            }
            
			Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
			int wX, wY;
			g_graphicDevice->GetWindowSize(wX, wY);
			int mX = inputWrapper.GetMouse()->GetX();
			int mY = inputWrapper.GetMouse()->GetY();

			LuaEmbedder::PushFloat(L, (float)(mX) / (float)(wX)-0.5f);
			LuaEmbedder::PushFloat(L, (float)(-mY) / (float)(wY)+0.5f);
			return 2;
		}

		int ResizeWindow(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
                return 0;
			int width = LuaEmbedder::PullInt(L, 1);
			int height = LuaEmbedder::PullInt(L, 2);
			g_graphicDevice->ResizeWindow(width, height);
			return 0;
		}

		int SetTitle(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
                return 0;
			std::string title = LuaEmbedder::PullString(L, 1);
			//GraphicDevice::SetTitle(title);
			return 0;
		}

		int RenderSimpleText(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
                return 0;
			std::string text = LuaEmbedder::PullString(L, 1);
			int x = LuaEmbedder::PullInt(L, 2);
			int y = LuaEmbedder::PullInt(L, 3);
			g_graphicDevice->RenderSimpleText(text, x, y);
			return 0;
		}

		int SetSimpleTextColor(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
                return 0;
			float r = LuaEmbedder::PullFloat(L, 1);
			float g = LuaEmbedder::PullFloat(L, 2);
			float b = LuaEmbedder::PullFloat(L, 3);
			float a = LuaEmbedder::IsFloat(L, 4) ? LuaEmbedder::PullFloat(L, 4) : 1.0f;
			g_graphicDevice->SetSimpleTextColor(r, g, b, a);
			return 0;
		}

		int GetCamera(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
                return 0;
			Camera* camera = g_graphicDevice->GetCamera();
			LuaEmbedder::PushObject<Camera>(L, "Camera", camera);
			return 1;
		}

		int SetCamera(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
                return 0;
			return 0;
		}

		int GetWindowSize(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
            {
                LuaEmbedder::PushInt(L, 0);
                LuaEmbedder::PushInt(L, 0);
                return 2;
            }
			int x, y;
			g_graphicDevice->GetWindowSize(x, y);
			LuaEmbedder::PushInt(L, x);
			LuaEmbedder::PushInt(L, y);
			return 2;
		}

		int SetAnimation(lua_State* L)
		{
			lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
			{
				LuaEmbedder::PushBool(L, false);
				return 1;
			}
			int modelId = LuaEmbedder::PullInt(L, 1);
			int animId = LuaEmbedder::PullInt(L, 2);
			bool result = g_graphicDevice->SetAnimation(modelId, animId);
			LuaEmbedder::PushBool(L, result);
			return 1;
		}

		int LoadModel(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
            {
                LuaEmbedder::PushInt(L, -1);
                return 1;
            }
			std::string dir = LuaEmbedder::PullString(L, 1);
			std::string file = LuaEmbedder::PullString(L, 2);
			LuaMatrix* matrix = LuaEmbedder::PullObject<LuaMatrix>(L, "Matrix", 3);
			int model = g_graphicDevice->LoadModel(dir, file, matrix->GetGlmMatrix());
			LuaEmbedder::PushInt(L, model);
			return 1;
		}

		int ChangeModelTexture(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
            {
                LuaEmbedder::PushBool(L, false);
                return 1;
            }
			int id = LuaEmbedder::PullInt(L, 1);
			std::string fileDir = LuaEmbedder::PullString(L, 2);
			bool result = g_graphicDevice->ChangeModelTexture(id, fileDir);
			LuaEmbedder::PushBool(L, result);
			return 1;
		}

		int ChangeModelNormalMap(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
            {
                LuaEmbedder::PushBool(L, false);
                return 1;
            }
			int id = LuaEmbedder::PullInt(L, 1);
			std::string fileDir = LuaEmbedder::PullString(L, 2);
			bool result = g_graphicDevice->ChangeModelNormalMap(id, fileDir);
			LuaEmbedder::PushBool(L, result);
			return 1;
		}

		int ChangeModelSpecularMap(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
            {
                LuaEmbedder::PushBool(L, false);
                return 1;
            }
			int id = LuaEmbedder::PullInt(L, 1);
			std::string fileDir = LuaEmbedder::PullString(L, 2);
			bool result = g_graphicDevice->ChangeModelSpecularMap(id, fileDir);
			LuaEmbedder::PushBool(L, result);
			return 1;
		}

		int AddFont(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
            {
                LuaEmbedder::PushInt(L, -1);
                return 1;
            }
			std::string filepath = LuaEmbedder::PullString(L, 1);
			int size = LuaEmbedder::PullInt(L, 2);
			int fontIndex = g_graphicDevice->AddFont(filepath, size);
			LuaEmbedder::PushInt(L, fontIndex);
			return 1;
		}

		int CreateTextTexture(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
            {
                LuaEmbedder::PushFloat(L, 0.0f);
                return 1;
            }
			std::string textureName = LuaEmbedder::PullString(L, 1);
			std::string textString = LuaEmbedder::PullString(L, 2);
			int fontIndex = LuaEmbedder::PullInt(L, 3);
			float r = LuaEmbedder::PullFloat(L, 4);
			float g = LuaEmbedder::PullFloat(L, 5);
			float b = LuaEmbedder::PullFloat(L, 6);
			SDL_Color color = { static_cast<Uint8>(255 * r), static_cast<Uint8>(255 * g), static_cast<Uint8>(255 * b), 0 };
			//int w = LuaEmbedder::PullInt(7);
			//int h = LuaEmbedder::PullInt(8);
			//glm::ivec2 size = glm::ivec2(w, h);
			float ratio = g_graphicDevice->CreateTextTexture(textureName, textString, fontIndex, color);
			LuaEmbedder::PushFloat(L, ratio);
			return 1;
		}

		int CreateWrappedTextTexture(lua_State* L)
		{
            lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
			if (LuaBridge::g_IOLuaState != parent)
                return 0;
            
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
			g_graphicDevice->CreateWrappedTextTexture(textureName, textString, fontIndex, color, wrapLength);
			return 0;
		}
	}
}
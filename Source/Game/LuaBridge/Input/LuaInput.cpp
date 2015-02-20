#include "LuaInput.h"
#include "Game/LuaBridge/LuaBridge.h"

namespace LuaBridge
{
  namespace LuaInput
  {
    int GetKeyState(lua_State* L);
    int GetMouseButtonState(lua_State* L);
    int GetMousePosition(lua_State* L);
    int GetDeltaMousePosition(lua_State* L); 
	int GetFingerState(lua_State* L);
	int GetFingerPosition(lua_State* L);
	int GetFingerDeltaPosition(lua_State* L);

	// Multi functions (mouse and touch)
	int GetTouchState(lua_State* L);
	
	int GetTextInput(lua_State* L);
	int SetTextInput(lua_State* L);
	int ResetTextInput(lua_State* L);
	int IsTextInputActive(lua_State* L);
	int StartTextInput(lua_State* L);
	int StopTextInput(lua_State* L);
    
    void Embed(lua_State* L)
    {
      LuaEmbedder::AddFunction(L, "GetKeyState", &GetKeyState, "Input");
      LuaEmbedder::AddFunction(L, "GetMouseButtonState", &GetMouseButtonState, "Input");
      LuaEmbedder::AddFunction(L, "GetMousePosition", &GetMousePosition, "Input");
      LuaEmbedder::AddFunction(L, "GetDeltaMousePosition", &GetDeltaMousePosition, "Input");
	  LuaEmbedder::AddFunction(L, "GetFingerState", &GetFingerState, "Input");
	  LuaEmbedder::AddFunction(L, "GetFingerPosition", &GetFingerPosition, "Input");
	  LuaEmbedder::AddFunction(L, "GetFingerDeltaPosition", &GetFingerDeltaPosition, "Input");
	
	  LuaEmbedder::AddFunction(L, "GetTouchState", &GetTouchState, "Input");
	  
	  LuaEmbedder::AddFunction(L, "GetTextInput", &GetTextInput, "Input");
	  LuaEmbedder::AddFunction(L, "SetTextInput", &SetTextInput, "Input");
	  LuaEmbedder::AddFunction(L, "ResetTextInput", &ResetTextInput, "Input");
	  LuaEmbedder::AddFunction(L, "IsTextInputActive", &IsTextInputActive, "Input");
	  LuaEmbedder::AddFunction(L, "StartTextInput", &StartTextInput, "Input");
	  LuaEmbedder::AddFunction(L, "StopTextInput", &StopTextInput, "Input");
      
      LuaEmbedder::AddInt(L, "Up", (int)Input::InputState::UP, "InputState");
      LuaEmbedder::AddInt(L, "Down", (int)Input::InputState::DOWN, "InputState");
      LuaEmbedder::AddInt(L, "Pressed", (int)Input::InputState::PRESSED, "InputState");
      LuaEmbedder::AddInt(L, "Released", (int)Input::InputState::RELEASED, "InputState");
      
      LuaEmbedder::AddInt(L, "X", (int)Input::Axis::X, "Axis");
      LuaEmbedder::AddInt(L, "Y", (int)Input::Axis::Y, "Axis");
      
      LuaEmbedder::AddInt(L, "A", (int)SDL_SCANCODE_A, "Key");
      LuaEmbedder::AddInt(L, "B", (int)SDL_SCANCODE_B, "Key");
      LuaEmbedder::AddInt(L, "C", (int)SDL_SCANCODE_C, "Key");
      LuaEmbedder::AddInt(L, "D", (int)SDL_SCANCODE_D, "Key");
      LuaEmbedder::AddInt(L, "E", (int)SDL_SCANCODE_E, "Key");
      LuaEmbedder::AddInt(L, "F", (int)SDL_SCANCODE_F, "Key");
      LuaEmbedder::AddInt(L, "G", (int)SDL_SCANCODE_G, "Key");
      LuaEmbedder::AddInt(L, "H", (int)SDL_SCANCODE_H, "Key");
      LuaEmbedder::AddInt(L, "I", (int)SDL_SCANCODE_I, "Key");
      LuaEmbedder::AddInt(L, "J", (int)SDL_SCANCODE_J, "Key");
      LuaEmbedder::AddInt(L, "K", (int)SDL_SCANCODE_K, "Key");
      LuaEmbedder::AddInt(L, "L", (int)SDL_SCANCODE_L, "Key");
      LuaEmbedder::AddInt(L, "M", (int)SDL_SCANCODE_M, "Key");
      LuaEmbedder::AddInt(L, "N", (int)SDL_SCANCODE_N, "Key");
      LuaEmbedder::AddInt(L, "O", (int)SDL_SCANCODE_O, "Key");
      LuaEmbedder::AddInt(L, "P", (int)SDL_SCANCODE_P, "Key");
      LuaEmbedder::AddInt(L, "Q", (int)SDL_SCANCODE_Q, "Key");
      LuaEmbedder::AddInt(L, "S", (int)SDL_SCANCODE_S, "Key");
      LuaEmbedder::AddInt(L, "T", (int)SDL_SCANCODE_T, "Key");
      LuaEmbedder::AddInt(L, "U", (int)SDL_SCANCODE_U, "Key");
      LuaEmbedder::AddInt(L, "V", (int)SDL_SCANCODE_V, "Key");
      LuaEmbedder::AddInt(L, "W", (int)SDL_SCANCODE_W, "Key");
      LuaEmbedder::AddInt(L, "X", (int)SDL_SCANCODE_X, "Key");
      LuaEmbedder::AddInt(L, "Y", (int)SDL_SCANCODE_Y, "Key");
      LuaEmbedder::AddInt(L, "Z", (int)SDL_SCANCODE_Z, "Key"); 
      LuaEmbedder::AddInt(L, "_1", (int)SDL_SCANCODE_1, "Key");
      LuaEmbedder::AddInt(L, "_2", (int)SDL_SCANCODE_2, "Key");
      LuaEmbedder::AddInt(L, "_3", (int)SDL_SCANCODE_3, "Key");
      LuaEmbedder::AddInt(L, "_4", (int)SDL_SCANCODE_4, "Key");
      LuaEmbedder::AddInt(L, "_5", (int)SDL_SCANCODE_5, "Key");
      LuaEmbedder::AddInt(L, "_6", (int)SDL_SCANCODE_6, "Key");
      LuaEmbedder::AddInt(L, "_7", (int)SDL_SCANCODE_7, "Key");
      LuaEmbedder::AddInt(L, "_8", (int)SDL_SCANCODE_8, "Key");
      LuaEmbedder::AddInt(L, "_9", (int)SDL_SCANCODE_9, "Key");
	  LuaEmbedder::AddInt(L, "_0", (int)SDL_SCANCODE_0, "Key");
	  LuaEmbedder::AddInt(L, "Left", (int)SDL_SCANCODE_LEFT, "Key");
	  LuaEmbedder::AddInt(L, "Right", (int)SDL_SCANCODE_RIGHT, "Key");
	  LuaEmbedder::AddInt(L, "Up", (int)SDL_SCANCODE_UP, "Key");
	  LuaEmbedder::AddInt(L, "Down", (int)SDL_SCANCODE_DOWN, "Key");
      LuaEmbedder::AddInt(L, "Return", (int)SDL_SCANCODE_RETURN, "Key");
      LuaEmbedder::AddInt(L, "Escape", (int)SDL_SCANCODE_ESCAPE, "Key");
      LuaEmbedder::AddInt(L, "Backspace", (int)SDL_SCANCODE_BACKSPACE, "Key");
      LuaEmbedder::AddInt(L, "Tab", (int)SDL_SCANCODE_TAB, "Key");
      LuaEmbedder::AddInt(L, "Space", (int)SDL_SCANCODE_SPACE, "Key");
      
      LuaEmbedder::AddInt(L, "LeftButton", (int)Input::MouseButton::LeftButton, "MouseButton");
      LuaEmbedder::AddInt(L, "MiddleButton", (int)Input::MouseButton::MiddleButton, "MouseButton");
      LuaEmbedder::AddInt(L, "RightButton", (int)Input::MouseButton::RightButton, "MouseButton");
      LuaEmbedder::AddInt(L, "Mouse4", (int)Input::MouseButton::Mouse4, "MouseButton");
      LuaEmbedder::AddInt(L, "Mouse5", (int)Input::MouseButton::Mouse5, "MouseButton");
    }
    
    int GetKeyState(lua_State* L)
    {

		lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
		if (LuaBridge::g_IOLuaState != parent)
		{
			LuaEmbedder::PushInt(L, Input::InputState::UP);
			return 1;
		}

      Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
      int keyState = inputWrapper.GetKeyboard()->GetKeyState((SDL_Scancode)LuaEmbedder::PullInt(L, 1));
      LuaEmbedder::PushInt(L, keyState);
      return 1;
    }
    
    int GetMouseButtonState(lua_State* L)
    {
		lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
		if (LuaBridge::g_IOLuaState != parent)
		{
			LuaEmbedder::PushInt(L, Input::InputState::UP);
			return 1;
		}
      Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
      int buttonState = inputWrapper.GetMouse()->GetButtonState((Input::MouseButton)LuaEmbedder::PullInt(L, 1));
      LuaEmbedder::PushInt(L, buttonState);
      return 1;
    }
    
    int GetMousePosition(lua_State* L)
    {
		lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
		if (LuaBridge::g_IOLuaState != parent)
		{
			LuaEmbedder::PushInt(L, -1);
			LuaEmbedder::PushInt(L, -1);
			return 2;
		}
      Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
      LuaEmbedder::PushInt(L, inputWrapper.GetMouse()->GetX());
      LuaEmbedder::PushInt(L, inputWrapper.GetMouse()->GetY());
      return 2;
    }
    
    int GetDeltaMousePosition(lua_State* L)
    {
		lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
		if (LuaBridge::g_IOLuaState != parent)
		{
			LuaEmbedder::PushInt(L, 0);
			LuaEmbedder::PushInt(L, 0);
			return 2;
		}
      Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
      LuaEmbedder::PushInt(L, inputWrapper.GetMouse()->GetdX());
      LuaEmbedder::PushInt(L, inputWrapper.GetMouse()->GetdY());
      return 2;
    }

	int GetFingerState(lua_State* L)
	{
		lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
		if (LuaBridge::g_IOLuaState != parent)
		{
			LuaEmbedder::PushInt(L, Input::InputState::UP);
			return 1;
		}
		Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		int fingerState = inputWrapper.GetTouch()->GetFingerState((SDL_FingerID)LuaEmbedder::PullInt(L, 1));
		LuaEmbedder::PushInt(L, fingerState);
		return 1;
	}

	int GetFingerPosition(lua_State* L)
	{
		lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
		if (LuaBridge::g_IOLuaState != parent)
		{
			LuaEmbedder::PushInt(L, -1);
			LuaEmbedder::PushInt(L, -1);
			return 2;
		}
		Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		SDL_FingerID finger = (SDL_FingerID)LuaEmbedder::PullInt(L, 1);
		LuaEmbedder::PushFloat(L, inputWrapper.GetTouch()->GetX(finger));
		LuaEmbedder::PushFloat(L, inputWrapper.GetTouch()->GetY(finger));
		return 2;
	}

	int GetFingerDeltaPosition(lua_State* L)
	{
		lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
		if (LuaBridge::g_IOLuaState != parent)
		{
			LuaEmbedder::PushInt(L, 0);
			LuaEmbedder::PushInt(L, 0);
			return 2;
		}
		Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		SDL_FingerID finger = (SDL_FingerID)LuaEmbedder::PullInt(L, 1);
		LuaEmbedder::PushFloat(L, inputWrapper.GetTouch()->GetdX(finger));
		LuaEmbedder::PushFloat(L, inputWrapper.GetTouch()->GetdY(finger));
		return 2;
	}

	int GetTouchState(lua_State* L)
	{
		lua_State* parent = LuaEmbedder::LuaChildrenParentMap.find(L) != LuaEmbedder::LuaChildrenParentMap.end() ? LuaEmbedder::LuaChildrenParentMap[L] : L;
		if (LuaBridge::g_IOLuaState != parent)
		{
			LuaEmbedder::PushInt(L, Input::InputState::UP);
			return 1;
		}
		#if defined(__ANDROID__) || defined(__IOS__)
		Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		int fingerState = inputWrapper.GetTouch()->GetFingerState((SDL_FingerID)LuaEmbedder::PullInt(L, 1));
		LuaEmbedder::PushInt(L, fingerState);
		return 1;
		#else
		Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		int buttonState = inputWrapper.GetMouse()->GetButtonState(Input::MouseButton::LeftButton);
		LuaEmbedder::PushInt(L, buttonState);
		return 1;
		#endif
	}
	
	int GetTextInput(lua_State* L)
	{
		Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		std::string text = std::string(inputWrapper.GetKeyboard()->GetTextInput());
		LuaEmbedder::PushString(L, text);
		return 1;
	}
	
	int SetTextInput(lua_State* L)
	{
		std::string text = LuaEmbedder::PullString(L, 1);
		Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		inputWrapper.GetKeyboard()->SetTextInput(text.c_str());
		return 0;
	}
	
	int ResetTextInput(lua_State* L)
	{
		Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		inputWrapper.GetKeyboard()->ResetTextInput();
		return 0;
	}
	
	int IsTextInputActive(lua_State* L)
	{
		Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		LuaEmbedder::PushBool(L, inputWrapper.GetKeyboard()->IsTextInputActive());
		return 1;
	}
	
	int StartTextInput(lua_State* L)
	{
		Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		inputWrapper.GetKeyboard()->StartTextInput();
		return 0;
	}

	int StopTextInput(lua_State* L)
	{
		Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
		inputWrapper.GetKeyboard()->StopTextInput();
		return 0;
	}
  }
}
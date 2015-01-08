#include "LuaInput.h"
#include "LuaEmbedder/LuaEmbedder.h"

namespace LuaBridge
{
  namespace LuaInput
  {
    int GetKeyState();
    int GetMouseButtonState();
    int GetMousePosition();
    int GetDeltaMousePosition(); 
    
    void Embed() 
    {
      LuaEmbedder::AddFunction("GetKeyState", &GetKeyState, "Input");
      LuaEmbedder::AddFunction("GetMouseButtonState", &GetMouseButtonState, "Input");
      LuaEmbedder::AddFunction("GetMousePosition", &GetMousePosition, "Input");
      LuaEmbedder::AddFunction("GetDeltaMousePosition", &GetDeltaMousePosition, "Input");
      
      LuaEmbedder::AddInt("Up", (int)Input::InputState::UP, "InputState");
      LuaEmbedder::AddInt("Down", (int)Input::InputState::DOWN, "InputState");
      LuaEmbedder::AddInt("Pressed", (int)Input::InputState::PRESSED, "InputState");
      LuaEmbedder::AddInt("Released", (int)Input::InputState::RELEASED, "InputState");
      
      LuaEmbedder::AddInt("X", (int)Input::Axis::X, "Axis");
      LuaEmbedder::AddInt("Y", (int)Input::Axis::Y, "Axis");
      
      LuaEmbedder::AddInt("A", (int)SDL_SCANCODE_A, "Key");
      LuaEmbedder::AddInt("B", (int)SDL_SCANCODE_B, "Key");
      LuaEmbedder::AddInt("C", (int)SDL_SCANCODE_C, "Key");
      LuaEmbedder::AddInt("D", (int)SDL_SCANCODE_D, "Key");
      LuaEmbedder::AddInt("E", (int)SDL_SCANCODE_E, "Key");
      LuaEmbedder::AddInt("F", (int)SDL_SCANCODE_F, "Key");
      LuaEmbedder::AddInt("G", (int)SDL_SCANCODE_G, "Key");
      LuaEmbedder::AddInt("H", (int)SDL_SCANCODE_H, "Key");
      LuaEmbedder::AddInt("I", (int)SDL_SCANCODE_I, "Key");
      LuaEmbedder::AddInt("J", (int)SDL_SCANCODE_J, "Key");
      LuaEmbedder::AddInt("K", (int)SDL_SCANCODE_K, "Key");
      LuaEmbedder::AddInt("L", (int)SDL_SCANCODE_L, "Key");
      LuaEmbedder::AddInt("M", (int)SDL_SCANCODE_M, "Key");
      LuaEmbedder::AddInt("N", (int)SDL_SCANCODE_N, "Key");
      LuaEmbedder::AddInt("O", (int)SDL_SCANCODE_O, "Key");
      LuaEmbedder::AddInt("P", (int)SDL_SCANCODE_P, "Key");
      LuaEmbedder::AddInt("Q", (int)SDL_SCANCODE_Q, "Key");
      LuaEmbedder::AddInt("S", (int)SDL_SCANCODE_S, "Key");
      LuaEmbedder::AddInt("T", (int)SDL_SCANCODE_T, "Key");
      LuaEmbedder::AddInt("U", (int)SDL_SCANCODE_U, "Key");
      LuaEmbedder::AddInt("V", (int)SDL_SCANCODE_V, "Key");
      LuaEmbedder::AddInt("W", (int)SDL_SCANCODE_W, "Key");
      LuaEmbedder::AddInt("X", (int)SDL_SCANCODE_X, "Key");
      LuaEmbedder::AddInt("Y", (int)SDL_SCANCODE_Y, "Key");
      LuaEmbedder::AddInt("Z", (int)SDL_SCANCODE_Z, "Key"); 
      LuaEmbedder::AddInt("_1", (int)SDL_SCANCODE_1, "Key");
      LuaEmbedder::AddInt("_2", (int)SDL_SCANCODE_2, "Key");
      LuaEmbedder::AddInt("_3", (int)SDL_SCANCODE_3, "Key");
      LuaEmbedder::AddInt("_4", (int)SDL_SCANCODE_4, "Key");
      LuaEmbedder::AddInt("_5", (int)SDL_SCANCODE_5, "Key");
      LuaEmbedder::AddInt("_6", (int)SDL_SCANCODE_6, "Key");
      LuaEmbedder::AddInt("_7", (int)SDL_SCANCODE_7, "Key");
      LuaEmbedder::AddInt("_8", (int)SDL_SCANCODE_8, "Key");
      LuaEmbedder::AddInt("_9", (int)SDL_SCANCODE_9, "Key");
	  LuaEmbedder::AddInt("_0", (int)SDL_SCANCODE_0, "Key");
	  LuaEmbedder::AddInt("Left", (int)SDL_SCANCODE_LEFT, "Key");
	  LuaEmbedder::AddInt("Right", (int)SDL_SCANCODE_RIGHT, "Key");
	  LuaEmbedder::AddInt("Up", (int)SDL_SCANCODE_UP, "Key");
	  LuaEmbedder::AddInt("Down", (int)SDL_SCANCODE_DOWN, "Key");
      LuaEmbedder::AddInt("Return", (int)SDL_SCANCODE_RETURN, "Key");
      LuaEmbedder::AddInt("Escape", (int)SDL_SCANCODE_ESCAPE, "Key");
      LuaEmbedder::AddInt("Backspace", (int)SDL_SCANCODE_BACKSPACE, "Key");
      LuaEmbedder::AddInt("Tab", (int)SDL_SCANCODE_TAB, "Key");
      LuaEmbedder::AddInt("Space", (int)SDL_SCANCODE_SPACE, "Key");
      
      LuaEmbedder::AddInt("LeftButton", (int)Input::MouseButton::LeftButton, "MouseButton");
      LuaEmbedder::AddInt("MiddleButton", (int)Input::MouseButton::MiddleButton, "MouseButton");
      LuaEmbedder::AddInt("RightButton", (int)Input::MouseButton::RightButton, "MouseButton");
      LuaEmbedder::AddInt("Mouse4", (int)Input::MouseButton::Mouse4, "MouseButton");
      LuaEmbedder::AddInt("Mouse5", (int)Input::MouseButton::Mouse5, "MouseButton");
    }
    
    int GetKeyState()
    {
      Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
      int keyState = inputWrapper.GetKeyboard()->GetKeyState((SDL_Scancode)LuaEmbedder::PullInt(1));
      LuaEmbedder::PushInt(keyState);
      return 1;
    }
    
    int GetMouseButtonState()
    {
      Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
      int buttonState = inputWrapper.GetMouse()->GetButtonState((Input::MouseButton)LuaEmbedder::PullInt(1));
      LuaEmbedder::PushInt(buttonState);
      return 1;
    }
    
    int GetMousePosition()
    {
      Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
      LuaEmbedder::PushInt(inputWrapper.GetMouse()->GetX());
      LuaEmbedder::PushInt(inputWrapper.GetMouse()->GetY());
      return 2;
    }
    
    int GetDeltaMousePosition()
    {
      Input::InputWrapper& inputWrapper = Input::InputWrapper::GetInstance();
      LuaEmbedder::PushInt(inputWrapper.GetMouse()->GetdX());
      LuaEmbedder::PushInt(inputWrapper.GetMouse()->GetdY());
      return 2;
    }
  }
}
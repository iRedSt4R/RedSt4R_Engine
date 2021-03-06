#include "../Core/stdafx.h"
#include "../Core/EngineLoop.h"
#include <SDL.h>

RedSt4R::EngineLoop *engineLoop;
bool ImGui_ImplSdlGL3_ProcessEvent(SDL_Event* event)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (event->type)
	{
	case SDL_TEXTINPUT:
	{
		io.AddInputCharactersUTF8(event->text.text);
		return true;
	}
	case SDL_KEYDOWN:
	case SDL_KEYUP:
	{
		int key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
		io.KeysDown[key] = (event->type == SDL_KEYDOWN);
		io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
		io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
		io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
		io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
		return true;
	}
	}
	return false;
}


int main(int argc, char* argv[])
{
	engineLoop = new RedSt4R::EngineLoop(GetActiveWindow());

	SDL_Event SDL_Event;
	const UINT8 *state = SDL_GetKeyboardState(NULL);
	bool bRunning = true;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* MainWindow;
	MainWindow = SDL_CreateWindow("Dragoner Engine", 100, 100, 1366, 768, SDL_WINDOW_SHOWN);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	if (MainWindow) RS_LOG("Sucessfully created SDL 2 Window!")

		engineLoop->Begin();

		while (bRunning)
		{
			engineLoop->dx11Engine->FPSCamera->dx = 0;
			engineLoop->dx11Engine->FPSCamera->dy = 0;
			while (SDL_PollEvent(&SDL_Event))
			{
			ImGui_ImplSdlGL3_ProcessEvent(&SDL_Event);
				switch (SDL_Event.type)
				{
				case SDL_KEYDOWN:
					if (SDL_Event.key.keysym.sym == SDLK_ESCAPE)
					{
						SDL_Quit();
						bRunning = false;
					}
				case SDL_WINDOWEVENT:
					if (SDL_Event.window.event == SDL_WINDOWEVENT_CLOSE)
					{
						SDL_Quit();
						bRunning = false;
					}
					break;
				case SDL_MOUSEMOTION:
					engineLoop->dx11Engine->FPSCamera->dx = SDL_Event.motion.xrel;
					engineLoop->dx11Engine->FPSCamera->dy = SDL_Event.motion.yrel;
					break;
				}
			}

			engineLoop->Update();
		}
	engineLoop->Close();
	return 0;
}

#include <Windows.h>
#include <iostream>
#include "api/api.h"
#include "Game/Game.h"






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
	//CreateConsole();
	Game::Start();
	SetMainFunction(Game::Main);
	Start("jogo foda", hInstance, nCmdShow);
}
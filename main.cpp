#include <iostream>
#include <windows.h>
#include <random>
#include <string>
#include <cstdlib>
#include <mhook-lib/mhook.h>

//======================================================== =
//SToFU Systems C++ / WinAPI Practical Assignment
//======================================================== =
//
//1. Build a C++ application using WinAPI that displays messages in the format `Random number is : <number>`
//in a message box, where `<number > ` is a randomly generated integer from 1 to 20, inclusive.
//
//2. Use the mhook library, development branch : https://github.com/SToFU-Systems/mhook/tree/development
//to hook the message box calls and implement the following behavior :
//-If the message would display the number `1`, replace it with `100`.
//- If the message would display the number `2`, terminate the application abnormally by calling `std::abort()` from `<cstdlib > `:
//```cpp
//std::abort();
//```
//
//3. Using Ghidra, an MCP server for Ghidra, and any HEX editor, patch the compiled application so that, when the number is `2`,
//the call to `std::abort()` is not executed and the application continues running normally.
//You may use an AI assistant to help install and configure the MCP server for Ghidra.
//
//4. Record a video of the entire solution process from the beginning, including your interactions
//with AI assistants and the prompts you use.
//
//The solution must include the source code, a video demonstrating the problem - solving process, and, optionally,
//an explanatory letter describing your solution.
//
//There is no time limit for completing this assignment.
//You may use an AI assistant.


// pointer
using MessageBox_t = int(WINAPI*)(HWND, LPCTSTR, LPCTSTR, UINT);
MessageBox_t TrueMessageBox = MessageBoxW;

//hook

int WINAPI HookedMessageBox(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) {
	std::wstring message = L"";

	if (lpText) {
		message = lpText;
	}

	if (message == L"Random number is: 1") {
		return TrueMessageBox(hWnd, L"Random number is: 100", lpCaption, uType);
	}
	else if (message == L"Random number is: 2") {
		std::abort();
	}

	return TrueMessageBox(hWnd, lpText, lpCaption, uType);

}

class Box {
public:
	Box() : rd_(), gen_(rd_()), dist_(min_, max_){};
	void Run() {
		for (;;) {
			unsigned short int number = dist_(gen_);
			std::wstring message = L"Random number is: " + std::to_wstring(number);

			int msgboxID = MessageBoxW(
				nullptr,
				message.c_str(),
				(LPCWSTR)L"StoFu",
				MB_OKCANCEL | MB_ICONINFORMATION
			);

			if (msgboxID == IDCANCEL)
				break;
		}
	}
private:
	const unsigned short int min_ = 1;
	const unsigned short int max_ = 20;
	std::random_device rd_;
	std::mt19937 gen_;
	std::uniform_int_distribution<> dist_;
};


int main() {

	if (Mhook_SetHook(reinterpret_cast<PVOID*>(&TrueMessageBox), reinterpret_cast<PVOID>(HookedMessageBox))) {
		std::cout << "HOOK\n";
	}

	int testNumber = 4;

	std::wstring message =
		L"Random number is: " + std::to_wstring(testNumber);

	MessageBoxW(
		nullptr,
		message.c_str(),
		L"StoFu",
		MB_OK | MB_ICONINFORMATION
	);

	Box box;
	box.Run();

	Mhook_Unhook(reinterpret_cast<PVOID*>(&TrueMessageBox));

	return 0;
}
# SToFU Systems — C++ / WinAPI Practical Assignment

A Windows x64 application that displays random numbers in a WinAPI message box, intercepts `MessageBoxW` using the **Mhook** library, and demonstrates binary patching with **Ghidra** and **HxD**.

## Video Walkthrough

**[Watch the solution walkthrough (Google Drive)](https://drive.google.com/drive/folders/16v9lZiL4v2Ulbt3ZZ9LJKFnhMn3t_wzT?usp=sharing)**

The video recording documents the solution process and is provided as part of this assignment submission.

## Assignment Behavior

| Generated number | Original application | Patched executable |
|---|---|---|
| 1 | Displays 100 | Displays 100 |
| 2 | Calls `std::abort()` | Displays 2 and continues running |
| 3–20 | Displays the generated number | Displays the generated number |

The application uses `std::mt19937` and `std::uniform_int_distribution` to generate integers from 1 to 20, inclusive.

## Project Files

- `main.cpp` — C++ source code with WinAPI message boxes and Mhook integration.
- `Test_task.sln` — Visual Studio 2022 solution.
- `Test_task.vcxproj` — project configuration.
- `PATCH_NOTES.md` — detailed explanation of the binary patch.
- `SToFU_Submission_Clean.zip` — complete project, Mhook dependency, and original and patched executables.

The complete project archive contains the original folder structure required for building.

## Build Instructions

**Requirements:** Windows x64, Visual Studio 2022, MSVC v143 and Windows SDK.

1. Download and extract `SToFU_Submission_Clean.zip`.
2. Open `StoFu/Test_task/Test_task.sln`.
3. Select **Debug | x64**.
4. Build the project using `Ctrl + Shift + B`.
5. Run the application.

The archive includes Mhook headers and a prebuilt x64 Debug library. To rebuild Mhook from source, use:

```bat
setup.bat --build x64-debug
```

The source includes an initial fixed-number test dialog (`testNumber = 4`) before the randomized loop. Rebuilding produces an unpatched executable. The previously patched executable is provided separately in the archive.

## Mhook Implementation

The application intercepts `MessageBoxW` using the Mhook library from the SToFU Systems `development` branch.

The hook performs the following operations:

- Replaces the displayed number 1 with 100.
- Calls `std::abort()` when the displayed number is 2.
- Forwards all other messages to the original function through the Mhook trampoline.

## Reverse Engineering and Binary Patching

The compiled executable was analyzed using **Ghidra 12.1.3** and patched using the **HxD HEX Editor**.

The conditional jump before `std::abort()` was identified at virtual address `0x1400295AC`.

At file offset `0x69AC`, the following modification was made:

```asm
Original: 74 07    JZ  +7
Patched:  EB 07    JMP +7
```

This changes a conditional jump into an unconditional jump, allowing the application to bypass `std::abort()` when the number is 2.

The original and patched executables differ by exactly one byte.

Additional details are available in `PATCH_NOTES.md`.

## Ghidra MCP Integration

The GhidraMCP plugin was installed and configured for Ghidra 12.1.3.

The local MCP server connection was verified at:

`http://127.0.0.1:8089/check_connection`

The Ghidra-side MCP server was confirmed operational. An external AI client connection through MCP was not completed.

## Technologies Used

- C++ / WinAPI
- Mhook (`development` branch)
- Visual Studio 2022
- Ghidra 12.1.3
- GhidraMCP
- HxD HEX Editor

**Mhook repository:** https://github.com/SToFU-Systems/mhook/tree/development

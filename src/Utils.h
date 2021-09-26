#pragma once

#include <Windows.h>
#include <string>

#define MAX(x, y) (x > y ? x : y)

std::string ShowSaveFileDialog(std::string ext = ".terr3d", HWND owner = NULL);

std::string openfilename(HWND owner = NULL);

std::string ShowOpenFileDialog(const char* ext = "*.glsl\0*.*\0", HWND owner = NULL);

std::string ReadShaderSourceFile(std::string path, bool* result);

void Log(const char* log);

void Log(std::string log);
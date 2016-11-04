#pragma once

#include <windows.h>
#include <iostream>
#include <cstdlib>

#define RS_ERROR(x) { HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hOut, FOREGROUND_RED|FOREGROUND_INTENSITY); std::cout<<x<<std::endl; }
#define RS_WARNING(x) { HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hOut, 14|FOREGROUND_INTENSITY); std::cout<<x<<std::endl; }
#define RS_LOG(x) { HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); SetConsoleTextAttribute(hOut, 1|7|FOREGROUND_INTENSITY); std::cout<<x<<std::endl; }
#define SAFE_RELEASE(x) if(x != nullptr) x->Release();
/*
void CheckVk(VkResult a_Result, char* a_FailedMessage)
{
if (a_Result != VK_SUCCESS)
{
RS_ERROR(a_FailedMessage)
}
}

void CheckVk(VkResult a_Result, char* a_FailedMessage, char* a_SuccessMessage)
{
if (a_Result != VK_SUCCESS)
{
RS_ERROR(a_FailedMessage)
}
else
{
RS_LOG(a_SuccessMessage)
}
}
*/
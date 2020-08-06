#pragma once
#define _CRT_SECURE_NO_WARNINGS


void log_debug(const char* fmt, ...)
{
    static char buffer[1000];//必须大于参数
    va_list args;
    va_start(args, fmt);
    vsprintf_s(buffer, fmt, args);
    va_end(args);
    FILE* fp = NULL;
    fopen_s(&fp, "e:\\shlog\\warspear.log", "a");
    fprintf_s(fp, buffer);
    fclose(fp);
}
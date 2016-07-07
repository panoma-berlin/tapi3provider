/*
	Copyright (c) 2005 Serban Iordache 
	
	All rights reserved. 
	
	Permission is hereby granted, free of charge, to any person obtaining a 
	copy of this software and associated documentation files (the 
	"Software"), to deal in the Software without restriction, including 
	without limitation the rights to use, copy, modify, merge, publish, 
	distribute, and/or sell copies of the Software, and to permit persons 
	to whom the Software is furnished to do so, provided that the above 
	copyright notice(s) and this permission notice appear in all copies of 
	the Software and that both the above copyright notice(s) and this 
	permission notice appear in supporting documentation. 
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT 
	OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
	HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL 
	INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING 
	FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, 
	NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION 
	WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
	
	Except as contained in this notice, the name of a copyright holder 
	shall not be used in advertising or otherwise to promote the sale, use 
	or other dealings in this Software without prior written authorization 
	of the copyright holder.
*/
#include "stdafx.h"
#include <time.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h> 
#include <malloc.h>
#include <signal.h>
#include <atlstr.h>
#include "Logger.h"

const int MAX_MSG_LEN = 1024;

FILE* Logger::logFile = NULL;

// constructor
Logger::Logger(const char* name) {
	m_name = new char[strlen(name)+1];
	strcpy_s(m_name, strlen(name)+1, name);
}


// destructor
Logger::~Logger() {
	delete[] m_name;
}

void Logger::setLogFile(const char* path) {
	if(logFile != NULL) {
		fclose(logFile);
	}
	//logFile = fopen(path, "a");
	fopen_s(&logFile, path, "at");
}

#define LOG_MESSAGE(level) \
	va_list args; \
	va_start(args, format); \
  int len=_vscprintf(format, args)+1; \
	char *buffer=(char *)malloc(len*sizeof(char)); \
	vsprintf_s( buffer, len, format, args ); \
	logMsg(level, buffer); \
  free(buffer);

void Logger::debug(const char* format, ...) {
	LOG_MESSAGE(DEBUG_LEVEL);
}

void Logger::info(const char* format, ...) {
	LOG_MESSAGE(INFO_LEVEL);
}

void Logger::warn(const char* format, ...) {
	LOG_MESSAGE(WARN_LEVEL);
}

void Logger::error(const char* format, ...) {
	LOG_MESSAGE(ERROR_LEVEL);
}

void Logger::fatal(const char* format, ...) {
	LOG_MESSAGE(FATAL_LEVEL);
}

const char* levelNames[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
void Logger::logMsg(int level, const char* msg) {
	if (logFile==NULL) return;

  time_t ltime;
  wchar_t buf[26];
  errno_t err;
  
  time(&ltime);
  err = _wctime_s(buf, 26, &ltime);
  if (err==0) {
    size_t origsize = wcslen(buf) + 1;
    size_t convertedChars = 0;

    const size_t newsize = origsize * 2;
    char *nstring = new char[newsize];

    wcstombs_s(&convertedChars, nstring, newsize, buf, _TRUNCATE);

    fprintf(logFile, "%s %-5s: %s - %s\n", nstring, levelNames[level], m_name, msg);
  }
  else {
    fprintf(logFile, "Invalid Arguments for _wctime_s. Error Code: %d\n", err);
    fprintf(logFile, "%-5s: %s - %s\n", levelNames[level], m_name, msg);
  }
}

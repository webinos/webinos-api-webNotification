/*******************************************************************************
*  Code contributed to the webinos project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Copyright 2014 Andreas Botsikas, NTUA
******************************************************************************/
#pragma once
#include <windows.h> 
#include <iostream>
#include <windows.h> 
#include <FileAPI.h>
#include <WinInet.h> 
#include "string.h"
#include <strsafe.h>


using namespace std;
class downloadImage
{
public:
	HRESULT static GetImage(const wchar_t* imageUrl, _Outptr_ wchar_t* savedFilePath);
private:
	HRESULT static extractFilenameFromUrl(const wchar_t* url, _Outptr_ wchar_t* fname);
	HRESULT static downloadFile(const wchar_t* url, wchar_t* saveToFolder, _Outptr_ wchar_t* savedFilePath);
};


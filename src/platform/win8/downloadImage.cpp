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
#include "downloadImage.h"

// extract the filename from the URL
HRESULT downloadImage::extractFilenameFromUrl(const wchar_t* url, wchar_t* fname)
{
	size_t url_length, i, j;
	url_length = wcslen(url);
	// Detect last /
	i = url_length - 1;
	while ((i > 0) && (url[i] != '/') && (url[i] != '\\'))  { i--; }
	j = 0; i++;
	// Copy the filename
	while (i < url_length) { fname[j++] = url[i++]; }
	fname[j] = '\0';
	return 0;
}

HRESULT downloadImage::downloadFile(const wchar_t* url, wchar_t* saveToFolder, _Outptr_ wchar_t* savedFilePath)
{
	HRESULT hr = -1;
	HINTERNET hInternet;
	HINTERNET  hConnect;
	wchar_t  fileName[MAX_PATH] = L"";
	wchar_t destinationFile[MAX_PATH] = L"";

	

	hInternet = InternetOpen(L"webinos", // agent
		INTERNET_OPEN_TYPE_DIRECT,  //__in  DWORD dwAccessType
		NULL,                       //__in  LPCTSTR lpszProxyName,
		NULL,                       //__in  LPCTSTR lpszProxyBypass,
		NULL                        //_in   DWORD dwFlags
		);
	// Dummy headers to get file
	TCHAR emptyHeaders[15];
	emptyHeaders[0] = '\0';

	if (!(hConnect = InternetOpenUrl(hInternet, url, emptyHeaders, 15, INTERNET_FLAG_DONT_CACHE, 0)))
	{
		std::cout << "Error: InternetOpenUrl" << std::endl;
	}
	else{
		// Get the filename
		extractFilenameFromUrl(url, fileName);
		hr = StringCchCat(destinationFile, ARRAYSIZE(destinationFile), saveToFolder);
		if (SUCCEEDED(hr)){
			hr = StringCchCat(destinationFile, ARRAYSIZE(destinationFile), fileName);
			// We have the final destination, so copy it to the output var
			wcsncpy(savedFilePath, destinationFile, wcslen(destinationFile));
			destinationFile[wcslen(destinationFile)] = '\0';
			if (SUCCEEDED(hr)){
				// Downlaod and save file
				FILE * outputFile;
				BYTE * buffer[1024];
				DWORD readSize;
				if (!(outputFile = _wfopen(destinationFile, L"wb")))
				{
					std::cerr << "Error _tfopen" << std::endl;
					return false;
				}
				do
				{
					// Keep copying in 1024 bytes chunks, while file has any data left.
					// Note: bigger buffer will greatly improve performance.
					if (!InternetReadFile(hConnect, buffer, 1024, &readSize))
					{
						fclose(outputFile);
						std::cerr << "Error InternetReadFile" << std::endl;
						return FALSE;
					}
					if (!readSize)
						break;  // Condition of readSize=0 indicate EOF. Stop.
					else
						fwrite(buffer, sizeof (BYTE), readSize, outputFile);
				}   // do
				while (TRUE);
				fflush(outputFile);
				fclose(outputFile);
			}
		}
	}
	InternetCloseHandle(hInternet);

	return hr;
}

HRESULT downloadImage::GetImage(const wchar_t* imageUrl, _Outptr_ wchar_t* savedFilePath){
	wchar_t windowsTempDir[MAX_PATH] = L"";
	HRESULT hr;
	hr = GetTempPath(MAX_PATH, windowsTempDir);
	if (SUCCEEDED(hr)){
		hr = downloadFile(imageUrl, windowsTempDir, savedFilePath);
	}
	else{
		std::cerr << "Error Getting temp folder" << std::endl;
	}
	return hr;
}
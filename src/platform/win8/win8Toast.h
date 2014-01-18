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
#include <SDKDDKVer.h>

// Windows Header Files:
#include <Windows.h>
#include <strsafe.h>
#include <intsafe.h>

// WinRT
#include <Psapi.h>
#include <ShObjIdl.h>
#include <propvarutil.h>
#include <functiondiscoverykeys.h>
//#include <roapi.h>
#include <wrl\implements.h>
#include <windows.ui.notifications.h>

#include "StringReferenceWrapper.h"

using namespace Microsoft::WRL;
using namespace ABI::Windows::UI::Notifications;
using namespace ABI::Windows::Data::Xml::Dom;
using namespace Windows::Foundation;

const wchar_t AppId[] = L"webinos.api.webNotification";

class win8Toast
{
public:
	win8Toast();
	~win8Toast();
	HRESULT DisplayToast(const wchar_t* title, const wchar_t* body);
private:
	HRESULT TryCreateShortcut();
	HRESULT InstallShortcut(_In_z_ wchar_t *shortcutPath);
	HRESULT CreateToastXml(
		_In_ ABI::Windows::UI::Notifications::IToastNotificationManagerStatics *toastManager,
		_Outptr_ ABI::Windows::Data::Xml::Dom::IXmlDocument **xml,
		const wchar_t* title,
		const wchar_t* body
		);
	HRESULT CreateToast(
		_In_ ABI::Windows::UI::Notifications::IToastNotificationManagerStatics *toastManager,
		_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument *xml
		);
	// Xml helpers
	HRESULT SetImageSrc(
		_In_z_ wchar_t *imagePath,
		_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument *toastXml
		);
	HRESULT SetTextValues(
		_In_reads_(textValuesCount) const wchar_t **textValues,
		_In_ UINT32 textValuesCount,
		_In_reads_(textValuesCount) UINT32 *textValuesLengths,
		_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument *toastXml
		);
	HRESULT SetNodeValueString(
		_In_ HSTRING onputString,
		_In_ ABI::Windows::Data::Xml::Dom::IXmlNode *node,
		_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument *xml
		);
};


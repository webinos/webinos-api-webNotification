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
#include "win8Toast.h"

win8Toast::win8Toast()
{
	RoInitialize(RO_INIT_SINGLETHREADED);
	// Try to create the shortcut 
	// that is required by winRT
	// http://msdn.microsoft.com/en-us/library/windows/desktop/hh802762(v=vs.85).aspx
	TryCreateShortcut();
}


win8Toast::~win8Toast()
{
	RoUninitialize();
}

bool win8Toast::DisplayToast(const wchar_t* title, const wchar_t* body, const wchar_t* icon){
	ComPtr<IToastNotificationManagerStatics> toastStatics;
	HRESULT hr = GetActivationFactory(StringReferenceWrapper(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager).Get(), &toastStatics);

	if (SUCCEEDED(hr))
	{
		ComPtr<IXmlDocument> toastXml;
		hr = CreateToastXml(toastStatics.Get(), &toastXml, title, body, icon);
		if (SUCCEEDED(hr))
		{
			hr = CreateToast(toastStatics.Get(), toastXml.Get());
		}
	}
	return SUCCEEDED(hr);
}


// Create the toast XML from a template
HRESULT win8Toast::CreateToastXml(_In_ IToastNotificationManagerStatics *toastManager, _Outptr_ IXmlDocument** inputXml,
	const wchar_t* title, const wchar_t* body, const wchar_t* icon)
{
	HRESULT hr;
	// Retrieve the template XML (One string of bold text on the first line, one string of regular text wrapped across the second and third lines.)
	// See http://msdn.microsoft.com/en-us/library/windows/apps/windows.ui.notifications.toasttemplatetype.Aspx
	if (wcslen(icon) > 0 && wcsncmp(icon, L"http", 4) != 0){
		// Web images are not supported in desktop apps
		// See "Desktop apps cannot use web images" in
		// http://msdn.microsoft.com/en-us/library/windows/apps/hh779727.aspx
		hr = toastManager->GetTemplateContent(ToastTemplateType_ToastImageAndText02, inputXml);
		if (SUCCEEDED(hr))
		{
			hr = SetImageSrc(icon, *inputXml);
		}
	}
	else{
		hr = toastManager->GetTemplateContent(ToastTemplateType_ToastText02, inputXml);
	}
	if (SUCCEEDED(hr))
	{
		const wchar_t* textValues[] = {
			title,
			body
		};

		UINT32 textLengths[] = { wcslen(title), wcslen(body) };

		hr = SetTextValues(textValues, 2, textLengths, *inputXml);

		/*
		Disabling the branding is not supported for windows apps :(
		if (SUCCEEDED(hr)){
			hr = DisableBranding(*inputXml);
		}
		*/
	}
	/*PCWSTR t = getXmlString(*inputXml);
	OutputDebugString(t);*/
	return hr;
}

// Create and display the toast
HRESULT win8Toast::CreateToast(_In_ IToastNotificationManagerStatics *toastManager, _In_ IXmlDocument *xml)
{
	ComPtr<IToastNotifier> notifier;
	HRESULT hr = toastManager->CreateToastNotifierWithId(StringReferenceWrapper(AppId).Get(), &notifier);
	if (SUCCEEDED(hr))
	{
		ComPtr<IToastNotificationFactory> factory;
		hr = GetActivationFactory(StringReferenceWrapper(RuntimeClass_Windows_UI_Notifications_ToastNotification).Get(), &factory);
		if (SUCCEEDED(hr))
		{
			ComPtr<IToastNotification> toast;
			hr = factory->CreateToastNotification(xml, &toast);
			if (SUCCEEDED(hr))
			{
				// Before showing, we could register event handlers like in
				// http://code.msdn.microsoft.com/windowsdesktop/sending-toast-notifications-71e230a2/
				hr = notifier->Show(toast.Get());
			}
		}
	}
	return hr;
}

// Create shortcut helpers


// In order to display toasts, a desktop application must have a shortcut on the Start menu.
// Also, an AppUserModelID must be set on that shortcut.
// The shortcut should be created as part of the installer. The following code shows how to create
// a shortcut and assign an AppUserModelID using Windows APIs. You must download and include the 
// Windows API Code Pack for Microsoft .NET Framework for this code to function
//
// Included in this project is a wxs file that be used with the WiX toolkit
// to make an installer that creates the necessary shortcut. One or the other should be used.

HRESULT win8Toast::TryCreateShortcut()
{
	wchar_t shortcutPath[MAX_PATH];
	DWORD charWritten = GetEnvironmentVariable(L"APPDATA", shortcutPath, MAX_PATH);
	HRESULT hr = charWritten > 0 ? S_OK : E_INVALIDARG;

	if (SUCCEEDED(hr))
	{
		errno_t concatError = wcscat_s(shortcutPath, ARRAYSIZE(shortcutPath), L"\\Microsoft\\Windows\\Start Menu\\Programs\\webinos.webNotifications.lnk");
		hr = concatError == 0 ? S_OK : E_INVALIDARG;
		if (SUCCEEDED(hr))
		{
			DWORD attributes = GetFileAttributes(shortcutPath);
			bool fileExists = attributes < 0xFFFFFFF;

			if (!fileExists)
			{
				hr = InstallShortcut(shortcutPath);
			}
			else
			{
				hr = S_FALSE;
			}
		}
	}
	return hr;
}

// Install the shortcut
HRESULT win8Toast::InstallShortcut(_In_z_ wchar_t *shortcutPath)
{

	ComPtr<IShellLink> shellLink;
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_LOCAL_SERVER, IID_PPV_ARGS(&shellLink));

	if (SUCCEEDED(hr))
	{
		hr = shellLink->SetPath(L"http://www.webinos.org");
		if (SUCCEEDED(hr))
		{
			hr = shellLink->SetArguments(L"");
			if (SUCCEEDED(hr))
			{
				ComPtr<IPropertyStore> propertyStore;

				hr = shellLink.As(&propertyStore);
				if (SUCCEEDED(hr))
				{
					PROPVARIANT appIdPropVar;
					hr = InitPropVariantFromString(AppId, &appIdPropVar);
					if (SUCCEEDED(hr))
					{
						hr = propertyStore->SetValue(PKEY_AppUserModel_ID, appIdPropVar);
						if (SUCCEEDED(hr))
						{
							hr = propertyStore->Commit();
							if (SUCCEEDED(hr))
							{
								ComPtr<IPersistFile> persistFile;
								hr = shellLink.As(&persistFile);
								if (SUCCEEDED(hr))
								{
									hr = persistFile->Save(shortcutPath, TRUE);
								}
							}
						}
						PropVariantClear(&appIdPropVar);
					}
				}
			}
		}
	}
	return hr;
}


// XML helpers
// Helper to visualize xml
PCWSTR win8Toast::getXmlString(_In_ IXmlDocument *toastXml){
	ComPtr<ABI::Windows::Data::Xml::Dom::IXmlNodeSerializer> s;
	ComPtr<ABI::Windows::Data::Xml::Dom::IXmlDocument> ss(toastXml);
	ss.As(&s);
	HSTRING string;
	s->GetXml(&string);
	return WindowsGetStringRawBuffer(string, NULL);
}

// Disable branding in order to avoid the shortcut icon
HRESULT win8Toast::DisableBranding(_In_ IXmlDocument *toastXml){
	ComPtr<IXmlNodeList> nodeList;
	HRESULT hr = toastXml->GetElementsByTagName(StringReferenceWrapper(L"binding").Get(), &nodeList);
	if (SUCCEEDED(hr))
	{
		ComPtr<IXmlNode> bindingNode;
		hr = nodeList->Item(0, &bindingNode);
		if (SUCCEEDED(hr))
		{
			ComPtr<IXmlNamedNodeMap> attributes;

			hr = bindingNode->get_Attributes(&attributes);
			if (SUCCEEDED(hr))
			{	
				ComPtr<IXmlNode> brandingAttribute;
				hr = attributes->GetNamedItem(StringReferenceWrapper(L"branding").Get(), &brandingAttribute);
				if (SUCCEEDED(hr))
				{
					ComPtr<ABI::Windows::Data::Xml::Dom::IXmlAttribute> newAttribute;
					toastXml->CreateAttribute(StringReferenceWrapper(L"branding").Get(), &newAttribute);
					newAttribute->put_Value(StringReferenceWrapper(L"none").Get());
					ComPtr<IXmlNode> node;
					hr = newAttribute.As(&node);
					if (SUCCEEDED(hr)){
						attributes->SetNamedItem(node.Get(), &brandingAttribute);
					}
				}
			}
		}
	}
	return hr;
}

// Set the value of the "src" attribute of the "image" node
HRESULT win8Toast::SetImageSrc(_In_z_ const wchar_t *imagePath, _In_ IXmlDocument *toastXml)
{
	wchar_t imageSrc[MAX_PATH] = L"";
	HRESULT hr = 0;
	// This is not a web resource
	if (wcsncmp(imagePath, L"http", 4) != 0){
		// Try as a file
		hr = StringCchCat(imageSrc, ARRAYSIZE(imageSrc), L"file:///");
	}
	if (SUCCEEDED(hr)){
		hr = StringCchCat(imageSrc, ARRAYSIZE(imageSrc), imagePath);
		if (SUCCEEDED(hr))
		{
			ComPtr<IXmlNodeList> nodeList;
			hr = toastXml->GetElementsByTagName(StringReferenceWrapper(L"image").Get(), &nodeList);
			if (SUCCEEDED(hr))
			{
				ComPtr<IXmlNode> imageNode;
				hr = nodeList->Item(0, &imageNode);
				if (SUCCEEDED(hr))
				{
					ComPtr<IXmlNamedNodeMap> attributes;

					hr = imageNode->get_Attributes(&attributes);
					if (SUCCEEDED(hr))
					{
						ComPtr<IXmlNode> srcAttribute;

						hr = attributes->GetNamedItem(StringReferenceWrapper(L"src").Get(), &srcAttribute);
						if (SUCCEEDED(hr))
						{
							hr = SetNodeValueString(StringReferenceWrapper(imageSrc).Get(), srcAttribute.Get(), toastXml);
						}
					}
				}
			}
		}
	}
	return hr;
}

// Set the values of each of the text nodes
HRESULT win8Toast::SetTextValues(_In_reads_(textValuesCount) const wchar_t **textValues, _In_ UINT32 textValuesCount, _In_reads_(textValuesCount) UINT32 *textValuesLengths, _In_ IXmlDocument *toastXml)
{
	HRESULT hr = textValues != nullptr && textValuesCount > 0 ? S_OK : E_INVALIDARG;
	if (SUCCEEDED(hr))
	{
		ComPtr<IXmlNodeList> nodeList;
		hr = toastXml->GetElementsByTagName(StringReferenceWrapper(L"text").Get(), &nodeList);
		if (SUCCEEDED(hr))
		{
			UINT32 nodeListLength;
			hr = nodeList->get_Length(&nodeListLength);
			if (SUCCEEDED(hr))
			{
				hr = textValuesCount <= nodeListLength ? S_OK : E_INVALIDARG;
				if (SUCCEEDED(hr))
				{
					for (UINT32 i = 0; i < textValuesCount; i++)
					{
						ComPtr<IXmlNode> textNode;
						hr = nodeList->Item(i, &textNode);
						if (SUCCEEDED(hr))
						{
							hr = SetNodeValueString(StringReferenceWrapper(textValues[i], textValuesLengths[i]).Get(), textNode.Get(), toastXml);
						}
					}
				}
			}
		}
	}
	return hr;
}

HRESULT win8Toast::SetNodeValueString(_In_ HSTRING inputString, _In_ IXmlNode *node, _In_ IXmlDocument *xml)
{
	ComPtr<IXmlText> inputText;

	HRESULT hr = xml->CreateTextNode(inputString, &inputText);
	if (SUCCEEDED(hr))
	{
		ComPtr<IXmlNode> inputTextNode;

		hr = inputText.As(&inputTextNode);
		if (SUCCEEDED(hr))
		{
			ComPtr<IXmlNode> pAppendedChild;
			hr = node->AppendChild(inputTextNode.Get(), &pAppendedChild);
		}
	}

	return hr;
}
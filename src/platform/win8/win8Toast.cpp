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
}


win8Toast::~win8Toast()
{
}

HRESULT win8Toast::DisplayToast(const wchar_t* title, const wchar_t* body){
	ComPtr<IToastNotificationManagerStatics> toastStatics;
	HRESULT hr = GetActivationFactory(StringReferenceWrapper(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager).Get(), &toastStatics);

	if (SUCCEEDED(hr))
	{
		ComPtr<IXmlDocument> toastXml;
		hr = CreateToastXml(toastStatics.Get(), &toastXml, title, body);
		if (SUCCEEDED(hr))
		{
			hr = CreateToast(toastStatics.Get(), toastXml.Get());
		}
	}
	return hr;
}


// Create the toast XML from a template
HRESULT win8Toast::CreateToastXml(_In_ IToastNotificationManagerStatics *toastManager, _Outptr_ IXmlDocument** inputXml, const wchar_t* title, const wchar_t* body)
{
	// Retrieve the template XML (One string of bold text on the first line, one string of regular text wrapped across the second and third lines.)
	// See http://msdn.microsoft.com/en-us/library/windows/apps/windows.ui.notifications.toasttemplatetype.Aspx
	HRESULT hr = toastManager->GetTemplateContent(ToastTemplateType_ToastText02, inputXml);
	if (SUCCEEDED(hr))
	{

				const wchar_t* textValues[] = {
					title,
					body
				};

				UINT32 textLengths[] = { 6, 6, 6 };

				hr = SetTextValues(textValues, 3, textLengths, *inputXml);
	}
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


// XML helpers


// Set the value of the "src" attribute of the "image" node
HRESULT win8Toast::SetImageSrc(_In_z_ wchar_t *imagePath, _In_ IXmlDocument *toastXml)
{
	wchar_t imageSrc[MAX_PATH] = L"file:///";
	HRESULT hr = StringCchCat(imageSrc, ARRAYSIZE(imageSrc), imagePath);
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
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
#include <notifications.h>
#include "win8Toast.h"

bool Notifications::show(string title, string body, string dir, string lang, string tag, string icon) {
	// Convert to wchar
	wstring titlews = wstring(title.begin(), title.end());
	wstring bodyws = wstring(body.begin(), body.end());
	wstring iconws = wstring(icon.begin(), icon.end());

	win8Toast* toastService = new win8Toast();
	return toastService->DisplayToast(titlews.c_str(), bodyws.c_str(), iconws.c_str());
}

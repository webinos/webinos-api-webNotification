# webinos web notifications API #

**Service Type**: http://webinos.org/api/notifications

The main concept of Web Notifications API is to display notifications to the end users outside the context of a web page. 
The NotificationManager interface is a discoverable interface through the webinos Discovery API findServices() method and 
thus inherits the Discovery API Service interface.

## Installation ##

To install the web notifications API you will need to npm the node module inside the webinos pzp.

For end users, you can simply open a command prompt in the root of your webinos-pzp and do: 

	npm install https://github.com/webinos/webinos-api-webNotification.git

For developers that want to tweak the API, you should fork this repository and clone your fork inside the node_module of your pzp.

	cd node_modules
	git clone https://github.com/<your GitHub account>/webinos-api-webNotification.git
	cd webinos-api-webNotification
	npm install


## Getting a reference to the service ##

To discover the service you will have to search for the "http://webinos.org/api/notifications" type. Example:

	var serviceType = "http://webinos.org/api/notifications";
	webinos.discovery.findServices( new ServiceType(serviceType), 
		{ 
			onFound: serviceFoundFn, 
			onError: handleErrorFn
		}
	);
	function serviceFoundFn(service){
		// Do something with the service
	};
	function handleErrorFn(error){
		// Notify user
		console.log(error.message);
	}

Alternatively you can use the webinos dashboard to allow the user choose the device orientation API to use. Example:
 	
	webinos.dashboard.open({
         module:'explorer',
	     data:{
         	service:[
            	'http://webinos.org/api/notifications'
         	],
            select:"services"
         }
     }).onAction(function successFn(data){
		  if (data.result.length > 0){
			// User selected some services
		  }
	 });

## Methods ##

Once you have a reference to an instance of a service and bind the service using the bindService method, you can use the following method:

### WebNotification (title, options)

Send a web notification to the bound service.
 

## Links ##

- [Specifications](http://dev.webinos.org/specifications/api/notifications.html)
- [Examples](https://github.com/webinos/webinos-api-webNotification/wiki/Examples)


## Installation considerations ##

You will need at least windows 8 to use the notifications. If you are attempting to build this api from source, you will need the [windows 8 sdk](http://msdn.microsoft.com/en-us/windows/desktop/hh852363.aspx). If you are using visual studio 2010, you will need to read [this stack overflow question](http://stackoverflow.com/questions/14760672/windows-8-sdk-platform-toolset-missing-in-visual-studio-2010). If you want to avoid opening visual studio, then edit the binding.gyp file and add the following sections in the windows configuration:

	'include_dirs':[
		'$(ProgramFiles)\Windows Kits\8.0\Include\um',
		'$(ProgramFiles)\Windows Kits\8.0\Include\shared',
		'$(ProgramFiles)\Windows Kits\8.0\Include\winrt',
		'src',
		'src\platform\win8'
	],
	'msvs_settings': {
		'VCLinkerTool': {
			'AdditionalLibraryDirectories': [
				'$(ProgramFiles)\Windows Kits\8.0\Lib\win8\um\\$(PlatformShortName)'
			]
		}
	}`

Modifying the 8.0 to 8.1 depending on the installed sdk. You will also need to apply the [asyncinfo.h patch described here](http://www.chromium.org/developers/how-tos/build-instructions-windows#TOC-Setting-up-the-environment-for-building-with-Visual-C-2010-Express-or-Windows-7.1-SDK).
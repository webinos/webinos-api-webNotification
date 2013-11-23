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
	cd webinos-api-mediaplay
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


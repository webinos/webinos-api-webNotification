/*******************************************************************************
 *  Code contributed to the webinos project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright 2013 Martin Lasak, Fraunhofer FOKUS
 ******************************************************************************/

beforeEach(function() {
	this.addMatchers({
		toBeEither: function() {
			for (var j=0;j<arguments.length;j++){
				if (arguments[j]===this.actual)	return true;
			}
			return false;
		},
		toBePrintableString: function(){
			return !(/[\x00-\x08\x0E-\x1F]/.test(this.actual)) && 
				 typeof this.actual === "string" && this.actual.length>0;
		},
		toBeValidAsSourceForVideoElement: function(){
			return typeof this.actual === "string" || 
			(typeof this.actual !== "undefined" && 
			 typeof this.actual.audioTracks !== "undefined" && 
			 typeof this.actual.ended !== "undefined" && 
			 typeof this.actual.label !== "undefined" &&
			 typeof this.actual.videoTracks !== "undefined"  )
		}
	});
});

describe("The Web Notifications API", function() {
	var notiService, notiInstance;

	var testTitle = "Example Title",
		testBody = "This is an example body.",
		testIcon = "http://webinos.github.io/css/images/webinos_logo.png",
		testLang = "de",
		testDir = "ltr",
		testTag = "42";

	webinos.discovery.findServices(new ServiceType("http://webinos.org/api/notifications"), {
		onFound: function (service) {
			notiService = service;
		}
	});

	beforeEach(function() {
		waitsFor(function() {
			return !!notiService;
		}, "the discovery to find an Web Notifications service", 10000);
	});

	it("should be available from the discovery", function() {
		expect(notiService).toBeDefined();
	});

	it("has the necessary properties it got inherited from the service object", function() {
		expect(notiService.state).toBeDefined();
		expect(notiService.api).toEqual(jasmine.any(String));
		expect(notiService.id).toEqual(jasmine.any(String));
		expect(notiService.displayName).toEqual(jasmine.any(String));
		expect(notiService.description).toEqual(jasmine.any(String));
		expect(notiService.icon).toEqual(jasmine.any(String));
		expect(notiService.bindService).toEqual(jasmine.any(Function));
	});

	it("can be bound", function() {
		var bound = false;

		notiService.bindService({onBind: function(service) {
			notiService = service;
			bound = true;
		}});

		waitsFor(function() {
			return bound;
		}, "the service to be bound", 1000);

		runs(function() {
			expect(bound).toEqual(true);
		});
	});

	//constructor tests
	it("can create a notification object", function() {
		notiInstance = new notiService.Notification(testTitle, {body: testBody, icon: testIcon, dir: testDir, lang: testLang, tag: testTag})
		expect(notiInstance).toEqual(jasmine.any(Object));
	});

	it("provides to set event handlers to the notification object", function() {
		notiInstance.onclick = function(){ console.log("onclick event"); };
		notiInstance.onshow = function(){ console.log("onshow event"); };
		notiInstance.onerror = function(){ console.log("onerror event"); };
		notiInstance.onclose = function(){ console.log("onclose event"); };
		expect(notiInstance.onclick).toEqual(jasmine.any(Function));
		expect(notiInstance.onshow).toEqual(jasmine.any(Function));
		expect(notiInstance.onerror).toEqual(jasmine.any(Function));
		expect(notiInstance.onclose).toEqual(jasmine.any(Function));
	});

	it("provides notification objects with inherited EventTarget methods", function() {
		expect(notiInstance.addEventListener).toEqual(jasmine.any(Function));
		expect(notiInstance.removeEventListener).toEqual(jasmine.any(Function));
		expect(notiInstance.dispatchEvent).toEqual(jasmine.any(Function));
	});

	it("can create notification objects with valid property type and value for the text direction property", function() {
		expect(notiInstance.dir).toEqual(jasmine.any(String));
		expect(notiInstance.dir).toEqual(testDir);
	});

	it("can create notification objects with valid property type and value for the body property", function() {
		expect(notiInstance.body).toEqual(jasmine.any(String));
		expect(notiInstance.body).toEqual(testBody);
	});

	it("can create notification objects with valid property type and value for the language property", function() {
		expect(notiInstance.lang).toEqual(jasmine.any(String));
		expect(notiInstance.lang).toEqual(testLang);
	});

	it("can create notification objects with valid property type and value for the tag property", function() {
		expect(notiInstance.tag).toEqual(jasmine.any(String));
		expect(notiInstance.tag).toEqual(testTag);
	});

	it("can create notification objects with valid property type and value for the icon property", function() {
		expect(notiInstance.icon).toEqual(jasmine.any(String));
		expect(notiInstance.icon).toEqual(testIcon);
	});

	it("can close created notifications", function() {
		expect(notiInstance.close).toEqual(jasmine.any(Function));
		var functionExecutionSuccess = false;
		try{
			notiInstance.close();
			functionExecutionSuccess = true;
		}catch(e){}
		expect(functionExecutionSuccess).toEqual(true);
	});

});

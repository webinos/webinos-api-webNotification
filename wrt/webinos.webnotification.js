/*******************************************************************************
*  Code contributed to the webinos project
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*  
*     http://www.apache.org/licenses/LICENSE-2.0
*  
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
* 
* Copyright 2011 Andre Paul, Fraunhofer FOKUS
* Copyright 2013 Martin Lasak, Fraunhofer FOKUS
******************************************************************************/
(function() {

	/**
	 * ...
	 * @constructor
	 * @param obj Object containing displayName, api, etc.
	 */
	var WebNotificationModule = function(obj) {
		WebinosService.call(this, obj);
	};
	// Inherit all functions from WebinosService
	WebNotificationModule.prototype = Object.create(WebinosService.prototype);	
	// The following allows the 'instanceof' to work properly
	WebNotificationModule.prototype.constructor = WebNotificationModule;
	// Register to the service discovery
	_webinos.registerServiceConstructor("http://webinos.org/api/notifications", WebNotificationModule);
	
	/**
	 * To bind the service.
	 * @param bindCB BindCallback object.
	 */
	WebNotificationModule.prototype.bindService = function (bindCB, serviceId) {
		// actually there should be an auth check here or whatever, but we just always bind

		var that = this;
		this.Notification = function(title, options){
			return new NotificatioImpl(that, title, options);;
		};

		if (typeof bindCB.onBind === 'function') {
			bindCB.onBind(this);
		};
	}

	var NotificatioImpl = function (serviceModule, title, options){
		if(typeof title != "string" || !title) { console.error("Wrong parameter."); return; }
		var that = this;
		that._serviceModule = serviceModule;
		that.dir = (options.dir === "ltr" || options.dir === "rtl") ? options.dir : "auto";
		that.lang = (typeof options.lang === "string") ? options.lang : "";
		that.body = (typeof options.body === "string") ? options.body : "";
		that.tag = (typeof options.tag === "string") ? options.tag : "";
		that.icon = (typeof options.icon === "string") ? options.icon : "";
		var rpc = webinos.rpcHandler.createRPC(that._serviceModule, "notify", [title, options]);
		webinos.rpcHandler.executeRPC(rpc,
				function (params){
					//on success
				 	if(params == 'onclick' && that.onclick){
				 		that.onclick(params);
				 	}
				 	else if(params == 'onshow' && that.onshow){
				 		that.onshow(params);
				 	}
				 	else if(params == 'onclose' && that.onclose){
				 		that.onclose(params);
				 	}
				},
				function (error){
					if(that.onerror){
				 		that.onerror(error);
				 	}
				}
		);
	};

	NotificatioImpl.prototype.onclick = Function;
	NotificatioImpl.prototype.onshow = Function;
	NotificatioImpl.prototype.onclose = Function;
	NotificatioImpl.prototype.onerror = Function;

	NotificatioImpl.prototype.addEventListener = function(type, callback, capture){
		if(typeof type != "string" || typeof callback != "function") return;
		callback._eventref = Math.random()+Date.now();
		var rpc = webinos.rpcHandler.createRPC(this._serviceModule, "addEventListener", [type, capture, callback._eventref]);
		webinos.rpcHandler.executeRPC(rpc,callback,Function);
	};	

	NotificatioImpl.prototype.removeEventListener = function(type, callback, capture){
		if(typeof type != "string" || typeof callback != "function" || !callback._eventref) return;
		var rpc = webinos.rpcHandler.createRPC(this._serviceModule, "removeEventListener", [type, capture, callback._eventref]);
		webinos.rpcHandler.executeRPC(rpc,Function,Function);
	};			

	NotificatioImpl.prototype.dispatchEvent = function(event){
		if(typeof type != "object") return;
		var rpc = webinos.rpcHandler.createRPC(this._serviceModule, "dispatchEvent", [event]);
		webinos.rpcHandler.executeRPC(rpc,Function,Function);
	};

	NotificatioImpl.prototype.dir = "auto";
	NotificatioImpl.prototype.body = "";
	NotificatioImpl.prototype.lang = "";
	NotificatioImpl.prototype.tag = "";
	NotificatioImpl.prototype.icon = "";

	NotificatioImpl.prototype.close = function(){
		console.log("close called")
		this.onshow = Function;
		this.onclick = Function;
		this.onerror = Function;
		var onclose = this.onclose || Function;
		this.onclose = Function;
		var rpc = webinos.rpcHandler.createRPC(this._serviceModule, "close");
		webinos.rpcHandler.executeRPC(rpc,onclose,Function);
	};	
	
}());
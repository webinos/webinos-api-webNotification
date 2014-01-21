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
* Copyright 2012 André Paul, Fraunhofer FOKUS
* Copyright 2013 Martin Lasak, Fraunhofer FOKUS
******************************************************************************/
(function() {
    var RPCWebinosService = require("webinos-jsonrpc2").RPCWebinosService;
    var exec = require('child_process').exec;

    var not = null;

    if (process.platform=='android') {
        not = require('bridge').load(require("../platform_interfaces.json").android.WebNotificationManagerImpl, this);
    }
    if (process.platform=='darwin' || process.platform == 'win32') {
        not = require('notifications');
        not.setup(); // Initialize platform (only on win8 currently)
    }

    /**
     * ...
     * 
     * ...
     * @constructor
     * @param rpcHandler A handler for functions that use RPC to deliver their result.
     * @param params Parameters to initialize the service.
     */
     var WebNotificationModule = function(rpcHandler, params) {
        // inherit from RPCWebinosService
        this.base = RPCWebinosService;
        this.base({
            api:'http://webinos.org/api/notifications',
            displayName:'Web Notification',
            description:'Web Notification API'
        });

        this.pzp = rpcHandler.parent;
    };

    WebNotificationModule.prototype = new RPCWebinosService;

    /**
     * ...
     * @param params Array with parameters.
     * @param successCB Success callback.
     * @param errorCB Error callback.
     * @param objectRef RPC object reference.
     */
    WebNotificationModule.prototype.notify = function(params, successCB, errorCB, objectRef) {
        console.log("notify was invoked");
        console.log(params);

        var title = params[0];
        var body = params[1].body || "";
        var dir = params[1].dir || "";
        var lang = params[1].lang || "";
        var tag = params[1].tag || "";
        var icon = params[1].icon || "";

        //FIXME: get the icon path
        //var icon = this.pzp.getWebinosPath() + "/" + params[1].iconUrl;

        if (process.platform === 'linux') {
            try{
            exec("notify-send \"" + title + "\" \"" + body + "\" -i \"" + icon + "\"", function(error, stdout, stderr){
                console.log("Result: " + error + " " + stdout + " " + stderr);

                if (error && typeof errorCB === "function") {
                    errorCB("Could not invoke native notification.");
                    return;
                }

                if (stdout.indexOf("CLICKED") > -1) {
                    successCB("onclick");
                }
                else {
                    if (stdout.indexOf("CLOSED") > -1) {
                        successCB("onclose");
                    }
                }
            });
            }catch(e){}

            //successCB("onShow");

        }
        if (process.platform === 'android') {
            var toAndroid = [];
            toAndroid.push(title);
            toAndroid.push(body);
            toAndroid.push(icon);
            not.notify(toAndroid,function (res) {
                if (res == "onclick") successCB("onclick");
                if (res == "onShow") successCB("onshow");
                if (res == "onClose") successCB("onclose");
                console.log(res);
            },
            function (err) {
                errorCB(err);
                console.log(err);
            });
        }
        if (process.platform === 'darwin' || process.platform === 'win32') {
            not.show(title, body, dir, lang, tag, icon,function (res) {
                console.log(res);
                successCB("onclick");
            },
            function (res) {
                console.log(res);
                successCB("onshow");
            },
            function (err) {
                console.log(err);
                errorCB(err);
            },
            function (res) {
                console.log(res);
                successCB("onclose");
            });
        }

    };

    /**
     * Close the notification.
     */
    WebNotificationModule.prototype.close = function() {
        console.log("Not yet.")
    };

    /**
     * Implementation of the EventTarget inherited method for adding a event handler.
     */
    WebNotificationModule.prototype.addEventListener = function(type, callback, capture) {
        if(typeof type != "string" || typeof callback != "function") return;
        if(typeof capture === "undefined") capture = false;
        console.log("Not yet.")
    };

    /**
     * Implementation of the EventTarget inherited method for removing a event handler.
     */
    WebNotificationModule.prototype.removeEventListener = function(type, callback, capture) {
        if(typeof type != "string" || typeof callback != "function") return;
        if(typeof capture === "undefined") capture = false;
        console.log("Not yet.")
    };

    /**
     * Implementation of the EventTarget inherited method for dispatching an event.
     */
    WebNotificationModule.prototype.dispatchEvent = function(event) {
        console.log("Not yet.")
    };

    // export our object
    exports.Service = WebNotificationModule;

})();

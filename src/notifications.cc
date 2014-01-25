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
 * Copyright 2013 Martin Lasak, Fraunhofer FOKUS
 ******************************************************************************/

#include <node.h>
#include <notifications.h>

using namespace v8;

Handle<Value> ShowMethod(const Arguments& args) {
  HandleScope scope;
  const unsigned resargc = 1;
  
  if (args.Length() != 10) {//title,body,dir,lang,tag,icon,onclick,onshow,onerror,onclose
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  //first argument is mandatory
  if (!args[0]->IsString()) {
    ThrowException(Exception::TypeError(String::New("Wrong argument type")));
    return scope.Close(Undefined());
  }

  String::Utf8Value title(args[0]->ToString());
  String::Utf8Value body(args[1]->ToString());
  String::Utf8Value dir(args[2]->ToString());
  String::Utf8Value lang(args[3]->ToString());
  String::Utf8Value tag(args[4]->ToString());
  String::Utf8Value icon(args[5]->ToString());
  Local<Function> onclick = Local<Function>::Cast(args[6]);
  Local<Function> onshow = Local<Function>::Cast(args[7]);
  Local<Function> onerror = Local<Function>::Cast(args[8]);
  Local<Function> onclose = Local<Function>::Cast(args[9]);
  bool result = Notifications::show(*title, *body, *dir, *lang, *tag, *icon);

  if(result){
    Local<Value> resargv[resargc] = { Local<Value>::New(String::New("onshow")) };
    onshow->Call(Context::GetCurrent()->Global(), resargc, resargv);
  }else{
    Local<Value> resargv[resargc] = { Local<Value>::New(String::New("onerror")) };
    onerror->Call(Context::GetCurrent()->Global(), resargc, resargv);
  }

  return scope.Close(True());
}

Handle<Value> SetupMethod(const Arguments& args) {
	HandleScope scope;
	if (Notifications::setup()){
		return scope.Close(True());
	}
	else{
		return scope.Close(False());
	}
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("show"),
	  FunctionTemplate::New(ShowMethod)->GetFunction());
  exports->Set(String::NewSymbol("setup"),
	  FunctionTemplate::New(SetupMethod)->GetFunction());
}

NODE_MODULE(notifications, init)

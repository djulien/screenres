/*
*  screenres.cc
*  David Mark Clements 2015
*  @davidmarkclem
*
*/

#include <node.h>
#include <nan.h>  // includes v8 too
#include <string>
#include <SDL.h>

#include "headers.h"

#define IF_DEBUG(stmt)  //stmt


//ANSI color codes:
//https://en.wikipedia.org/wiki/ANSI_escape_code
#define RED_LT  "\x1b[1;31m" //too dark: "\e[0;31m"
#define GREEN_LT  "\x1b[1;32m"
#define YELLOW_LT  "\x1b[1;33m"
#define BLUE_LT  "\x1b[1;34m"
#define MAGENTA_LT  "\x1b[1;35m"
#define CYAN_LT  "\x1b[1;36m"
#define GRAY  "\x1b[0;37m"
#define NORMAL  "\x1b[0m"


/*
using v8::Local;
using v8::Handle;
using v8::Object;
using v8::Array;
using v8::Number;
using v8::String;
using v8::FunctionTemplate;

void init(Handle<Object>)
*/


//some example code at https://nodeaddons.com/c-processing-from-node-js/
//and https://github.com/freezer333/nodecpp-demo


//NAN_METHOD(set)
Nan::NAN_METHOD_RETURN_TYPE set(Nan::NAN_METHOD_ARGS_TYPE args)
{
//  NanScope();
    v8::Isolate* isolate = args.GetIsolate(); //v8::Isolate::GetCurrent();
//  int code = changeRes(args[0]->Uint32Value(), args[1]->Uint32Value());
    uint32_t new_w = args[0]->Uint32Value(), new_h = args[1]->Uint32Value();
    IF_DEBUG(printf(CYAN_LT "Set display mode: %d x %d" NORMAL "\n", new_w, new_h));
//TODO?	SDL_Window* wnd = SDL_CreateWindow("SDL test", IGNORED_X_Y_W_H, SDL_WINDOW_FULLSCREEN_DESKTOP); //SDL_WINDOW_SHOWN);
//TODO? int code = SDL_SetWindowFullscreen(wnd, SDL_WINDOW_FULLSCREEN); //SDL_WINDOW_FULLSCREEN_DESKTOP); //TODO: real or fake mode?
//    if (!code) {
//  NanReturnUndefined();
//        args.GetReturnValue().SetUndefined();
//#define SDL_SetVideoMode(w, h, bpp, flags)  1 //TODO: need to create new window
//binding.gyp selects SDL version
#if (SDL_MAJOR_VERSION == 1) && (SDL_MINOR_VERSION == 2)
    if (!SDL_SetVideoMode(new_w, new_h, 0 /*use current BPP*/, 0 /*TODO: which flags to use?*/)) {
        args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, SDL_GetError()));
#elif SDL_MAJOR_VERSION == 2
    how to do this without creating a new window?
#else
    unsupported version of SDL!
#endif
    } else { //success
//    NanReturnValue(NanNew<Number>(code));
//??    info.GetReturnValue().Set(scope.Escape(Nan::New<v8::Integer>(retval)));
//	args.GetReturnValue().Set(code);
//	args.GetReturnValue().Set(scope.Escape(Nan::New<v8::Integer>(code)));
//        args.GetReturnValue().Set(v8::Number::New(isolate, code));
//       v8::NanNew<v8::Number>(res.width))
        args.GetReturnValue().SetUndefined();
    }
}


//NAN_METHOD(get)
Nan::NAN_METHOD_RETURN_TYPE get(Nan::NAN_METHOD_ARGS_TYPE args)
{
//  NanScope();
    v8::Isolate* isolate = args.GetIsolate(); //v8::Isolate::GetCurrent();
    Resolution res = {0, 0}; //fetchRes();
//binding.gyp selects SDL version
#if (SDL_MAJOR_VERSION == 1) && (SDL_MINOR_VERSION == 2)
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    res.width = info->current_w;
    res.height = info->current_h;
#elif SDL_MAJOR_VERSION == 2
    for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
    {
        SDL_DisplayMode mode;
        if (SDL_GetCurrentDisplayMode(i, &mode) < 0) //NOTE: SDL_GetDesktopDisplayMode returns previous mode if full screen mode
        {
            IF_DEBUG(printf(RED_LT "Can't get display mode: %s" NORMAL, SDL_GetError()));
//            continue;
            args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, SDL_GetError()));
            return;            
        }
        IF_DEBUG(printf(BLUE_LT "Display mode[%d/%d]: %dx%d px @%dHz, %i bbp %s" NORMAL "\n", i, SDL_GetNumVideoDisplays(), mode.w, mode.h, mode.refresh_rate, SDL_BITSPERPIXEL(mode.format), SDL_GetPixelFormatName(mode.format)));
        res.width = mode.w;
        res.height = mode.h;
        break; //TODO: take first one or last one?
    }
#else
    unsupported version of SDL!
#endif
//  v8::Local<v8::Array> arr = v8::NanNew<v8::Array>(2);
//  v8::Local<v8::ArrayBuffer> arr = v8::ArrayBuffer::New(isolate, (void *)data, size);
  v8::Local<v8::Array> arr = v8::Array::New(isolate);
//  arr->Set(0, v8::NanNew<v8::Number>(res.width));
  arr->Set(0, v8::Number::New(isolate, res.width));
  arr->Set(1, v8::Number::New(isolate, res.height));
//  NanReturnValue(arr);
  args.GetReturnValue().Set(arr);
}


//tell Node.js about my entry points:
//void init(Handle<Object> exports) {
//  exports->Set(NanNew<String>("set"), NanNew<FunctionTemplate>(set)->GetFunction());
//  exports->Set(NanNew<String>("get"), NanNew<FunctionTemplate>(get)->GetFunction());
//}
//void Initialize(v8::Handle<v8::Object> target)
//void init(v8::Local<v8::Object> exports)
void init(v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module)
{
    Nan::HandleScope scope; //for v8 GC
    Nan::SetMethod(exports, "get", get);
    Nan::SetMethod(exports, "set", set);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
//        args.GetReturnValue().Set(v8::Number::New(isolate, code));
//        SDL_GetError();
        return;
    }
    atexit(SDL_Quit);
}


NODE_MODULE(screenres, init)

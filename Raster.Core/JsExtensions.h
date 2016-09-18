
#pragma once

#include "RasterCore.h"

namespace v8
{
    using PersistentCopyable = Persistent<Function, CopyablePersistentTraits<Function>>;
    using FunctionArgs = const FunctionCallbackInfo<Value>&;

    void AttachFunction(Local<Object>& obj, std::string key, FunctionCallback function);
    void AttachBoolean(Local<Object>& obj, std::string key, SDL_bool boolean);
    void AttachNumber(Local<Object>& obj, std::string key, int number);
    void AttachString(Local<Object>& obj, std::string key, std::string str);

    void Throw(const FunctionCallbackInfo<Value>& args, std::string msg);
    void CatchExceptions(TryCatch& trycatch);

    Local<v8::String> NewString(std::string value);
    Local<Object> WrapPointer(void* pointer);
}
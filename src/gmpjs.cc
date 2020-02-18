#include <nan.h>
#include "mpz.h"

void InitAll (v8::Local<v8::Object> exports) {
  MPZ::Init(exports);
}

NODE_MODULE(gmpjs, InitAll);

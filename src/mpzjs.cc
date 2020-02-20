#include <napi.h>
#include <uv.h>
#include "mpz.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return MPZ::Init(env, exports);
}

NODE_API_MODULE(mpzjs, InitAll);

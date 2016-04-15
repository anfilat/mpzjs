#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <iostream>

#include <nan.h>

#include <gmp.h>
#include <map>
#include <utility>

using namespace v8;
using namespace node;
using namespace std;

#define REQ_STR_ARG(I, VAR)                                   \
  if (info.Length()<= (I) || !info[I]->IsString()) {          \
    Nan::ThrowTypeError("Argument " #I " must be a string");    \
    return;                                     \
  }                                                           \
  Local<String> VAR = Local<String>::Cast(info[I]);

#define REQ_UTF8_ARG(I, VAR)                                  \
  if (info.Length() <= (I) || !info[I]->IsString()) {         \
    Nan::ThrowTypeError(                                        \
      "Argument " #I " must be a utf8 string");               \
    return;                                     \
  }                                                           \
  String::Utf8Value VAR(info[I]->ToString());

#define REQ_INT32_ARG(I, VAR)                                 \
  if (info.Length() <= (I) || !info[I]->IsInt32()) {          \
    Nan::ThrowTypeError("Argument " #I " must be an int32");    \
    return;                                     \
  }                                                           \
  int32_t VAR = info[I]->ToInt32()->Value();

#define REQ_UINT32_ARG(I, VAR)                                \
  if (info.Length() <= (I) || !info[I]->IsUint32()) {         \
    Nan::ThrowTypeError("Argument " #I " must be a uint32");    \
    return;                                     \
  }                                                           \
  uint32_t VAR = info[I]->ToUint32()->Value();

#define REQ_INT64_ARG(I, VAR)                                 \
  if (info.Length() <= (I) || !info[I]->IsNumber()) {         \
    Nan::ThrowTypeError("Argument " #I " must be an int64");    \
    return;                                     \
  }                                                           \
  int64_t VAR = info[I]->ToInteger()->Value();

#define REQ_UINT64_ARG(I, VAR)                                \
  if (info.Length() <= (I) || !info[I]->IsNumber()) {         \
    Nan::ThrowTypeError("Argument " #I " must be a uint64");    \
    return;                                     \
  }                                                           \
  uint64_t VAR = info[I]->ToInteger()->Value();

#define REQ_BOOL_ARG(I, VAR)                                  \
  if (info.Length() <= (I) || !info[I]->IsBoolean()) {        \
    Nan::ThrowTypeError("Argument " #I " must be a boolean");   \
    return;                                     \
  }                                                           \
  bool VAR = info[I]->ToBoolean()->Value();

#define WRAP_RESULT(RES, VAR)                                           \
  Local<Value> arg[1] = { Nan::New<External>(static_cast<mpz_t *>(RES)) };  \
  Local<Object> VAR = Nan::New<FunctionTemplate>(constructor_template)->      \
    GetFunction()->NewInstance(1, arg);

class BigInt : public Nan::ObjectWrap {
  public:
    static void Initialize(Local<Object> target);
    mpz_t *bigint_;
    static Nan::Persistent<Function> js_conditioner;
    static void SetJSConditioner(Local<Function> constructor);

  protected:
    static Nan::Persistent<FunctionTemplate> constructor_template;

    BigInt(const String::Utf8Value& str, uint64_t base);
    BigInt(uint64_t num);
    BigInt(int64_t num);
    BigInt(mpz_t *num);
    BigInt();
    ~BigInt();

    static NAN_METHOD(New);
    static NAN_METHOD(ToString);
    static NAN_METHOD(Badd);
    static NAN_METHOD(Bsub);
    static NAN_METHOD(Bmul);
    static NAN_METHOD(Bdiv);
    static NAN_METHOD(Uadd);
    static NAN_METHOD(Usub);
    static NAN_METHOD(Umul);
    static NAN_METHOD(Udiv);
    static NAN_METHOD(Umul_2exp);
    static NAN_METHOD(Udiv_2exp);
    static NAN_METHOD(Babs);
    static NAN_METHOD(Bneg);
    static NAN_METHOD(Bmod);
    static NAN_METHOD(Umod);
    static NAN_METHOD(Bpowm);
    static NAN_METHOD(Upowm);
    static NAN_METHOD(Upow);
    static NAN_METHOD(Uupow);
    static NAN_METHOD(Brand0);
    static NAN_METHOD(Probprime);
    static NAN_METHOD(Nextprime);
    static NAN_METHOD(Bcompare);
    static NAN_METHOD(Scompare);
    static NAN_METHOD(Ucompare);
    static NAN_METHOD(Band);
    static NAN_METHOD(Bor);
    static NAN_METHOD(Bxor);
    static NAN_METHOD(Binvertm);
    static NAN_METHOD(Bsqrt);
    static NAN_METHOD(Broot);
    static NAN_METHOD(BitLength);
    static NAN_METHOD(Bgcd);
};

static gmp_randstate_t *		randstate	= NULL;

Nan::Persistent<FunctionTemplate> BigInt::constructor_template;

Nan::Persistent<Function> BigInt::js_conditioner;

void BigInt::SetJSConditioner(Local<Function> constructor) {
  js_conditioner.Reset(constructor);
}

void BigInt::Initialize(v8::Local<v8::Object> target) {
  Nan::HandleScope scope;

  Local<FunctionTemplate> tmpl = Nan::New<FunctionTemplate>(New);
  constructor_template.Reset(tmpl);

  tmpl->InstanceTemplate()->SetInternalFieldCount(1);
  tmpl->SetClassName(Nan::New("BigInt").ToLocalChecked());

  Nan::SetPrototypeMethod(tmpl, "toString", ToString);
  Nan::SetPrototypeMethod(tmpl, "badd", Badd);
  Nan::SetPrototypeMethod(tmpl, "bsub", Bsub);
  Nan::SetPrototypeMethod(tmpl, "bmul", Bmul);
  Nan::SetPrototypeMethod(tmpl, "bdiv", Bdiv);
  Nan::SetPrototypeMethod(tmpl, "uadd", Uadd);
  Nan::SetPrototypeMethod(tmpl, "usub", Usub);
  Nan::SetPrototypeMethod(tmpl, "umul", Umul);
  Nan::SetPrototypeMethod(tmpl, "udiv", Udiv);
  Nan::SetPrototypeMethod(tmpl, "umul2exp", Umul_2exp);
  Nan::SetPrototypeMethod(tmpl, "udiv2exp", Udiv_2exp);
  Nan::SetPrototypeMethod(tmpl, "babs", Babs);
  Nan::SetPrototypeMethod(tmpl, "bneg", Bneg);
  Nan::SetPrototypeMethod(tmpl, "bmod", Bmod);
  Nan::SetPrototypeMethod(tmpl, "umod", Umod);
  Nan::SetPrototypeMethod(tmpl, "bpowm", Bpowm);
  Nan::SetPrototypeMethod(tmpl, "upowm", Upowm);
  Nan::SetPrototypeMethod(tmpl, "upow", Upow);
  Nan::SetPrototypeMethod(tmpl, "uupow", Uupow);
  Nan::SetPrototypeMethod(tmpl, "brand0", Brand0);
  Nan::SetPrototypeMethod(tmpl, "probprime", Probprime);
  Nan::SetPrototypeMethod(tmpl, "nextprime", Nextprime);
  Nan::SetPrototypeMethod(tmpl, "bcompare", Bcompare);
  Nan::SetPrototypeMethod(tmpl, "scompare", Scompare);
  Nan::SetPrototypeMethod(tmpl, "ucompare", Ucompare);
  Nan::SetPrototypeMethod(tmpl, "band", Band);
  Nan::SetPrototypeMethod(tmpl, "bor", Bor);
  Nan::SetPrototypeMethod(tmpl, "bxor", Bxor);
  Nan::SetPrototypeMethod(tmpl, "binvertm", Binvertm);
  Nan::SetPrototypeMethod(tmpl, "bsqrt", Bsqrt);
  Nan::SetPrototypeMethod(tmpl, "broot", Broot);
  Nan::SetPrototypeMethod(tmpl, "bitLength", BitLength);
  Nan::SetPrototypeMethod(tmpl, "bgcd", Bgcd);

  target->Set(Nan::New("BigInt").ToLocalChecked(), tmpl->GetFunction());
}

BigInt::BigInt (const v8::String::Utf8Value& str, uint64_t base) : Nan::ObjectWrap ()
{
  bigint_ = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*bigint_);

  mpz_set_str(*bigint_, *str, base);
}

BigInt::BigInt (uint64_t num) : Nan::ObjectWrap ()
{
  bigint_ = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*bigint_);

  mpz_set_ui(*bigint_, num);
}

BigInt::BigInt (int64_t num) : Nan::ObjectWrap ()
{
  bigint_ = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*bigint_);

  mpz_set_si(*bigint_, num);
}

BigInt::BigInt (mpz_t *num) : Nan::ObjectWrap ()
{
  bigint_ = num;
}

BigInt::BigInt () : Nan::ObjectWrap ()
{
  bigint_ = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*bigint_);

  mpz_set_ui(*bigint_, 0);
}

BigInt::~BigInt ()
{
  mpz_clear(*bigint_);
  free(bigint_);
}

NAN_METHOD(BigInt::New)
{
  if(!info.IsConstructCall()) {
    int len = info.Length();
    Handle<Value>* newArgs = new Local<Value>[len];
    for(int i = 0; i < len; i++) {
      newArgs[i] = info[i];
    }
    Local<Value> newInst = Nan::New<FunctionTemplate>(constructor_template)->
        GetFunction()->NewInstance(len, newArgs);
    delete[] newArgs;
    info.GetReturnValue().Set(newInst);
    return;
  }

  Nan::HandleScope scope;
  BigInt *bigint;
  uint64_t base;

  if(info[0]->IsExternal()) {
    mpz_t *num =  static_cast<mpz_t *>(External::Cast(*(info[0]))->Value());

    bigint = new BigInt(num);
  } else {
    int len = info.Length();
    Local<Object> ctx = Nan::New<Object>();
    Local<Value>* newArgs = new Local<Value>[len];
    for(int i = 0; i < len; i++) {
      newArgs[i] = info[i];
    }
    Local<Value> obj = Nan::New<Function>(js_conditioner)->
      Call(ctx, info.Length(), newArgs);

    if(!*obj) {
      Nan::ThrowError("Invalid type passed to BigInt constructor");
      return;
    }

    String::Utf8Value str(obj->ToObject()->Get(Nan::New("num").ToLocalChecked())->ToString());
    base = obj->ToObject()->Get(Nan::New("base").ToLocalChecked())->ToNumber()->Value();

    bigint = new BigInt(str, base);
    delete[] newArgs;
  }

  bigint->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(BigInt::ToString)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  uint64_t base = 10;

  if(info.Length() > 0) {
    REQ_UINT64_ARG(0, tbase);
      if(tbase < 2 || tbase > 62) {
         return Nan::ThrowError("base should be : 2<= base <= 62");
      }
    base = tbase;
  }
  char *to = mpz_get_str(0, base, *bigint->bigint_);

  Local<Value> result = Nan::New<String>(to).ToLocalChecked();
  free(to);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Badd)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  BigInt *bi = Nan::ObjectWrap::Unwrap<BigInt>(info[0]->ToObject());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  mpz_add(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(res, result);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Bsub)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  BigInt *bi = Nan::ObjectWrap::Unwrap<BigInt>(info[0]->ToObject());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_sub(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(res, result);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Bmul)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  Nan::HandleScope scope;

  BigInt *bi = Nan::ObjectWrap::Unwrap<BigInt>(info[0]->ToObject());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_mul(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Bdiv)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  Nan::HandleScope scope;

  BigInt *bi = Nan::ObjectWrap::Unwrap<BigInt>(info[0]->ToObject());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_div(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Uadd)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  Nan::HandleScope scope;

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_add_ui(*res, *bigint->bigint_, x);

  WRAP_RESULT(res, result);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Usub)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  Nan::HandleScope scope;

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_sub_ui(*res, *bigint->bigint_, x);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Umul)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  Nan::HandleScope scope;

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_mul_ui(*res, *bigint->bigint_, x);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Udiv)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  Nan::HandleScope scope;

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_div_ui(*res, *bigint->bigint_, x);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Umul_2exp)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  Nan::HandleScope scope;

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_mul_2exp(*res, *bigint->bigint_, x);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Udiv_2exp)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  Nan::HandleScope scope;

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_div_2exp(*res, *bigint->bigint_, x);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);

}

NAN_METHOD(BigInt::Babs)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_abs(*res, *bigint->bigint_);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Bneg)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_neg(*res, *bigint->bigint_);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Bmod)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  BigInt *bi = Nan::ObjectWrap::Unwrap<BigInt>(info[0]->ToObject());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_mod(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Umod)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_mod_ui(*res, *bigint->bigint_, x);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Bpowm)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  BigInt *bi1 = Nan::ObjectWrap::Unwrap<BigInt>(info[0]->ToObject());
  BigInt *bi2 = Nan::ObjectWrap::Unwrap<BigInt>(info[1]->ToObject());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_powm(*res, *bigint->bigint_, *bi1->bigint_, *bi2->bigint_);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Upowm)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  REQ_UINT64_ARG(0, x);
  BigInt *bi = Nan::ObjectWrap::Unwrap<BigInt>(info[1]->ToObject());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_powm_ui(*res, *bigint->bigint_, x, *bi->bigint_);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Upow)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_pow_ui(*res, *bigint->bigint_, x);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

/*
 * This makes no sense?  It doesn't act on the object but is a
 * prototype method.
 */
NAN_METHOD(BigInt::Uupow)
{
  REQ_UINT64_ARG(0, x);
  REQ_UINT64_ARG(1, y);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_ui_pow_ui(*res, x, y);

  WRAP_RESULT(res, result);

  info.GetReturnValue().Set(result);
}


NAN_METHOD(BigInt::Brand0)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if(randstate == NULL) {
    randstate = (gmp_randstate_t *) malloc(sizeof(gmp_randstate_t));
    gmp_randinit_default(*randstate);
    unsigned long seed = rand() + (time(NULL) * 1000) + clock();
          gmp_randseed_ui(*randstate, seed);
  }

  mpz_urandomm(*res, *randstate, *bigint->bigint_);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Probprime)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  REQ_UINT32_ARG(0, reps);
  info.GetReturnValue().Set(Nan::New<Number>(mpz_probab_prime_p(*bigint->bigint_, reps)));
}

NAN_METHOD(BigInt::Nextprime)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_nextprime(*res, *bigint->bigint_);

  WRAP_RESULT(res, result);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Bcompare)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  BigInt *bi = Nan::ObjectWrap::Unwrap<BigInt>(info[0]->ToObject());

  info.GetReturnValue().Set(Nan::New<Number>(mpz_cmp(*bigint->bigint_, *bi->bigint_)));
}

NAN_METHOD(BigInt::Scompare)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  REQ_INT64_ARG(0, x);

  info.GetReturnValue().Set(Nan::New<Number>(mpz_cmp_si(*bigint->bigint_, x)));
}

NAN_METHOD(BigInt::Ucompare)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  REQ_UINT64_ARG(0, x);

  info.GetReturnValue().Set(Nan::New<Number>(mpz_cmp_ui(*bigint->bigint_, x)));
}

NAN_METHOD(BigInt::Band)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  BigInt *bi = Nan::ObjectWrap::Unwrap<BigInt>(info[0]->ToObject());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_and(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(res, result);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Bor)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  BigInt *bi = Nan::ObjectWrap::Unwrap<BigInt>(info[0]->ToObject());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_ior(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(res, result);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Bxor)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  BigInt *bi = Nan::ObjectWrap::Unwrap<BigInt>(info[0]->ToObject());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_xor(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Binvertm)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  BigInt *bi = Nan::ObjectWrap::Unwrap<BigInt>(info[0]->ToObject());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_invert(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Bsqrt)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_sqrt(*res, *bigint->bigint_);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::Broot)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_root(*res, *bigint->bigint_, x);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(BigInt::BitLength)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  int size = mpz_sizeinbase(*bigint->bigint_, 2);
  Local<Value> result = Nan::New<Integer>(size);
  info.GetReturnValue().Set(result);
}
NAN_METHOD(BigInt::Bgcd)
{
  BigInt *bigint = Nan::ObjectWrap::Unwrap<BigInt>(info.This());
  BigInt *bi = Nan::ObjectWrap::Unwrap<BigInt>(info[0]->ToObject());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_gcd(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(res, result);
  info.GetReturnValue().Set(result);
}

static NAN_METHOD(SetJSConditioner)
{
  BigInt::SetJSConditioner(Local<Function>::Cast(info[0]));
  return;
}

extern "C" void
init (Local<Object> target)
{
  BigInt::Initialize(target);
  Nan::SetMethod(target, "setJSConditioner", SetJSConditioner);
}

NODE_MODULE(bigint, init);

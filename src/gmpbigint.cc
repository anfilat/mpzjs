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

#define REQ_UTF8_ARG(CONTEXT, I, VAR)                                  \
  if (info.Length() <= (I) || !info[I]->IsString()) {         \
    Nan::ThrowTypeError(                                        \
      "Argument " #I " must be a utf8 string");               \
    return;                                     \
  }                                                           \
  String::Utf8Value VAR(info[I]->ToString(CONTEXT).ToLocalChecked());

#define REQ_INT32_ARG(I, VAR)                                 \
  if (info.Length() <= (I) || !info[I]->IsInt32()) {          \
    Nan::ThrowTypeError("Argument " #I " must be an int32");    \
    return;                                     \
  }                                                           \
  int32_t VAR = Nan::To<int32_t>(info[I]).FromJust();

#define REQ_UINT32_ARG(I, VAR)                                \
  if (info.Length() <= (I) || !info[I]->IsUint32()) {         \
    Nan::ThrowTypeError("Argument " #I " must be a uint32");    \
    return;                                     \
  }                                                           \
  uint32_t VAR = Nan::To<int32_t>(info[I]).FromJust();

#define REQ_INT64_ARG(I, VAR)                                 \
  if (info.Length() <= (I) || !info[I]->IsNumber()) {         \
    Nan::ThrowTypeError("Argument " #I " must be an int64");    \
    return;                                     \
  }                                                           \
  int64_t VAR = Nan::To<int64_t>(info[I]).FromJust();

#define REQ_UINT64_ARG(I, VAR)                                \
  if (info.Length() <= (I) || !info[I]->IsNumber()) {         \
    Nan::ThrowTypeError("Argument " #I " must be a uint64");    \
    return;                                     \
  }                                                           \
  uint64_t VAR = Nan::To<int64_t>(info[I]).FromJust();

#define REQ_BOOL_ARG(I, VAR)                                  \
  if (info.Length() <= (I) || !info[I]->IsBoolean()) {        \
    Nan::ThrowTypeError("Argument " #I " must be a boolean");   \
    return;                                     \
  }                                                           \
  bool VAR = info[I]->ToBoolean()->Value();

#define WRAP_RESULT(CONTEXT, RES, VAR)                                           \
  Local<Value> arg[1] = { Nan::New<External>(static_cast<mpz_t *>(RES)) };  \
  Local<Value> VAR = Nan::New<Function>(constructor_template)->      \
    NewInstance(CONTEXT, 1, arg).ToLocalChecked();

class GmpBigInt : public Nan::ObjectWrap {
  public:
    static void Initialize(Local<Object> target);
    mpz_t *bigint_;

  protected:
    static Nan::Persistent<Function> constructor_template;

    GmpBigInt(const String::Utf8Value& str, int64_t base);
    GmpBigInt(double num);
    GmpBigInt(mpz_t *num);
    GmpBigInt();
    ~GmpBigInt();

    static NAN_METHOD(New);
    static NAN_METHOD(ToString);
    static NAN_METHOD(ToNumber);
    static NAN_METHOD(Add);
    static NAN_METHOD(AssignAdd);
    static NAN_METHOD(Sub);
    static NAN_METHOD(AssignSub);
    static NAN_METHOD(Mul);
    static NAN_METHOD(AssignMul);
    static NAN_METHOD(Div);
    static NAN_METHOD(AssignDiv);
    static NAN_METHOD(Umul_2exp);
    static NAN_METHOD(Udiv_2exp);
    static NAN_METHOD(Babs);
    static NAN_METHOD(Bneg);
    static NAN_METHOD(Bmod);
    static NAN_METHOD(Umod);
    static NAN_METHOD(Bpowm);
    static NAN_METHOD(Upowm);
    static NAN_METHOD(Upow);
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

static gmp_randstate_t *randstate = NULL;

Nan::Persistent<Function> GmpBigInt::constructor_template;

void GmpBigInt::Initialize(Local<Object> target) {
  Local<Context> context = target->CreationContext();
  Nan::HandleScope scope;

  Local<FunctionTemplate> tmpl = Nan::New<FunctionTemplate>(New);
  tmpl->SetClassName(Nan::New("GmpBigInt").ToLocalChecked());
  tmpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tmpl, "toString", ToString);
  Nan::SetPrototypeMethod(tmpl, "toNumber", ToNumber);
  Nan::SetPrototypeMethod(tmpl, "add", Add);
  Nan::SetPrototypeMethod(tmpl, "assignAdd", AssignAdd);
  Nan::SetPrototypeMethod(tmpl, "sub", Sub);
  Nan::SetPrototypeMethod(tmpl, "assignSub", AssignSub);
  Nan::SetPrototypeMethod(tmpl, "mul", Mul);
  Nan::SetPrototypeMethod(tmpl, "assignMul", AssignMul);
  Nan::SetPrototypeMethod(tmpl, "div", Div);
  Nan::SetPrototypeMethod(tmpl, "assignDiv", AssignDiv);
  Nan::SetPrototypeMethod(tmpl, "umul2exp", Umul_2exp);
  Nan::SetPrototypeMethod(tmpl, "udiv2exp", Udiv_2exp);
  Nan::SetPrototypeMethod(tmpl, "babs", Babs);
  Nan::SetPrototypeMethod(tmpl, "bneg", Bneg);
  Nan::SetPrototypeMethod(tmpl, "bmod", Bmod);
  Nan::SetPrototypeMethod(tmpl, "umod", Umod);
  Nan::SetPrototypeMethod(tmpl, "bpowm", Bpowm);
  Nan::SetPrototypeMethod(tmpl, "upowm", Upowm);
  Nan::SetPrototypeMethod(tmpl, "upow", Upow);
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

  constructor_template.Reset(tmpl->GetFunction(context).ToLocalChecked());
  target->Set(context, Nan::New("GmpBigInt").ToLocalChecked(), tmpl->GetFunction(context).ToLocalChecked());
}

GmpBigInt::GmpBigInt (const v8::String::Utf8Value& str, int64_t base) : Nan::ObjectWrap () {
  bigint_ = (mpz_t *) malloc(sizeof(mpz_t));

  mpz_init_set_str(*bigint_, *str, base);
}

GmpBigInt::GmpBigInt (double num) : Nan::ObjectWrap () {
  bigint_ = (mpz_t *) malloc(sizeof(mpz_t));

  mpz_init_set_d(*bigint_, num);
}

GmpBigInt::GmpBigInt (mpz_t *num) : Nan::ObjectWrap () {
  bigint_ = num;
}

GmpBigInt::GmpBigInt () : Nan::ObjectWrap () {
  bigint_ = (mpz_t *) malloc(sizeof(mpz_t));

  mpz_init(*bigint_);
}

GmpBigInt::~GmpBigInt () {
  mpz_clear(*bigint_);
  free(bigint_);
}

NAN_METHOD(GmpBigInt::New) {
  Nan::HandleScope scope;
  GmpBigInt *bigint;

  if (info.Length() == 0) {
    bigint = new GmpBigInt();
  } else if (info[0]->IsExternal()) {
    mpz_t *num = static_cast<mpz_t *>(External::Cast(*(info[0]))->Value());

    bigint = new GmpBigInt(num);
  } else if (info[0]->IsNumber()) {
    double num = Nan::To<double>(info[0]).FromJust();

    bigint = new GmpBigInt(num);
  } else {
    Local<Context> context = info.GetIsolate()->GetCurrentContext();
    Isolate* isolate = context->GetIsolate();

    String::Utf8Value str(isolate, info[0]->ToString(context).ToLocalChecked());
    int64_t base = Nan::To<int64_t>(info[1]).FromJust();

    bigint = new GmpBigInt(str, base);
  }

  bigint->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(GmpBigInt::ToString) {
  auto *self = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  uint64_t base = 10;
  if (info.Length() > 0) {
    REQ_UINT64_ARG(0, tbase);
    base = tbase;
  }

  char *to = mpz_get_str(0, base, *self->bigint_);

  info.GetReturnValue().Set(Nan::New<String>(to).ToLocalChecked());
  free(to);
}

NAN_METHOD(GmpBigInt::ToNumber) {
  auto *self = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  info.GetReturnValue().Set(Nan::New<Number>(mpz_get_d(*self->bigint_)));
}

NAN_METHOD(GmpBigInt::Add) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    if (num >= 0) {
      mpz_add_ui(*res, *self->bigint_, num);
    } else {
      mpz_sub_ui(*res, *self->bigint_, -num);
    }
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());
    mpz_add(*res, *self->bigint_, *num->bigint_);
  }

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::AssignAdd) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    if (num2 >= 0) {
      mpz_add_ui(*self->bigint_, *num1->bigint_, num2);
    } else {
      mpz_sub_ui(*self->bigint_, *num1->bigint_, -num2);
    }
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[1]->ToObject(context).ToLocalChecked());
    mpz_add(*self->bigint_, *num1->bigint_, *num2->bigint_);
  }
}

NAN_METHOD(GmpBigInt::Sub) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    if (num >= 0) {
      mpz_sub_ui(*res, *self->bigint_, num);
    } else {
      mpz_add_ui(*res, *self->bigint_, -num);
    }
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());
    mpz_sub(*res, *self->bigint_, *num->bigint_);
  }

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::AssignSub) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    if (num2 >= 0) {
      mpz_sub_ui(*self->bigint_, *num1->bigint_, num2);
    } else {
      mpz_add_ui(*self->bigint_, *num1->bigint_, -num2);
    }
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[1]->ToObject(context).ToLocalChecked());
    mpz_sub(*self->bigint_, *num1->bigint_, *num2->bigint_);
  }
}

NAN_METHOD(GmpBigInt::Mul) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    mpz_mul_ui(*res, *self->bigint_, num);
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());
    mpz_mul(*res, *self->bigint_, *num->bigint_);
  }

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::AssignMul) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    mpz_mul_ui(*self->bigint_, *num1->bigint_, num2);
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[1]->ToObject(context).ToLocalChecked());
    mpz_mul(*self->bigint_, *num1->bigint_, *num2->bigint_);
  }
}

NAN_METHOD(GmpBigInt::Div) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    mpz_div_ui(*res, *self->bigint_, num);
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());
    mpz_div(*res, *self->bigint_, *num->bigint_);
  }

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::AssignDiv) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    mpz_div_ui(*self->bigint_, *num1->bigint_, num2);
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[1]->ToObject(context).ToLocalChecked());
    mpz_div(*self->bigint_, *num1->bigint_, *num2->bigint_);
  }
}

NAN_METHOD(GmpBigInt::Umul_2exp) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());
  Nan::HandleScope scope;

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_mul_2exp(*res, *bigint->bigint_, x);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Udiv_2exp) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());
  Nan::HandleScope scope;

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_div_2exp(*res, *bigint->bigint_, x);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Babs) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_abs(*res, *bigint->bigint_);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Bneg) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_neg(*res, *bigint->bigint_);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Bmod) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  GmpBigInt *bi = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_mod(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Umod) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_mod_ui(*res, *bigint->bigint_, x);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Bpowm) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  GmpBigInt *bi1 = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());
  GmpBigInt *bi2 = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[1]->ToObject(context).ToLocalChecked());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_powm(*res, *bigint->bigint_, *bi1->bigint_, *bi2->bigint_);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Upowm) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  REQ_UINT64_ARG(0, x);
  GmpBigInt *bi = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[1]->ToObject(context).ToLocalChecked());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_powm_ui(*res, *bigint->bigint_, x, *bi->bigint_);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Upow) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_pow_ui(*res, *bigint->bigint_, x);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Brand0) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if(randstate == NULL) {
    randstate = (gmp_randstate_t *) malloc(sizeof(gmp_randstate_t));
    gmp_randinit_default(*randstate);
    unsigned long seed = rand() + (time(NULL) * 1000) + clock();
          gmp_randseed_ui(*randstate, seed);
  }

  mpz_urandomm(*res, *randstate, *bigint->bigint_);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Probprime) {
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  REQ_UINT32_ARG(0, reps);
  info.GetReturnValue().Set(Nan::New<Number>(mpz_probab_prime_p(*bigint->bigint_, reps)));
}

NAN_METHOD(GmpBigInt::Nextprime) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_nextprime(*res, *bigint->bigint_);

  WRAP_RESULT(context, res, result);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Bcompare) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());
  GmpBigInt *bi = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());

  info.GetReturnValue().Set(Nan::New<Number>(mpz_cmp(*bigint->bigint_, *bi->bigint_)));
}

NAN_METHOD(GmpBigInt::Scompare) {
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());
  REQ_INT64_ARG(0, x);

  info.GetReturnValue().Set(Nan::New<Number>(mpz_cmp_si(*bigint->bigint_, x)));
}

NAN_METHOD(GmpBigInt::Ucompare) {
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());
  REQ_UINT64_ARG(0, x);

  info.GetReturnValue().Set(Nan::New<Number>(mpz_cmp_ui(*bigint->bigint_, x)));
}

NAN_METHOD(GmpBigInt::Band) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  GmpBigInt *bi = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_and(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(context, res, result);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Bor) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  GmpBigInt *bi = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_ior(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(context, res, result);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Bxor) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());
  GmpBigInt *bi = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_xor(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Binvertm) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  GmpBigInt *bi = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_invert(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Bsqrt) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_sqrt(*res, *bigint->bigint_);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Broot) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());

  REQ_UINT64_ARG(0, x);
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_root(*res, *bigint->bigint_, x);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::BitLength) {
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());
  int size = mpz_sizeinbase(*bigint->bigint_, 2);
  Local<Value> result = Nan::New<Integer>(size);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpBigInt::Bgcd) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpBigInt *bigint = Nan::ObjectWrap::Unwrap<GmpBigInt>(info.This());
  GmpBigInt *bi = Nan::ObjectWrap::Unwrap<GmpBigInt>(info[0]->ToObject(context).ToLocalChecked());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_gcd(*res, *bigint->bigint_, *bi->bigint_);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

extern "C" void
init (Local<Object> target) {
  GmpBigInt::Initialize(target);
}

NODE_MODULE(gmpbigint, init);

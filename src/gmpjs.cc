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

#define REQ_UINT32_ARG(I, VAR)                                \
  if (info.Length() <= (I) || !info[I]->IsUint32()) {         \
    Nan::ThrowTypeError("Argument " #I " must be a uint32");    \
    return;                                     \
  }                                                           \
  uint32_t VAR = Nan::To<int32_t>(info[I]).FromJust();

#define REQ_UINT64_ARG(I, VAR)                                \
  if (info.Length() <= (I) || !info[I]->IsNumber()) {         \
    Nan::ThrowTypeError("Argument " #I " must be a uint64");    \
    return;                                     \
  }                                                           \
  uint64_t VAR = Nan::To<int64_t>(info[I]).FromJust();

#define WRAP_RESULT(CONTEXT, RES, VAR)                                           \
  Local<Value> arg[1] = { Nan::New<External>(static_cast<mpz_t *>(RES)) };  \
  Local<Value> VAR = Nan::New<Function>(constructor_template)->      \
    NewInstance(CONTEXT, 1, arg).ToLocalChecked();

class GmpJS : public Nan::ObjectWrap {
  public:
    static void Initialize(Local<Object> target);
    mpz_t *value;

  protected:
    static Nan::Persistent<Function> constructor_template;

    GmpJS(const String::Utf8Value& str, int64_t base);
    GmpJS(double num);
    GmpJS(mpz_t *num);
    GmpJS();
    ~GmpJS();

    static NAN_METHOD(New);
    static NAN_METHOD(ToString);
    static NAN_METHOD(ToNumber);
    static NAN_METHOD(Set);
    static NAN_METHOD(Add);
    static NAN_METHOD(AssignAdd);
    static NAN_METHOD(Sub);
    static NAN_METHOD(AssignSub);
    static NAN_METHOD(Mul);
    static NAN_METHOD(AssignMul);
    static NAN_METHOD(Div);
    static NAN_METHOD(AssignDiv);
    static NAN_METHOD(Mod);
    static NAN_METHOD(AssignMod);
    static NAN_METHOD(AssignAddMul);
    static NAN_METHOD(AssignSubMul);
    static NAN_METHOD(And);
    static NAN_METHOD(AssignAnd);
    static NAN_METHOD(Or);
    static NAN_METHOD(AssignOr);
    static NAN_METHOD(Xor);
    static NAN_METHOD(AssignXor);
    static NAN_METHOD(Not);
    static NAN_METHOD(AssignNot);
    static NAN_METHOD(ShiftLeft);
    static NAN_METHOD(AssignShiftLeft);
    static NAN_METHOD(ShiftRight);
    static NAN_METHOD(AssignShiftRight);
    static NAN_METHOD(Abs);
    static NAN_METHOD(AssignAbs);
    static NAN_METHOD(Neg);
    static NAN_METHOD(AssignNeg);
    static NAN_METHOD(Sqrt);
    static NAN_METHOD(AssignSqrt);
    static NAN_METHOD(Root);
    static NAN_METHOD(AssignRoot);
    static NAN_METHOD(Powm);
    static NAN_METHOD(AssignPowm);
    static NAN_METHOD(Pow);
    static NAN_METHOD(AssignPow);
    static NAN_METHOD(Compare);
    static NAN_METHOD(Brand0);
    static NAN_METHOD(Probprime);
    static NAN_METHOD(Nextprime);
    static NAN_METHOD(Binvertm);
    static NAN_METHOD(Bgcd);
    static NAN_METHOD(BitLength);
};

static gmp_randstate_t *randstate = NULL;

Nan::Persistent<Function> GmpJS::constructor_template;

void GmpJS::Initialize(Local<Object> target) {
  Local<Context> context = target->CreationContext();
  Nan::HandleScope scope;

  Local<FunctionTemplate> tmpl = Nan::New<FunctionTemplate>(New);
  tmpl->SetClassName(Nan::New("MPZInternal").ToLocalChecked());
  tmpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tmpl, "toString", ToString);
  Nan::SetPrototypeMethod(tmpl, "toNumber", ToNumber);
  Nan::SetPrototypeMethod(tmpl, "set", Set);
  Nan::SetPrototypeMethod(tmpl, "add", Add);
  Nan::SetPrototypeMethod(tmpl, "assignAdd", AssignAdd);
  Nan::SetPrototypeMethod(tmpl, "sub", Sub);
  Nan::SetPrototypeMethod(tmpl, "assignSub", AssignSub);
  Nan::SetPrototypeMethod(tmpl, "mul", Mul);
  Nan::SetPrototypeMethod(tmpl, "assignMul", AssignMul);
  Nan::SetPrototypeMethod(tmpl, "div", Div);
  Nan::SetPrototypeMethod(tmpl, "assignDiv", AssignDiv);
  Nan::SetPrototypeMethod(tmpl, "mod", Mod);
  Nan::SetPrototypeMethod(tmpl, "assignMod", AssignMod);
  Nan::SetPrototypeMethod(tmpl, "assignAddMul", AssignAddMul);
  Nan::SetPrototypeMethod(tmpl, "assignSubMul", AssignSubMul);
  Nan::SetPrototypeMethod(tmpl, "and", And);
  Nan::SetPrototypeMethod(tmpl, "assignAnd", AssignAnd);
  Nan::SetPrototypeMethod(tmpl, "or", Or);
  Nan::SetPrototypeMethod(tmpl, "assignOr", AssignOr);
  Nan::SetPrototypeMethod(tmpl, "xor", Xor);
  Nan::SetPrototypeMethod(tmpl, "assignXor", AssignXor);
  Nan::SetPrototypeMethod(tmpl, "not", Not);
  Nan::SetPrototypeMethod(tmpl, "assignNot", AssignNot);
  Nan::SetPrototypeMethod(tmpl, "shiftLeft", ShiftLeft);
  Nan::SetPrototypeMethod(tmpl, "assignShiftLeft", AssignShiftLeft);
  Nan::SetPrototypeMethod(tmpl, "shiftRight", ShiftRight);
  Nan::SetPrototypeMethod(tmpl, "assignShiftRight", AssignShiftRight);
  Nan::SetPrototypeMethod(tmpl, "abs", Abs);
  Nan::SetPrototypeMethod(tmpl, "assignAbs", AssignAbs);
  Nan::SetPrototypeMethod(tmpl, "neg", Neg);
  Nan::SetPrototypeMethod(tmpl, "assignNeg", AssignNeg);
  Nan::SetPrototypeMethod(tmpl, "sqrt", Sqrt);
  Nan::SetPrototypeMethod(tmpl, "assignSqrt", AssignSqrt);
  Nan::SetPrototypeMethod(tmpl, "root", Root);
  Nan::SetPrototypeMethod(tmpl, "assignRoot", AssignRoot);
  Nan::SetPrototypeMethod(tmpl, "powm", Powm);
  Nan::SetPrototypeMethod(tmpl, "assignPowm", AssignPowm);
  Nan::SetPrototypeMethod(tmpl, "pow", Pow);
  Nan::SetPrototypeMethod(tmpl, "assignPow", AssignPow);
  Nan::SetPrototypeMethod(tmpl, "compare", Compare);
  Nan::SetPrototypeMethod(tmpl, "brand0", Brand0);
  Nan::SetPrototypeMethod(tmpl, "probprime", Probprime);
  Nan::SetPrototypeMethod(tmpl, "nextprime", Nextprime);
  Nan::SetPrototypeMethod(tmpl, "binvertm", Binvertm);
  Nan::SetPrototypeMethod(tmpl, "bgcd", Bgcd);
  Nan::SetPrototypeMethod(tmpl, "bitLength", BitLength);

  constructor_template.Reset(tmpl->GetFunction(context).ToLocalChecked());
  target->Set(context, Nan::New("MPZInternal").ToLocalChecked(), tmpl->GetFunction(context).ToLocalChecked());
}

GmpJS::GmpJS (const v8::String::Utf8Value& str, int64_t base) : Nan::ObjectWrap () {
  value = (mpz_t *) malloc(sizeof(mpz_t));

  mpz_init_set_str(*value, *str, base);
}

GmpJS::GmpJS (double num) : Nan::ObjectWrap () {
  value = (mpz_t *) malloc(sizeof(mpz_t));

  mpz_init_set_d(*value, num);
}

GmpJS::GmpJS (mpz_t *num) : Nan::ObjectWrap () {
  value = num;
}

GmpJS::GmpJS () : Nan::ObjectWrap () {
  value = (mpz_t *) malloc(sizeof(mpz_t));

  mpz_init(*value);
}

GmpJS::~GmpJS () {
  mpz_clear(*value);
  free(value);
}

NAN_METHOD(GmpJS::New) {
  Nan::HandleScope scope;
  GmpJS *self;

  if (info.Length() == 0) {
    self = new GmpJS();
  } else if (info[0]->IsExternal()) {
    mpz_t *num = static_cast<mpz_t *>(External::Cast(*(info[0]))->Value());

    self = new GmpJS(num);
  } else if (info[0]->IsNumber()) {
    auto num = Nan::To<double>(info[0]).FromJust();

    self = new GmpJS(num);
  } else {
    auto context = info.GetIsolate()->GetCurrentContext();
    auto isolate = context->GetIsolate();

    String::Utf8Value str(isolate, info[0]->ToString(context).ToLocalChecked());
    auto base = Nan::To<int64_t>(info[1]).FromJust();

    self = new GmpJS(str, base);
  }

  self->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(GmpJS::ToString) {
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  uint64_t base = 10;
  if (info.Length() > 0) {
    REQ_UINT64_ARG(0, tbase);
    base = tbase;
  }

  char *to = mpz_get_str(0, base, *self->value);

  info.GetReturnValue().Set(Nan::New<String>(to).ToLocalChecked());
  free(to);
}

NAN_METHOD(GmpJS::ToNumber) {
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  info.GetReturnValue().Set(Nan::New<Number>(mpz_get_d(*self->value)));
}

NAN_METHOD(GmpJS::Set) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  if (info[0]->IsNumber()) {
    auto num = Nan::To<double>(info[0]).FromJust();
    mpz_set_d(*self->value, num);
  } else if (info[0]->IsString()) {
    auto isolate = context->GetIsolate();

    String::Utf8Value str(isolate, info[0]->ToString(context).ToLocalChecked());
    auto base = Nan::To<int64_t>(info[1]).FromJust();
    mpz_set_str(*self->value, *str, base);
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
    mpz_set(*self->value, *num->value);
  }
}

NAN_METHOD(GmpJS::Add) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    if (num >= 0) {
      mpz_add_ui(*res, *self->value, num);
    } else {
      mpz_sub_ui(*res, *self->value, -num);
    }
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
    mpz_add(*res, *self->value, *num->value);
  }

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignAdd) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    if (num2 >= 0) {
      mpz_add_ui(*self->value, *num1->value, num2);
    } else {
      mpz_sub_ui(*self->value, *num1->value, -num2);
    }
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<GmpJS>(info[1]->ToObject(context).ToLocalChecked());
    mpz_add(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(GmpJS::Sub) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    if (num >= 0) {
      mpz_sub_ui(*res, *self->value, num);
    } else {
      mpz_add_ui(*res, *self->value, -num);
    }
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
    mpz_sub(*res, *self->value, *num->value);
  }

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignSub) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    if (num2 >= 0) {
      mpz_sub_ui(*self->value, *num1->value, num2);
    } else {
      mpz_add_ui(*self->value, *num1->value, -num2);
    }
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<GmpJS>(info[1]->ToObject(context).ToLocalChecked());
    mpz_sub(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(GmpJS::Mul) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    mpz_mul_ui(*res, *self->value, num);
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
    mpz_mul(*res, *self->value, *num->value);
  }

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignMul) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    mpz_mul_ui(*self->value, *num1->value, num2);
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<GmpJS>(info[1]->ToObject(context).ToLocalChecked());
    mpz_mul(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(GmpJS::Div) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    mpz_div_ui(*res, *self->value, num);
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
    mpz_div(*res, *self->value, *num->value);
  }

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignDiv) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    mpz_div_ui(*self->value, *num1->value, num2);
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<GmpJS>(info[1]->ToObject(context).ToLocalChecked());
    mpz_div(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(GmpJS::Mod) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    mpz_mod_ui(*res, *self->value, num);
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
    mpz_mod(*res, *self->value, *num->value);
  }

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignMod) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    mpz_mod_ui(*self->value, *num1->value, num2);
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<GmpJS>(info[1]->ToObject(context).ToLocalChecked());
    mpz_mod(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(GmpJS::AssignAddMul) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    mpz_addmul_ui(*self->value, *num1->value, num2);
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<GmpJS>(info[1]->ToObject(context).ToLocalChecked());
    mpz_addmul(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(GmpJS::AssignSubMul) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    mpz_submul_ui(*self->value, *num1->value, num2);
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<GmpJS>(info[1]->ToObject(context).ToLocalChecked());
    mpz_submul(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(GmpJS::And) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  mpz_and(*res, *self->value, *num->value);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignAnd) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  auto *num2 = Nan::ObjectWrap::Unwrap<GmpJS>(info[1]->ToObject(context).ToLocalChecked());

  mpz_and(*self->value, *num1->value, *num2->value);
}

NAN_METHOD(GmpJS::Or) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  mpz_ior(*res, *self->value, *num->value);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignOr) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  auto *num2 = Nan::ObjectWrap::Unwrap<GmpJS>(info[1]->ToObject(context).ToLocalChecked());

  mpz_ior(*self->value, *num1->value, *num2->value);
}

NAN_METHOD(GmpJS::Xor) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  mpz_xor(*res, *self->value, *num->value);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignXor) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  auto *num2 = Nan::ObjectWrap::Unwrap<GmpJS>(info[1]->ToObject(context).ToLocalChecked());

  mpz_xor(*self->value, *num1->value, *num2->value);
}

NAN_METHOD(GmpJS::Not) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  mpz_com(*res, *self->value);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignNot) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());

  mpz_com(*self->value, *num->value);
}

NAN_METHOD(GmpJS::ShiftLeft) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  auto num = Nan::To<int64_t>(info[0]).FromJust();
  mpz_mul_2exp(*res, *self->value, num);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignShiftLeft) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  auto num2 = Nan::To<int64_t>(info[1]).FromJust();

  mpz_mul_2exp(*self->value, *num1->value, num2);
}

NAN_METHOD(GmpJS::ShiftRight) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  auto num = Nan::To<int64_t>(info[0]).FromJust();
  mpz_div_2exp(*res, *self->value, num);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignShiftRight) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  auto num2 = Nan::To<int64_t>(info[1]).FromJust();

  mpz_div_2exp(*self->value, *num1->value, num2);
}

NAN_METHOD(GmpJS::Abs) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  mpz_abs(*res, *self->value);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignAbs) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());

  mpz_abs(*self->value, *num->value);
}

NAN_METHOD(GmpJS::Neg) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  mpz_neg(*res, *self->value);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignNeg) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());

  mpz_neg(*self->value, *num->value);
}

NAN_METHOD(GmpJS::Sqrt) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  mpz_sqrt(*res, *self->value);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignSqrt) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());

  mpz_sqrt(*self->value, *num->value);
}

NAN_METHOD(GmpJS::Root) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  auto num = Nan::To<int64_t>(info[0]).FromJust();
  mpz_root(*res, *self->value, num);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignRoot) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  auto num2 = Nan::To<int64_t>(info[1]).FromJust();

  mpz_root(*self->value, *num1->value, num2);
}

NAN_METHOD(GmpJS::Powm) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *mod = Nan::ObjectWrap::Unwrap<GmpJS>(info[1]->ToObject(context).ToLocalChecked());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if (info[0]->IsNumber()) {
    auto exp = Nan::To<int64_t>(info[0]).FromJust();
    mpz_powm_ui(*res, *self->value, exp, *mod->value);
  } else {
    auto *exp = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
    mpz_powm(*res, *self->value, *exp->value, *mod->value);
  }

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignPowm) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *base = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  auto *mod = Nan::ObjectWrap::Unwrap<GmpJS>(info[2]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto exp = Nan::To<int64_t>(info[1]).FromJust();
    mpz_powm_ui(*self->value, *base->value, exp, *mod->value);
  } else {
    auto *exp = Nan::ObjectWrap::Unwrap<GmpJS>(info[1]->ToObject(context).ToLocalChecked());
    mpz_powm(*self->value, *base->value, *exp->value, *mod->value);
  }
}

NAN_METHOD(GmpJS::Pow) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  auto exp = Nan::To<int64_t>(info[0]).FromJust();
  mpz_pow_ui(*res, *self->value, exp);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::AssignPow) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  auto *base = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  auto exp = Nan::To<int64_t>(info[1]).FromJust();

  mpz_pow_ui(*self->value, *base->value, exp);
}

NAN_METHOD(GmpJS::Compare) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  if (info[0]->IsNumber()) {
    auto op = Nan::To<double>(info[0]).FromJust();
    info.GetReturnValue().Set(Nan::New<Number>(mpz_cmp_d(*self->value, op)));
  } else {
    auto *op = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
    info.GetReturnValue().Set(Nan::New<Number>(mpz_cmp(*self->value, *op->value)));
  }
}

NAN_METHOD(GmpJS::Brand0) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpJS *bigint = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);

  if(randstate == NULL) {
    randstate = (gmp_randstate_t *) malloc(sizeof(gmp_randstate_t));
    gmp_randinit_default(*randstate);
    unsigned long seed = rand() + (time(NULL) * 1000) + clock();
          gmp_randseed_ui(*randstate, seed);
  }

  mpz_urandomm(*res, *randstate, *bigint->value);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::Probprime) {
  GmpJS *bigint = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  REQ_UINT32_ARG(0, reps);
  info.GetReturnValue().Set(Nan::New<Number>(mpz_probab_prime_p(*bigint->value, reps)));
}

NAN_METHOD(GmpJS::Nextprime) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpJS *bigint = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_nextprime(*res, *bigint->value);

  WRAP_RESULT(context, res, result);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::Binvertm) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpJS *bigint = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  GmpJS *bi = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_invert(*res, *bigint->value, *bi->value);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::Bgcd) {
  Local<Context> context = info.GetIsolate()->GetCurrentContext();
  GmpJS *bigint = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());
  GmpJS *bi = Nan::ObjectWrap::Unwrap<GmpJS>(info[0]->ToObject(context).ToLocalChecked());
  mpz_t *res = (mpz_t *) malloc(sizeof(mpz_t));
  mpz_init(*res);
  mpz_gcd(*res, *bigint->value, *bi->value);

  WRAP_RESULT(context, res, result);
  info.GetReturnValue().Set(result);
}

NAN_METHOD(GmpJS::BitLength) {
  auto *self = Nan::ObjectWrap::Unwrap<GmpJS>(info.This());

  int size = mpz_sizeinbase(*self->value, 2);

  auto result = Nan::New<Integer>(size);
  info.GetReturnValue().Set(result);
}

extern "C" void
init (Local<Object> target) {
  GmpJS::Initialize(target);
}

NODE_MODULE(gmpjs, init);

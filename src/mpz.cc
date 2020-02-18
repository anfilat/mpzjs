#include "mpz.h"

static gmp_randstate_t *randstate = NULL;

Nan::Persistent<v8::Function> MPZ::constructor;

void MPZ::Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::Context> context = exports->CreationContext();
  Nan::HandleScope scope;

  auto tmpl = Nan::New<v8::FunctionTemplate>(New);
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
  Nan::SetPrototypeMethod(tmpl, "rand", Rand);
  Nan::SetPrototypeMethod(tmpl, "assignRand", AssignRand);
  Nan::SetPrototypeMethod(tmpl, "probPrime", ProbPrime);
  Nan::SetPrototypeMethod(tmpl, "nextPrime", NextPrime);
  Nan::SetPrototypeMethod(tmpl, "assignNextPrime", AssignNextPrime);
  Nan::SetPrototypeMethod(tmpl, "invert", Invert);
  Nan::SetPrototypeMethod(tmpl, "assignInvert", AssignInvert);
  Nan::SetPrototypeMethod(tmpl, "gcd", Gcd);
  Nan::SetPrototypeMethod(tmpl, "assignGcd", AssignGcd);
  Nan::SetPrototypeMethod(tmpl, "bitLength", BitLength);

  constructor.Reset(tmpl->GetFunction(context).ToLocalChecked());
  exports->Set(context, Nan::New("MPZInternal").ToLocalChecked(), tmpl->GetFunction(context).ToLocalChecked());
}

MPZ::MPZ (const v8::String::Utf8Value& str, int64_t base) : Nan::ObjectWrap () {
  value = (mpz_t *) malloc(sizeof(mpz_t));

  mpz_init_set_str(*value, *str, base);
}

MPZ::MPZ (double num) : Nan::ObjectWrap () {
  value = (mpz_t *) malloc(sizeof(mpz_t));

  mpz_init_set_d(*value, num);
}

MPZ::MPZ () : Nan::ObjectWrap () {
  value = (mpz_t *) malloc(sizeof(mpz_t));

  mpz_init(*value);
}

MPZ::~MPZ () {
  mpz_clear(*value);
  free(value);
}

NAN_METHOD(MPZ::New) {
  MPZ *self;

  if (info.Length() == 0) {
    self = new MPZ();
  } else if (info[0]->IsNumber()) {
    auto num = Nan::To<double>(info[0]).FromJust();

    self = new MPZ(num);
  } else {
    auto context = info.GetIsolate()->GetCurrentContext();
    auto isolate = context->GetIsolate();

    v8::String::Utf8Value str(isolate, info[0]->ToString(context).ToLocalChecked());
    auto base = Nan::To<int64_t>(info[1]).FromJust();

    self = new MPZ(str, base);
  }

  self->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(MPZ::ToString) {
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  uint64_t base = 10;
  if (info.Length() > 0) {
    base = Nan::To<int64_t>(info[0]).FromJust();
  }

  char *to = mpz_get_str(0, base, *self->value);

  info.GetReturnValue().Set(Nan::New<v8::String>(to).ToLocalChecked());
  free(to);
}

NAN_METHOD(MPZ::ToNumber) {
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  info.GetReturnValue().Set(Nan::New<v8::Number>(mpz_get_d(*self->value)));
}

NAN_METHOD(MPZ::Set) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  if (info[0]->IsNumber()) {
    auto num = Nan::To<double>(info[0]).FromJust();
    mpz_set_d(*self->value, num);
  } else if (info[0]->IsString()) {
    auto isolate = context->GetIsolate();

    v8::String::Utf8Value str(isolate, info[0]->ToString(context).ToLocalChecked());
    auto base = Nan::To<int64_t>(info[1]).FromJust();
    mpz_set_str(*self->value, *str, base);
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
    mpz_set(*self->value, *num->value);
  }
}

NAN_METHOD(MPZ::Add) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    if (num >= 0) {
      mpz_add_ui(*res->value, *self->value, num);
    } else {
      mpz_sub_ui(*res->value, *self->value, -num);
    }
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
    mpz_add(*res->value, *self->value, *num->value);
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignAdd) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    if (num2 >= 0) {
      mpz_add_ui(*self->value, *num1->value, num2);
    } else {
      mpz_sub_ui(*self->value, *num1->value, -num2);
    }
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());
    mpz_add(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(MPZ::Sub) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    if (num >= 0) {
      mpz_sub_ui(*res->value, *self->value, num);
    } else {
      mpz_add_ui(*res->value, *self->value, -num);
    }
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
    mpz_sub(*res->value, *self->value, *num->value);
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignSub) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    if (num2 >= 0) {
      mpz_sub_ui(*self->value, *num1->value, num2);
    } else {
      mpz_add_ui(*self->value, *num1->value, -num2);
    }
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());
    mpz_sub(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(MPZ::Mul) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    mpz_mul_ui(*res->value, *self->value, num);
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
    mpz_mul(*res->value, *self->value, *num->value);
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignMul) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    mpz_mul_ui(*self->value, *num1->value, num2);
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());
    mpz_mul(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(MPZ::Div) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    mpz_div_ui(*res->value, *self->value, num);
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
    mpz_div(*res->value, *self->value, *num->value);
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignDiv) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    mpz_div_ui(*self->value, *num1->value, num2);
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());
    mpz_div(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(MPZ::Mod) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  if (info[0]->IsNumber()) {
    auto num = Nan::To<int64_t>(info[0]).FromJust();
    mpz_mod_ui(*res->value, *self->value, num);
  } else {
    auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
    mpz_mod(*res->value, *self->value, *num->value);
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignMod) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    mpz_mod_ui(*self->value, *num1->value, num2);
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());
    mpz_mod(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(MPZ::AssignAddMul) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    mpz_addmul_ui(*self->value, *num1->value, num2);
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());
    mpz_addmul(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(MPZ::AssignSubMul) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto num2 = Nan::To<int64_t>(info[1]).FromJust();
    mpz_submul_ui(*self->value, *num1->value, num2);
  } else {
    auto *num2 = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());
    mpz_submul(*self->value, *num1->value, *num2->value);
  }
}

NAN_METHOD(MPZ::And) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  mpz_and(*res->value, *self->value, *num->value);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignAnd) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  auto *num2 = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());

  mpz_and(*self->value, *num1->value, *num2->value);
}

NAN_METHOD(MPZ::Or) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  mpz_ior(*res->value, *self->value, *num->value);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignOr) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  auto *num2 = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());

  mpz_ior(*self->value, *num1->value, *num2->value);
}

NAN_METHOD(MPZ::Xor) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  mpz_xor(*res->value, *self->value, *num->value);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignXor) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  auto *num2 = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());

  mpz_xor(*self->value, *num1->value, *num2->value);
}

NAN_METHOD(MPZ::Not) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  mpz_com(*res->value, *self->value);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignNot) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  mpz_com(*self->value, *num->value);
}

NAN_METHOD(MPZ::ShiftLeft) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  auto num = Nan::To<int64_t>(info[0]).FromJust();
  mpz_mul_2exp(*res->value, *self->value, num);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignShiftLeft) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  auto num2 = Nan::To<int64_t>(info[1]).FromJust();

  mpz_mul_2exp(*self->value, *num1->value, num2);
}

NAN_METHOD(MPZ::ShiftRight) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  auto num = Nan::To<int64_t>(info[0]).FromJust();
  mpz_div_2exp(*res->value, *self->value, num);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignShiftRight) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  auto num2 = Nan::To<int64_t>(info[1]).FromJust();

  mpz_div_2exp(*self->value, *num1->value, num2);
}

NAN_METHOD(MPZ::Abs) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  mpz_abs(*res->value, *self->value);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignAbs) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  mpz_abs(*self->value, *num->value);
}

NAN_METHOD(MPZ::Neg) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  mpz_neg(*res->value, *self->value);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignNeg) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  mpz_neg(*self->value, *num->value);
}

NAN_METHOD(MPZ::Sqrt) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  mpz_sqrt(*res->value, *self->value);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignSqrt) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  mpz_sqrt(*self->value, *num->value);
}

NAN_METHOD(MPZ::Root) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  auto num = Nan::To<int64_t>(info[0]).FromJust();
  mpz_root(*res->value, *self->value, num);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignRoot) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  auto num2 = Nan::To<int64_t>(info[1]).FromJust();

  mpz_root(*self->value, *num1->value, num2);
}

NAN_METHOD(MPZ::Powm) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *mod = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  if (info[0]->IsNumber()) {
    auto exp = Nan::To<int64_t>(info[0]).FromJust();
    mpz_powm_ui(*res->value, *self->value, exp, *mod->value);
  } else {
    auto *exp = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
    mpz_powm(*res->value, *self->value, *exp->value, *mod->value);
  }

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignPowm) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *base = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  auto *mod = Nan::ObjectWrap::Unwrap<MPZ>(info[2]->ToObject(context).ToLocalChecked());

  if (info[1]->IsNumber()) {
    auto exp = Nan::To<int64_t>(info[1]).FromJust();
    mpz_powm_ui(*self->value, *base->value, exp, *mod->value);
  } else {
    auto *exp = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());
    mpz_powm(*self->value, *base->value, *exp->value, *mod->value);
  }
}

NAN_METHOD(MPZ::Pow) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  auto exp = Nan::To<int64_t>(info[0]).FromJust();
  mpz_pow_ui(*res->value, *self->value, exp);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignPow) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *base = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  auto exp = Nan::To<int64_t>(info[1]).FromJust();

  mpz_pow_ui(*self->value, *base->value, exp);
}

NAN_METHOD(MPZ::Compare) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  if (info[0]->IsNumber()) {
    auto op = Nan::To<double>(info[0]).FromJust();
    info.GetReturnValue().Set(Nan::New<v8::Number>(mpz_cmp_d(*self->value, op)));
  } else {
    auto *op = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
    info.GetReturnValue().Set(Nan::New<v8::Number>(mpz_cmp(*self->value, *op->value)));
  }
}

void initRand() {
    randstate = (gmp_randstate_t *) malloc(sizeof(gmp_randstate_t));
    gmp_randinit_default(*randstate);
    unsigned long seed = rand() + (time(NULL) * 1000) + clock();
    gmp_randseed_ui(*randstate, seed);
}

NAN_METHOD(MPZ::Rand) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  if (randstate == NULL) {
    initRand();
  }
  mpz_urandomm(*res->value, *randstate, *self->value);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignRand) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *n = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  if (randstate == NULL) {
    initRand();
  }
  mpz_urandomm(*self->value, *randstate, *n->value);
}

NAN_METHOD(MPZ::ProbPrime) {
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto reps = Nan::To<int64_t>(info[0]).FromJust();

  info.GetReturnValue().Set(Nan::New<v8::Number>(mpz_probab_prime_p(*self->value, reps)));
}

NAN_METHOD(MPZ::NextPrime) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  mpz_nextprime(*res->value, *self->value);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignNextPrime) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());

  mpz_nextprime(*self->value, *num->value);
}

NAN_METHOD(MPZ::Invert) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  mpz_invert(*res->value, *self->value, *num->value);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignInvert) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  auto *num2 = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());

  mpz_invert(*self->value, *num1->value, *num2->value);
}

NAN_METHOD(MPZ::Gcd) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  auto result = Nan::New<v8::Function>(constructor)->NewInstance(context, 0, nullptr).ToLocalChecked();
  auto *res = Nan::ObjectWrap::Unwrap<MPZ>(result);

  auto *num = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  mpz_gcd(*res->value, *self->value, *num->value);

  info.GetReturnValue().Set(result);
}

NAN_METHOD(MPZ::AssignGcd) {
  auto context = info.GetIsolate()->GetCurrentContext();
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());
  auto *num1 = Nan::ObjectWrap::Unwrap<MPZ>(info[0]->ToObject(context).ToLocalChecked());
  auto *num2 = Nan::ObjectWrap::Unwrap<MPZ>(info[1]->ToObject(context).ToLocalChecked());

  mpz_gcd(*self->value, *num1->value, *num2->value);
}

NAN_METHOD(MPZ::BitLength) {
  auto *self = Nan::ObjectWrap::Unwrap<MPZ>(info.This());

  int size = mpz_sizeinbase(*self->value, 2);

  auto result = Nan::New<v8::Integer>(size);
  info.GetReturnValue().Set(result);
}

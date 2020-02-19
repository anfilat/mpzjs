#include "mpz.h"

using namespace Napi;

static gmp_randstate_t *randstate = NULL;

FunctionReference MPZ::constructor;

Object MPZ::Init(Napi::Env env, Object exports) {
  HandleScope scope(env);

  Function func = DefineClass(env, "MPZInternal", {
    InstanceMethod("toString", &MPZ::ToString),
    InstanceMethod("toNumber", &MPZ::ToNumber),
    InstanceMethod("set", &MPZ::Set),
    InstanceMethod("add", &MPZ::Add),
    InstanceMethod("assignAdd", &MPZ::AssignAdd),
    InstanceMethod("sub", &MPZ::Sub),
    InstanceMethod("assignSub", &MPZ::AssignSub),
    InstanceMethod("mul", &MPZ::Mul),
    InstanceMethod("assignMul", &MPZ::AssignMul),
    InstanceMethod("div", &MPZ::Div),
    InstanceMethod("assignDiv", &MPZ::AssignDiv),
    InstanceMethod("mod", &MPZ::Mod),
    InstanceMethod("assignMod", &MPZ::AssignMod),
    InstanceMethod("assignAddMul", &MPZ::AssignAddMul),
    InstanceMethod("assignSubMul", &MPZ::AssignSubMul),
    InstanceMethod("and", &MPZ::And),
    InstanceMethod("assignAnd", &MPZ::AssignAnd),
    InstanceMethod("or", &MPZ::Or),
    InstanceMethod("assignOr", &MPZ::AssignOr),
    InstanceMethod("xor", &MPZ::Xor),
    InstanceMethod("assignXor", &MPZ::AssignXor),
    InstanceMethod("not", &MPZ::Not),
    InstanceMethod("assignNot", &MPZ::AssignNot),
    InstanceMethod("shiftLeft", &MPZ::ShiftLeft),
    InstanceMethod("assignShiftLeft", &MPZ::AssignShiftLeft),
    InstanceMethod("shiftRight", &MPZ::ShiftRight),
    InstanceMethod("assignShiftRight", &MPZ::AssignShiftRight),
    InstanceMethod("abs", &MPZ::Abs),
    InstanceMethod("assignAbs", &MPZ::AssignAbs),
    InstanceMethod("neg", &MPZ::Neg),
    InstanceMethod("assignNeg", &MPZ::AssignNeg),
    InstanceMethod("sqrt", &MPZ::Sqrt),
    InstanceMethod("assignSqrt", &MPZ::AssignSqrt),
    InstanceMethod("root", &MPZ::Root),
    InstanceMethod("assignRoot", &MPZ::AssignRoot),
    InstanceMethod("powm", &MPZ::Powm),
    InstanceMethod("assignPowm", &MPZ::AssignPowm),
    InstanceMethod("pow", &MPZ::Pow),
    InstanceMethod("assignPow", &MPZ::AssignPow),
    InstanceMethod("compare", &MPZ::Compare),
    InstanceMethod("rand", &MPZ::Rand),
    InstanceMethod("assignRand", &MPZ::AssignRand),
    InstanceMethod("probPrime", &MPZ::ProbPrime),
    InstanceMethod("nextPrime", &MPZ::NextPrime),
    InstanceMethod("assignNextPrime", &MPZ::AssignNextPrime),
    InstanceMethod("invert", &MPZ::Invert),
    InstanceMethod("assignInvert", &MPZ::AssignInvert),
    InstanceMethod("gcd", &MPZ::Gcd),
    InstanceMethod("assignGcd", &MPZ::AssignGcd),
    InstanceMethod("bitLength", &MPZ::BitLength)
  });

  constructor = Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("MPZInternal", func);
  return exports;
}

MPZ::MPZ(const CallbackInfo& info) : ObjectWrap<MPZ>(info) {
  Napi::Env env = info.Env();
  HandleScope scope(env);

  this->value = (mpz_t *) malloc(sizeof(mpz_t));

  if (info.Length() == 0) {
    mpz_init(*this->value);
  } else if (info[0].IsNumber()) {
    auto num = info[0].As<Number>().DoubleValue();
    mpz_init_set_d(*this->value, num);
  } else {
    auto base = info[1].As<Number>().Int64Value();
    mpz_init_set_str(*value, info[0].As<String>().Utf8Value().c_str(), base);
  }
}

MPZ::~MPZ () {
  mpz_clear(*this->value);
  free(this->value);
}

Value MPZ::ToString(const CallbackInfo& info) {
  Napi::Env env = info.Env();

  uint64_t base = 10;
  if (info.Length() > 0) {
    base = info[0].As<Number>().Int64Value();
  }

  char *to = mpz_get_str(0, base, *this->value);

  return String::New(env, to);
  free(to);
}

Value MPZ::ToNumber(const CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Number::New(env, mpz_get_d(*this->value));
}

void MPZ::Set(const CallbackInfo& info) {
  if (info[0].IsNumber()) {
    auto num = info[0].As<Number>().DoubleValue();
    mpz_set_d(*this->value, num);
  } else if (info[0].IsString()) {
    auto base = info[1].As<Number>().Int64Value();
    mpz_set_str(*this->value, info[0].As<String>().Utf8Value().c_str(), base);
  } else {
    auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
    mpz_set(*this->value, *num->value);
  }
}

Value MPZ::Add(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  if (info[0].IsNumber()) {
    auto num = info[0].As<Number>().Int64Value();
    if (num >= 0) {
      mpz_add_ui(*res->value, *this->value, num);
    } else {
      mpz_sub_ui(*res->value, *this->value, -num);
    }
  } else {
    auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
    mpz_add(*res->value, *this->value, *num->value);
  }

  return result;
}

void MPZ::AssignAdd(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  if (info[1].IsNumber()) {
    auto num2 = info[1].As<Number>().Int64Value();
    if (num2 >= 0) {
      mpz_add_ui(*this->value, *num1->value, num2);
    } else {
      mpz_sub_ui(*this->value, *num1->value, -num2);
    }
  } else {
    auto *num2 = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());
    mpz_add(*this->value, *num1->value, *num2->value);
  }
}

Value MPZ::Sub(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  if (info[0].IsNumber()) {
    auto num = info[0].As<Number>().Int64Value();
    if (num >= 0) {
      mpz_sub_ui(*res->value, *this->value, num);
    } else {
      mpz_add_ui(*res->value, *this->value, -num);
    }
  } else {
    auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
    mpz_sub(*res->value, *this->value, *num->value);
  }

  return result;
}

void MPZ::AssignSub(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  if (info[1].IsNumber()) {
    auto num2 = info[1].As<Number>().Int64Value();
    if (num2 >= 0) {
      mpz_sub_ui(*this->value, *num1->value, num2);
    } else {
      mpz_add_ui(*this->value, *num1->value, -num2);
    }
  } else {
    auto *num2 = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());
    mpz_sub(*this->value, *num1->value, *num2->value);
  }
}

Value MPZ::Mul(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  if (info[0].IsNumber()) {
    auto num = info[0].As<Number>().Int64Value();
    mpz_mul_ui(*res->value, *this->value, num);
  } else {
    auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
    mpz_mul(*res->value, *this->value, *num->value);
  }

  return result;
}

void MPZ::AssignMul(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  if (info[1].IsNumber()) {
    auto num2 = info[1].As<Number>().Int64Value();
    mpz_mul_ui(*this->value, *num1->value, num2);
  } else {
    auto *num2 = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());
    mpz_mul(*this->value, *num1->value, *num2->value);
  }
}

Value MPZ::Div(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  if (info[0].IsNumber()) {
    auto num = info[0].As<Number>().Int64Value();
    mpz_div_ui(*res->value, *this->value, num);
  } else {
    auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
    mpz_div(*res->value, *this->value, *num->value);
  }

  return result;
}

void MPZ::AssignDiv(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  if (info[1].IsNumber()) {
    auto num2 = info[1].As<Number>().Int64Value();
    mpz_div_ui(*this->value, *num1->value, num2);
  } else {
    auto *num2 = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());
    mpz_div(*this->value, *num1->value, *num2->value);
  }
}

Value MPZ::Mod(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  if (info[0].IsNumber()) {
    auto num = info[0].As<Number>().Int64Value();
    mpz_mod_ui(*res->value, *this->value, num);
  } else {
    auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
    mpz_mod(*res->value, *this->value, *num->value);
  }

  return result;
}

void MPZ::AssignMod(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  if (info[1].IsNumber()) {
    auto num2 = info[1].As<Number>().Int64Value();
    mpz_mod_ui(*this->value, *num1->value, num2);
  } else {
    auto *num2 = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());
    mpz_mod(*this->value, *num1->value, *num2->value);
  }
}

void MPZ::AssignAddMul(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  if (info[1].IsNumber()) {
    auto num2 = info[1].As<Number>().Int64Value();
    mpz_addmul_ui(*this->value, *num1->value, num2);
  } else {
    auto *num2 = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());
    mpz_addmul(*this->value, *num1->value, *num2->value);
  }
}

void MPZ::AssignSubMul(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  if (info[1].IsNumber()) {
    auto num2 = info[1].As<Number>().Int64Value();
    mpz_submul_ui(*this->value, *num1->value, num2);
  } else {
    auto *num2 = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());
    mpz_submul(*this->value, *num1->value, *num2->value);
  }
}

Value MPZ::And(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  mpz_and(*res->value, *this->value, *num->value);

  return result;
}

void MPZ::AssignAnd(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  auto *num2 = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());

  mpz_and(*this->value, *num1->value, *num2->value);
}

Value MPZ::Or(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  mpz_ior(*res->value, *this->value, *num->value);

  return result;
}

void MPZ::AssignOr(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  auto *num2 = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());

  mpz_ior(*this->value, *num1->value, *num2->value);
}

Value MPZ::Xor(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  mpz_xor(*res->value, *this->value, *num->value);

  return result;
}

void MPZ::AssignXor(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  auto *num2 = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());

  mpz_xor(*this->value, *num1->value, *num2->value);
}

Value MPZ::Not(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  mpz_com(*res->value, *this->value);

  return result;
}

void MPZ::AssignNot(const CallbackInfo& info) {
  auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  mpz_com(*this->value, *num->value);
}

Value MPZ::ShiftLeft(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  auto num = info[0].As<Number>().Int64Value();
  mpz_mul_2exp(*res->value, *this->value, num);

  return result;
}

void MPZ::AssignShiftLeft(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  auto num2 = info[1].As<Number>().Int64Value();

  mpz_mul_2exp(*this->value, *num1->value, num2);
}

Value MPZ::ShiftRight(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  auto num = info[0].As<Number>().Int64Value();
  mpz_div_2exp(*res->value, *this->value, num);

  return result;
}

void MPZ::AssignShiftRight(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  auto num2 = info[1].As<Number>().Int64Value();

  mpz_div_2exp(*this->value, *num1->value, num2);
}

Value MPZ::Abs(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  mpz_abs(*res->value, *this->value);

  return result;
}

void MPZ::AssignAbs(const CallbackInfo& info) {
  auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  mpz_abs(*this->value, *num->value);
}

Value MPZ::Neg(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  mpz_neg(*res->value, *this->value);

  return result;
}

void MPZ::AssignNeg(const CallbackInfo& info) {
  auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  mpz_neg(*this->value, *num->value);
}

Value MPZ::Sqrt(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  mpz_sqrt(*res->value, *this->value);

  return result;
}

void MPZ::AssignSqrt(const CallbackInfo& info) {
  auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  mpz_sqrt(*this->value, *num->value);
}

Value MPZ::Root(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  auto num = info[0].As<Number>().Int64Value();
  mpz_root(*res->value, *this->value, num);

  return result;
}

void MPZ::AssignRoot(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  auto num2 = info[1].As<Number>().Int64Value();

  mpz_root(*this->value, *num1->value, num2);
}

Value MPZ::Powm(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  auto *mod = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());
  if (info[0].IsNumber()) {
    auto exp = info[0].As<Number>().Int64Value();
    mpz_powm_ui(*res->value, *this->value, exp, *mod->value);
  } else {
    auto *exp = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
    mpz_powm(*res->value, *this->value, *exp->value, *mod->value);
  }

  return result;
}

void MPZ::AssignPowm(const CallbackInfo& info) {
  auto *base = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  auto *mod = ObjectWrap<MPZ>::Unwrap(info[2].As<Object>());

  if (info[1].IsNumber()) {
    auto exp = info[1].As<Number>().Int64Value();
    mpz_powm_ui(*this->value, *base->value, exp, *mod->value);
  } else {
    auto *exp = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());
    mpz_powm(*this->value, *base->value, *exp->value, *mod->value);
  }
}

Value MPZ::Pow(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  auto exp = info[0].As<Number>().Int64Value();
  mpz_pow_ui(*res->value, *this->value, exp);

  return result;
}

void MPZ::AssignPow(const CallbackInfo& info) {
  auto *base = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  auto exp = info[1].As<Number>().Int64Value();

  mpz_pow_ui(*this->value, *base->value, exp);
}

Value MPZ::Compare(const CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info[0].IsNumber()) {
    auto op = info[0].As<Number>().DoubleValue();
    return Number::New(env, mpz_cmp_d(*this->value, op));
  } else {
    auto *op = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
    return Number::New(env, mpz_cmp(*this->value, *op->value));
  }
}

void initRand() {
    randstate = (gmp_randstate_t *) malloc(sizeof(gmp_randstate_t));
    gmp_randinit_default(*randstate);
    unsigned long seed = rand() + (time(NULL) * 1000) + clock();
    gmp_randseed_ui(*randstate, seed);
}

Value MPZ::Rand(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  if (randstate == NULL) {
    initRand();
  }
  mpz_urandomm(*res->value, *randstate, *this->value);

  return result;
}

void MPZ::AssignRand(const CallbackInfo& info) {
  auto *n = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  if (randstate == NULL) {
    initRand();
  }
  mpz_urandomm(*this->value, *randstate, *n->value);
}

Value MPZ::ProbPrime(const CallbackInfo& info) {
  Napi::Env env = info.Env();
  auto reps = info[0].As<Number>().Int64Value();

  return Number::New(env, mpz_probab_prime_p(*this->value, reps));
}

Value MPZ::NextPrime(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  mpz_nextprime(*res->value, *this->value);

  return result;
}

void MPZ::AssignNextPrime(const CallbackInfo& info) {
  auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());

  mpz_nextprime(*this->value, *num->value);
}

Value MPZ::Invert(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  mpz_invert(*res->value, *this->value, *num->value);

  return result;
}

void MPZ::AssignInvert(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  auto *num2 = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());

  mpz_invert(*this->value, *num1->value, *num2->value);
}

Value MPZ::Gcd(const CallbackInfo& info) {
  auto result = constructor.New({});
  auto *res = ObjectWrap<MPZ>::Unwrap(result);

  auto *num = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  mpz_gcd(*res->value, *this->value, *num->value);

  return result;
}

void MPZ::AssignGcd(const CallbackInfo& info) {
  auto *num1 = ObjectWrap<MPZ>::Unwrap(info[0].As<Object>());
  auto *num2 = ObjectWrap<MPZ>::Unwrap(info[1].As<Object>());

  mpz_gcd(*this->value, *num1->value, *num2->value);
}

Value MPZ::BitLength(const CallbackInfo& info) {
  Napi::Env env = info.Env();

  return Number::New(env, mpz_sizeinbase(*this->value, 2));
}

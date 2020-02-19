#ifndef MPZ_H
#define MPZ_H

#include <napi.h>
#include <uv.h>
#include <gmp.h>

class MPZ : public Napi::ObjectWrap<MPZ> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    MPZ(const Napi::CallbackInfo& info);
    ~MPZ();

  private:
    Napi::Value ToString(const Napi::CallbackInfo& info);
    Napi::Value ToNumber(const Napi::CallbackInfo& info);
    void Set(const Napi::CallbackInfo& info);
    Napi::Value Add(const Napi::CallbackInfo& info);
    void AssignAdd(const Napi::CallbackInfo& info);
    Napi::Value Sub(const Napi::CallbackInfo& info);
    void AssignSub(const Napi::CallbackInfo& info);
    Napi::Value Mul(const Napi::CallbackInfo& info);
    void AssignMul(const Napi::CallbackInfo& info);
    Napi::Value Div(const Napi::CallbackInfo& info);
    void AssignDiv(const Napi::CallbackInfo& info);
    Napi::Value Mod(const Napi::CallbackInfo& info);
    void AssignMod(const Napi::CallbackInfo& info);
    void AssignAddMul(const Napi::CallbackInfo& info);
    void AssignSubMul(const Napi::CallbackInfo& info);
    Napi::Value And(const Napi::CallbackInfo& info);
    void AssignAnd(const Napi::CallbackInfo& info);
    Napi::Value Or(const Napi::CallbackInfo& info);
    void AssignOr(const Napi::CallbackInfo& info);
    Napi::Value Xor(const Napi::CallbackInfo& info);
    void AssignXor(const Napi::CallbackInfo& info);
    Napi::Value Not(const Napi::CallbackInfo& info);
    void AssignNot(const Napi::CallbackInfo& info);
    Napi::Value ShiftLeft(const Napi::CallbackInfo& info);
    void AssignShiftLeft(const Napi::CallbackInfo& info);
    Napi::Value ShiftRight(const Napi::CallbackInfo& info);
    void AssignShiftRight(const Napi::CallbackInfo& info);
    Napi::Value Abs(const Napi::CallbackInfo& info);
    void AssignAbs(const Napi::CallbackInfo& info);
    Napi::Value Neg(const Napi::CallbackInfo& info);
    void AssignNeg(const Napi::CallbackInfo& info);
    Napi::Value Sqrt(const Napi::CallbackInfo& info);
    void AssignSqrt(const Napi::CallbackInfo& info);
    Napi::Value Root(const Napi::CallbackInfo& info);
    void AssignRoot(const Napi::CallbackInfo& info);
    Napi::Value Powm(const Napi::CallbackInfo& info);
    void AssignPowm(const Napi::CallbackInfo& info);
    Napi::Value Pow(const Napi::CallbackInfo& info);
    void AssignPow(const Napi::CallbackInfo& info);
    Napi::Value Compare(const Napi::CallbackInfo& info);
    Napi::Value Rand(const Napi::CallbackInfo& info);
    void AssignRand(const Napi::CallbackInfo& info);
    Napi::Value ProbPrime(const Napi::CallbackInfo& info);
    Napi::Value NextPrime(const Napi::CallbackInfo& info);
    void AssignNextPrime(const Napi::CallbackInfo& info);
    Napi::Value Invert(const Napi::CallbackInfo& info);
    void AssignInvert(const Napi::CallbackInfo& info);
    Napi::Value Gcd(const Napi::CallbackInfo& info);
    void AssignGcd(const Napi::CallbackInfo& info);
    Napi::Value BitLength(const Napi::CallbackInfo& info);

    static Napi::FunctionReference constructor;

    mpz_t *value;
};

#endif

#ifndef MPZ_H
#define MPZ_H

#include <nan.h>
#include <gmp.h>

class MPZ : public Nan::ObjectWrap {
  public:
    static void Init(v8::Local<v8::Object> exports);

  private:
    MPZ(const v8::String::Utf8Value& str, int64_t base);
    MPZ(double num);
    MPZ();
    ~MPZ();

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
    static NAN_METHOD(Rand);
    static NAN_METHOD(AssignRand);
    static NAN_METHOD(ProbPrime);
    static NAN_METHOD(NextPrime);
    static NAN_METHOD(AssignNextPrime);
    static NAN_METHOD(Invert);
    static NAN_METHOD(AssignInvert);
    static NAN_METHOD(Gcd);
    static NAN_METHOD(AssignGcd);
    static NAN_METHOD(BitLength);

    static Nan::Persistent<v8::Function> constructor;

    mpz_t *value;
};

#endif

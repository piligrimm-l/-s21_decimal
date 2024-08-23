#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  e_return_value_t return_value = CONVERSION_ERROR;
  int power = getPower(src);
  int sign = getSign(src);

  if (dst && power != -1) {
    *dst = 0;
    for (int i = 0; i <= lastBit(src); i++) {
      *dst += (getMantissaBit(src, i) * pow(2, i));
    }

    if (power > 0) {
      *dst /= pow(10, power);
    }

    if (sign) {
      *dst = -*dst;
    }

    return_value = OK;
  }

  return return_value;
}
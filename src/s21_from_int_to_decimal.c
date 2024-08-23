#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  e_return_value_t result = CONVERSION_ERROR;

  if (dst) {
    *dst = defaultDecimal();
    if (src < 0) {
      setSign(dst, 1);
      src = -src;
    }
    dst->bits[0] = src;
    result = OK;

  } else {
    fprintf(stderr, "s21_from_int_to_decimal: ERROR");
  }

  return result;
}
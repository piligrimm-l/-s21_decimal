#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  e_return_value_t res = CALCULATION_ERROR;

  if (result) {
    *result = value;
    int sign = getSign(value);
    if (sign != -1) {
      if (sign == 1) {
        setSign(result, 0);
      } else if (getSign(value) == 0) {
        setSign(result, 1);
      }
      res = OK;
    }
  }

  return res;
}
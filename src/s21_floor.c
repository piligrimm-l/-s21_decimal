#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int return_value = OK;
  *result = value;
  int power = getPower(value);
  int sign = getSign(value);
  s21_decimal ten_in_power = defaultDecimal();
  ten_in_power.bits[0] = 1;
  for (int i = 1; i <= power && return_value == OK; i++) {
    return_value = mulBy10(&ten_in_power);
  }

  if (return_value == OK && power > 0) {
    *result = defaultDecimal();
    s21_decimal quotient = defaultDecimal();

    return_value = divSimple(value, ten_in_power, &quotient, NULL);
    setSign(&quotient, sign);

    s21_decimal one = defaultDecimal();
    one.bits[0] = 1;

    if (sign == 0) {
      *result = quotient;
    } else {
      return_value = s21_sub(quotient, one, result);
    }
  }

  if (return_value != OK) {
    return_value = CALCULATION_ERROR;
  }

  return return_value;
}
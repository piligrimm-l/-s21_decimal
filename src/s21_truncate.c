#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int return_value = OK;

  if (result) {
    int power = getPower(value);
    s21_decimal ten_in_power = defaultDecimal();
    ten_in_power.bits[0] = 1;
    for (int i = 1; i <= power && return_value == OK; i++) {
      return_value = mulBy10(&ten_in_power);
    }

    if (return_value == OK) {
      return_value = divSimple(value, ten_in_power, result, NULL);
      setSign(result, getSign(value));
    }
  }

  if (return_value != OK) {
    return_value = CALCULATION_ERROR;
  }

  return return_value;
}
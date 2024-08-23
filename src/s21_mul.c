#include "s21_decimal.h"

int s21_mul(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
  int return_value = OK;
  *result = defaultDecimal();

  int sign1 = getSign(value1);
  int sign2 = getSign(value2);
  int sign_result = 0;
  if (sign1 != sign2 && (sign1 == 1 || sign2 == 1)) {
    sign_result = 1;
  }

  int power_result = getPower(value1) + getPower(value2);

  if (lastBit(value1) < 0 || lastBit(value2) < 0) {
    sign_result = 0;
    power_result = 0;
  }

  value1.bits[3] = 0;
  value2.bits[3] = 0;

  for (int bit = 0; bit <= lastBit(value2) && return_value == OK; bit++) {
    s21_decimal temp = value1;
    if (getMantissaBit(value2, bit) == 1) {
      int err = shiftLeft(&temp, bit);
      if (err == OK) {
        err = addSimple(*result, temp, result);
      }

      if (err != OK) {
        if (power_result > 0) {
          setPower(&value1, 1);
          s21_round(value1, &value1);
          power_result--;
        } else {
          return_value = sign_result == 0 ? EXCEEDING_THE_POSITIVE_TYPE_LIMIT
                                          : EXCEEDING_THE_NEGATIVE_TYPE_LIMIT;
        }
      }
    }
  }

  if (return_value == OK && power_result > 28) {
    int off = power_result - 28;
    setPower(result, off);
    s21_round(*result, result);
    power_result -= off;
  }

  if (return_value == OK) {
    setSign(result, sign_result);
    setPower(result, power_result);
  }

  return return_value;
}
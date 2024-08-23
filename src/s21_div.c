// By Admiralm
#include "s21_decimal.h"

int s21_div(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
  int return_value = OK;

  if (value2.bits[2] == 0 && value2.bits[1] == 0 && value2.bits[0] == 0) {
    return_value = DIVISION_BY_ZERO;
  } else {
    *result = defaultDecimal();
    int power1 = getPower(value1);
    int power2 = getPower(value2);
    int power_result = power1 - power2;

    int sign1 = getSign(value1);
    int sign2 = getSign(value2);

    int sign_result = 0;
    if (sign1 != sign2) {
      sign_result = 1;
    }

    return_value = divSimple(value1, value2, result, NULL);

    setSign(result, sign_result);

    //Если степень отрицательная, то надо её уменьшить до 0
    while (return_value == OK && power_result < 0) {
      return_value = mulBy10(result);
      power_result++;
    }

    setPower(result, power_result);

    if (return_value == EXCEEDING_THE_POSITIVE_TYPE_LIMIT && sign_result == 1) {
      return_value = EXCEEDING_THE_NEGATIVE_TYPE_LIMIT;
    }
  }

  return return_value;
}
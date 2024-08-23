#include "s21_decimal.h"

int s21_add(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
  int return_value = OK;
  int sign1 = getSign(value1);
  int sign2 = getSign(value2);
  int power1 = getPower(value1);
  int power2 = getPower(value2);
  *result = defaultDecimal();

  if (power1 != power2) {
    normalize(&value1, &value2);
  }

  if (sign1 == sign2) {
    return_value = addSimple(value1, value2, result);
  } else {
    additionalCode(&value1);
    additionalCode(&value2);
    addSimpleForAdditionalCode(value1, value2, result);
    additionalCode(result);
  }

  if (return_value == EXCEEDING_THE_POSITIVE_TYPE_LIMIT && sign1 == 1) {
    return_value = EXCEEDING_THE_NEGATIVE_TYPE_LIMIT;
  }

  return return_value;
}
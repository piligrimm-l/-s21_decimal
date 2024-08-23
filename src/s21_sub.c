// By admiralm

#include "s21_decimal.h"

int s21_sub(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
  int return_value = OK;

  s21_negate(value2, &value2);
  return_value = s21_add(value1, value2, result);

  return return_value;
}
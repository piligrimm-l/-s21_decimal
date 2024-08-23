#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int return_value = OK;
  int power = getPower(value);

  return_value = roundBank(&value, power);
  *result = value;

  return return_value;
}
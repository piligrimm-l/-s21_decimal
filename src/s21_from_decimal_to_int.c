// Есть неучтённые моменты с большими decimal. Если не вмешается выдовать
// ошибку. Использовать s21_truncate()

// By admiralm

#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int return_value = CONVERSION_ERROR;

  if (dst) {
    int sign = getSign(src);
    s21_decimal temp = defaultDecimal();
    return_value = s21_truncate(src, &temp);

    if (return_value == OK && temp.bits[2] == 0 && temp.bits[1] == 0) {
      int value = temp.bits[0];

      if (sign == 1) {
        value = -value;
      }

      *dst = value;
      return_value = OK;
    } else {
      return_value = CONVERSION_ERROR;
    }
  }

  return return_value;
}
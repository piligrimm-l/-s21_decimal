#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  e_return_value_t return_value = CONVERSION_ERROR;

  if (dst) {
    *dst = defaultDecimal();
    if ((fabs(src) >= 1e-28 && fabs(src) < pow(2, 96)) || fabs(src) == 0.0) {
      char float_to_string[20];
      sprintf(float_to_string, "%.6e", fabs(src));

      unsigned int *ptr_float = (unsigned int *)&src;
      unsigned int sign_float = *ptr_float >> 31;

      if (sign_float == 1) {
        setSign(dst, 1);
        src = -src;
      }

      int power_of_float = 0;
      unsigned int integer_of_float =
          parseFloatString(float_to_string, &power_of_float);

      // Убираем лишние нули в целой части
      while (integer_of_float % 10 == 0 && power_of_float < 0) {
        integer_of_float /= 10;
        power_of_float++;
      }

      if (power_of_float >= -28) {
        dst->bits[0] = integer_of_float;

        // Избавляемся от положительной степени
        while (power_of_float > 0) {
          mulBy10(dst);
          power_of_float--;
        }

        setPower(dst, abs(power_of_float));

        return_value = OK;
      }
    }
  }

  if (return_value == CONVERSION_ERROR) {
    fprintf(stderr,
            "s21_from_float_to_decimal: WARNING: Very large or very small the "
            "number for s21_decimal!!!\n");
  }

  return return_value;
}

int parseFloatString(char *str, int *power) {
  int integer_part = 0;
  bool power_flag = false;
  *power = 0;
  int sign_power = 1;
  char integer_part_string[8] = {'\0'};
  char power_part_string[3] = {'\0'};
  int i = 0, j = 0;

  while (*str != '\0') {
    if (isdigit(*str) && !power_flag) {
      integer_part_string[i++] = *str;
    }

    if (power_flag) {
      if (*str == '-') {
        sign_power = -1;
      } else if (isdigit(*str)) {
        power_part_string[j++] = *str;
      }
    }

    if (*str == 'e') {
      power_flag = true;
    }

    str++;
  }
  integer_part = strtol(integer_part_string, NULL, 10);
  *power = sign_power * strtol(power_part_string, NULL, 10) - 6;

  return integer_part;
}
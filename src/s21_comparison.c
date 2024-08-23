#include "s21_decimal.h"

int s21_compare(s21_decimal num1, s21_decimal num2) {
  int zero = num1.bits[0] == 0 && num1.bits[1] == 0 && num1.bits[2] == 0 &&
             num2.bits[0] == 0 && num2.bits[1] == 0 && num2.bits[2] == 0;
  if (zero) {
    return 0;
  }

  if (getPower(num1) != getPower(num2)) {
    int normalize_error = normalize(&num1, &num2);
    if (normalize_error) {
      fprintf(stderr, "Error during normalization.\n");
      return normalize_error;
    }
  }

  int sign1 = getSign(num1);
  int sign2 = getSign(num2);
  if (sign1 != sign2) {
    return sign1 > sign2 ? -1 : 1;
  }

  for (int i = 2; i >= 0; i--) {
    if (num1.bits[i] != num2.bits[i]) {
      return sign1 ? (num1.bits[i] < num2.bits[i] ? 1 : -1)
                   : (num1.bits[i] > num2.bits[i] ? 1 : -1);
    }
  }
  return 0;
}

int s21_is_equal(s21_decimal num1, s21_decimal num2) {
  return s21_compare(num1, num2) == 0;
}

int s21_is_not_equal(s21_decimal num1, s21_decimal num2) {
  return s21_is_equal(num1, num2) == 0;
}

int s21_is_less(s21_decimal num1, s21_decimal num2) {
  int result = 0;
  int sign1 = getSign(num1);
  int sign2 = getSign(num2);
  int zero = num1.bits[0] == 0 && num1.bits[1] == 0 && num1.bits[2] == 0 &&
             num2.bits[0] == 0 && num2.bits[1] == 0 && num2.bits[2] == 0;
  if (zero) {
    result = 0;
  } else if (sign1 == 1 && sign2 == 0) {
    result = 1;
  } else if (sign1 == 0 && sign2 == 1) {
    result = 0;
  } else if (sign1 == sign2) {
    normalize(&num1, &num2);
    for (int i = 2; i >= 0; i--) {
      if (num1.bits[i] < num2.bits[i]) {
        result = (sign1 == 1) ? 0 : 1;
        break;
      } else if (num1.bits[i] > num2.bits[i]) {
        result = (sign1 == 1) ? 1 : 0;
        break;
      }
    }
  }
  return result;
}

int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2) {
  return (s21_is_greater(num1, num2)) ? 0 : 1;
}

int s21_is_greater(s21_decimal num1, s21_decimal num2) {
  int result = 0;
  int sign1 = getSign(num1);
  int sign2 = getSign(num2);
  int zero = num1.bits[0] == 0 && num1.bits[1] == 0 && num1.bits[2] == 0 &&
             num2.bits[0] == 0 && num2.bits[1] == 0 && num2.bits[2] == 0;
  if (zero) {
    result = 0;
  } else if (sign1 == 0 && sign2 == 1) {
    result = 1;
  } else if (sign1 == 1 && sign2 == 0) {
    result = 0;
  } else if (sign1 == sign2) {
    normalize(&num1, &num2);
    for (int i = 2; i >= 0; i--) {
      if (num1.bits[i] > num2.bits[i]) {
        result = (sign1 == 0) ? 1 : 0;
        break;
      } else if (num1.bits[i] < num2.bits[i]) {
        result = (sign1 == 0) ? 0 : 1;
        break;
      }
    }
  }
  return result;
}

int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2) {
  return (s21_is_less(num1, num2)) ? 0 : 1;
}

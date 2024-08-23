// By Admiralm
// Extra functions.

#include "s21_decimal.h"

// Значение По умолчанию Decimal = 0
s21_decimal defaultDecimal() {
  s21_decimal value;

  for (int i = 0; i < INT_BLOCK; i++) {
    value.bits[i] = 0;
  }

  return value;
}

// Получение значения бита мантиссы, по указанному номеру бита
int getMantissaBit(s21_decimal value, int bit) {
  int result = -1;

  if (bit >= 0 && bit < MANTISSA_BITS) {
    int mask = 1 << (bit % INT_BITS);
    result = (value.bits[bit / INT_BITS] & mask) >> (bit % INT_BITS);
    if (result < 0) {
      result = -result;
    }
  } else {
    fprintf(stderr, "getMantissaBit: ERROR: Accessing an invalid bit!!!\n");
  }

  return result;
}

// Установить значения бита мантиссы, по указанному номеру бита
bool setMantissaBit(s21_decimal *value_decimal, int bit, int value) {
  bool result = false;

  if (bit >= 0 && bit < MANTISSA_BITS) {
    int mask = 1 << (bit % INT_BITS);
    if (value == 1) {
      value_decimal->bits[bit / INT_BITS] |= mask;
      result = true;
    } else if (value == 0) {
      value_decimal->bits[bit / INT_BITS] &= ~mask;
      result = true;
    } else {
      fprintf(stderr, "setMantissaBit: ERROR: value can be only 1 or 0 !!!\n");
    }
  } else {
    fprintf(stderr, "setMantissaBit: ERROR: Accessing an invalid bit!!!\n");
  }

  return result;
}

// Получить значения знака (+ или -)
int getSign(s21_decimal value) {
  unsigned int sign_bit = 31;
  int result = value.bits[3] >> sign_bit;
  /* if (result < 0) {
    result = -result;
  } */

  return result;
}

// Установить значения знака (+ или -)
bool setSign(s21_decimal *value, int sign) {
  bool result = false;
  const int sign_bit = 31;
  int mask = 1 << sign_bit;

  if (sign == 1) {
    value->bits[3] |= mask;
    result = true;
  } else if (sign == 0) {
    value->bits[3] &= ~mask;
    result = true;
  } else {
    fprintf(stderr, "setSign: ERROR: sign value can be only 1 or 0 !!!\n");
  }

  return result;
}

// Получить степень 10-ки
int getPower(s21_decimal value) {
  int result = -1;

  result = (char)(value.bits[3] >> 16);

  if (result > 28 || result < 0) {
    fprintf(stderr, "get_power: ERROR: value_power = %d. It is wrong!!!\n",
            result);
    result = -1;
  }

  return result;
}

// Установить степень 10-ки
bool setPower(s21_decimal *value_decimal, int value_power) {
  bool result = false;

  if (value_power >= 0 && value_power <= 28) {
    int temp = getSign(*value_decimal);
    value_decimal->bits[3] = 0;
    value_decimal->bits[3] = value_power << 16;
    setSign(value_decimal, temp);
    result = true;
  } else {
    fprintf(stderr,
            "set_power: ERROR: value_power = %d. It is wrong! Can be only from "
            "0 to 28!\n",
            value_power);
  }

  return result;
}

// Получение любого бита в s21_decimal
int getBit(s21_decimal value, int bit) {
  int result = -1;

  if (bit >= 0 && bit < (INT_BLOCK * INT_BITS)) {
    int mask = 1 << (bit % INT_BITS);
    result = (value.bits[bit / INT_BITS] & mask) >> (bit % INT_BITS);
    // самый первый бит даёт минус, надо от него избавится
    if (result < 0) {
      result = -result;
    }
  } else {
    fprintf(stderr, "get_bit: ERROR: Accessing an invalid bit!!!\n");
  }

  return result;
}

// Нормализация. Уравнивает степени 10-ки
// Нужна функция div, mod, roundBank, для ситуации с переполнением
int normalize(s21_decimal *value1, s21_decimal *value2) {
  int result = 1;
  int power1 = getPower(*value1);
  int power2 = getPower(*value2);
  int difference = 0;

  if ((value1 && value2) && power1 != -1 && power2 != -1) {
    s21_decimal *greater_power_decimal = NULL;
    s21_decimal *less_power_decimal = NULL;

    if (power1 > power2) {
      greater_power_decimal = value1;
      less_power_decimal = value2;
      difference = power1 - power2;
    } else {
      greater_power_decimal = value2;
      less_power_decimal = value1;
      difference = power2 - power1;
    }

    // int greater_power = getPower(*greater_power_decimal);
    int less_power = getPower(*less_power_decimal);

    for (int i = difference; i > 0; i--) {
      result = mulBy10(less_power_decimal);

      if (result == OK) {
        less_power++;
      } else {
        result = roundBank(greater_power_decimal, i);
        i = 0;
      }
    }

    // setPower(greater_power_decimal, greater_power);
    setPower(less_power_decimal, less_power);
  }

  return result;
}

// Последний бит в decimal
int lastBit(s21_decimal value) {
  int result = -1;
  int i = MANTISSA_BITS - 1;

  while (i >= 0 && getMantissaBit(value, i) != 1) {
    i--;
  }
  result = i;

  return result;
}

// Сдвиг decimal влево на offset бит
//  a << 1 == a * 2, а зничить возможно переполнение
int shiftLeft(s21_decimal *value, int offset) {
  int return_value = OK;
  int overflow = lastBit(*value) + offset;

  if (overflow < MANTISSA_BITS) {
    for (int i = 1; i <= offset; i++) {
      value->bits[2] <<= 1;
      setMantissaBit(value, 64, getMantissaBit(*value, 63));
      value->bits[1] <<= 1;
      setMantissaBit(value, 32, getMantissaBit(*value, 31));
      value->bits[0] <<= 1;
    }
  } else {
    return_value = EXCEEDING_THE_POSITIVE_TYPE_LIMIT;
  }

  return return_value;
}

// Простое сложение, где все числа положительные и степени 10-ки одинаковые
// Знаки и степени в этой функции у value1 и value2 должны быть одинаковые
int addSimple(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
  int return_value = OK;
  s21_decimal value3 = defaultDecimal();
  int temp = 0;
  int sum = 0;
  int last_bit_value1 = lastBit(value1);
  int last_bit_value2 = lastBit(value2);
  int max_last_bit =
      last_bit_value1 >= last_bit_value2 ? last_bit_value1 : last_bit_value2;

  for (int bit = 0; bit <= max_last_bit || temp == 1; bit++) {
    sum = getMantissaBit(value1, bit) + getMantissaBit(value2, bit) + temp;

    if (bit == MANTISSA_BITS - 1 && sum > 1) {
      return_value = EXCEEDING_THE_POSITIVE_TYPE_LIMIT;
      break;
    }

    temp = sum / 2;
    sum = sum % 2;
    setMantissaBit(&value3, bit, sum);
  }
  setSign(&value3, getSign(value1));
  setPower(&value3, getPower(value1));

  if (return_value == OK) {
    *result = value3;
  }

  return return_value;
}

// нет переполнения. лишний бит отбрасывается
void addSimpleForAdditionalCode(s21_decimal value1, s21_decimal value2,
                                s21_decimal *result) {
  s21_decimal value3 = defaultDecimal();
  int temp = 0;
  int sum = 0;

  // 96 бит включительно для знакового бита
  for (int bit = 0; bit <= MANTISSA_BITS; bit++) {
    if (bit < MANTISSA_BITS) {
      sum = getMantissaBit(value1, bit) + getMantissaBit(value2, bit) + temp;

      temp = sum / 2;
      sum = sum % 2;

      setMantissaBit(&value3, bit, sum);
    } else if (bit == MANTISSA_BITS) {
      sum = getSign(value1) + getSign(value2) + temp;
      sum = sum % 2;

      setSign(&value3, sum);
    }
  }

  setPower(&value3, getPower(value1));

  *result = value3;
}

// Умножение на 10
// a * 10 = a * 2^3 + a * 2^1 = a << 3 + a << 1
int mulBy10(s21_decimal *value) {
  int return_value = OK;
  s21_decimal temp1 = *value;
  s21_decimal temp2 = *value;

  return_value = shiftLeft(&temp1, 3);
  shiftLeft(&temp2, 1);

  if (return_value == OK) {
    return_value = addSimple(temp1, temp2, value);
  }

  return return_value;
}

// Дополнительный код
int additionalCode(s21_decimal *value) {
  int return_value = 0;

  if (getSign(*value) == 1 && lastBit(*value) != -1) {
    value->bits[2] = ~value->bits[2];
    value->bits[1] = ~value->bits[1];
    value->bits[0] = ~value->bits[0];

    s21_decimal one = defaultDecimal();
    s21_decimal result = defaultDecimal();
    one.bits[0] = 1;

    return_value = addSimple(*value, one, &result);
    *value = result;
  }

  return return_value;
}

// Простое деление. знаки +, степени одинаковые
int divSimple(s21_decimal divident, s21_decimal divisor, s21_decimal *quotient,
              s21_decimal *remainder) {
  int return_value = OK;

  if (divisor.bits[2] == 0 && divisor.bits[1] == 0 && divisor.bits[0] == 0) {
    return_value = DIVISION_BY_ZERO;
  } else {
    // обнуляем знак и степень. Знак +, степень 0
    divident.bits[3] = 0;
    divisor.bits[3] = 0;

    *quotient = defaultDecimal();

    while (s21_is_greater_or_equal(divident, divisor) && return_value == OK) {
      int i = 0;
      s21_decimal temp = divisor;
      int err = OK;

      while (s21_is_greater_or_equal(divident, temp) && err == OK) {
        i++;
        err = shiftLeft(&temp, 1);
      }
      temp = divisor;
      i--;
      shiftLeft(&temp, i);

      s21_decimal one_mul_2_in_power_i = defaultDecimal();
      one_mul_2_in_power_i.bits[0] = 1;
      shiftLeft(&one_mul_2_in_power_i, i);

      return_value = s21_add(*quotient, one_mul_2_in_power_i, quotient);

      if (return_value == OK) {
        return_value = s21_sub(divident, temp, &divident);
      }
    }
    if (remainder != NULL) {
      *remainder = divident;
    }
  }

  return return_value;
}

// Банковское округление
int roundBank(s21_decimal *value, int offset) {
  int return_value = OK;

  s21_decimal ten_in_offset = defaultDecimal();
  ten_in_offset.bits[0] = 1;
  for (int i = 1; i <= offset && return_value == OK; i++) {
    return_value = mulBy10(&ten_in_offset);
  }

  if (return_value == OK) {
    int new_power = getPower(*value) - offset;
    int sign = 0;
    if (getSign(*value) == 1) {
      sign = 1;
    }

    s21_decimal quotient = defaultDecimal();
    s21_decimal remainder = defaultDecimal();

    return_value = divSimple(*value, ten_in_offset, &quotient, &remainder);
    setPower(&remainder, offset);

    s21_decimal one = defaultDecimal();
    one.bits[0] = 1;

    s21_decimal two = defaultDecimal();
    two.bits[0] = 2;

    s21_decimal zero = defaultDecimal();
    s21_decimal zero_dot_six = defaultDecimal();
    s21_decimal zero_dot_five = defaultDecimal();
    zero_dot_six.bits[0] = 6;
    zero_dot_five.bits[0] = 5;
    setPower(&zero_dot_six, 1);
    setPower(&zero_dot_five, 1);

    s21_decimal temp_quotient = defaultDecimal();
    s21_decimal temp_remainder = defaultDecimal();
    divSimple(quotient, two, &temp_quotient, &temp_remainder);

    if (s21_is_equal(temp_remainder, zero)) {
      if (s21_is_less(remainder, zero_dot_six)) {
        *value = quotient;
      } else {
        addSimple(quotient, one, value);
      }
    } else {
      if (s21_is_less(remainder, zero_dot_five)) {
        *value = quotient;
      } else {
        addSimple(quotient, one, value);
      }
    }

    setPower(value, new_power);
    setSign(value, sign);
  }

  if (return_value != OK) {
    return_value = CALCULATION_ERROR;
  }

  return return_value;
}

// Вывод decimal на экран в двоичном виде
void displayDecimal(const s21_decimal *value) {
  bool flag = false;
  for (int i = (INT_BLOCK * INT_BITS) - 1; i >= 0; i--) {
    if (!flag) {
      printf("bits[%d]: ", i / INT_BITS);
      flag = true;
    }

    printf("%d", getBit(*value, i));

    if (i % 4 == 0) {
      putchar(' ');
    }

    if (i % INT_BITS == 0) {
      putchar('\n');
      flag = false;
    }
  }
  putchar('\n');
}

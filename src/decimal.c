#include "s21_decimal.h"

void clearDecimal(s21_decimal* value) {
  value->bits[0] = 0;
  value->bits[1] = 0;
  value->bits[2] = 0;
  value->bits[3] = 0;
}

// Функция получения масштаба числа
int getScale(const s21_decimal value) {
  const unsigned int scale_mask = 0xFF;  // предположим, что масштаб хранится в
                                         // младших 8 битах 4-го элемента
  return (value.bits[3] >> 16) &
         scale_mask;  // сдвиг на 16 бит вправо и применение маски
}

// Функция установки масштаба числа
void setScale(s21_decimal* value, unsigned int scale) {
  const unsigned int max_scale = (1 << 24) - 1;  // 24 бита для масштаба
  if (scale <= max_scale) {
    const unsigned int scale_position = 16;  // Позиция масштаба в битах
    const unsigned int scale_mask = max_scale << scale_position;
    value->bits[3] &= ~scale_mask;  // Очистка текущей области масштаба
    value->bits[3] |= (scale << scale_position);  // Установка нового масштаба
  } else {
    fprintf(stderr, "set_scale: ERROR: invalid scale value !!!\n");
  }
}

// Функция получения мантиссы числа
int64_t getMantissa(s21_decimal value) {
  int64_t mantissa = 0;
  for (int i = 0; i < 3; i++) {
    mantissa |= ((int64_t)value.bits[i]) << (i * 32);
  }
  return mantissa;
}

// Функция установки мантиссы числа
void setMantissa(s21_decimal* value, uint64_t mantissa) {
  for (int i = 0; i < 3; i++) {
    value->bits[i] = (int)(mantissa >> (i * 32));
  }
}

int s21_decimal_mul_by_10(s21_decimal* value) {
  if (value == NULL) {
    return 1;
  }

  uint64_t carry = 0;
  for (int i = 0; i < 4 - 1; i++) {
    uint64_t current = (uint64_t)value->bits[i] * 10 + carry;
    value->bits[i] = current & 0xFFFFFFFF;
    carry = current >> 32;
  }

  if (carry > 0) {
    return 1;
  }

  return 0;
}

int s21_is_zero(s21_decimal num) {
  int is_zero = 1;  // Предполагаем, что число равно нулю

  // Проверяем, есть ли ненулевые биты в значащей части числа
  for (int i = 0; i < 3; i++) {
    if (num.bits[i] != 0) {
      is_zero = 0;  // Нашли ненулевой бит, число не ноль
      break;
    }
  }

  return is_zero;
}

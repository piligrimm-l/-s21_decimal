#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRNUM_SIZE 30
//======================= ++ admiralm ++ ====================
#define INT_BLOCK 4
#define INT_BITS 32
#define MANTISSA_BITS 96
//======================== ++ admiralm ++ ===================
#define BIN_SIZE 32

#define BITS_IN_INT (CHAR_BIT * sizeof(int))  // размер типа int в битах
#define BITS_IN_MANTISSA \
  (3 * BITS_IN_INT)  // размер мантиссы числа decimal в битах
#define MAX_MANTISSA_ALLOWED 4294967295
#define BIG_DECIMAL_SIZE 8  // Пример размера для большей точности
#define BASE 10
#define SAFE_SCALE_DIFFERENCE 28

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  int bits[BIG_DECIMAL_SIZE];  // Массив для хранения мантиссы
  int scale;                   // Масштаб числа
  int sign;  // Знак числа: 0 - положительный, 1 - отрицательный
} s21_big_decimal;

// тип с возвращаемыми значениями для функций
typedef enum {
  OK,
  EXCEEDING_THE_POSITIVE_TYPE_LIMIT,
  EXCEEDING_THE_NEGATIVE_TYPE_LIMIT,
  DIVISION_BY_ZERO,
  INCORRECT_INPUT,
  FALSE = 0,
  TRUE,
  CONVERSION_ERROR = 1,
  CALCULATION_ERROR = 1
} e_return_value_t;

// тип для хранения числа (int) в двоичном виде
typedef struct {
  char binnum[BIN_SIZE];
} s_binary_t;

// тип для хранения числа (int) преобразованного в систему счисления с
// основанием 2^30
typedef struct {
  unsigned num[BITS_IN_MANTISSA];
} s_transformations_t;

// math
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// compare
int s21_compare(s21_decimal num1, s21_decimal num2);
int s21_is_equal(s21_decimal num1, s21_decimal num2);
int s21_is_not_equal(s21_decimal num1, s21_decimal num2);
int s21_is_less(s21_decimal num1, s21_decimal num2);
int s21_is_less_or_equal(s21_decimal num1, s21_decimal num2);
int s21_is_greater(s21_decimal num1, s21_decimal num2);
int s21_is_greater_or_equal(s21_decimal num1, s21_decimal num2);

// convert
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);

// others
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

//====================================  ++  admiralm  ++

// in the extra.c
// Значение По умолчанию Decimal = 0
s21_decimal defaultDecimal();

// Получить или Установить значения бита, по указанному номеру бита
int getMantissaBit(s21_decimal value, int bit);
bool setMantissaBit(s21_decimal *value_decimal, int bit, int value);

// Получить или Установить значения знака (+ или -)
int getSign(s21_decimal value);
bool setSign(s21_decimal *value, int sign);

// Получить или Установить степень 10-ки
int getPower(s21_decimal value);
bool setPower(s21_decimal *value_decimal, int value_power);

// Нормализация. Уравнивает степени 10-ки.
int normalize(s21_decimal *value1, s21_decimal *value2);

// Последний бит в decimal
int lastBit(s21_decimal value);

// Сдвиг decimal влево на offset бит
int shiftLeft(s21_decimal *value, int offset);

// Простое сложение, где все числа с одинаковым знаком
// Знаки и степени в этой функции у value1 и value2 должны быть одинаковые
int addSimple(s21_decimal value1, s21_decimal value2, s21_decimal *result);
void addSimpleForAdditionalCode(s21_decimal value1, s21_decimal value2,
                                s21_decimal *result);

// Умножение на 10
int mulBy10(s21_decimal *value);

// Простое деление. знаки +, степени одинаковые
int divSimple(s21_decimal divident, s21_decimal divisor, s21_decimal *quotient,
              s21_decimal *remainder);

// Дополнительный код
int additionalCode(s21_decimal *value);

// Банковское округление
int roundBank(s21_decimal *value, int offset);

// Функция нужна для s21_from_float_to_decimal()
int parseFloatString(char *str, int *power);

// Вывод decimal на экран в двоичном виде
void displayDecimal(const s21_decimal *value);
int getBit(s21_decimal value, int bit);

//============================  --  baraccol  --

void init_decimal(const char *strnum, s21_decimal *bignum);
char readbit(const s21_decimal *bignum, const size_t shift, const size_t width);
void writebit(s21_decimal *bignum, const unsigned bit, const size_t shift,
              const size_t width);
void print_decimal(const s21_decimal *bignum);
void strnum_to_binary(char *strnum, s21_decimal *bignum);
void strnum_div2(char *strnum);
char sign_read(const s21_decimal *bignum);
void sign_write(s21_decimal *bignum, const unsigned sign_bit);
char exponent_read(const s21_decimal *bignum);
void exponent_write(s21_decimal *bignum, const unsigned exponent);
size_t decimallen(void);
void reduction_of_denominators(s21_decimal *value_1, s21_decimal *value_2);
int multiplication_by_10(s21_decimal *bignum);
size_t begin_num(const s21_decimal *bignum);
int left_shift(s21_decimal *bignum, const size_t shift);
int add_mantissas(const s21_decimal value_1, const s21_decimal value_2,
                  s21_decimal *result);
int overflow_error(s21_decimal *bignum);

//====================================  ++  dejaelen  ++

void clearDecimal(s21_decimal *value);
int getScale(const s21_decimal value);
void setScale(s21_decimal *value, unsigned int scale);
int64_t getMantissa(s21_decimal value);
void setMantissa(s21_decimal *value, uint64_t mantissa);
int s21_decimal_mul_by_10(s21_decimal *value);
int s21_is_zero(s21_decimal num);

#endif  // S21_DECIMAL_H

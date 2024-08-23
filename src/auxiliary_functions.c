#include "s21_decimal.h"

// init_decimal() - парсит строку strnum: выделяет знак числа, а также порядок
// дробной части и записывает в структуру bignum, число введенное в виде строки
// strnum
void init_decimal(const char* strnum, s21_decimal* bignum) {
  char cp_strnum[STRNUM_SIZE] = "+00000000000000000000000000000";
  char max_strnum[STRNUM_SIZE] = "+79228162514264337593543950335";
  size_t decimal_point = 0;
  size_t bitslen = decimallen();
  for (size_t i = STRNUM_SIZE - 1, j = strlen(strnum); i > 0 && j > 0;
       --i, --j) {
    if (isdigit(strnum[j - 1])) {
      cp_strnum[i - 1] = strnum[j - 1];
    } else if (strnum[j - 1] == '.') {
      decimal_point = STRNUM_SIZE - i - 1;
      ++i;
    } else if (!(j - 1) && (strnum[j - 1] == '-' || strnum[j - 1] == '+')) {
      max_strnum[0] = cp_strnum[0] = strnum[0];
    } else {
      fprintf(stderr, "Error: Incorrect input\n");
      exit(INCORRECT_INPUT);
    }
  }
  if (strncmp(max_strnum, cp_strnum, STRNUM_SIZE - 1) < 0) {
    if (cp_strnum[0] == '-') {
      exit(EXCEEDING_THE_NEGATIVE_TYPE_LIMIT);
    } else if (cp_strnum[0] == '+') {
      exit(EXCEEDING_THE_POSITIVE_TYPE_LIMIT);
    }
  } else {
    bignum->bits[bitslen - 1] |= decimal_point << 16;
    if (cp_strnum[0] == '-') {
      bignum->bits[bitslen - 1] |= 1 << 31;
    } else if (cp_strnum[0] == '+') {
      bignum->bits[bitslen - 1] &= ~(1 << 31);
    }
    strnum_to_binary(cp_strnum, bignum);
  }
}

/*void decimal_to_strnum(char* strnum, const s21_decimal* bignum) {
  size_t bitslen = decimallen();
  size_t mantissa_len = (bitslen - 1) * BITS_IN_INT;
  for (size_t i = 0; i < mantissa_len; ++i) {
    read_bit(bignum, i, bitslen - 1);
    char  =
  }
}*/

// print_decimal() - выводит в stdout элементы массива bignum.bits[i], где i
// изменяется [0; 3], сначала в виде десятичных чисел bits[3], bits[2], bits[1],
// bits[0], а затем в двоичной системе счисления
void print_decimal(const s21_decimal* bignum) {
  size_t bitslen = decimallen();
  for (size_t i = bitslen; i != 0; --i) {
    printf("%u\n", bignum->bits[i - 1]);
  }
  for (size_t i = BITS_IN_INT * bitslen; i > 0; --i) {
    printf("%d", readbit(bignum, i - 1, bitslen));
    if ((i - 1) % BITS_IN_INT == 0) {
      putchar('\n');
    }
  }
}

// readbit() - cчитывает bit находящийся в позиции shift в числе bignum
// width количество элементов в массива bits[width] в структуре s21_decimal
// bignum задавая ширину вы можете считывать биты из нужного числа элементов
// bits[]
char readbit(const s21_decimal* bignum, const size_t shift,
             const size_t bitslen) {
  char bit = 0;
  if (shift < BITS_IN_INT * bitslen) {
    bit = (bignum->bits[shift / BITS_IN_INT] >> shift % BITS_IN_INT) & 1;
  } else {
    fprintf(stderr, "Error: incorrect input (0 <= shift < %lu)\n",
            BITS_IN_INT * bitslen);
    exit(INCORRECT_INPUT);
  }
  return bit;
}

// writebit() - заменяет бит, находящийся в позиции shift, в числе bignum,
// на число new_bit
void writebit(s21_decimal* bignum, const unsigned new_bit, const size_t shift,
              const size_t bitslen) {
  if (new_bit == 1 && shift < BITS_IN_INT * bitslen) {
    bignum->bits[shift / BITS_IN_INT] |= new_bit << (shift % BITS_IN_INT);
  } else if (new_bit == 0 && shift < BITS_IN_INT * bitslen) {
    bignum->bits[shift / BITS_IN_INT] &= ~(!new_bit << (shift % BITS_IN_INT));
  } else {
    fprintf(stderr, "Error: incorrect input\n");
    exit(INCORRECT_INPUT);
  }
}

// sign_read() - возвращает значение знака числа bignum: 1 = '-'; 0 = '+'
char sign_read(const s21_decimal* bignum) {
  size_t bitslen = decimallen();
  return bignum->bits[bitslen - 1] & (1 << 31);
}

// sign_write() - записывает знак '+' = 0 или '-' = 1 для числа bignum
void sign_write(s21_decimal* bignum, const unsigned sign_bit) {
  size_t bitslen = decimallen();
  writebit(bignum, sign_bit, BITS_IN_INT * bitslen - 1, bitslen);
}

// exponent_read() - возвращает значение показателя степени делителя числа
// bignum в десятичном виде
char exponent_read(const s21_decimal* bignum) {
  size_t bitslen = decimallen();
  char mask = 0b11111111;
  return (bignum->bits[bitslen - 1] >> 16) & mask;
}

// exponent_write() - записывает новое значение показателя степени делителя
// числа bignum
void exponent_write(s21_decimal* bignum, const unsigned exponent) {
  size_t bitslen = decimallen();
  unsigned mask = exponent << 16;
  bignum->bits[bitslen - 1] &= 1;
  bignum->bits[bitslen - 1] |= mask;
}

// decimallen() - возвращает количество чисел типа unsigned, используемых для
// записи числа s21_decimal, включая число хранящее порядок и знак bignum
size_t decimallen(void) { return sizeof(s21_decimal) / sizeof(unsigned); }

// reduction_of_denominators() - приведение чисел value_1 и value_2 к общему
// знаменателю (одинаковому показателю степени делителей)
/*void reduction_of_denominators(s21_decimal* value_1, s21_decimal* value_2) {
  size_t bitslen = decimallen();
  char exp_1 = exponent_read(value_1);
  char exp_2 = exponent_read(value_2);
  char diffexp = 0;
  if (exp_1 != exp_2) {
    diffexp = exp_1 - exp_2;
    while (diffexp) {
      if (diffexp < 0) {
        multiplication_by_10(&value_1);
        exponent_write(value_1, ++exp_1);
      } else {
        multiplication_by_10(&value_2);
        exponent_write(value_2, ++exp_2);
      }
    }
    //diffexp = exp_1 > exp_2 ? exp_1 - exp_2 : exp_2 - exp_1;
  }
}

int division_by_01(s21_decimal* bignum) {
  int result = OK;
  s21_decimal divisible = *bignum;
  s21_decimal divisor = {0};
  //init_decimal("0.1", &divisor);
  exponent_write(&divisor, 1);
  divisor.bits[0] = 1;

  return result;
}*/

// multiplication_by_10() - умножение мантиссы числа bignum на 10 и увеличение
// показателя делителя на 1
int multiplication_by_10(s21_decimal* bignum) {
  int result = OK;
  s21_decimal cp1_bignum = *bignum;
  s21_decimal cp2_bignum = *bignum;
  size_t shift = 3;
  if (left_shift(&cp1_bignum, shift) == OK) {
    shift = 1;
    left_shift(&cp2_bignum, shift);
    add_mantissas(cp1_bignum, cp2_bignum, bignum);
  } else {
    result = overflow_error(bignum);
  }
  return result;
}

int add_mantissas(const s21_decimal value_1, const s21_decimal value_2,
                  s21_decimal* bignum) {
  int result = OK;
  size_t bitslen = decimallen();
  size_t mantissa_len = (bitslen - 1) * BITS_IN_INT;
  char tmp = 0;
  char bit = 0;
  for (size_t i = 0; i < mantissa_len; ++i) {
    char bit_v1 = readbit(&value_1, i, bitslen - 1);
    char bit_v2 = readbit(&value_2, i, bitslen - 1);
    if ((!bit_v1 && !bit_v2) || (bit_v1 && bit_v2)) {
      bit = tmp;
      tmp = bit_v1;
    } else {
      bit = tmp ? 0 : 1;
      tmp = tmp ? 1 : 0;
    }
    writebit(bignum, bit, i, bitslen - 1);
  }
  if (tmp) {
    result = overflow_error(bignum);
  }
  return result;
}

int left_shift(s21_decimal* bignum, const size_t shift) {
  int result = OK;
  size_t bitslen = decimallen();
  size_t mantissa_len = (bitslen - 1) * BITS_IN_INT;
  size_t begin_bit = begin_num(bignum);
  if (begin_bit + shift < mantissa_len) {
    for (size_t i = begin_bit + 1; i > 0; --i) {
      char bit = readbit(bignum, i - 1, bitslen - 1);
      writebit(bignum, bit, i + shift - 1, bitslen - 1);
    }
    for (size_t i = shift; i > 0; --i) {
      writebit(bignum, 0, i - 1, bitslen - 1);
    }
  } else {
    result = overflow_error(bignum);
  }
  return result;
}

int overflow_error(s21_decimal* bignum) {
  int result = OK;
  if (!sign_read(bignum)) {
    result = EXCEEDING_THE_POSITIVE_TYPE_LIMIT;
  } else {
    result = EXCEEDING_THE_NEGATIVE_TYPE_LIMIT;
  }
  return result;
}

// begin_num() - возвращает местоположение первого не нулевого бита в мантиссе
// числа bignum
size_t begin_num(const s21_decimal* bignum) {
  size_t bitslen = decimallen();
  size_t shift = (bitslen - 1) * BITS_IN_INT;
  for (size_t i = shift; i > 0; --i) {
    if (readbit(bignum, i - 1, bitslen - 1)) {
      shift = i - 1;
      break;
    }
  }
  return shift;
}

// strnum_to_binary() - преобразует число записанное в strnum в двоичную систему
// и записывает его в bignum
void strnum_to_binary(char* strnum, s21_decimal* bignum) {
  size_t shift = 0;
  size_t width = sizeof(bignum->bits) / sizeof(unsigned);
  do {
    char num = (strnum[strlen(strnum) - 1] - '0') % 2;
    writebit(bignum, num, shift++, width - 1);
    strnum_div2(strnum);
  } while (shift < BITS_IN_INT * (width - 1));
}

// strnum_div2 - делит число записанное в strnum на 2
void strnum_div2(char* strnum) {
  char carry = 0;
  size_t begin = 1;
  for (size_t i = 1; i < strlen(strnum); ++i) {
    char tmp = 0;
    if (begin == 1) {
      while (strnum[i] == '0') {
        if (i < strlen(strnum) - 1) {
          ++i;
        } else {
          break;
        }
      }
      begin = 0;
    }
    if (strnum[i] + carry == '1') {
      strnum[i] = '0';
      if (i < strlen(strnum) - 1) {
        ++i;
        carry = 10;
      }
    }
    tmp = strnum[i];
    strnum[i] = (carry + strnum[i] - '0') / 2 + '0';
    carry = (carry + tmp - '0') % 2 ? 10 : 0;
  }
}

// decimal_to_basisNS переводит число из десятичной системы счисления в систему
// счисления с основанием basis
// number - число в десятичном виде
// transnum - структура в которой хранится массив типа unsigned в каждом
// элементе которого сохраняются цифры преобразованного числа basis - oснование
// системы счисления
void decimal_to_basisNS(unsigned number, s_transformations_t* transnum,
                        size_t basis) {
  for (int i = BITS_IN_MANTISSA - 1; i > -1 && number; --i) {
    if (basis) {
      transnum->num[i] = number % basis;
      number /= basis;
    }
  }
}

// print_transnum - выводит в stdout элементы массива num[] в структуре transnum
void print_transnum(const s_transformations_t* transnum) {
  for (size_t i = 0; i != BITS_IN_MANTISSA; ++i) {
    printf("%u", transnum->num[i]);
  }
  putchar('\n');
}

// zeroing_transnum - обнуляет элементы массива num[] в структуре transnum
void zeroing_transnum(s_transformations_t* transnum) {
  for (size_t i = 0; i != BITS_IN_MANTISSA; ++i) {
    transnum->num[i] = 0;
  }
}

// print_binnum - выводит в stdout число преобразованное функцией
// decimal_to_basisNS()
void print_binnum(const s_binary_t* binary) {
  for (size_t i = 0; i != BITS_IN_MANTISSA; ++i) {
    printf("%c", binary->binnum[i] > 9 ? binary->binnum[i] + 55
                                       : binary->binnum[i] + 48);
  }
  putchar('\n');
}

// decimal_to_2in30() - переводит число unsigned в систему счисления по
// основанию 2 в 30 степени
void decimal_to_2in30(unsigned number, s_transformations_t* transnum,
                      size_t basis) {
  for (int i = BITS_IN_MANTISSA / 3 - 1; i > -1 && number; --i) {
    if (basis) {
      transnum->num[i] = number % basis;
      number /= basis;
    }
  }
}

// char_to_binary() - выводит в stdout код символа в двоичной системе счисления
void char_to_binary(char ch) {
  for (int i = CHAR_BIT - 1; i > -1; --i) {
    int bit = (ch >> i) & 1;
    printf("%d", bit);
  }
  putchar('\n');
}

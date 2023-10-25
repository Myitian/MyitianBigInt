#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

namespace myitian
{
    static inline char __digit_i64(int64_t n)
    {
        if (n >= 100000)
            if (n >= 10000000)
                if (n >= 100000000)
                    return 9;
                else
                    return 8;
            else
                if (n >= 1000000)
                    return 7;
                else
                    return 6;
        else
            if (n >= 1000)
                if (n >= 10000)
                    return 5;
                else
                    return 4;
            else
                if (n >= 100)
                    return 3;
                else
                    if (n >= 10)
                        return 2;
                    else
                        return 1;
    }

    /// <summary>
    /// https://github.com/Myitian/MyitianBigInt
    /// </summary>
    typedef struct bigint
    {
        static const int64_t BIGINT_PART_CAP = 1000000000LL;
        static const int64_t BIGINT_ADD_CAP = BIGINT_PART_CAP * BIGINT_PART_CAP;
        static const int64_t BIGINT_PART_LIMIT = BIGINT_PART_CAP - 1;
        static const char BIGINT_CAP_DIGITS = 9;

        bool is_negative;
        std::vector<int64_t> number;

        bigint()
        {
            is_negative = false;
            number.push_back(0);
        }
        bigint(size_t size)
        {
            is_negative = false;
            number.push_back(0);
            number.resize(size);
        }
        bigint(uint64_t num, bool is_negative)
        {
            this->is_negative = is_negative;
            number.push_back(num % BIGINT_PART_CAP);
            num /= BIGINT_PART_CAP;
            number.push_back(num % BIGINT_PART_CAP);
            num /= BIGINT_PART_CAP;
            number.push_back(num);
        }
        bigint(std::string& digits, bool is_negative)
        {
            this->is_negative = is_negative;
            size_t digit_count = digits.size();
            size_t bigint_part_count = digit_count / BIGINT_CAP_DIGITS;
            char ext = (char)(digit_count % BIGINT_CAP_DIGITS);
            char digit_part_pos;
            if (ext)
            {
                bigint_part_count++;
                digit_part_pos = BIGINT_CAP_DIGITS - ext;
            }
            else
                digit_part_pos = 0;
            number.resize(bigint_part_count);
            int64_t tmp;
            size_t digit_pos = 0;
            size_t i = bigint_part_count;
            while (i-- > 0)
            {
                tmp = 0;
                while (digit_part_pos < BIGINT_CAP_DIGITS)
                {
                    tmp *= 10;
                    tmp += digits[digit_pos++] - '0';
                    digit_part_pos++;
                }
                number[i] = tmp;
                digit_part_pos = 0;
            }
        }
        bigint(std::vector<int64_t> number, bool is_negative)
        {
            this->is_negative = is_negative;
            this->number = number;
        }

        bigint operator-() const
        {
            return bigint(number, !is_negative);
        }
        bigint operator+() const
        {
            return *this;
        }
        bool operator==(bigint& that) const
        {
            return __private_equals(that);
        }
        bool operator!=(bigint& that) const
        {
            return !__private_equals(that);
        }
        bool operator<=(bigint& that) const
        {
            return !__private_greater_than(that);
        }
        bool operator>=(bigint& that) const
        {
            return !__private_less_than(that);
        }
        bool operator<(bigint& that) const
        {
            return __private_less_than(that);
        }
        bool operator>(bigint& that) const
        {
            return __private_greater_than(that);
        }
        bool operator==(int64_t that) const
        {
            return __private_equals(that);
        }
        bool operator!=(int64_t that) const
        {
            return !__private_equals(that);
        }
        bool operator<=(int64_t that) const
        {
            return !(*this > that);
        }
        bool operator>=(int64_t that) const
        {
            return !(*this < that);
        }
        bool operator<(int64_t that) const
        {
            if (is_negative != (that < 0))
                return is_negative;
            return is_negative
                ? __private_greater_than__ignore_sign(-that)
                : __private_less_than__ignore_sign(that);
        }
        bool operator>(int64_t that) const
        {
            if (is_negative != (that < 0))
                return !is_negative;
            return is_negative
                ? __private_less_than__ignore_sign(-that)
                : __private_greater_than__ignore_sign(that);
        }
        bigint& operator++()
        {
            return is_negative
                ? __private_sub_self_9digit__ignore_sign(1)
                : __private_add_self_18digit__ignore_sign(1);
        }
        bigint operator++(int)
        {
            bigint r = *this;
            ++*this;
            return r;
        }
        bigint& operator--()
        {
            return is_negative
                ? __private_add_self_18digit__ignore_sign(1)
                : __private_sub_self_9digit__ignore_sign(1);
        }
        bigint operator--(int)
        {
            bigint r = *this;
            --*this;
            return r;
        }
        bigint operator<<(size_t n) const
        {
            return +*this <<= n;
        }
        bigint& operator<<= (size_t n)
        {
            return __private_left_shift_self(n);
        }
        bigint operator>>(size_t n) const
        {
            return +*this >>= n;
        }
        bigint& operator>>=(size_t n)
        {
            return __private_right_shift_self(n);
        }
        bigint operator+(bigint& that) const
        {
            return +*this += that;
        }
        bigint operator+(int64_t that) const
        {
            return +*this += that;
        }
        bigint& operator+=(bigint& that)
        {
            return is_negative == that.is_negative
                ? __private_add_self__ignore_sign(that)
                : __private_sub_self__ignore_sign(that);
        }
        bigint& operator+=(int64_t that)
        {
            return is_negative == (that < 0)
                ? __private_add_self_18digit__ignore_sign(abs(that))
                : __private_sub_self_9digit__ignore_sign(abs(that));
        }
        bigint operator-(bigint& that) const
        {
            return +*this -= that;
        }
        bigint operator-(int64_t that) const
        {
            return +*this -= that;
        }
        bigint& operator-=(bigint& that)
        {
            return is_negative == that.is_negative
                ? __private_sub_self__ignore_sign(that)
                : __private_add_self__ignore_sign(that);
        }
        bigint& operator-=(int64_t that)
        {
            return is_negative == (that < 0)
                ? __private_sub_self_9digit__ignore_sign(abs(that))
                : __private_add_self_18digit__ignore_sign(abs(that));
        }
        bigint operator*(bigint& that) const
        {
            return +*this *= that;
        }
        bigint operator*(int64_t that) const
        {
            return +*this *= that;
        }
        bigint& operator*=(bigint& that)
        {
            return __private_mul_self(that);
        }
        bigint& operator*=(int64_t that)
        {
            return __private_mul_self_9digit(that);
        }
        bigint operator/(bigint& that) const
        {
            return +*this /= that;
        }
        bigint operator/(int64_t that) const
        {
            return +*this /= that;
        }
        bigint& operator/=(bigint& that)
        {
            return __private_div_self(that);
        }
        bigint& operator/=(int64_t that)
        {
            return __private_div_self_9digit(that);
        }
        bigint operator%(bigint& that) const
        {
            return +*this %= that;
        }
        bigint operator%(int64_t that) const
        {
            return +*this %= that;
        }
        bigint& operator%=(bigint& that)
        {
            return __private_mod_self(that);
        }
        bigint& operator%=(int64_t that)
        {
            return __private_mod_self_9digit(that);
        }

        bool is_error() const
        {
            return number.size() == 0;
        }
        bool is_zero() const
        {
            int64_t i = (int64_t)number.size();
            while (i-- > 0 && !number[i]);
            return i < 0;
        }
        void trim_leading_zero()
        {
            size_t i = number.size();
            while (i-- > 0 && !number[i])
                number.pop_back();
            if (number.size() == 0)
            {
                number.push_back(0);
                is_negative = false;
            }
        }
        size_t real_size() const
        {
            if (is_error())
                return 0;
            size_t i = number.size();
            while (i-- > 0 && !number[i]);
            return i < 0 ? 1 : i + 1;
        }
        void carry()
        {
            size_t this_size = real_size(), i = 0;
            int64_t c, n;
            while (i < this_size)
            {
                n = number[i];
                number[i] = n % BIGINT_PART_CAP;
                c = n / BIGINT_PART_CAP;
                i++;
                if (i == number.size())
                    number.push_back(c);
                else
                    number[i] += c;
            }
        }

        int fprint(FILE* stream) const
        {
            if (is_error())
            {
                return fprintf(stream, "ERROR!");
            }
            int len = 0;
            size_t num_size = number.size();
            bool print0 = true;
            if (is_negative)
            {
                putc('-', stream);
                len++;
            }
            int64_t n;
            size_t i = num_size;
            while (i-- > 0)
            {
                if ((n) = number[i])
                {
                    print0 = false;
                    len += fprintf(stream, "%lld", n);
                    break;
                }
            }
            if (print0)
            {
                putc('0', stream);
                len++;
            }
            else
            {
                while (i-- > 0)
                {
                    n = number[i];
                    char d_0 = BIGINT_CAP_DIGITS - __digit_i64(n);
                    while (d_0-- > 0)
                    {
                        putc('0', stream);
                        len++;
                    }
                    len += fprintf(stream, "%lld", n);
                }
            }
            return len;
        }
        int sprint(char* buffer) const
        {
            if (is_error())
            {
                return sprintf(buffer, "ERROR!");
            }
            int offset;
            char* start = buffer;
            size_t num_size = number.size();
            bool print0 = true;
            if (is_negative)
            {
                *buffer = '-';
                buffer++;
            }
            int64_t n;
            size_t i = num_size;
            while (i-- > 0)
            {
                if ((n) = number[i])
                {
                    print0 = false;
                    offset = sprintf(buffer, "%lld", n);
                    buffer += offset;
                    break;
                }
            }
            if (print0)
            {
                *buffer = '0';
                buffer++;
            }
            else
            {
                while (i-- > 0)
                {
                    n = number[i];
                    char d_0 = BIGINT_CAP_DIGITS - __digit_i64(n);
                    while (d_0-- > 0)
                    {
                        *buffer = '0';
                        buffer++;
                    }
                    offset = sprintf(buffer, "%lld", n);
                    buffer += offset;
                }
            }
            *buffer = 0;
            return (int)(buffer - start);
        }
        inline int print() const
        {
            return fprint(stdout);
        }

        void divrem(bigint& that, bigint& quotient, bigint& remainder) const
        {
            if (is_error() || that.is_error())
            {
                quotient.number.clear();
                remainder.number.clear();
                return;
            }
            if (that.real_size() == 1)
            {
                int64_t n = that.number[0];
                if (that.is_negative)
                {
                    n = -n;
                }
                divrem(n, quotient, remainder);
                return;
            }
            bool is_neg_orig = is_negative;
            bigint x_this = *this;
            x_this.is_negative = false;
            if (x_this < that)
            {
                quotient.re_init(0, false);
                remainder = *this;
                return;
            }
            x_this.is_negative = is_neg_orig;
            quotient = *this;
            quotient /= that;
            bigint tmp = quotient;
            tmp *= that;
            remainder = *this;
            remainder -= tmp;
            return;
        }
        void divrem(int64_t that, bigint& quotient, bigint& remainder) const
        {
            quotient = *this;
            quotient /= that;
            remainder = *this;
            remainder %= that;
            return;
        }
        int64_t divrem(int64_t that, bigint& quotient)
        {
            quotient = *this;
            quotient %= that;
            int64_t remainder;
            if (quotient.is_error())
            {
                remainder = INT64_MAX;
            }
            else
            {
                remainder = quotient.number[0];
            }
            if (is_negative)
            {
                remainder = -remainder;
            }
            quotient = *this;
            quotient /= that;
            return remainder;
        }

        void re_init(std::string& digits, bool is_negative)
        {
            this->is_negative = is_negative;
            size_t digit_count = digits.size();
            size_t bigint_part_count = digit_count / BIGINT_CAP_DIGITS;
            char ext = (char)(digit_count % BIGINT_CAP_DIGITS);
            char digit_part_pos;
            if (ext)
            {
                bigint_part_count++;
                digit_part_pos = BIGINT_CAP_DIGITS - ext;
            }
            else
                digit_part_pos = 0;
            number.resize(bigint_part_count);
            int64_t tmp;
            size_t digit_pos = 0;
            size_t i = bigint_part_count;
            while (i-- > 0)
            {
                tmp = 0;
                while (digit_part_pos < BIGINT_CAP_DIGITS)
                {
                    tmp *= 10;
                    tmp += digits[digit_pos++] - '0';
                    digit_part_pos++;
                }
                number[i] = tmp;
                digit_part_pos = 0;
            }
        }
        void re_init(uint64_t num, bool is_negative)
        {
            this->is_negative = is_negative;
            number.clear();
            number.push_back(num % BIGINT_PART_CAP);
            num /= BIGINT_PART_CAP;
            number.push_back(num % BIGINT_PART_CAP);
            num /= BIGINT_PART_CAP;
            number.push_back(num);
        }

        int fscanbigint(FILE* stream)
        {
            char c;
        SCAN_START:
            switch (c = getc(stream))
            {
            case EOF:
                return EOF;
            case '-':
                is_negative = true;
                break;
            case '+':
                is_negative = false;
                break;
            case '\r':
            case '\n':
            case '\t':
            case '\v':
            case ' ':
                goto SCAN_START;
            default:
                if (c >= '0' && c <= '9')
                {
                    ungetc(c, stream);
                    break;
                }
                else
                    return 0;
            }

            std::string digits = std::string();

            c = getc(stream);
            while (c >= '0' && c <= '9')
            {
                digits.push_back(c);
                c = getc(stream);
            }
            ungetc(c, stream);
            re_init(digits, is_negative);
            return c == EOF ? EOF : 1;
        }
        int sscanbigint(const char* str)
        {
            char c;
            int offset = 0;
        SCAN_START:
            switch (c = str[offset++])
            {
            case '\0':
                return EOF;
            case '-':
                is_negative = true;
                break;
            case '+':
                is_negative = false;
                break;
            case '\r':
            case '\n':
            case '\t':
            case '\v':
            case ' ':
                goto SCAN_START;
            default:
                if (c >= '0' && c <= '9')
                {
                    offset--;
                    break;
                }
                else
                    return 0;
            }

            std::string digits = std::string();

            c = str[offset++];
            while (c >= '0' && c <= '9')
            {
                digits.push_back(c);
                c = str[offset++];
            }
            re_init(digits, is_negative);
            return c ? 1 : EOF;
        }
        int scanbigint()
        {
            return fscanbigint(stdin);
        }

        static int fscanbigint(bigint& num, FILE* stream)
        {
            return num.fscanbigint(stream);
        }
        static int sscanbigint(bigint& num, const char* str)
        {
            return num.sscanbigint(str);
        }
        static int scanbigint(bigint& num)
        {
            return num.scanbigint();
        }

    private:
        bool __private_equals(bigint& that) const
        {
            size_t this_size = real_size();
            size_t that_size = that.real_size();
            size_t i = this_size, j = that_size;
            if (i != j)
                return false;
            if (is_zero() && that.is_zero())
                return true;
            if (is_negative != that.is_negative)
                return false;
            while (i-- > 0)
                if (number[i] != that.number[i])
                    return false;
            return true;
        }
        bool __private_equals(int64_t that) const
        {
            if (is_error())
                return false;
            if (is_negative && that > 0)
                return false;
            if (!is_negative && that < 0)
                return false;

            size_t this_size = real_size();
            if (this_size > 3)
                return false;
            if (number[2] > 9)
                return false;
            if (number[2] == 9)
            {
                if (number[1] > 223372036)
                    return false;
                if (number[1] == 223372036)
                {
                    if (number[1] > 854775808)
                        return false;
                    if (!is_negative && number[1] == 854775808)
                        return false;
                }
            }
            int64_t n = number[2] * BIGINT_ADD_CAP + number[1] * BIGINT_PART_CAP;
            if (is_negative)
            {
                n = -n;
                n -= number[0];
            }
            else
                n += number[0];
            return n == that;
        }
        bool __private_less_than(bigint& that) const
        {
            size_t this_size = real_size();
            size_t that_size = that.real_size();
            if (is_zero() && that.is_zero())
                return false;
            if (is_negative != that.is_negative)
                return is_negative;
            if (this_size != that_size)
                return this_size < that_size;
            size_t i = this_size;
            while (i-- > 0)
                if (number[i] != that.number[i])
                    return number[i] < that.number[i];
            return false;
        }
        bool __private_less_than__ignore_sign(uint64_t that) const
        {
            if (is_error())
                return false;

            size_t i = real_size();
            if (i > 3)
                return false;
            uint64_t n = 0;
            switch (i)
            {
            case 3:
                if (number[2] > 18)
                    return false;
                if (number[2] == 18)
                {
                    if (number[1] > 446744073)
                        return false;
                    if (number[1] == 446744073 && number[1] >= 709551615)
                        return false;
                }
                n = (uint64_t)number[2] * BIGINT_ADD_CAP;
            case 2:
                n += number[1] * BIGINT_PART_CAP;
            case 1:
                n += number[0];
                break;
            }
            return n < that;
        }
        bool __private_greater_than(bigint& that) const
        {
            size_t this_size = real_size();
            size_t that_size = that.real_size();
            if (is_zero() && that.is_zero())
                return false;
            if (is_negative != that.is_negative)
                return that.is_negative;
            if (this_size != that_size)
                return this_size > that_size;
            size_t i = this_size;
            while (i-- > 0)
                if (number[i] != that.number[i])
                    return number[i] > that.number[i];
            return false;
        }
        bool __private_greater_than__ignore_sign(uint64_t that) const
        {
            if (is_error())
                return false;

            size_t i = real_size();
            if (i > 3)
                return true;
            uint64_t n = 0;
            switch (i)
            {
            case 3:
                if (number[2] > 18)
                    return true;
                if (number[2] == 18)
                {
                    if (number[1] > 446744073)
                        return true;
                    if (number[1] == 446744073 && number[0] > 709551615)
                        return true;
                }
                n = (uint64_t)number[2] * BIGINT_ADD_CAP;
            case 2:
                n += number[1] * BIGINT_PART_CAP;
            case 1:
                n += number[0];
                break;
            }
            return n < that;
        }
        bigint& __private_left_shift_self(size_t n)
        {
            if (is_error())
                return *this;
            size_t i = real_size();
            number.resize(i + n);
            while (i-- > 0)
                number[i + n] = number[i];
            i = n;
            while (i-- > 0)
                number[i] = 0;
            return *this;
        }
        bigint& __private_right_shift_self(size_t n)
        {
            if (is_error())
                return *this;
            size_t this_size = real_size();
            size_t this_size_mn = this_size - n, i = 0;
            for (; i < this_size_mn; i++)
                number[i] = number[i + n];
            number.resize(i - n + 1);
            return *this;
        }
        bigint& __private_add_self__ignore_sign(bigint& that)
        {
            if (is_error() || that.is_error())
            {
                number.clear();
                return *this;
            }
            size_t that_size = that.number.size();

            int64_t c = 0, n;
            size_t i = 0;
            while (i < that_size)
            {
                n = number[i] + that.number[i];
                c = n / BIGINT_PART_CAP;
                number[i] = n % BIGINT_PART_CAP;
                i++;
                if (i == number.size())
                    number.push_back(c);
                else
                    number[i] += c;
            }
            n = number[i];
            c = n / BIGINT_PART_CAP;
            while (c)
            {
                number[i] = n % BIGINT_PART_CAP;
                i++;
                if (i == number.size())
                {
                    number.push_back(c);
                    break;
                }
                number[i] += c;
                n = number[i];
                c = n / BIGINT_PART_CAP;
            }
            return *this;
        }
        bigint& __private_add_self_18digit__ignore_sign(int64_t that)
        {
            if (is_error())
                return *this;
            that %= BIGINT_ADD_CAP;

            int64_t c = 0, n;
            size_t i = 0;
            n = number[i] + that;
            c = n / BIGINT_PART_CAP;
            number[i] = n % BIGINT_PART_CAP;
            i++;
            if (i == number.size())
            {
                number.push_back(c);
                return *this;
            }
            number[i] += c;
            n = number[i];
            c = n / BIGINT_PART_CAP;
            while (c)
            {
                number[i] = n % BIGINT_PART_CAP;
                i++;
                if (i == number.size())
                {
                    number.push_back(c);
                    break;
                }
                number[i] += c;
                n = number[i];
                c = n / BIGINT_PART_CAP;
            }
            return *this;
        }
        bigint& __private_sub_self__ignore_sign(bigint& that)
        {
            if (is_error() || that.is_error())
            {
                number.clear();
                return *this;
            }
            if (__private_equals(that))
            {
                number.clear();
                number.push_back(0);
                is_negative = false;
                return *this;
            }
            size_t that_size = that.number.size();
            size_t this_size = number.size();

            bool step2 = this_size > that_size;

            int64_t n;
            size_t i = 0, ip1 = 1;
            while (i < that_size)
            {
                if (ip1 == number.size())
                    number.push_back(0);
                n = number[i] - that.number[i];
                if (n < 0)
                {
                    number[i] = BIGINT_PART_CAP + n;
                    number[ip1]--;
                }
                else
                    number[i] = n;
                i++;
                ip1++;
            }
            if (step2)
            {
                while (i < this_size)
                {
                    if (ip1 == number.size())
                        number.push_back(0);
                    n = number[i];
                    if (n < 0)
                    {
                        number[i] = BIGINT_PART_CAP + n;
                        number[ip1]--;
                    }
                    i++;
                    ip1++;
                }
            }
            if (number[i] == -1)
            {
                number[i] = 0;
                while (i-- > 0)
                    number[i] = BIGINT_PART_LIMIT - number[i];
                ++*this;
                is_negative = !is_negative;
            }
            return *this;
        }
        bigint& __private_sub_self_9digit__ignore_sign(int64_t that)
        {
            if (is_error())
                return *this;
            that %= BIGINT_PART_CAP;

            size_t this_size = number.size();

            int64_t n;
            if (number.size() == 1)
                number.push_back(0);
            n = number[0] - that;
            if (n < 0)
            {
                number[0] = BIGINT_PART_CAP + n;
                number[1]--;
            }
            else
                number[0] = n;
            size_t i = 1, ip1 = 2;
            while (i < this_size)
            {
                if (ip1 == number.size())
                    number.push_back(0);
                n = number[i];
                if (n < 0)
                {
                    number[i] = BIGINT_PART_CAP + n;
                    number[ip1]--;
                }
                i++;
                ip1++;
            }
            if (number[i] == -1)
            {
                number[i] = 0;
                while (i-- > 0)
                    number[i] = BIGINT_PART_LIMIT - number[i];
                ++*this;
                is_negative = !is_negative;
            }
            return *this;
        }
        bigint& __private_mul_self(bigint& that)
        {
            if (is_error() || that.is_error())
            {
                number.clear();
                return *this;
            }
            if (is_zero() || that.is_zero())
            {
                number.clear();
                number.push_back(0);
                is_negative = false;
                return *this;
            }
            bigint tmp = *this;
            number.clear();
            size_t this_size = tmp.real_size();
            size_t that_size = that.real_size();
            is_negative = is_negative != that.is_negative;
            size_t i, j;
            for (i = 0; i < that_size; i++)
            {
                int64_t n2 = that.number[i];
                if (n2 == 0)
                    continue;
                for (j = 0; j < this_size; j++)
                {
                    int64_t n1 = tmp.number[j];
                    if (n1 == 0)
                        continue;
                    if (i + j == number.size())
                        number.push_back(0);
                    number[i + j] += n1 * n2;
                }
                carry();
            }
            return *this;
        }
        bigint& __private_mul_self_9digit(int64_t that)
        {
            if (is_error())
                return *this;
            if (that == BIGINT_PART_CAP)
                return __private_left_shift_self(1);
            that %= BIGINT_PART_CAP;
            if (that == 0)
            {
                number.clear();
                number.push_back(0);
                is_negative = false;
                return *this;
            }
            if (is_zero())
            {
                is_negative = false;
                return *this;
            }
            if (that < 0)
            {
                that = -that;
                is_negative = !is_negative;
            }
            if (that == 1)
                return *this;

            size_t this_size = real_size(), i = 0;
            while (i < this_size)
                number[i++] *= that;
            carry();
            return *this;
        }
        bigint& __private_div_self(bigint& that)
        {
            if (is_error() || that.is_error() || that.is_zero())
            {
                number.clear();
                return *this;
            }
            if (that.real_size() == 1)
                return __private_div_self_9digit(that.is_negative ? -that.number[0] : that.number[0]);
            bool is_neg_orig = is_negative != that.is_negative;
            is_negative = false;
            std::vector<bigint> num_mul_2pow = { that.is_negative ? -that : that };
            if (*this < num_mul_2pow[0])
            {
                is_negative = false;
                number.clear();
                number.push_back(0);
                return *this;
            }

            std::vector<bigint> _2pow = { bigint(1, false) };
            bigint tmp;
            int i = 0;
            while (!__private_less_than(num_mul_2pow[i])) // num_mul_2pow[i] <= *this
            {
                tmp = _2pow[i];
                tmp *= 2;
                _2pow.push_back(tmp);
                tmp = num_mul_2pow[i];
                tmp *= 2;
                num_mul_2pow.push_back(tmp);
                i++;
            }
            tmp = *this;
            re_init(0, false);
            while (i-- > 0)
            {
                if (num_mul_2pow[i] <= tmp)
                {
                    tmp -= num_mul_2pow[i];
                    __private_add_self__ignore_sign(_2pow[i]);
                }
            }
            is_negative = is_neg_orig;
            return *this;
        }
        bigint& __private_div_self_9digit(int64_t that)
        {
            if (is_error())
                return *this;
            if (that == BIGINT_PART_CAP)
                return __private_right_shift_self(1);
            that %= BIGINT_PART_CAP;
            if (that == 0)
            {
                number.clear();
                is_negative = false;
                return *this;
            }
            if (is_zero())
            {
                is_negative = false;
                return *this;
            }
            if (that < 0)
            {
                that = -that;
                is_negative = !is_negative;
            }
            if (that == 1)
                return *this;

            size_t i = real_size();
            int64_t n, c = 0;
            while (i-- > 0)
            {
                n = number[i] + c * BIGINT_PART_CAP;
                c = n % that;
                number[i] = n / that;
            }
            return *this;
        }
        bigint& __private_mod_self(bigint& that)
        {
            if (is_error() || that.is_error())
            {
                number.clear();
                return *this;
            }
            if (that.real_size() == 1)
                return __private_mod_self_9digit(that.number[0]);
            bool is_neg_orig = is_negative;
            is_negative = false;
            if (*this < that)
                return *this;
            bigint tmp = *this;
            tmp /= that;
            tmp *= that;
            __private_sub_self__ignore_sign(tmp);
            is_negative = is_neg_orig;
            return *this;
        }
        bigint& __private_mod_self_9digit(int64_t that)
        {
            if (is_error())
                return *this;
            int64_t m;
            if (that == BIGINT_PART_CAP)
                m = number[0];
            else
            {
                that %= BIGINT_PART_CAP;
                if (that == 0)
                {
                    number.clear();
                    return *this;
                }
                bool is_neg_orig = is_negative;
                is_negative = false;
                if (*this < that)
                    return *this;
                bigint tmp = *this;
                tmp /= that;
                tmp *= that;
                __private_sub_self__ignore_sign(tmp);
                is_negative = is_neg_orig;
                m = number[0];
            }
            number.clear();
            number.push_back(m);
            return *this;
        }
    } bigint;
}
int main()
{
    myitian::bigint x, y, q, m;

    printf("x = ");
    x.scanbigint();

    printf("y = ");
    y.scanbigint();

    printf("x + y = ");
    (x + y).print();
    putchar('\n');

    printf("x - y = ");
    (x - y).print();
    putchar('\n');

    printf("x * y = ");
    (x * y).print();
    putchar('\n');

    printf("x / y = ");
    (x / y).print();
    putchar('\n');

    printf("x %% y = ");
    (x % y).print();
    putchar('\n');

    printf("divrem\n");
    x.divrem(y, q, m);
    q.print();
    putchar('\n');
    m.print();
    putchar('\n');

    printf("x++\n");
    x++.print();
    putchar('\n');
    x.print();
    putchar('\n');

    printf("++x\n");
    (++x).print();
    putchar('\n');

    printf("y--\n");
    y--.print();
    putchar('\n');
    y.print();
    putchar('\n');

    printf("--y\n");
    (--y).print();
    putchar('\n');
}
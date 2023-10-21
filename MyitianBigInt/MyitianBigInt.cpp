#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

namespace myitian
{
    static inline char __digit_ll(long long n)
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

    typedef struct bigint
    {
        static const long long BIGINT_PART_CAP = 1000000000LL;
        static const long long BIGINT_ADD_CAP = BIGINT_PART_CAP * BIGINT_PART_CAP;
        static const long long BIGINT_PART_LIMIT = BIGINT_PART_CAP - 1;
        static const char BIGINT_CAP_DIGITS = 9;

        bool is_negative;
        std::vector<long long> number;

        bigint()
        {
            is_negative = false;
        }
        bigint(size_t size)
        {
            is_negative = false;
            number.resize(size);
        }
        bigint(unsigned long long num, bool is_negative)
        {
            this->is_negative = is_negative;
            number.push_back(num % BIGINT_PART_CAP);
            num /= BIGINT_PART_CAP;
            number.push_back(num % BIGINT_PART_CAP);
            num /= BIGINT_PART_CAP;
            number.push_back(num);
        }

        bool operator==(bigint& that)
        {
            return __private_equals(that);
        }
        bool operator!=(bigint& that)
        {
            return !__private_equals(that);
        }
        bigint& operator++()
        {
            return is_negative
                ? __private_sub_self_9digit(1)
                : __private_add_self_18digit(1);
        }
        bigint& operator--()
        {
            return is_negative
                ? __private_add_self_18digit(1)
                : __private_sub_self_9digit(1);
        }
        bigint& operator+=(bigint& that)
        {
            return is_negative == that.is_negative
                ? __private_add_self(that)
                : __private_sub_self(that);
        }
        bigint& operator+=(long long that)
        {
            return is_negative == (that < 0)
                ? __private_add_self_18digit(abs(that))
                : __private_sub_self_9digit(abs(that));
        }
        bigint& operator-=(bigint& that)
        {
            return is_negative == that.is_negative
                ? __private_sub_self(that)
                : __private_add_self(that);
        }
        bigint& operator-=(long long that)
        {
            return is_negative == (that < 0)
                ? __private_sub_self_9digit(abs(that))
                : __private_add_self_18digit(abs(that));
        }
        bigint& operator*=(bigint& that)
        {
            return __private_mul_self(that);
        }
        bigint& operator*=(long long that)
        {
            return __private_mul_self_9digit(that);
        }

        bool is_zero()
        {
            size_t i = number.size();
            while (i-- > 0 && !number[i]);
            return i < 0;
        }
        void trim_leading_zero()
        {
            size_t i = number.size();
            while (i-- > 0 && !number[i])
            {
                number.pop_back();
            }
            if (number.size() == 0)
            {
                number.push_back(0);
                is_negative = false;
            }
        }
        size_t real_size()
        {
            size_t i = number.size();
            while (i-- > 0 && !number[i]);
            return i < 0 ? 1 : i + 1;
        }
        void carry()
        {
            size_t this_size = real_size(), i = 0;
            long long c, n;
            while (i < this_size)
            {
                n = number[i];
                number[i] = n % BIGINT_PART_CAP;
                c = n / BIGINT_PART_CAP;
                i++;
                if (i == number.size())
                {
                    number.push_back(c);
                }
                else
                {
                    number[i] += c;
                }
            }
        }

        int fprint(FILE* stream) const
        {
            int len = 0;
            size_t num_size = number.size();
            bool print0 = true;
            if (is_negative)
            {
                putc('-', stream);
                len++;
            }
            long long n;
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
                    char d_0 = BIGINT_CAP_DIGITS - __digit_ll(n);
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
            int offset;
            char* start = buffer;
            size_t num_size = number.size();
            bool print0 = true;
            if (is_negative)
            {
                *buffer = '-';
                buffer++;
            }
            long long n;
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
                    char d_0 = BIGINT_CAP_DIGITS - __digit_ll(n);
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
            return buffer - start;
        }
        inline int print() const
        {
            return fprint(stdout);
        }

        void re_init(unsigned long long num, bool is_negative)
        {
            this->is_negative = is_negative;
            number.clear();
            number.push_back(num % BIGINT_PART_CAP);
            num /= BIGINT_PART_CAP;
            number.push_back(num % BIGINT_PART_CAP);
            num /= BIGINT_PART_CAP;
            number.push_back(num);
        }

        void fscanbigint(FILE* stream)
        {
            char c;
        SCAN_START:
            switch (c = getc(stream))
            {
            case '-':
                is_negative = true;
                break;
            case '+':
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
                    return;
            }

            std::string digits = std::string();

            c = getc(stream);
            while (c >= '0' && c <= '9')
            {
                digits.push_back(c);
                c = getc(stream);
            }
            ungetc(c, stream);
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
            {
                digit_part_pos = 0;
            }
            number.resize(bigint_part_count);
            long long tmp;
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
        inline void scanbigint()
        {
            fscanbigint(stdin);
        }

        static void fscanbigint(bigint& num, FILE* stream)
        {
            num.fscanbigint(stream);
        }
        static inline void scanbigint(bigint& num)
        {
            num.scanbigint();
        }
    private:
        bool __private_equals(bigint& that)
        {
            size_t this_size = number.size();
            size_t that_size = that.number.size();
            size_t i = this_size, j = that_size;
            while (i-- > 0 && !number[i]);
            while (j-- > 0 && !that.number[j]);
            if (i != j)
                return false;
            if (i < 0)
                return true;
            if (is_negative != that.is_negative)
                return false;
            i++;
            while (i-- > 0)
                if (number[i] != that.number[i])
                    return false;
            return true;
        }
        bigint& __private_add_self(bigint& that)
        {
            size_t that_size = that.number.size();

            long long c = 0, n;
            size_t i = 0;
            while (i < that_size)
            {
                n = number[i] + that.number[i];
                c = n / BIGINT_PART_CAP;
                number[i] = n % BIGINT_PART_CAP;
                i++;
                if (i == number.size())
                {
                    number.push_back(c);
                }
                else
                {
                    number[i] += c;
                }
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
        bigint& __private_add_self_18digit(long long that)
        {
            that %= BIGINT_ADD_CAP;

            long long c = 0, n;
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
        bigint& __private_sub_self(bigint& that)
        {
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

            long long n;
            size_t i = 0, ip1 = 1;
            while (i < that_size)
            {
                if (ip1 == number.size())
                {
                    number.push_back(0);
                }
                n = number[i] - that.number[i];
                if (n < 0)
                {
                    number[i] = BIGINT_PART_CAP + n;
                    number[ip1]--;
                }
                else
                {
                    number[i] = n;
                }
                i++;
                ip1++;
            }
            if (step2)
            {
                while (i < this_size)
                {
                    if (ip1 == number.size())
                    {
                        number.push_back(0);
                    }
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
                {
                    number[i] = BIGINT_PART_LIMIT - number[i];
                }
                __private_add_self_18digit(1);
                is_negative = !is_negative;
            }
            return *this;
        }
        bigint& __private_sub_self_9digit(long long that)
        {
            that %= BIGINT_PART_CAP;

            size_t this_size = number.size();

            long long n;
            if (number.size() == 1)
            {
                number.push_back(0);
            }
            n = number[0] - that;
            if (n < 0)
            {
                number[0] = BIGINT_PART_CAP + n;
                number[1]--;
            }
            else
            {
                number[0] = n;
            }
            size_t i = 1, ip1 = 2;
            while (i < this_size)
            {
                if (ip1 == number.size())
                {
                    number.push_back(0);
                }
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
                {
                    number[i] = BIGINT_PART_LIMIT - number[i];
                }
                __private_add_self_18digit(1);
                is_negative = !is_negative;
            }
            return *this;
        }
        bigint& __private_mul_self(bigint& that)
        {
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
                long long n2 = that.number[i];
                if (n2 == 0)
                    continue;
                for (j = 0; j < this_size; j++)
                {
                    long long n1 = tmp.number[j];
                    if (n1 == 0)
                        continue;
                    if (i + j == number.size())
                    {
                        number.push_back(0);
                    }
                    number[i + j] += n1 * n2;
                }
                carry();
            }
            return *this;
        }
        bigint& __private_mul_self_9digit(long long that)
        {
            if (that == 0)
            {
                number.clear();
                number.push_back(0);
                is_negative = false;
                return *this;
            }
            if (is_zero())
            {
                return *this;
            }
            if (that < 0)
            {
                that = -that;
                is_negative = !is_negative;
            }
            that %= BIGINT_PART_CAP;

            size_t this_size = number.size(), i = 0;
            while (i < this_size)
            {
                number[i++] *= that;
            }
            carry();
            return *this;
        }

    } bigint;
}
char getnwc()
{
    char c;
L:
    switch (c = getchar())
    {
    case ' ':
    case '\r':
    case '\n':
    case '\t':
    case '\v':
        goto L;
    default:
        return c;
    }
}
int main()
{
    char d, b[2570];
    int n, x, c;
    scanf("%d", &n);
    myitian::bigint m(286);
    while (n-->0)
    {
        c = 0;
        scanf("%d %c", &x, &d);
        m.re_init(x, false);
        while (x-->1)
            m *= x;
        x = m.sprint(b);
        while (x-->0)
            if (b[x] == d)
                c++;
        printf("%d\n", c);
    }
}
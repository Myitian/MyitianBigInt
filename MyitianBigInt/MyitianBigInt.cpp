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

        bool negative;
        std::vector<long long> number;

        bigint()
        {
            negative = false;
            number = std::vector<long long>();
        }
        bigint(size_t size)
        {
            negative = false;
            number = std::vector<long long>(size);
        }

        inline bool operator==(bigint& that)
        {
            return __private_equals(that);
        }
        inline bool operator!=(bigint& that)
        {
            return !__private_equals(that);
        }
        inline bigint& operator++()
        {
            return this->negative
                ? __private_sub_self_9digit(1)
                : __private_add_self_18digit(1);
        }
        inline bigint& operator--()
        {
            return this->negative
                ? __private_add_self_18digit(1)
                : __private_sub_self_9digit(1);
        }
        bigint& operator+=(bigint& that)
        {
            if (this->negative == that.negative)
                return __private_add_self(that);
            else
                return __private_sub_self(that);
        }
        bigint& operator+=(long long that)
        {
            if (this->negative == (that < 0))
                return __private_add_self_18digit(abs(that));
            else
                return __private_sub_self_9digit(abs(that));
        }
        bigint& operator-=(bigint& that)
        {
            if (this->negative == that.negative)
                return __private_sub_self(that);
            else
                return __private_add_self(that);
        }
        bigint& operator-=(long long that)
        {
            if (this->negative == (that < 0))
                return __private_sub_self_9digit(abs(that));
            else
                return __private_add_self_18digit(abs(that));
        }

        bool iszero()
        {
            size_t i = this->number.size();
            while (i-- > 0 && !this->number[i]);
            return i < 0;
        }

        void fprint(FILE* stream) const
        {
            size_t num_size = this->number.size();
            bool print0 = true;
            if (negative)
            {
                putc('-', stream);
            }
            long long n;
            size_t i = num_size;
            while (i-- > 0)
            {
                if ((n) = this->number[i])
                {
                    print0 = false;
                    fprintf(stream, "%lld", n);
                    break;
                }
            }
            if (print0)
            {
                putc('0', stream);
            }
            else
            {
                while (i-- > 0)
                {
                    n = this->number[i];
                    char d_0 = BIGINT_CAP_DIGITS - __digit_ll(n);
                    while (d_0-- > 0)
                    {
                        putc('0', stream);
                    }
                    fprintf(stream, "%lld", n);
                }
            }
        }
        inline void print() const
        {
            fprint(stdout);
        }

        static void fscanbigint(bigint& num, FILE* stream)
        {
            char c;
        SCAN_START:
            switch (c = getc(stream))
            {
            case '-':
                num.negative = true;
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
            num.number.resize(bigint_part_count);
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
                num.number[i] = tmp;
                digit_part_pos = 0;
            }

        }
        static inline void scanbigint(bigint& num)
        {
            fscanbigint(num, stdin);
        }
    private:
        bool __private_equals(bigint& that)
        {
            size_t this_size = this->number.size();
            size_t that_size = that.number.size();
            size_t i = this_size, j = that_size;
            while (i-- > 0 && !this->number[i]);
            while (j-- > 0 && !that.number[j]);
            if (i != j)
                return false;
            if (i < 0)
                return true;
            if (this->negative != that.negative)
                return false;
            i++;
            while (i-- > 0)
                if (this->number[i] != that.number[i])
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
                n = this->number[i] + that.number[i];
                c = n / BIGINT_PART_CAP;
                this->number[i] = n % BIGINT_PART_CAP;
                i++;
                if (i == this->number.size())
                {
                    this->number.push_back(c);
                }
                else
                {
                    this->number[i] += c;
                }
            }
            n = this->number[i];
            c = n / BIGINT_PART_CAP;
            while (c)
            {
                this->number[i] = n % BIGINT_PART_CAP;
                i++;
                if (i == this->number.size())
                {
                    this->number.push_back(c);
                    break;
                }
                this->number[i] += c;
                n = this->number[i];
                c = n / BIGINT_PART_CAP;
            }
            return *this;
        }
        bigint& __private_add_self_18digit(long long that)
        {
            that %= BIGINT_ADD_CAP;

            long long c = 0, n;
            size_t i = 0;
            n = this->number[i] + that;
            c = n / BIGINT_PART_CAP;
            this->number[i] = n % BIGINT_PART_CAP;
            i++;
            if (i == this->number.size())
            {
                this->number.push_back(c);
                return *this;
            }
            this->number[i] += c;
            n = this->number[i];
            c = n / BIGINT_PART_CAP;
            while (c)
            {
                this->number[i] = n % BIGINT_PART_CAP;
                i++;
                if (i == this->number.size())
                {
                    this->number.push_back(c);
                    break;
                }
                this->number[i] += c;
                n = this->number[i];
                c = n / BIGINT_PART_CAP;
            }
            return *this;
        }
        bigint& __private_sub_self(bigint& that)
        {
            if (__private_equals(that))
            {
                this->number.clear();
                this->number.push_back(0);
                this->negative = false;
                return *this;
            }
            size_t that_size = that.number.size();
            size_t this_size = this->number.size();

            bool step2 = this_size > that_size;

            long long n;
            size_t i = 0, ip1 = 1;
            while (i < that_size)
            {
                if (ip1 == this->number.size())
                {
                    this->number.push_back(0);
                }
                n = this->number[i] - that.number[i];
                if (n < 0)
                {
                    this->number[i] = BIGINT_PART_CAP + n;
                    this->number[ip1]--;
                }
                else
                {
                    this->number[i] = n;
                }
                i++;
                ip1++;
            }
            if (step2)
            {
                while (i < this_size)
                {
                    if (ip1 == this->number.size())
                    {
                        this->number.push_back(0);
                    }
                    n = this->number[i];
                    if (n < 0)
                    {
                        this->number[i] = BIGINT_PART_CAP + n;
                        this->number[ip1]--;
                    }
                    i++;
                    ip1++;
                }
            }
            if (this->number[i] == -1)
            {
                this->number[i] = 0;
                while (i-- > 0)
                {
                    this->number[i] = BIGINT_PART_LIMIT - this->number[i];
                }
                __private_add_self_18digit(1);
                this->negative = !this->negative;
            }
            return *this;
        }
        bigint& __private_sub_self_9digit(long long that)
        {
            that %= BIGINT_PART_CAP;

            size_t this_size = this->number.size();

            long long n;
            if (this->number.size() == 1)
            {
                this->number.push_back(0);
            }
            n = this->number[0] - that;
            if (n < 0)
            {
                this->number[0] = BIGINT_PART_CAP + n;
                this->number[1]--;
            }
            else
            {
                this->number[0] = n;
            }
            size_t i = 1, ip1 = 2;
            while (i < this_size)
            {
                if (ip1 == this->number.size())
                {
                    this->number.push_back(0);
                }
                n = this->number[i];
                if (n < 0)
                {
                    this->number[i] = BIGINT_PART_CAP + n;
                    this->number[ip1]--;
                }
                i++;
                ip1++;
            }
            if (this->number[i] == -1)
            {
                this->number[i] = 0;
                while (i-- > 0)
                {
                    this->number[i] = BIGINT_PART_LIMIT - this->number[i];
                }
                __private_add_self_18digit(1);
                this->negative = !this->negative;
            }
            return *this;
        }
    } bigint;
}

int main()
{
    myitian::bigint n1 = myitian::bigint(512), n2 = myitian::bigint(512);
    myitian::bigint::scanbigint(n1);
    long long LL2;
    scanf("%lld", &LL2);
    n1+=LL2;
    n1.print();
}
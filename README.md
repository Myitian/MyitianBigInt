# MyitianBigInt

压9位bigint，方便做题，加减乘除，取模，大小比较都有。

## Example
```cpp
myitian::bigint n1, n2(888888888888, true);
n1.scanbigint();
--n1;
n1 *= n2;
n2.re_init(2333, false);
n1 += n2;
n1.print();
```
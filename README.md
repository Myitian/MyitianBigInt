# MyitianBigInt

压9位bigint，方便做题，目前只实现了+=，-=，*=，前置++和前置--。

## Example
```cpp
myitian::bigint n1(512), n2(888888888888, true);
myitian::bigint::scanbigint(n1);
--n1;
n1 *= n2;
n1 += n2;
n1.print();
```
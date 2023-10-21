# MyitianBigInt

压9位bigint，方便做题，目前只实现了+=，-=，*=，前置++和前置--。

## Example
```cpp
myitian::bigint n1 = myitian::bigint(512),
                n2 = myitian::bigint(512);
myitian::bigint::scanbigint(n1);
myitian::bigint::scanbigint(n2);
n1 *= n2;
n1.print();
```
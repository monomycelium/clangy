### `dl`

`dl`: a dynamic library tester. `dl` opens a shared object file, resolves the symbol `int32_t add(int32_t x, int32_t y)`, and executes the function with the supplied arguments.

#### example
``` shell
# make a demo shared symbol
echo "int add(int x, int y) { return x + y; }" > /tmp/add.c
gcc -shared -o /tmp/libadd.so /tmp/add.c

# build and run `dl`
make
./dl /tmp/libadd.so 5 7 # add(5, 7) = 12
```

concepts: dynamic library–loading, error-handling

TODO: let user supply function name.
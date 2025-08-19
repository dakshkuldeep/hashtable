# Hash Table using Linked-list

__*a barebone hash table implimentation*__

`#include <hashtable.c>`  <br>
into the .c file. using <> because `make install` did its job

`gcc -o <file> <file>.c -lht`

### installation troubleshooting

after error free compilation, if there a `error while loading shared libraries: libht.so: cannot open shared object file: No such file or directory`

usually means ldconfig isnt setup to look at `/usr/local/lib` yet... how to fix that?

```bash
$> echo "/usr/local/lib" | sudo tee /etc/ld.so.conf.d/local.conf
$> sudo ldconfig
```
all this does is gives ldconfig a new location to look at and its fixed!


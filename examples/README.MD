To start this example make sure the library is installed in your machine.

- In linux:

```bash
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:<lib-path>"
g++ ./example.cpp -o example -lQSDLPP
./example
```

`<lib-path>` is commonly: "/usr/local/lib"

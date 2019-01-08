# defer

Simple resource cleanup macros

## Installation

```bash
$ clib install cstdx/defer
```

## Usage

```c
#include <defer/defer.h>
#include <external/library.h>

int main() {
    gcinit(GCDFAULT);   // allocate 2 * sizeof(void*) * GCDFAULT bytes on stack
    do {
        if (library_init()) {
            break;
        }
        gcdefer(library_free, NULL);

        resource1_t* r1 = library_create_resource1();
        if (!r1) {
            break;
        }
        gcdefer(library_delete_resource1, r1);

        resource1_t* r2 = library_create_resource2(r1);
        if (!r2) {
            break;
        }
        gcdefer(library_delete_resource2, r2);

        resource1_t* r3 = library_create_resource3(r1, r2);
        if (!r3) {
            break;
        }
        gcdefer(library_delete_resource3, r3);

        library_do_some_workr(r1, r2, r3);
    } while (0);
    gclean();   // calls
                // library_delete_resource3(r3)
                // library_delete_resource2(r2)
                // library_delete_resource1(r1)
                // library_free()
    return 0;
}
```


## License

See the LICENSE file.


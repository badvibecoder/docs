# Understanding the Odin Programming Language

Book: https://zylinski.se/
Github: https://github.com/odin-lang/Odin , https://github.com/karl-zylinski
Website: https://odin-lang.org/

Taking a tour of different lower level languages, c/cpp/zig and now odin. Ideally I would have the patience for cpp and dive harder into SYCL but I find the language non-sticky with my brain. 

Odin has a pretty small keyword set and forces you into the cone of memory management in what seems like a more failure free method. It also does things like ditch the while statement.

### Language Overview

- Compiled
- Simple
- Manual Memory Mgmt
- Procedural, not Object-Oriented

Each chapter just covers a single part of the lang.

The book is not a code-along. It does provide many how to examples but its written to give you a better understanding on why and how things work under the hood.

### Compiler

This is essentially just a cloned github project via zip. Extract it and add it to your path.

In VS Code, edit coderunner executor mapping and add:

```json
"odin": "cd $dir && c:\\odin\\odin.exe run ."
```

The executable will be auto deleted after running by default.

https://github.com/odin-lang/Odin/blob/master/examples/README.md

Many references on how to do things in odin.

# Chapter 2 Hellope! A Tiny Program

```odin
package hellope

import "core:fmt"

main :: proc() {
	fmt.println("Hellope!")
}
```

Execute with: `odin run .`. The `.` refers to current folder. You can also build without running `odin build .`.

To keep the executable after running you can add `odin run . -keep-executable` otherwise `odin run .` will remove the executable.

- Main procedure `main :: proc()`
    - like a `func main()` the program starts in the main procedure
- Import the fmt package
    - Part of the core collection with the compiler
    - https://pkg.odin-lang.org/core/
    - fmt = Formatted I/O

Semicolons are optional but discouraged from use.

Core packages are just files, imports are just files. Core packages are stored in `c:\odin\core\fmt` for example.

Compile a single odin file instead of all the files in the folder: `odin run hellope.odin -file`

# Chapter 3 Variables and Constants
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

Up above we will also want:

```json
"code-runner.fileDirectoryAsCwd": true,
```

This will force the active dir to the current odin files dir, without this you need to run the code with multiple failures before it runs in the current dir.

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

Declare vars, assign vars, or declare and assign, very similar to go:

```odin
myvar: int
myvar = 7
```

```odin
myvar := 7
```

```odin
myvar: int = 7
```

Odin will infer the dtype from the value.

NAME->COLONDECLARATION->TYPE->ASIGNMENT->VALUE

Over write vars

```odin
package variable_example
import "core:fmt"
main :: proc() {
    number := 7
    fmt.println(number)
    number = 12
    fmt.println(number)
}
```

I might have missed it, but `package somename` is just the logical namespace for the compiler to reference. All code in the file belongs to that package.

Comments in odin:

```odin
// some comments

/*  some
    block
    comment */
```

Float32 or `f32`

```odin
decimal_num: f32 = 7 // I prefer this method to explicitly state and assign
// or
decimal_num2 := f32(7)
```

For floats if you infer with `decimal_num := 5.23` it will default to a f64.

Odin is strongly typed so you cannot just reassign an int var to a float. You would want to declare that var first as a f32 then assign it a whole number.

### Constants

Only exist at compilation, they cannot change once the program is running.

```odin
MY_CONST :: 12
```

Use `::` instead of `:=`. This is during the creation of the const. Once created we can assign a var with the value of the const. The value of a const is copy/pasta into whatever you are using.

```odin
MY_CONST :: 12
my_num := MY_CONST
// same as
my_num := 12
```

Constants have a slightly different type system than vars.

Untyped floats can be cast to int if the decimal is 0's.

### Type Inference Defaults

- untyped f64/float64
    - `mynum := 13.34`
- untyped int
    - `MYCONST :: 7`

### Constants of Explicit Type

You can force a type on a constant by inserting the dtype between the double `::`

```odin
MYCONST: f32 : 7.42
```

No type conversion can happen with constants so you will need to cast the constant to move from an f32 to f16/64 etc...

Casting

```odin
MYCONST: f32 : 7.42
// cast
myvar: f64 = f64(MYCONST)
```

### Basic Types Summary

- Signed Integers
    - int i8 i16 i32 i64 i128
    - -/+ whole numbers
    - int = i64 on a 64bit platform and i32 on 32bit
- Unsigned Integers
    - uint u8 u16 u32 u64 u128 uintptr
    - Positive whole numbers including 0
    - uint is u64 on 64bit platform and u32 on 32bit
    - uintptr is special, refers to an unsigned int that is always of pointer size
- Floating Point
    - f16 f32 f64
    - -/+ whole and decimal including 0
    - Numbers can be infinitely large, when its too big the float will take on the value of positive or negative infinity
- Boolean Types
    - bool b8 b16 b32 b64
    - `true` or `false`
    - 0 = false
    - bool is b8, 8 bits/1 byte
    - b32 is mostly for interfaces with other languages
- Strings
    - string
    - UTF-8
    - "" = 0 value
    - cstring type exists to interface with c
    - string16 and cstring16 that use UTF-16 instead of UTF-8, used for interface with windows OS API
- Runes
    - rune
    - A single UTF-8 string code point
    - 32bit signed int internally

### Untyped Types Summary

Used by constants since they only exist at compile time.

- untyped bool
    - true false
    - can be cast to bool b32 b64
    - default/inferred to b8
- untyped int
    - can be cast to floats
    - 64bit systems will default to int64 etc...
- untyped floats
    - can be cast to any floating point type
    - can be cast to int if all decimals are 0
    - 64bit systems will default to f64 etc...
- untyped strings
    - "anything" or `things that be multiline`
    - can be cast to string cstring string16 cstring16
    - inferred/default is string
- untyped runes
    - single char constants like 'A'
    - can be cast to rune or int that can hold the underlying numerical value of the char/rune
    - inferred type is rune
- untyped nil
    - type is nil
    - cast to anything that supports nil state, pointers enums and unions
    - cannot be inferred

# Chapter 4 Some Additional Basics

Procedures, if statements, arrays, loops.

A procedure is what functions are in most other langs.

```odin
is_bigger_than :: proc(number: int, compare_to: int) -> bool {
	return number > compare_to
}

result := is_bigger_than(3, 2)
```

The procedure is a `::` constant procedure, which takes in 2 int args. The expected output will be a bool based on the result of comparing the args.

```odin
package procs

import "core:fmt"

print_hello :: proc() {
    fmt.println("Hello")
}

main :: proc() {
	
    print_hello()

}
```

You can omit a return type if there is no return. 

### If Statements (4.2)

```odin
if condition {
    // do thing here
}
```

The condition must be a bool:

- x > 7
- x == true
- x != y

etc...

- Comparison Operators
    - ==
    - !=
    - <
    - >
    - <=
    - >=

OR || where the left side or right side can be true.

AND && where the left and right side must be true.

```odin
if var == 7 && something() {
    // var might be 7
    // something() may return true
}
```

Flip bool

```odin
somevar := 10
othervar := somevar == 7

if !othervar {
    fmt.println("othervar is not 7")
}
```

If else

```odin
if some > 33 {
    // do something
} else if someother == 5 {
    // some must be <= 33
    // someother is 5
} else {
    // must be something else
}
```

If statement must have `{}`

But you can use single line variant: `if some == 7 { dothing() }`

You can also use `do` to run it inline: `if some == 7 do some_proc()`

### Loops (4.3)

While odin has multiple loop types they all use one keyword `for`

Loop forever:

```odin
for {
    // something
}
```

You need to conditionally break this loop

```odin
for {
    if condition {
        break
    }

    // something
}
```

You can assign a condition to the for loop, it will run until the condition is false.

```odin
for condition {
    // something
}
```

Typical for loop example:

```odin
for i := 0; i< 10; i += 1 {
    // something
}
```

You can do the same with a range

```odin
for i in -..=10 {
    // something
}
```

You can nest loops and label the loops with `somename: ` giving you a namespace around the loop you want to break:

```odin
outerloop: for x in 0..=20 {
    for y in 0..=20 {
        if x >= 5 {
            break outerloop
        }
    }
}
```

We can use `continue` to skip a specific loop:

```odin
for x in 0..=10 {
    if x == 8 {
        continue
    }
}
```

You can also use labels with `continue` just like break.

### Fixed Arrays (4.4)

Fixed arrays have a known, preset, length that cannot grow or shrink. 

Create a fixed array with a variable, that holds 10 integers: `my_array: [10]int`

If you want to assign values (default will be 0) you can assign in a block:

```odin
my_array := [10]int {
    1,2,3,4,5,6,7,8,9,10
}
```

Grab an item in the array:

```odint
my_item := my_array[2]
```
This will grab the third item, 3.

You can also set individual items with brackets:

```odin
my_array[5] = 6
```

This will set the int 6 into the position 5.

Loop through all elements in an array:

```odin
for n in my_array {
    fmt.println(n)
}
```

Go through the elements in reverse:

```odin
reverse for n in my_array {
    fmd.println(n)
}
```

### Test Program

```odin
package test_program

import "core:fmt"

main :: proc() {

    treats := 5
    fav := 3

    pets := [8]int {
        1,2,3,4,5,6,7,8
    }

    for t := 0; t <= treats; t += 1 {

        for i := 0; i < 8; i += 1 {
            fmt.printfln("Pet #%v has gotten a treat", pets[i])
            if i == fav {
                most_loved(i)
            }
        }
    }
}

most_loved :: proc(pet: int) {
    fmt.printfln("Pet %v is the most favorite", pet)
}
```

# Chapter 5 Making New Types

Group basic types into more complex types named structs.

A struct is made of fields, each field has a type and name. 

```odin
Rectangle :: struct {
    x: f32,
    y: f32,
    width: f32,
    height: f32,
}
```

:: really means compile-time declaration.

We have defined a new type named `Rectangle` which we can initialize with `myrect: Rectangle`.

We can now treat it like any other dtype on instantiation:

```odin
myrect: Rectangle = {
    width = 20,
    height = 10,
}
```

or

```odin
myrect := Rectangle {
    width = 20,
    height = 10,
}
```

You can reassign the struct without issue:

```odin
myrect: Rectangle = {
    width = 20,
    height = 10,
}

// then, since its already created just reassign

myrect = {
    x = 1,
    y = 1,
    width = 10,
    height = 5,
}
```

The positions of the values within the struct hold their positions so you can also just pass in the values:

```odin
myrect := Rectangle {
    1,1,20,10,
}
```

When you initialize without specifying the fields you have to enter all fields. 

### Structs within structs (5.1.1)
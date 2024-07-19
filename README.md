# Daedalus Entropia

Entropia is a test language for the Daedalus core.

To use it, follow these steps :

## Run with main.cpp

If you wish to run Entropia with the given main file, you can run either the `win-run.bat` or `linux-run.bat` depending on your OS.

It will automatically build and run the main.cpp file and its dependancies.

## Build as static library

To build the Entropia language as a static library, you first need to run `win-init.bat` or `linux-init.sh` depending on your OS.

Once this is done, you only need to run the following make command :

```sh
make config=static-build
```

This will generate the file `Daedalus-Entropia.lib` in `build\daedalus-entropia\bin\static-build\`.

## Build as dynamic library

To build the Entropia language as a dynamic library, you first need to run `win-init.bat` or `linux-init.sh` depending on your OS.

Once this is done, you only need to run the following make command :

```sh
make config=dynamic-build
```

This will generate the files `Daedalus-Entropia.dll` and `Daedalus-Entropia.lib` in `build\daedalus-entropia\bin\dynamic-build\`.

# Wiki

## Nodes

### Identifier

Identifiers can be composed of `a-z`, `A-Z` or `_`.

### Declaration Expression

Assignation expressions are immutable by default and follow this pattern:

```
let myVar: i32 = 12;
```

> Note
>
> Supported types are the following:
> - Integer types: `i8`, `i16`, `i32`, `i64`
>
> - Unsigned integer types: `u8`, `u16`, `u32`, `u64`
>
> - Float types: `f32`, `f64`
>
> - Boolean type: `bool`
>
> - Character type: `char`
>
> - String type: `str`

If you want a mutable value, you can use this syntax:

```
let mut myVar: i32 = 12;
```

### Unary Expression

Here is the list of all valid unary operators:

| Operator | Type | Effect |
| - | - | - |
| `!` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64`, `bool`, `char`, `str` | Get the inverse of the boolean value of an expression |
| `-` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64` | Make a number negative |

### Binary Expression

Here is the list of all valid binary operators:

| Operator | Types | Effect |
| - | - | - |
| `&&` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64`, `bool`, `char`, `str` | Apply a logical AND to two values |
| `\|\|` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64`, `bool`, `char`, `str` | Apply a logical OR to two values |
| `==` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64`, `bool`, `char`, `str` | Apply a logical EQUALITY CHECK to two values |
| `!=` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64`, `bool`, `char`, `str` | Apply a logical INEQUALITY CHECK to two values |
| `<` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64` | Apply a logical LESS THAN to two values |
| `>` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64` | Apply a logical MORE THAN to two values |
| `<=` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64` | Apply a logical EQUAL OR LESS THAN to two values |
| `>=` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64` | Apply a logical EQUAL OR MORE THAN to two values |
| `+` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64` | Get the sum of two numbers |
| `-` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64` | Get the difference of two numbers |
| `*` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64` | Get the product of two numbers |
| `/` | `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64` | Get the result of the division of two numbers |

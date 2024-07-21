# Daedalus Entropia

Entropia is a test language for the Daedalus core.

To use it, follow these steps :

## Build / Run

To run Entropia, you can run either the `win-run.bat` or `linux-run.bat` depending on your OS.

You have to supply two arguments:

1. The configuration (debug / release)
2. The platform (run / static-build / dynamic-build)

The output will be in `build\daedalus-entropia\bin\<platform>`.

### Example

```sh
.\scripts\win-run debug run
```

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

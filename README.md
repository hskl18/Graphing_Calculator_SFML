# SFML Graphing Calculator

This project is a desktop graphing calculator written in C++17 with SFML 2.
It tokenizes a mathematical expression, converts infix notation to postfix notation, evaluates the result with reverse Polish notation, and samples the expression across a visible x range.

![Graphing calculator screenshot](assets/demo.png)

## Features

- Graphs expressions containing the variable `x`.
- Supports `+`, `-`, `*`, `/`, and `^` with parentheses and unary negative values.
- Supports `sin`, `cos`, `tan`, `cot`, `sec`, `csc`, inverse trigonometric functions, hyperbolic functions, `ln`, `log`, `abs`, `min`, and `max`.
- Keeps the five most recent valid expressions in an LRU history sidebar.
- Provides keyboard zoom controls and mouse-selectable history entries.
- Exposes a headless `ExpressionEvaluator` interface for deterministic tests without a display server.

## Architecture

| Component | Responsibility |
| --- | --- |
| `includes/tokenizer` | Splits expression text into typed tokens. |
| `includes/lib/shunting_yard` | Converts infix tokens to postfix order. |
| `includes/lib/rpn` | Evaluates postfix tokens for a supplied value of `x`. |
| `includes/evaluator` | Provides a small headless API around tokenization and evaluation. |
| `includes/plot` | Samples evaluated points and converts them to SFML vertices. |
| `includes/engine` | Owns the window loop, keyboard input, zoom state, and rendering. |
| `includes/lib/lru` | Stores recent valid expressions for the history sidebar. |

Additional implementation notes are available in [design.txt](design.txt).
The canonical repository is [hskl18/Graphing_Calculator_SFML](https://github.com/hskl18/Graphing_Calculator_SFML).

## Requirements

- CMake 3.20 or newer.
- A compiler with C++17 support.
- SFML 2.5 or 2.6 with the graphics, window, and system components.

### macOS

Install the native Homebrew SFML 2 formula.
The formula is keg-only, so pass its prefix to CMake.

```bash
brew install sfml@2
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH="$(brew --prefix sfml@2)"
```

### Ubuntu

```bash
sudo apt-get update
sudo apt-get install -y cmake g++ libsfml-dev
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

## Build and run

```bash
cmake --build build --parallel
./build/graphing_calculator
```

Assets are resolved from the repository at compile time, so the application can be launched from any working directory.

## Controls

- Press `Enter` to select the expression input.
- Press `Escape` to leave the expression input.
- Press `Up` to graph the current expression.
- Press `Down` to clear the graph and error state.
- Press `Right` to zoom in.
- Press `Left` to zoom out.
- Click a history entry to restore its expression and graph.

## Test

```bash
ctest --test-dir build --output-on-failure
```

The headless suite verifies precedence, parentheses, variables, supported functions, unary values, invalid syntax, non-finite results, and LRU eviction behavior.
CI compiles the full SFML application on Ubuntu and macOS, then runs the headless suite on both platforms.

To work only on the evaluator without installing SFML, disable the desktop target.

```bash
cmake -S . -B build-core \
  -DGRAPHING_CALCULATOR_BUILD_APP=OFF \
  -DBUILD_TESTING=ON
cmake --build build-core --parallel
ctest --test-dir build-core --output-on-failure
```

## Limitations

This is an educational graphing calculator rather than a computer algebra system.
It supports one variable, uses floating-point evaluation, and samples points at a fixed interval.
It does not perform symbolic simplification, equation solving, discontinuity detection, implicit multiplication, or export to image and data formats.
The automated suite validates the evaluator and LRU behavior but does not automate window interaction or visual rendering.

## License

This project is available under the [MIT License](LICENSE).

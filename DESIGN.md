# Design and expression contract

The calculator separates a headless expression pipeline from the SFML desktop shell.
This keeps mathematical behavior deterministic and testable without a display server.

## Expression pipeline

1. `Tokenizer` scans the complete input into typed lexemes.
2. A grammar pass validates adjacency, function parentheses, argument separators, arity, and delimiter balance.
3. `ShuntingYard` converts the validated infix queue to postfix notation.
4. `RPN` evaluates the postfix queue for a supplied value of `x`.
5. `ExpressionEvaluator` rejects syntax errors and non-finite final values through a result object.

`Tokenizer` owns every polymorphic token with `std::unique_ptr`.
The queues used by shunting-yard, RPN, and plotting contain non-owning pointers whose lifetime is bounded by that tokenizer.
The headless evaluator and desktop plotting path therefore use the same ownership model.

## Grammar

Numbers support integer, trailing-decimal, leading-decimal, and ordinary decimal forms such as `1`, `1.`, `.5`, and `1.5`.
Scientific notation is not supported.
Malformed numbers such as `1.2.3` and `2..3` are rejected rather than partially parsed.

Functions require parentheses.
Unary functions accept one argument, while `min` and `max` require exactly two comma-separated arguments.
Implicit multiplication is not supported, so inputs such as `2x`, `2(3+4)`, and `(2)(3)` are rejected.

Unary minus is a prefix operator with lower precedence than exponentiation and higher precedence than multiplication.
Consequently, `-2^2` evaluates to `-4`, `(-2)^2` evaluates to `4`, and `2^-2` evaluates to `0.25`.

## Desktop resources

The application uses one packaged Poppins font under the SIL Open Font License 1.1.
Decorative and error-state graphics are code-native SFML shapes.
CMake copies runtime assets beside the executable and the application resolves them relative to its executable path.
Missing resources produce a clear startup exception.

## Plotting limits

The plotter samples a fixed visible x interval with floating-point arithmetic.
It does not perform symbolic simplification, adaptive sampling, discontinuity detection, or domain segmentation.
The evaluator rejects non-finite final results.
The desktop skips individual non-finite samples so domain-limited functions can still render, and reports an error when no visible point can be produced.

## Evidence boundaries

Headless tests cover the full documented function set, number grammar, operator precedence, unary minus, arity, invalid adjacency, delimiters, non-finite domains, and LRU behavior.
Plot tests cover finite vertex output, symmetric coordinate mapping, and canvas clipping.
Full application builds and a resource smoke verify SFML integration and executable-relative font loading.
Two checks run by hand: the `--smoke-test` flag graphs the startup expression and checks history recovery, and the `GRAPHING_CALCULATOR_ENABLE_SANITIZERS` CMake option covers the headless expression pipeline and token ownership.
Automated tests do not claim pixel-level rendering correctness or complete interactive event coverage.

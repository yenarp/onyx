# Contributing to onyx

Thank you for your interest in contributing to onyx. This document outlines the
process and expectations for contributions.

## Getting Started

Before contributing, make sure you can build the project successfully by running
`make floppy` and testing with `make run-floppy` (QEMU) or `make run-bochs-floppy`.

## Build Prerequisites

You will need fasmg, an LLVM toolchain capable of cross compiling targeting
i386, GNU Make, and either QEMU or Bochs for testing.

## Branching and Commits

Work on a feature branch off `master` named `your-name/feature-description`.

Commits must use a typed prefix:

- `feat:` - new functionality
- `fix:` - bug fixes
- `refactor:` - restructuring without behavior change
- `docs:` - documentation additions or changes

Follow the prefix with a short imperative summary. Add a body after a blank line
if the change isn't trivial:

```commit
feat: load kernel from protected mode

Real-mode segment limits were preventing access to memory
above 1 MiB needed for the kernel load address.
```

```commit
fix: correct CHS conversion for drives with >2 heads
```

```commit
refactor: extract FAT12 cluster walk into subroutine
```

Open a pull request against `master` when your branch is ready. Describe what
the change does and how you tested it.

## Code Style

### Assembler

- **Four space** indentation for instructions.
- **Zero indentation** for directives labels, and section markers.
- Lowercase mnemonics.
- Comments explaining non-obvious register usage or hardware interactions.

### C

Write clean C23. Four-space indentation, no tabs.

| Element              | Convention          | Example                    |
|----------------------|---------------------|----------------------------|
| Functions            | `snake_case`        | `disk_read_sectors`        |
| Macros               | `snake_case`        | `align_up`                 |
| Constants / defines  | `SCREAMING_SNAKE`   | `PAGE_SIZE`, `MAX_ENTRIES` |
| Structs / types      | `PascalCase`        | `PageTable`, `GdtEntry`    |
| Variables / members  | `snake_case`        | `sector_count`, `buf.head` |

No specific formatter is enforced, but keep things readable and consistent with
surrounding code.
Seek to omit braces LLVM style when possible.

## Testing

At minimum, verify your changes boot and run correctly under both QEMU and
Bochs. Test with a full clean `make clean && make` cycle. Note any hardware
or emulator-specific behavior in your pull request.

## Reporting Issues

Open an issue with a clear description of the problem, the emulator or hardware
you're running on, and any relevant log output (`qemu_interrupt.log` or
Bochs debugger output).

## Use of AI-Assisted Code

Please review AGENTS.md before using any form of AI assistance.

## License

By contributing, you agree that your contributions will be licensed under the
same terms as the rest of the project.

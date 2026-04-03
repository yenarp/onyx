# AI Coding Agents

++++++++++++++++

This document provides guidance for AI agents and developers using
AI-assisted code generation when contributing to the onyx project.

AI agents helping with onyx development should follow the standard
onyx development process:

* See CONTRIBUTING.md for details

## Licensing and Legal Requirements

All contributions must comply with the project's licensing requirements:

* See LICENSE.md for details

## Code Review and Developer Responsibility

AI agents MUST NOT sign off on contributions. Only humans can certify
code quality and architectural correctness. The human reviewer is
responsible for:

* Reviewing all AI-generated code for correctness and style
* Ensuring compliance with onyx architecture and design patterns
* Verifying subsystem boundaries and phase integration
* Adding their own sign-off to certify the contribution
* Taking full responsibility for the contribution

## Attribution

When AI tools assist in compiler development, proper attribution helps
track the evolving role of AI in the development process. Contributions
should include an Assisted-by tag in the following format::

  Assisted-by: AGENT_NAME:MODEL_VERSION [TOOL1] [TOOL2]

Where:

* ``AGENT_NAME`` is the name of the AI tool or framework
* ``MODEL_VERSION`` is the specific model version used
* ``[TOOL1] [TOOL2]`` are optional specialized analysis tools used

Basic development tools (git, clang, make, editors) should not be listed.

Example::

  Assisted-by: Claude:claude-haiku-4.5 clang-tidy

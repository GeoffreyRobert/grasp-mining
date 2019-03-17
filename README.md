# grasp-mining - Hybrid job shop scheduler

Metaheuristic combining GRASP with scheduling patterns data mining

## Table of Contents

*   [About grasp-mining](#about-grasp-mining)
*   [Codemap](#codemap)
*   [Installation](#installation)
*   [Build with CMake](#build-with-cmake)
*   [Quick Start](#quick-start)
*   [Documentation](#documentation)
*   [Contributing](#contributing)
*   [License](#license)

## About grasp-mining

grasp-mining is an efficient implementation of a hybrid metaheurstic for 
solving the job shop scheduling problem.

The project contains:
* A job shop example generator;
* A data parser;
* A GRASP solver for job shop problem;
* A data mining module (combined with a controler);

## Codemap

This project includes the following components:

```
├── CMakeLists.txt        <- Top-level CMakeLists.txt
├── cmake                 <- Subsidiary CMake files
├── src                   <- Root directory for Source code
│   ├── controler         <- Controler to use mined knowledge
│   ├── grasp             <- GRASP implementation
│   ├── mining            <- Data mining module
│   ├── solver            <- Basic solver
│   └── util              <- Utilities needed by the solver
└── LICENSE.txt           <- License
```

## Installation

This software suite has been tested under:
- Ubuntu 16.04 and up (64-bit);
- Microsoft Windows with Visual Studio 2017 (64-bit).

The project is built with CMake.<br>Please check the
[CMake build instructions](cmake/README.md).

## Documentation

The complete documentation for OR-Tools is available at:
xxx
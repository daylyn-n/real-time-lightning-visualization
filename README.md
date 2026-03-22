# Real-Time Lightning Visualizer

A real-time lightning strike visualization system built in C++ using OpenGL for rendering and WebSockets for live data ingestion.
This project was developed to deepen understanding of graphics pipelines, concurrent systems, and real-time data processing.

---

## Overview

This application streams live lightning strike data and renders it in real time using a custom OpenGL pipeline. Each strike is visualized dynamically with attributes such as polarity and intensity.
The system is designed to handle continuous incoming data while maintaining smooth rendering performance.

---

## Key Features

- Real-time lightning data ingestion via WebSockets  
- OpenGL-based rendering pipeline for efficient visualization  
- Concurrent data processing using threads and mutexes  
- Visual encoding of strike properties (e.g., polarity, intensity)  
- Scalable architecture for handling continuous data streams  

---

## Tech Stack

- **C++**
- **OpenGL**
- **ixwebsocket**
- **Multithreading (std::thread, mutexes)**

---

## What This Project Demonstrates

- Practical use of the OpenGL rendering pipeline (VBOs, shaders, draw calls)  
- Integration of real-time network data into a graphics application  
- Thread-safe data handling in a concurrent environment  
- System design for continuous, high-frequency data streams  

---

## Motivation

The goal of this project was to move beyond static graphics and build a system that integrates real-time data, networking, and rendering.
It serves as a foundation for future work in graphics programming, simulation systems, and real-time visualization tools.


## Getting started

Configure and build with debug symbols:

```bash
cmake --preset debug
cmake --build --preset debug
./build/debug/src/prog
```

Configure and build with AddressSanitizer + UBSan:

```bash
cmake --preset asan
cmake --build --preset asan
./build/asan/src/prog
```

Quick non-interactive crash trace for the debug build:

```bash
gdb -q -batch -ex run -ex "bt full" --args ./build/debug/src/prog
```


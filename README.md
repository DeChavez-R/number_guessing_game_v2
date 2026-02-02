# ​🎮 C++ Number Guessing Engine v2.0
​A robust, object-oriented CLI game engine featuring multiple difficulty levels, persistent leaderboards, and real-time session statistics. This project demonstrates intermediate C++ concepts including smart pointers, file serialization, and custom input validation.
​🚀 Features
​Dynamic Difficulty Scaling: Four distinct modes (Easy, Medium, Hard, Expert) with varying ranges and attempt limits.
​Persistent Leaderboard: High scores are automatically serialized to leaderboard.txt and sorted using custom comparators.
​Proximity Hints: An intelligent feedback system that tells players if they are "Hot" or "Cold" based on the percentage distance from the target.
​Session Analytics: Tracks win rates, average attempts, and best scores during a single session.
​Robust Validation: Custom input handling to prevent crashes from invalid data types or buffer overflows.
​🛠️ Technical Highlights
​Memory Management: Utilizes std::unique_ptr for automatic resource management and to prevent memory leaks.
​Modern C++: Implements <random> (Mersenne Twister) for high-quality entropy and <chrono> for precise timestamping.
​OOP Principles: Features a decoupled architecture where the GameEngine orchestrates specialized classes like Leaderboard, RandomGenerator, and GuessingGame.
​Formatting: Leverages std::iomanip and std::stringstream for clean, table-based UI rendering in the console.
​📊 Scoring Logic
​The engine uses a weighted scoring algorithm:
You sent
# 🎮 Number Guessing Game – Modern C++ Edition

A feature-rich, console-based number guessing game written in *modern C++*.
This project is a *refactored and expanded version* of my first C++ guessing game, focused on clean architecture, modern language features, and extensibility.

    📌 This repository represents **Version 2** of the project and demonstrates my growth from a simple procedural program to a structured, object-oriented design.



---

## 
🚀 Features

- 🎯 Multiple difficulty levels (Easy, Medium, Hard, Expert)
- 🎲 Modern random number generation (<random>)
- 🧠 Smart hints system (hot / warm / cold)
- 🏆 Persistent leaderboard (file-based)
- 📊 Session statistics tracking
- 🧱 Clean class-based architecture
- 🧼 Input validation & error handling
- 📁 Single-binary, no external dependencies

---

## 🧠 What This Project Demonstrates

This project was built to practice and demonstrate:

- Modern C++ (C++17+)
- RAII and smart pointers (std::unique_ptr)
- enum class usage for strong typing
- Separation of concerns
- File I/O for persistence
- STL algorithms and containers
- Defensive input handling
- Game-loop style program flow
- Incremental refactoring and design thinking

---

## 🏗️ Architecture Overview

High-level structure:
You sent
Utilities ├── RandomGenerator ├── InputValidator
Core Domain ├── Difficulty (enum class) ├── GameConfig ├── PlayerScore ├── Leaderboard
Game Logic ├── GuessingGame
Engine Layer ├── GameStatistics ├── GameEngine
You sent
The *GameEngine* orchestrates the application, while game logic, utilities, and persistence are clearly separated.

---

## 🕹️ How to Play

1. Enter your player name
2. Choose a difficulty level
3. Guess the randomly generated number within the allowed attempts
4. Receive feedback and hints after each guess
5. Win to earn a score and appear on the leaderboard!

---

## 🧩 Difficulty Levels

| Difficulty | Range | Chances | Multiplier |
|-----------|-------|---------|------------|
| Easy      | 1–50  | 10      | x1.0 |
| Medium    | 1–100 | 7       | x1.5 |
| Hard      | 1–200 | 6       | x2.0 |
| Expert    | 1–500 | 8       | x2.5 |

---

## 🏆 Scoring System

- Base Score: *1000*
- +100 points per unused attempt
- +500 bonus for guessing correctly on the first try
- Final score is multiplied by difficulty level

---

## 🛠️ Build & Run

### Requirements
- C++17 or later
- g++ or clang++

### Compile
```bash
g++ -std=c++17 main.cpp -o guessing_game
```

### Run
```
Copy code
Bash
./guessing_game
```

# 📊 Data Persistence
Leaderboard data is stored locally in leaderboard.txt
Top 10 scores are kept and sorted automatically
📈 Project Evolution
Version 1: Simple procedural guessing game
Version 2 (this repo): Modular, extensible, modern C++ refactor
🔗 The original beginner version is preserved in a separate repository to show learning progression.
# 🔮 Possible Future Improvements
Split code into header/source files
Add unit tests
Add CMake support
Difficulty-specific leaderboards
Cross-session player profiles
Configurable scoring rules
# 👤 Author
Arjhay De Chavez
Beginner-to-intermediate C++ developer
Interested in game development, engines, and systems programming

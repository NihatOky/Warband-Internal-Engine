# Warband Internal Engine

Bu proje, Mount & Blade: Warband üzerinde bellek manipülasyonu ve vtable hooking tekniklerini kullanarak geliştirilmiş bir internal modüldür.

## Teknik Detaylar
- **Dil:** C++
- **Teknikler:** VTable Hooking, Runtime Patching, Windows API kullanımı.
- **Odak:** Oyun mantığını (logic) memory seviyesinde manipüle ederek optimizasyon ve özellik geliştirme.

*Not: Bu proje eğitim amaçlıdır ve sistem mimarisi üzerine teknik bir denemedir.*

# Warband Internal Engine

This project is an advanced internal module developed for *Mount & Blade: Warband*, focusing on memory manipulation and low-level system engineering.

## Technical Overview
- **Language:** C++
- **Core Techniques:** VTable Hooking, Runtime Memory Patching, Windows API.
- **Objective:** To analyze and manipulate the game's core logic at the memory level, demonstrating a deep understanding of game architecture and system-level programming.

## Key Features
- **VTable Hooking:** Intercepting virtual functions to redirect execution flow within the game engine.
- **Runtime Patching:** Utilizing Assembly instructions (such as `JE` patching) to modify game logic in real-time.
- **Memory Management:** Efficiently handling process memory reading and writing using Windows API functions.

## Technical Architecture
This engine is designed as an internal module, injecting its logic directly into the target process. It serves as a practical implementation of low-level game hacking principles, emphasizing performance and architectural stability.

## Disclaimer
This project is strictly for educational purposes and technical research regarding game architecture and reverse engineering.

---

## How to Compile
1. Open the project in Visual Studio.
2. Ensure the platform is set to `x86` (to match Warband's architecture).
3. Build the solution to generate the `.dll` file.

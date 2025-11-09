

# 🎵 JUCE Audio Player – OOP Assignment 🎯

**Submitted to:** Dr. Mohamed Elramly
**Course:** CS213: Object-Oriented Programming
**Group:** S27-28
**Academic Year:** 2025-2026, First Semester

| 📌 Team Members | ID | Email |
| :--- | :--- | :--- |
| **Haneen Fouad Abo Al-Yazid** | 20240175 | 20240175@stud.fci-cu.edu.eg |
| **Sara Ali Refaey Abdelhamed** | 20242148 | 20242148@stud.fci-cu.edu.eg |
| **Sarah Mohamed Salah El-Din** | 20242150 | 20242150@stud.fci-cu.edu.eg |

---

## 📘 Project Overview

This project is part of the **Object-Oriented Programming (OOP)** course assignment. It focuses on creating a robust **Audio Player** application using the **JUCE framework**, emphasizing **modular design**, **separation of concerns**, and the strict application of core OOP principles (**A PIE: Abstraction, Polymorphism, Inheritance, Encapsulation**).

The architecture is designed to clearly separate the **Audio Engine** (`PlayerAudio`) from the **User Interface** (`PlayerGUI`).

---

## 🧩 Modular Project Structure

The project is segmented into clear files to maintain modularity and clarity:

| File | Description | Role in OOP |
| :--- | :--- | :--- |
| `PlayerAudio.h / .cpp` | **Core Audio Engine:** Manages all complex audio logic, playback, speed, looping, and state persistence. | **Encapsulation/Abstraction** |
| `PlayerGUI.h / .cpp` | **Graphical User Interface:** Handles all buttons, sliders, labels, marker lists, and user interaction logic. | **Abstraction** |
| `MainComponent.h / .cpp` | **Integrator:** Combines `PlayerAudio` and `PlayerGUI`, inheriting necessary JUCE audio capabilities. | **Inheritance** |
| `Main.cpp` | Entry point of the JUCE application. | - |

---

## ✅ Implemented Features (Tasks 1, 2, 3, and Beyond)

The player includes extensive control features going beyond the basic requirements:

### 1. 🎧 Core Playback and Transport
* **Refactored & Modular Design:** Functionality split into `PlayerAudio` (audio processing) and `PlayerGUI` (UI).
* **Basic Control:** **Play / Pause**, **Stop**, and **Replay** (restart playback).
* **Navigation:** **Go to Start (|◄)** and **Go to End (►|)** buttons.
* **Skipping:** Functions to **skip forward/backward** by a specified time interval (`skipForward(double)`/`skipBackward(double)`).
* **Speed Control:** Adjustable playback speed via a slider, implemented using `ResamplingAudioSource`.

### 2. 🔊 Volume, State, and Looping
* **Mute / Unmute Functionality:** Toggles silence while preserving the previous volume state.
* **Gain Control:** Slider to adjust audio volume (`setGain(float)`).
* **Position Slider:** Displays current playback position and allows scrubbing (`setPosition(double)`).
* **Looping:** Ability to **set loop points (A/B)** and **enable/disable looping** between those points.
* **State Persistence:** Functions (`saveState`/`loadState`) to remember the last played file and position.

### 3. 🔖 Markers and Metadata
* **Marker System:** Ability to **add**, **delete**, and **clear** markers at specific track positions.
* **Marker List Box:** Dedicated GUI component (`markerListBox` and `markerListModel`) to display and manage markers.
* **File Info:** Retrieval and display of track **Artist** and **Title** metadata.

---

## 🧠 Application of OOP Principles (A PIE)

The project design rigorously implements the four core principles of Object-Oriented Programming:

### 1. 🔒 Encapsulation
* **Mechanism:** Data and control logic are bundled within the `PlayerAudio` and `PlayerGUI` classes.
* **Example:** In `PlayerAudio`, complex components like `AudioTransportSource` and `ResamplingAudioSource` are kept **private**. The outside world (i.e., `PlayerGUI`) can only interact with them through public, controlled methods like `togglePlayPause()` or `setGain()`, protecting the audio engine's internal state.

### 2. 🗺️ Abstraction
* **Mechanism:** Focuses on essential features by hiding unnecessary implementation details.
* **Example:** The **`PlayerGUI`** calls `playerAudio.setSpeed(value)` without needing to know the complex steps involved in resampling the audio data or managing the underlying `ResamplingAudioSource`. The implementation details are **abstracted away** within the `PlayerAudio` class.

### 3. ⛓️ Inheritance
* **Mechanism:** Classes inherit properties and methods from parent classes to promote code reuse and adhere to the JUCE framework structure.
* **Examples:**
    * `class MainComponent : public juce::AudioAppComponent` (inherits core audio system).
    * `class PlayerGUI : public juce::Component, public juce::Button::Listener` (inherits screen drawing and button event handling).

### 4. 🎭 Polymorphism
* **Mechanism:** Subclasses provide their own specific implementations for methods inherited from base classes (often using the `override` keyword).
* **Examples:**
    * **`paint(juce::Graphics& g) override`:** Implemented differently by `PlayerGUI` (draws background/components) and the optional `WaveForm` (draws the audio thumbnail and position line).
    * **`buttonClicked(juce::Button* button) override`:** `PlayerGUI` implements this virtual method to check which button was pressed and execute a unique action for each (e.g., Load, Play, Mute).

---

## 🛠 Technologies Used

* **Programming Language:** C++
* **Framework:** JUCE
* **IDE:** Visual Studio

---

## ▶ How to Run

1.  Open the project in **Visual Studio**.
2.  Ensure all required **JUCE modules** are correctly linked.
3.  **Build** and **run** the application.
4.  Use the **Load** button to select an audio file (e.g., **.mp3** or **.wav**).
5.  Control playback using the comprehensive GUI controls (Play, Speed, Mute, Looping, Markers).


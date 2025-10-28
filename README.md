
# 🎵 JUCE Audio Player – OOP Assignment

📌 Team Members

 Haneen Fouad Abo Al-Yazid   ( ID : 20240175 )  

 Sara Ali Refaey Abdalhamed  ( ID : 20242148 )

 Sara Mohamed Salah El-Din   ( ID : 20242150 )

---
## 📘 Overview
This project is part of the *Object-Oriented Programming (OOP)* course assignment.  
It aims to create a simple *Audio Player* application using the *JUCE framework* with proper modular design.  
The focus of this task is on *encapsulation, separation of concerns, and reusability* of components.

-
## 🧩 Project Structure
The project is divided into several files and classes to maintain modularity and clarity:

| File | Description |
|------|--------------|
| *PlayerAudio.h / .cpp* | Manages the audio logic (loading, playing, stopping, volume control, etc.). |
| *PlayerGUI.h / .cpp* | Handles the graphical user interface and user interaction (buttons, sliders, etc.). |
| *MainComponent.h / .cpp* | Integrates both PlayerAudio and PlayerGUI components together. |
| *Main.cpp* | Entry point of the JUCE application. |

---

## ✅ Implemented Tasks
The following tasks (1, 2, and 3) were implemented as required:

1. *Refactoring & Modular Design*  
   - Separated the player functionality into two components:  
     - PlayerAudio: handles all audio processing.  
     - PlayerGUI: handles all user interface elements.  
   - Improved code structure for better readability and maintenance.

2. *New Buttons Added*  
   - *Play / Pause* button.  
   - *Go to Start (|◄)* and *Go to End (►|)* buttons.  
   - These allow better control over playback.

3. *Mute / Unmute Functionality*  
   - Added a mute button that toggles between silent and previous volume state.


## 🛠 Technologies Used
- *Programming Language:* C++  
- *Framework:* JUCE  
- *IDE:* Visual Studio  
- *OOP Concepts Used:* Classes, Encapsulation, Modularity, Event Handling

## ▶ How to Run
1. Open the project in *Visual Studio*.  
2. Make sure JUCE modules are linked correctly.  
3. Build and run the application.  
4. Use the *Load* button to select an audio file (.mp3 or .wav).  
5. Control playback with the GUI buttons (Play, Pause, Stop, Mute, etc.).


# ScrEn
## A basic window for Raylib projects
I always wanted to code a basic, from scratch game, but getting it to physically exist was always annoying. Windows wouldn't work well and things always broke, or I had to use large game engines like Unity just to get these basic features, when all I needed was a place to display my code. So finally I made ScrEn, a customizable C++ window. It comes with:
- A customizable window
- Stable/Constant visuals and code, even during resizing
- Simple function and design functions called every tick
- Multi-OS support (Windows, MacOS, and Linux... and technically Android and IOS. Why would you use it there? Idk. But you can.)
Along with ALL Raylib features.
## How do I use it?
Everything is quite self explanatory, and startup was built to be easy. You should be able to tell where most things are just by looking at it, but for a basic guide:
### To change how the window looks:
1. Open `windowSettings.cpp`
2. Change variables as desired

### To create a project from scratch:
1. Download and open ScrEn in the IDE of your choice
2. Open `Program.cpp`
3. Program functional code into `UserUpdate`
4. Then, put any visual or last-second processing into `UserDraw`

### To add it to an existing project:
\(Feature in progress\)

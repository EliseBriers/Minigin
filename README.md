<p align="center">
<img src="https://www.digitalartsandentertainment.be/dae/images/DAE_headerLogo.png" length=20% width=20%>
</p>
<h1 align="center">Minigin</h1>
<p align="center">A small 2D game engine created during my <a href="https://digitalartsandentertainment.be/page/51/Programming+4">Programming 4</a> course, with a Qbert clone to demonstrate how it works.<br>
<sub>By <a href="https://elisebriers.github.io/">Elise Briers</a></sub></p>
<p align="center">
    <a href="https://github.com/EliseBriers/Minigin/actions/workflows/msbuild.yml">
        <img src="https://github.com/EliseBriers/Minigin/actions/workflows/msbuild.yml/badge.svg">
    </a>
</p>

***

# Concept
Minigin is a minimal 2d object-oriented game engine written in C++.

# Features
The engine is equipped with most basic functionality you'd expect from a minimal engine:
 * Rendering speite-sheets to the screen
 * Game Objects and Components
 * Ability to play sound
 * A frame based game loop

# Design
## Avoidance of singletons
A singleton if a useful pattern but it's often misused or overused. It can lead to less maintainable code and more, harder to debug bugs. Singletons are also often used to avoid thinking about the structure of a certain project. To force myself to think about the structure of my program in a clean way, I gave myself the restriction of not using singletons in my project.

## Restrictive programming
To avoid the user of the engine from making mistakes, I try to structure the engine in such a way that making mistakes is as difficult as possible. The avoidance of singletons plays a big part in this, because it allows me to only give access to certain functionality in the methods I want to user to have this access. An example of how this is used is only giving the user access to performance heavy functions in the `Init` function of a `GameObject`, ensuring these function don't run every frame.

## Component pattern
Minigin makes use of the [component pattern](https://gameprogrammingpatterns.com/component.html). This pattern is used to decouple functionality and reduce monolithical classes. It also helps reducing the amout of layers of inheritance, I gererally tend to limit myself to only having 1 layer of inheritance at most, prefering composition in situations more complexity is required.

## RAII Wrappers
Throughout the code, multiple [RAII wrappers](https://en.cppreference.com/w/cpp/language/raii) are used to increase memory safety. It ensures resources are initialized and destructed correctly. This makes the project easier to manage and decreases the chances of unmanaged memory.

## UUID
A UUID class is provided to make it easy to create unique identifiers. It's implemented to be as easy as possible to use. The UUID class makes it easy to compare complex objects as you only need to compare the ID, and not the entire object. This makes it perfect for comparing massive classes such as GameObjects with many components.

To use it, simply add the following line in the .h file of a class:

```cpp
const UUID<SomeClass> Id;
```

The UUID class will automatically create a new unique identifier in it's constructor.

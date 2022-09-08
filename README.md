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
## Table of contents
 - [Introduction](#introduction)
 - [The design patterns used](#the-design-patterns-used)
   * [Game Loop](#game-loop)
   * [Component](#component)
   * [Observer](#observer)
   * [Factory](#factory)
 - [Conclusion](#conclusion)
 
## Introduction
Minigin was a very minimal framework provided to us by the teachers. It only had some basic SDL functionality built in, it could draw images to the screen, play sounds and process input. We where tasked to create a fully usable game-engine using this framework as a starting point. To prove that the project works, we had to implement a simple [qbert](https://en.wikipedia.org/wiki/Q*bert) clone to show the engine in action.

The end product had the following requirements:
 * Solution is split up in Game-Engine project and Game project.
 * The project is built with warning level 4 and treat warnings as errors.
 * Software patterns from the book [Game Programming Patterns](http://gameprogrammingpatterns.com/) are correctly applied.
 * Code is clean and follows best practices.

## The design patterns used
This is an overview of the design patterns that I've implemented in the Minigin project.

### Game Loop
Every single game-engine needs a [Game Loop](http://gameprogrammingpatterns.com/game-loop.html). The game loop is the main loop in which all game-play logic takes place.

A super minimal implementation could look like this:
```cpp
while( gameIsActive )
{
    ProcessInput( );
    Update( );
    Render( );
}
```

However, this naive approach assumes that all hardware is relatively similar, as it doesn't take into account the time between frames. If we want the game to run at the same speed regardless of frame-rate, we need to introduce the concept of `deltaTime`, a value indicating the time between 2 frames. In this project, the name `elapsedSec` is used instead of `deltaTime`, because it is more descriptive.

A simple implementation of this concept could look like this:
```cpp
auto lastUpdate = high_resolution_clock::now( );
while( gameIsActive )
{
    // Calculate the difference between the previous frame and the current frame.
    const auto now = high_resolution_clock::now( );
    const auto elapsedSec = duration_cast<duration<float>>( now - lastUpdate ).count( );
    lastUpdate = now;

    ProcessInput( );
    Update( elapsedSec );
    Render( );
}
```

This code is already good, but still has one issue: float values are not super consistent. This causes some calculations to be still slightly different depending on framerate. The way we fix this is by implementing a fixed update, and update function that uses a constant `deltaTime`.
> **_NOTE:_**  Not all games need a fixed update, in this case, qbert doesn't need it, but I still wanted to include it in the project fo be feature complete.

With this in mind, this is what a complete implementation could look like:
```cpp
constexpr float FIXED_ELAPSED_SEC = 1.f / 60.f; // simulates 60 fps

auto lastUpdate = high_resolution_clock::now( );
float timeSinceUpdate;
while( gameIsActive )
{
    const auto now = high_resolution_clock::now( );
    const auto elapsedSec = duration_cast<duration<float>>( now - lastUpdate ).count( );
    lastUpdate = now;
    // Increase time since update with correct amount.
    timeSinceUpdate += elapsedSec;

    ProcessInput( );
    // Basic update
    Update( elapsedSec );
    // Update with constant elapsedSec
    while( timeSinceUpdate >= FIXED_ELAPSED_SEC )
	{
		FixedUpdate( FixedDeltaTime );
		timeSinceUpdate -= FIXED_ELAPSED_SEC;
	}
    Render( );
}
```

### Component
The [Component Pattern](http://gameprogrammingpatterns.com/component.html) is a pattern that makes it easy to create multiple objects that share functionality without using complex inheritance trees. The way it works is populating one `GameObject` class with multiple components. These components are multiple different classes inheriting from the `BaseComponent` class. This workflow prevents multiple layers of inheritance, limiting it to just 1.

An example implementation of this pattern could look like this:
```cpp
// Base class all components inherit from
class BaseComponent{
public:
    void SomeVirtualFunction(){};
}

// Class containing multiple components.
class GameObject{
public:
	void AddComponent( std::unique_ptr<BaseComponent> pComponent );
    // Loops through components and calls the virtual function we created
    void CallTheVirtualFunction();
private:
    std::vector<std::unique_ptr<BaseComponent>> m_pComponents;
}
```

The `GameObject` doesn't contain much functionality on its own, but can be given a more complex behavior by adding the correct components.
> **_Note:_** The industry is slowly moving away from this approach in favor of the more performant [ECS](https://en.wikipedia.org/wiki/Entity_component_system) technique. However, this was outside of the scope of this project as it's far more difficult to implement in a performant way.

### Observer
At the start of my project I decided to not implement an [observer](http://gameprogrammingpatterns.com/observer.html) yet, as I didn't yet know how or where I would use it. However, throughout the project I started to notice a certain pattern in my main game. Throughout multiple classes in my game, I checked if a certain variable has changed, in the event it has changed, I call a certain method to process this.

I figured I could clean up a big amount of code by just implementing a specific type of observer that I called the `ObservableVariable`. The concept is very simple: it's a templated class that cans store a value and a list of function pointers, when the variable changes, these function pointers get executed.

The implementation of this class looks like this:
```cpp
template <typename T>
class ObservableVariable
{
public:
	using observer_t = std::function<void( const T& )>;
	ObservableVariable( const T& var );
	ObservableVariable( );
	void AddObserver( observer_t observer );

	void Set( const T& var );
	const T& Get( ) const;
	bool Equals( const T& var ) const;
private:
	void Notify( );

	std::vector<observer_t> m_Observers;
	T m_Var;
	};
```

An example usage of this Observable variable:

```cpp
ObservableVariable<float> health { 10 };
health.AddObserver([]( const float& value ){ std::cout << value; });
health.Set( 9 );
// Console Output: 9
```

Many of the patterns I used in this project are used because they're obvious patterns to use, such as the game loop and the component patterns. However, the `ObservableVariable` was implemented because I saw repetition in my code and I had the knowledge of what pattern could solve this. Implementing this pattern made my code more readable, maintainable and reusable. This experience was very satisfying and rewarding, and has opened my eyes to the usefulness of knowing about many different patterns. It has sparked an interest in ma about learning multiple different patterns in the future.

### Factory
The [factory](https://en.wikipedia.org/wiki/Factory_%28object-oriented_programming%29) pattern was used multiple times throughout this project. The entire game is loaded from json files and factories are used to convert this json data to the correct classes that can be used in the game. Since the factory is a relatively straight forward pattern, I will not go in depth on the implementation of this, instead, refer to the [Github repo](https://github.com/EliseBriers/Minigin).

## Conclusion
This project has been an amazing way to learn how to develop a big project while keeping it maintainable and clean. The design patterns and techniques I learned prepared me to work on bigger projects in the future. It also sparked my interest in learning more patterns into the future to make myself the best programmer I can be.

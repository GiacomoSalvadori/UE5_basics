# UE5_basics
A project about the basics of the engine. This is used on a 8 lessons programm for students.

## Lesson 1 - slides
- Slide 1 to 5
- Let's open the engine: take a tour of everything we have
- Slide 6 to 10, there's everything just search + Unity vs UE
- Let's start from scratch: create a TPS C++ project and create a simple blueprint!
- Let's take a look to class architecture: slide 11, 14
- Go further: my first class and basic function: BeginPlay and Tick
- Make me collide: physics basics
- Apply the rule! We already have everything! Let's spot the player inside the AVOLUME class

Homework: create a solid class, what happen when I start the game inside the collider?
Create also a pickable object.


## Lesson 2 - no slides
- Character class: overview, let's study the ThirdPersonCharacter
- UWorld: this is the top level object
- Level blueprint: yes, yes you can do strange stuffs...
- Let's add the FirstPersonClass to our project...
- ...but wait, I need to configure something. An overview on the project settings and how we are gonna change the default pawn class.
- Game Mode class: We don't need it for this tour of lessons but it's very important.
- Implement shooting mechanic with line trace, via blueprint
- Delegate in C++
- Let's implement an ActorComponent, an health system used to apply damage and manage the death of a character.

Homework: Students implements the shooting mechanic with c++. They also prepare some slides where they are gonna presents their own project, they define some objectives.


## Lesson 3 - no slides
- Review of the videos.
- AI: what does it means inside a big project? Is the land of opportunity, a never ending work.
- Behaviour trees
- AI inside Unreal: classes overview and how it works inside the engine. Blueprint first and C++ for second.
- Let's tackle a real problem: implement an AI in a few time. Students are going to reproduce an AI, from scratch, inside the level.
- Students are going to create a shooting AI

Homework: students implements at least one AI.


## Lesson 4 - no slides
- Review of the homework. We are going to talk about shooting and the project.
- Subsystem: what they are and How to use.
- Let's implement a subsystem: students are going to implement a subsystem that counts the enemy in the level.
- Animation: import the animation package for ThirdPerson, let's take a look to the FSM and animation blending.
- Students start to implement some animation.
- Camera shake
- Let's add the camera shake to our FirstPersonCharacter
- Create an arsenal: students implement different weapons with custom range, ratio, mag capacity and damage and efx.

Homework: implement the combat state manager subsystem.


## Lesson 5 - slides
- Overview on multiplayer
- Let's take a look to the engine, UE4's tools for multiplayer games.
- Overview Custom movement character

Homework: implement the map gym.

## Lesson 6 - no slides
- Project review: looking at students work.

Homewrok: Implement gates.


## Lesson 7 - no slides
- Implement the boss fight! This is a training before the the test.

Homewrok: Finish the boss fight.

## Lesson 8 - no slides
- Assignment with evaluation!
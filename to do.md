<h1 style="text-align: center; font-weight: bold;"> 
TO DO </h1>

<h2 style="text-align: center; font-weight: bold;"> OPTIMIZATION </h2>

**RENDER**
- **multithreading**
  - separate render targets for main target and overlays
  - employ separate renderers for thread safety
  - re-render textures only during update cycle (thread safety with event-handling)
- **efficiency**
  - re-render a target if and only if there were changes (simple geometric shapes can be re-drawn everytime since these are about as efficient as copying a pre-rendered texture: maybe not for more complicated figures?)

**UPDATE**
- **multithreading**
  - have event-handlers + callbacks request / set-up updates, but only change objects during update calls
- **efficiency**
  - flag updates on each specific texture / element for re-rendering
  - have update() return a boolean as to whether anything was updated, if not skip render

**EVENT HANDLING**
- **multithreading**
  - run event handling in its own thread: global event handling, check for blocking animation (if so skip over custom input handler), custom-input handling; make update requests

**PROGRAMMING**
- **efficiency**
  - replace smart pointers with properly handled raw pointers to dynamically allocated memory in the internal implementation of the framework
  - purge unnecessary internal encapsulation (classes -> structs, private + setters/getters -> public...)
  - purge unnecessary stack depth: unnecessary function calls, recusive -> iterative
  - purge unnecessary resource copying and moving around, prefer direct access, passing by reference copying raw pointers (with appropriate sanity checks), and moving resources: draw diagram of resource ownership, scope, expected lifetime, and observations

<h2 style="text-align: center; font-weight: bold;"> BUGS </h2>

**RENDER**
  - text in input_field and layout containers often overflowing, and height is overestimated
**UPDATE**

**EVENT HANDLING**
  - animations are getting stuck if a button is held, rather than just clicked

**PROGRAMMING**

<h2 style="text-align: center; font-weight: bold;"> QUALITY IMPROVEMENTS </h2>

**RENDER**

**UPDATE**

**EVENT HANDLING**

**PROGRAMMING**
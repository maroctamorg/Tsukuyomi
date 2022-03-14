<h1 style="text-align: center; font-weight: bold;"> 
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
  - purge unnecessary resource copying and moving around, prefer direct access, passing by reference/copying raw pointers (with appropriate sanity checks), and moving resources: draw diagram of resource ownership, scope, expected lifetime, and observations

<h2 style="text-align: center; font-weight: bold;"> BUGS </h2>

**RENDER**
  - text in input_field and layout containers often overflowing, and height is overestimated

**UPDATE**

**EVENT HANDLING**
  - animations are getting stuck if a button is held, rather than just clicked
  - hide and show animations also getting stuck

**PROGRAMMING**

<h2 style="text-align: center; font-weight: bold;"> QUALITY IMPROVEMENTS </h2>

**RENDER**
  - have the following concurrent threads:
    - event-handling + update + request re-render
    - (clear + re-render) + display

**PROGRAMMING**
  - separate responsibilities:
    - thread-safety considerations: std::mutex / std::atomic
    - Layout should handle only the resizing and positioning of the elements, not their rendering nor especially update, this should be handled by the UI_Handler and Menu (rendering target) classes : this could perhaps be easily implemented without overcomplicating things for the user by implementing a getChildren() function for the Layout objects
    - implement a proper exception handler class with custom exceptions derived from std::exception, prepare useful error messages, code some built-in error tolerance and handle errors gracefully
    - check overlap between Menu and UI_Handler classes
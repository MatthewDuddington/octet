# Introduction to Programming 2016 - Matthew Duddington

--------------------------------------------------------------------------------
(work to 80-character line limit as suggested in the Google code standards)

Beginning from a background experience of being an artist trained in Animation,
whose existing experience of code has come primarily through scripting C# within
Unity, my goals for this first assignment are at present:

1. To become familiar with some core differences of working with C++, both 
   syntactically and in approach.
   (feeling more confident following week 3 lecture, which has pointed me to
   some expansions and clarifications on specific fundamentals)

2. Understand how to import data files (CSV, txt etc.) into a C++ program and
   use that data to define content within a game.

3. Understand how to define and manipulate shaders.

--------------------------------------------------------------------------------

Some core learning points from lectures:

  - Headerless C++ (advisory approach), wherein code is limited to as few .cpp
    modules as possible, with variables and objects aggregated into classes.

  - Assume we have ~16.6 milliseconds to calculate everything we need to do.

  - Avoid initialising new things into memory during the game loop. Do it at
    scene start-up and have objects sit inactive and off screen until needed.

  - Avoid 'new'ing anything.

  - Avoid '#define'ing anything other than '#include's.

  - Avoid global and Static variables, as they are dangerous in multithreading
    contexts, as no one thread's code can rely on the value being what it
    expects based on its own behaviour alone.

  - std:: library is full of good functions and data types (though they are not 
    yet of common use in industry due to prior inefficiency).

  - Use std::vector<T> rather than other grouped data types as it is high
    efficacy and has mutable length. But don't have a vector of vectors!

  - Use std::shared_ptr (memory managed; multiple owners of an object where the
    last one standing has authority to delete the object when the pointer itself
    is deleted)
    and std::unique_ptr (assumes it is the sole owner and thus always has
    authority to delete object when the pointer itself is destroyed),
    rather than raw pointers.

  - N.B. In this laptop there is 16GB of RAM and four 2.6GHz cores. (^_^)/

  - Classes should expose the smallest practical public area. This keeps their
    interface as simple as possible and reduces the chance of unexpected uses.

  - When in doubt return a reference to the function's class instance using
    'this' which will enable dot behaviour stacking.
    class_type &function_name() { return *this; }

--------------------------------------------------------------------------------

Initial stages of this project have involved 'hacking' the example Space
Invaiderers game to mutate it in various ways.

To begin with I focused on being able to instantiate new sprites into the scene
and located both the "get_texture_handle" and "sprite.init" functions. I
successfully interpreted how these worked together, in combination with the 
enum, and was able to modify the initialisation loops for the invaiderer's grid
to replace every other invaiderer with a new tree sprite. As well as changing 
the texture of existing sprites, I also introduced a new sprite into the scene 
(a dog's head that was 'artistically' rendered in MS paint using a trackpad
while on a train) to represent a second ship.

--------------------------------------------------------------------------------

// TODO Write these up:

Added transform on new WASD keys
Rotation methods dug up in mat4t class
Added via QE keys
Remapped the missile firing to the dog head.

Key criteria is to read from external file to generate levels.
Have never done file opening before
Seemed to be getting part way.
Collaborated with Robert Doig to work through possible ways of doing this.
Elegant solutions were getting too complicated to get working without knowing
what assumptions were valid. So decided to get basic and inefficient working
first and we'll clean up as we go.

Game Idea:
Maze sneaking
Guards with LoS and rotation
When spotted change sprite colour to yellow using shader
When alarm triggered change sprite colours to red using shader
Simple animations from moving UVs

Simple menu
Change imported language from choice at start
Save and load?
Guards can move?

Using unstable branches as part of github useage.

Wednesday 26th, spent almost all day with assisting others rather than working on my own code oops!
Assisted Pablo Larenas with debugging level instatiation loops and explained concept of arrays and indexes. Also assisted Pablo with interpreting and altering Nick's random positioning code to differentiate where the player, traps, guards and goal would be positioned at instantiation. I also helped him to work through the start of some code to read a few numbers form a text file.

Assisted Delainey Ackerman with debugging her file input loop.

Nick Veselov assisted me by explaining overview of how to add new properties to a shader.

Suggestion from Jack Evans to store neighbours in MapCell so don't need to calculate each time.

Robert Doig showed me a Shader Sandbox example (http://glslsandbox.com/e#36439.0) he had discovered which implemented the kind of procedual pixel effect I was looking for.

TODO List (final 10 days)
- Spawn guards
- Apply line of sight check to guards (basic functionality already within map cell)


Door & Switch Ideas:
- Suffix number e.g. D1 and T1 would link together
- Use a set of letters as doors and have Uppercase and Lowercase relate to door and switch pairs (Thanks to Luke Sanderson for this idea)

--------------------------------------------------------------------------------

Additional Discoveries Worth Remembering:

- Static class member variables must be initialised with a value (otherwise Visual Studio returns a generic compilation linking error (LNK2001) pointing to line 1). However, such variables cannot be initialised in a header, which causes a problem for Header Only C++ methodology.
  http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4424.pdf
  http://stackoverflow.com/questions/18860895/how-to-initialize-static-members-in-the-header
  One solution is to create a static function which contains a static variable of the type desired - as the variable can be declaired and initialised at the same time within function scope.

- Shaders don't appear to allow all C++ code to run inside them in the same way as conventional .h files (Declaration order matters more so. Also it cannot read class member variables outside the scope of the shader structure - hence some of the reason for the multistep declaration and setting of uniforms). An invalid result creates a white square only.

- Resources (such as textures and sounds) must be loaded to the resource dictionary within a function and cannot simply be kept as class members otherwise an out of range error occours during runtime.

--------------------------------------------------------------------------------

Additional Acknowledgements:

Basis for textures:
(all applicable licences permit reuse and modification)
Grass 512x512 https://sftextures.com/2014/08/06/green-grass-mixed-weed-and-clover-plant-seamless-texture/
Concrete 509x512 https://sftextures.com/2015/04/27/concrete-road-block-squared-black-and-white-marble-noisy-surface-texture/
Wire mesh https://pixabay.com/en/fence-iron-fence-mesh-wire-mesh-1094920/
Bush http://plants.swtexture.com/2009/08/tree-ficus-benyamina-hilli.html
Clouds (unused) https://pixabay.com/en/clouds-mammatus-mammatus-cloud-747254/

Sounds:
(all applicable licences permit reuse and modification)
Sfx_Wrong https://www.freesound.org/people/Isaac200000/sounds/188013/

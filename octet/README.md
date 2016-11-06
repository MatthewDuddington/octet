# Introduction to Programming 2016 - Assignment 1  
# Matthew Duddington

--------------------------------------------------------------------------------

**|| Overview of game code ||**

Structure / Approach:

- Separation of several concepts into class modules:  
  NPCs & the player - actor.h  
  Overall game level and map - level.h  
  Individual map cell logic - map_cell.h  
  Reading of level design files (see File Reading below) - level_file_handler.h  
  Core maintenance functionality and master loop remain in invaderers_app.h  

  - Extracted existing sprite class and sound functionality into separate
    modules (sprite.h and sound_manager.h respectively) and expanded upon these.

- Relatively frequent use of 'static' variables and functions, often including
  pointers, to enable cross module access and more encapsulated responsibility.  
  (Is this an appropriate pattern to be using? Statics are not always going to
  be multithread safe, depending on their function.)

  - Core game object identities such as the 'Level', 'Player', 'Actors' etc.
    are accessible to the gameplay scripting in each other module. My intention
    was to have these objects look after their respective tasks where possible.

  - Sounds can be requested to be played from most modules.  
    Similarly, the is_key_down() functionality has been made available to
    modules beyond invaiderers_app.h.

- Broadly, I have tried to adhere to the Google C++ style guide.  
  https://google.github.io/styleguide/cppguide.html  
  Though, I have chosen to avoid refactoring too much existing Octet / 
  Invaiderers code, mainly for greater clarity in what I have added, but also 
  to avoid inadvertently breaking non-obvious calls.  
  (Thus, I apologise for the resultant mixed formatting in some modules!)

  - A few significant diverges however:  
  - Firstly, I drop opening code block braces onto new lines, except for simple,
    single line blocks.  
  - Secondly, I add a space inside function declaration parameter brackets and
    in call argument brackets if there is more than one element.  
  - Finally, where I have multiple long parameters or arguments, I split these 
    onto new lines. Separating tokens go below the calling token. This gives an 
    eyeline to follow and helps prevent the IDE from realigning things 
    improperly with the auto indent.  
    Where parameters or arguments are very short, or are fundamentally related 
    (such as an X, Y, Z position) and also short, they are kept on the same 
    line.  
   - Rule #1 is apparently: readability and clarity above all other rules.  
     Where I need to adhere to a house standard, retrospective auto formatting is possible before commits are merged)

Gameplay:

- Player token can be moved one square at a time in the primary N,S,E,W
  directions within a grid based level.  
  (Using W,A,S,D key paradigm)

- Successfully navigating to a 'goal' type map cell activates the next level.

- Walls, bushes, fences etc. cannot be passed through (via cell identity
  checking rather than collision detection) and so a path must be found through
  the level.

- 'Guard' NPCs have line of sight and will capture the player (restarts the
  level) if they are triggered. Thus, the player must avoid them.

File Reading:

- level_file_handler.h reads in a text file and produces a string that it will
  feed back char by char to the separate level.h module.

- File path of text to be read is partially dynamic and enables the loading of
  multiple levels.

Procedural Generation: // TODO

- level_file_handler.h is also conceptually overloaded with the capability to
  generate a semi randomised level layout. Walls always spawn at outer edges.
  
- A flood fill algorithm ensures all 'walkable' areas are joined together.

Shader:

- Added new uniform variables and initialised them from arguments passed to the
  sprite.render() function.

- Created optional 'blending mode' parameter to the texture_shader with
  multiply, screen and overlay capabilities.

- Generative grass background texture via the fragment shader.  
  (pattern generation adapted from code from http://glslsandbox.com/e#36439.0
  - thanks to Robert Doig for pointing me towards this.)


--------------------------------------------------------------------------------

**|| Project aims ||**

Beginning from a background experience of being an artist trained in Animation,
whose existing experience of code has come primarily through scripting C# within
Unity, my aims for this first assignment are at present:

1. To become familiar with some core functional differences of working with C++
   both syntactically and in approach.  
   (I am more confident following week 3 lecture and brief chats with Andy,
   which have pointed me to some clarifications on specific fundamentals)

2. Understand how to import data files (CSV, txt etc.) into a C++ program and
   use that data to define content within a game.

3. Understand how to define and manipulate shaders.

--------------------------------------------------------------------------------

**|| Selection of core learning points from lectures ||**

  - Headerless C++ (advisory approach), wherein code is limited to as few .cpp
    modules as possible, with variables and objects aggregated into classes.

  - Assume we have ~16.6 milliseconds to calculate everything we need to do.

  - Avoid initialising new things into memory during the game loop. Do it at
    scene start-up and have objects sit inactive and off screen until needed.

  - Avoid 'new'ing anything. Allow the std:: objects to handle heap allocation.

  - Avoid '#define'ing anything other than '#include's and '#ifdef's.

  - Avoid global variables, as they are dangerous in multithreading contexts,
    as no one thread's code can rely on the value being what it expects based
    on its own behaviour alone. Similarly, be wary of statics.

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

**|| Record of assignment process ||**

Initial stages of this project have involved 'hacking' the example Space
Invaiderers game to mutate it in various ways.

To begin with I focused on being able to instantiate new sprites into the scene
and located both the "get_texture_handle" and "sprite.init" functions.
I successfully interpreted how these worked together, in combination with the 
enum, and was able to modify the initialisation loops for the invaiderer's grid
to replace every other invaiderer with a new tree sprite. As well as changing 
the texture of existing sprites, I also introduced a new sprite into the scene 
(a dog's head that was 'artistically' rendered in MS paint using a trackpad
while on a train) to represent a second ship.

Adjustment of the inputs and changing additional transform properties were my 
next goals. Amending the "keys" enum in app_common.h enabled WASD control to be 
added using their Hexadecimal ASCII values. Searching the mat4t.h module 
revealed the source of matrix transformations and many pre-existing functions. 
Rotation was added to the 'dogs head' ship via QE keys, and a quick remapping of
the missile firing relative sprite position resulted in a mobile, tank-control
like entity.

From this initial tinkering I felt I was able to draw similarities to my 
experience with using the Unity library and producing unique scripts. So I 
embarked on the initial stages of the adapted game.

A key criteria for the assignment was to make use of external data sources to 
effect the game in some way. I chose to pursue a simple level design text file 
parser, partly as I had never worked in that way before but also because 
accessing external files through code was new to me.

Working with **Robert Doig** (MSc) and later with reference to Andy's example 
csv reader code, together we gradually worked out a simple system to read out a 
char stream from the text file and store it for subsequent use. Initially we had
attempted to construct something more sophisticated and elegant, however we 
realised that, for the scope of what it needed to do, it seemed more prudent to 
follow a specific rather than generic approach. We each then developed an stage 
two system to process the chars into a tiled sprite level appropriate for our 
individual games.

My idea was to develop a maze like, sneaking strategy type MVP, as this largely 
appeared to give scope for the types of behaviours I wanted to try and produce. 
The Squaresoft 'Go' games were my basic target influence.

Some elements of this idea I was unable to add within the time, such as moving 
NPCs, animations through moving UV positions on a sprite sheet and procedural, 
flood fill algorithm based basic level generation. However, I successfully 
achieved an adaptable and reliable level design parser, added algorithmic grass 
to the shader based on my understanding of an online example, added a simple 
blending mode system to the texture shader and successfully built the core 
essential functions for the gameplay.

Likewise, I feel more confident in certain aspects of understanding and writing 
C++ code and substantially more aware of a basic level debugging capabilities in
Visual Studio. Particularly satisfying was the epiphany moment following two 
weeks of encountering the unspecific LNK2001 error code - which turned out to 
relate to non initialised static variables. This particular challenge was 
probably the most disruptive to my progression as I was unable to solve this for
some time. However, I was very satisfied at the eventual answer I was able to 
develop which is seemingly similar to a singleton pattern variation. 

Working with the shader was the next significant milestone after the level 
creation code as this was the second main requirement of the project.

**Nick Veselov** (MSc) gave me a very useful explanation of the four key places 
that needed to be amended to add new functionality to the fragment shader. From 
his advice I was able to work out how these elements interacted - A GLuint acts 
as a flag to which the address of a uniform variable can be added. The incoming 
arguments from the render() call can then be passed into the shader by using the
GLuint flag as a go-between.

Later in the term **Robert Doig** (MSc) pointed me to an ideal example of shader
code (http://glslsandbox.com/e#36439.0) that would create a noisy texture. I was
able to gradually deconstruct the deceptively simple set of intertwining 
functions to understand what was going on and slightly adapted it to include it 
within my texture shader. Prior to this, I had been attempting to make use of 
random number generation to seed the fragment shader but had been struggling to 
find functions and approaches that did not cause the shader to break and display
only a white square.

Much of the gameplay aspects followed in the final two weeks of the assignment
as I had achieved a critical mass of foundational code that I was able to add
and iterate with substantially fewer instances of the game not running at all.

Prior to this point I had begun to make use of Unstable Branches within my 
GitHub repository so that I was able to make a few commits even on days where 
the code did not compile successfully for a long time. Towards the end, this was
much less of an issue.

I was able to create a couple of different approaches to moving / positioning
the camera based on either the player location (sliding to follow or popping to 
match positions) or the level design (zooming itself backwards to fit the 
longest edge of the level snugly within the window).

This lead to a basic menu screen where different options could be applied to the
game upon start up, and later functionality like resetting the game, skipping 
levels and so forth were added to assist with demoing and testing.

For fun on the last day I also polished up the sounds and bookend screens just 
to make it feel a little more presentable.

As well as receiving help, slightly to my surprise I was also able to provide 
useful help to quite a few other students over the course of the assignment. 
Much of this was on account of simply being present in the shared lab space on 
most days, and so mini discussions would often reveal some new idea or 
understanding between us all. Such as **Jack Evans** (MSc) offering the 
particularly helpful suggestion of storing awareness of adjacent cells within 
each cell, so that I didn't need to derive it from fresh each time. Jack also 
helped me earlier on in the term with understanding some differences in 
conventions between C# and C++.

Specifically, I spent a notable length of time assisting **Pablo Larenas** (MA) 
with understanding, debugging and expanding upon certain aspects of the code 
other students had assisted him with. Furthermore, I was able to work through 
the concept of Arrays and indexes with Pablo to the stage where he was able to 
understand how to manipulate and interpret them himself. By the end of our half-
day he had been able to change and control the random locations of instantiated 
npcs, level objects etc.

Similarly, during reading week I was able to spend a morning passing on my 
understanding of basic shader access to **Eloise Calandre** (MA), as well as 
explain the basic principle and structure behind classes in OOP and their 
constituent members.

**Delainey Ackerman** (MA) requested my help to debug the file parsing code she 
was working with and we were able to chop and change parts of it to reach a 
successfully functioning state. Likewise, was able to help **Luke Sanderson** 
(MSc) with an example of what Robert and I had done with the file parser as well
as assisting him with a basic understating of where to integrate random numbers
into his code to randomly assign positions.

// IMAGE TO GO HERE OF WHITEBOARD

--------------------------------------------------------------------------------

**|| Additional discoveries worth remembering ||**

- Static class member variables must be initialised with a value (otherwise 
  Visual Studio returns a generic compilation linking error (LNK2001 pointing to
  line 1). However, such variables cannot be initialised in a header, which
  causes a problem for Header Only C++ methodology.  
  http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4424.pdf  
  http://stackoverflow.com/questions/18860895/how-to-initialize-static-members-in-the-header  
  One solution is to create a static function which contains a static variable
  of the type desired - as the variable can be declared and initialised at the
  same time within function scope.

- Shaders don't appear to allow all C++ code to run inside them in the same way 
  as conventional .h files.  
  Declaration order matters. It cannot read class member variables outside the 
  scope of the shader structure - hence some of the reason for the multi-step 
  declaration and setting of uniforms. Some standard functions don't seem to be 
  compatible. Cannot use 'f' to define floats unless using later version. 
  An invalid result creates a white square only.

- Resources (such as textures and sounds) must be loaded to the resource 
  dictionary within a function and cannot simply be kept as class members 
  otherwise an out of range error occurs during runtime.

--------------------------------------------------------------------------------

**|| Additional acknowledgements and references ||**

Basis for textures:  
(all applicable licences permit reuse and modification)  
Grass (original prototype)  
https://sftextures.com/2014/08/06/green-grass-mixed-weed-and-clover-plant-seamless-texture/  
Wall  
https://sftextures.com/2015/04/27/concrete-road-block-squared-black-and-white-marble-noisy-surface-texture/  
Fence  
https://pixabay.com/en/fence-iron-fence-mesh-wire-mesh-1094920/  
Bush 
http://plants.swtexture.com/2009/08/tree-ficus-benyamina-hilli.html
Menu BG composite  
https://pixabay.com/en/texture-wall-oyster-oyster-colored-1590106/  
https://pixabay.com/en/moss-wall-green-rock-texture-457387/  
Goal  
https://pixabay.com/en/emergency-exit-exit-door-way-sign-98585/  
Win screen composite
http://www.geograph.org.uk/photo/1520138
"TF6024 : A gap in the bushes"
(c) Richard Humphrey (geograph.org.uk/profile/39484)  
This file is licensed under the Creative Commons Attribution-Share Alike 2.0
Generic license.
http://www.geograph.org.uk/photo/1508666  
"SN8056 : Hillside with rowans, Powys"  
(c) Roger Kidd (geograph.org.uk/profile/12192)  
This file is licensed under the Creative Commons Attribution-Share Alike 2.0
Generic license.  

Music:  
BGM  
http://www.incompetech.com/music/royalty-free/index.html?isrc=USUAN1100754  
"Rising"  
Kevin MacLeod (incompetech.com)  
Licensed under Creative Commons: By Attribution 3.0  
http://creativecommons.org/licenses/by/3.0/  

Sounds:  
(all applicable licences permit reuse and modification)  
Sfx_Wrong  
https://www.freesound.org/people/Isaac200000/sounds/188013/  
Sfx_Caught  
http://www.freesound.org/people/Robinhood76/sounds/60521/  
sfx_Success  
http://www.freesound.org/people/InspectorJ/sounds/347246/  
Sfx_Win  
http://www.freesound.org/people/squashy555/sounds/269244/  

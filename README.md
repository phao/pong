Pong Clone
==========
This is a pong clone I've made to practice game development, C and SDL.

Some images can be found in here: http://imgur.com/zMOV4r7,m6ZRKcs,RCdrZ6V

The project is pretty much done, not in the sense that I can't make it
better, but in the sense that I'm unlikely to modify it again.

An interesting microbenchmark came up in the middle of development. Check
folder mbench for it, it's about clamping floating point numbers to be
within [0,m] where `m` is a parameter to the clamping function.

While there is a make file for the project, you should be able to get this
working with a single command line:

  * `gcc main.c -lm -lSDL2`

If you're interested in changing the source code to do something on your own,
make sure to set the warning levels up and set some debugging information:

  * `gcc -Wall -Wextra -std=c99 -pedantic -g main.c -lm -lSDL2`

There is a video I recorded of me playing, and loosing, the game =D =>
http://youtu.be/W5hXXdwJsQU

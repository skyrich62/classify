# classify
A small example of AI (deductive reasoning) to classify things based on is-a relationships in C++

When I first wrote this code more than a year ago, I wrote a custom parser for the commands.  I've since re-worked it in PEGTL, 
and it's much better and much less error prone.

Build, (you'll need to fork PEGTL from github here: https://github.com/taocpp/PEGTL), using the supplied Makefile.  I'm using GCC, but you can modify things to suite your compiler, but make sure it supports modern C++.

Run the code, and it will give a helpful information screen with example commands.
HINT: try "load data.txt" this will load an initial database of facts into the system.  After that try various things like:
"is a carrot a plant?" or "why is a human a vertebrate?"  Play with it a little bit, you'll get the idea in no time.

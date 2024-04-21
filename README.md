# Casio Invaders

C++ console recreation of Invaders game from the Casio MG-880 and SL-880 calculators.

This program uses ncurses for non-blocking keyboard input on Unix systems. The libncurses-dev package should be installed: 

```apt install libncurses-dev```

The program can then be built using:

```make```

The binary is available in bin:

```./bin/invaders```

Keys:
- q: Quit
- z: Increatement missile number
- [space]: Fire missile
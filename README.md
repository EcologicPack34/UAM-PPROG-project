# Ant HELL

A small sized first year university project consiting on making a small game engine and a sample game

## About the Project

This project was developed as part of the first year at Universidad Autónoma de Madrid.

The main goal of this project is to create a terminal text based game that can be configured with a configuration file.

The project is purely made in C and with Linux in mind. 

Most of the project and documentation is in Spanish due to the nature of it.

### Note

As this was a first-year university project, the project structure and code quality may not follow best practices. The organization is not ideal, and some parts of the code could be improved or refactored.

### Characteristics

- Configurable map, enemies, players, object, stats, player abilities, shops and more.

- Saves. The game can be saved and loaded in another moment (when not in combat).

- Multiplayer. Up to 2 players are supported.

- Turn based combat with multiplayer support.

- Custom TUI renderer with support for colored text via custom syntax.

- Custom dialogue system with support for colored text via custom syntax.

- Command system separated for each player and game state, it even remembers your last command.

- Minimap, generated on the fly from the map data.

- Music and basic volume control via [miniaudio.h](https://github.com/mackron/miniaudio).

- 40 minutes long gaming experience included with the project.

- And more stuff I'm probably forgetting about.

## Contributors

- [@EcologicPack34](https://github.com/EcologicPack34)

- [@makpol83](https://github.com/makpol83)

- [@madrilus](https://github.com/mandrilus)

- [@BlueScreenOfDeath314](https://github.com/BlueScreenOfDeath314)

## Project Info
<pre>
.
├── include/              (contains all header files)
├── src/                  (contains all .c files)
│   └── test/             (contains .c files with tests for some types)
├── sounds/               (music used by the game)
├── Project_INFO/         (contains information explaining how the game works)
├── libraries/            (legacy folder, not used in the current version)
├── AntHell/              (contains settings for the game)
├── Project_Management/   (project management information required for submission)
├── Makefile              (makefile used to compile the project)
├── OldMakefile           (legacy makefile from an older version of the project)
├── Doxyfile              (configuration file for Doxygen documentation)
├── game_launcher         (launcher for the game, simplifying game and save loading)
├── LEER ANTES.txt        (information about the different project submissions)
└── other files           (miscellaneous project files)
</pre>



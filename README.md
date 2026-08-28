## rough todo list

* get doomstm compiling w/o stdlib
* set up proper error handling
* wad pruning
    * use a declarative python script to prune from shareware
    * as we prune, slowly work on macro'ing out every renderer path fails in doomstm
* get doomstm to build on stm target with silly linker stuff

# doomsat

doomsat is an attempt to run an extremely limited port of DOOM on the Pixelsat I satellite based on [doomgeneric](https://github.com/ozkl/doomgeneric). tentatively, the satellite's STM32H753ZI obc will be supplemented with a STM32F767xI coprocessor, which may be used for science payload processing, but will be mostly dedicated to doom. 

**resource limits**
- cpu:
    - we have availability of the full cpu!
    - most likely will clock to 168 MHz for power purposes
    - doom will be the highest-priority rtic task, at least unless there is a science experiment
- ram:
    - while we have full ram access, it's best for doom to allocate entirely in one sector, for now, sram1 (368kib)
    - the rust portion will of course allocate in dtcm
    - the goal is to avoid a heap beyond the one used by doom which lives in sram
- flash/binary size:
    - flash bank 1 (1mib) includes the binary (rtic + linked doomgeneric port)
    - flash bank 2 (1mib) includes doom1.wad
    - the doomgeneric binary will most likely statically link into the rust rtic binary

here's how the port will work (roughly):
- we mutilate doomgeneric into two separate "forks".
    - in one fork (call this doomstm), we rip out the entire renderer and implement the necessary functions on the Rust side via extern "C". instead of rendering the buffer (or even having a screen buffer anymore), we instead serialize the game state (most likely to rkyv, but serde etc. are still open) and transmit them. the goal is to ultimately transmit these over RF, but for now we'll use a simpler backend like UNIX sockets. input handling is also handled in doomstm.
    - in another fork (call this doomclient), we rip out the entire business logic and instead expose functions from the C side for the Rust client to set game state that are received over RF or sockets from doomstm. then, the Rust client (which statically links into doomclient, since this is not running on the sat) receives & decodes game states over sockets/RF, sets game state, and manually drives the renderer to render the game state to the screen buffer, after which it uses any variety of graphics libraries to render the game (for example, SDL, for which a doomgeneric implementer example already exists). input is also handled and transmitted.
- we cut out significant parts of doom1.wad on the doomstm side -- every episode and map except for episode 1's map 1, audio, any graphic assets that are not strictly needed for the game logic (beyond tiny stubs), etc. the pruned doom1.wad is then compressed with deflate then loaded into the aforementioned flash. at runtime, the shareware file will be decompressed into RAM which allows us to provide a fopen-compat interface for doomgeneric/doomstm to access. if there is space left over, the additional maps or demos may be considered.
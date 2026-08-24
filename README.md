## various todos

* add logging via callbacks

# doomsat

doomsat is an attempt to run an extremely limited port of DOOM on the Pixelsat I satellite based on [doomgeneric](https://github.com/ozkl/doomgeneric).  

**resource limits**
- cpu:
    - stm32 cpu speed
    - must yield often enough for any other lower-priority task to run
    - must be prepared for possibly significantly lengthy suspensions by the RTIC scheduler for higher-priority tasks
- ram:
    - all significant allocations must live in SRAM1 + SRAM2 + SRAM3 (288kb)
    - no heap use beyond sram, the rust portion of course can store small amounts of data on the regular rust stack
- flash/binary size:
    - we are going to free up four bank 2 sectors by 1) using the previously reserved bank 2 sector 6, 2) removing 2 image sectors (so max image size is now 256kib as opposed to previously 512kib), 3) moving the tle storage sector from bank 2 to bank 1 
        - this does possibly mean that we will be unable to capture large full res (1600x1200) images because of size constraints, but most recent estimates suggest such images will come in at around 80kib. the uniformity of pictures of the earth/space should also help us in this regard for compression.
    - totaling 512kib
    - the doomgeneric binary must either statically link into the RTIC binary without bringing its size above 384kib (the RTIC binary is currently approx. 242kib) or be part of the flash

here's how the port will work (roughly):
- we mutilate doomgeneric into two separate "forks".
    - in one fork (call this doomstm), we rip out the entire renderer and implement the necessary functions on the Rust side via extern "C". instead of rendering the buffer (or even having a screen buffer anymore), we instead serialize the game state (most likely to rkyv, but serde etc. are still open) and transmit them. the goal is to ultimately transmit these over RF, but for now we'll use a simpler backend like UNIX sockets. input handling is also handled in doomstm.
    - in another fork (call this doomclient), we rip out the entire business logic and instead expose functions from the C side for the Rust client to set game state that are received over RF or sockets from doomstm. then, the Rust client (which statically links into doomclient, since this is not running on the sat) receives & decodes game states over sockets/RF, sets game state, and manually drives the renderer to render the game state to the screen buffer, after which it uses any variety of graphics libraries to render the game (for example, SDL, for which a doomgeneric implementer example already exists). input is also handled and transmitted.
- we cut out significant parts of doom1.wad on the doomstm side -- every episode and map except for episode 1's map 1, audio, any graphic assets that are not strictly needed for the game logic (beyond tiny stubs), etc. the pruned doom1.wad is then compressed with deflate then loaded into the aforementioned flash. at runtime, the shareware file will be decompressed into RAM which allows us to provide a fopen-compat interface for doomgeneric/doomstm to access.
- the biggest worry of the pixelsat team, including team lead (ashwin) and the rest of the software leads (ona), is that running DOOM will crash the satellite or otherwise prevent the team from accomplishing any of the other mission objectives. to alleviate this, several measures are taken:
    - doom is hard-gated behind a flag in backup sram (which is persisted across reboots). after the team leads feel that the rest of the mission is sufficiently done, an uplink message (implementation TBD) will be sent up which sets the flag (really a magic byte) in bsram for read on the next boot. on the next boot, the init routine will see the flag and consequently start the doom task. 
    - the doom task is almost always dormant, the only resource allocation when idle is the 512kib ram used. doom starts only when ground sends up an uplink packet to start doom, at which point it decompresses and loads doom1.wad, initializes doomgeneric/doomstm, and starts the frame loop. it's also priority 1 so literally any other task in the firmware can preempt it.
    - if ground loses connection or the pass ends, the task promptly ends, deallocating from its memory (however keeping that memory to itself), unloading the shareware, and ending the doomstm runtime. this will happen in any of the following cases: 8 minutes have elapsed since the start of gameplay, 10 seconds have elapsed since the last keepalive (doom heartbeat) or keyboard input packet from ground
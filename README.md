# Multiple-Squares
Multiple squares
V1:
Added networking function, join, leave, room, enter code,...
V2 
Fixbug: Networking Info_Reply false reception and reaction from old players.
Optimize: Removed the function changeto (to change gamestate), and instead changing it directly
V3 (lastest stable)
Added Pposition update, but setting position continuosly 
Optimize: Added performance control in main loop, only using SDL_PollEvent when needed. Else use SDL_WaitEvent/SDL_WaitEventTimeout
V4 (working on)
Position interpolation


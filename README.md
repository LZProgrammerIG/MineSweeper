# MineSweeper
The classic Mine Sweeper game created in C++ by using the cross platform WxWidgets GUI Library.

For those who don't know, the game creates a minefield with a finite number of mines scattered randomly acorss the minefield. The player is tasked with uncovering all the safe places while avoiding all the mines. If the player steps on a mine, then the game's over. When player uncovers a safe spot, it will display a number which indicates the total number of mines around it in its adjacent cells. Each cell can contain atmost one mine. The player can use this knowledge, combined with a bit of luck, to avoid the mines and uncover all the safe spots. Happy Hunting !!

## Features
1. A main menu screen where the player can choose the difficulty.
2. The ability to mark a flag on potential mines by clicking the cell with right mouse button.
3. Automatically reveals all the adjacent cells of the cell which doesn't has any mine around it.
4. Finally, a stopwatch for those who like to flex their Mine Sweeping Speed.

## Pre Requisites
1. A C++ language compiler.
2. Visual Studio or any other IDE which can open .sln files.
3. WxWidgets Library installed and already compiled on your system.

https://youtu.be/FOIbK4bJKS8?si=WN9qbtPVJxfJDM4P
You can use this link to install and build the wxwidgets library on your system. Skip to 10:20.

## Most Important step!!!
Make sure to add the path of the WxWidgets folder installed on your system to the Environment Variables and name this variable as WXWIN  .
You can refer to the Youtube video link posted above and skip to 17:25.


## Running the Application
1. Download all the files in this repository.
2. Open the Guess The Number.sln file in Visual Studio or any other suitable IDE.  
3. Click Run or simply press Ctrl + F5 in Visual Studio and enjoy!! :D


## Thanks for trying out my game. :)
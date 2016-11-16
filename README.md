# Cavecopter
Simple helicopter game created by C (Allegro 5)

### Description
This game, Cavecopter Lite, is simple but addictive game. Guide the helicopter through the cave without hitting spikes or obstacles. hold space bar button to go up/ Release to go down.

![Cavecopter Screenshot](http://i.imgur.com/mt47Wsf.png "Main menu")
![Cavecopter Screenshot](http://i.imgur.com/qGeW8dU.png "Gameplay")

### Tool
C with Allegro 5

More information about [Allegro 5 (A game programming library)](http://liballeg.org/)

### Developer Guide
* `#define _CRT_SECURE_NO_WARNINGS` is to prevent a warning in Visual Studio 2015
* `enum STATE { TITLE, PLAYING, GAMEOVER };` consist of 3 game states. 
  * `TITLE` is title menu which instructions is included. player have to press 'a' in order to play
  * `PLAYING` is main gameplay. (Note: The helicopter will go up instantly)
  * `GAMEOVER` is a screen which shows score and wait for the player to press 'play again' button.
  
### Resources
* **Game Library** - I created this game in 3 days with this wonderful library [Allegro 5](http://liballeg.org/)
* **Font** - [RD CHULAJARUEK Regular](http://www.f0nt.com/release/rd-chulajaruek-regular/)
* **Background** - [CuteSbBoy Newsgrounds.org](http://www.newgrounds.com/art/view/cutesbboy/beacon-cave-background)

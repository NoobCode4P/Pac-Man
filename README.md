# **PAC-MAN**  <img src="PacMan/Resources/Textures/pacman.png" width=8% height=8%>

This game is written in C++ using SFML

---

***Pac-Man*** is one of the most popular classic games and is known to exist with countless variations across different gaming platforms. But in this project, for simplicity, I chose the original version *(1980, Arcade)* as a fundamental game mechanism to implement.

# Game Mechanisms
## 1. Meet the main characters:

>|Pac-Man|Blinky|Pinky|Inky|Clyde|
>|:-----:|:----:|:---:|:--:|:---:|
>|![](PacMan/Resources/Textures/pac_man_icon.png)|![](PacMan/Resources/Textures/3d-pixel-ghost-red.png)|![](PacMan/Resources/Textures/3d-pixel-ghost-pink.png)|![](PacMan/Resources/Textures/3d-pixel-ghost-blue.png)|![](PacMan/Resources/Textures/3d-pixel-ghost-orange.png)|


## 2. Rules
- In this game, your job is to solely manipulate the main character ***Pac-Man*** to move around the maze to clear all the small dots called ***Pellets*** without letting it collide with those ghosts. Were Pac-Man to get impacted, he would lose a life. Unless Pac-Man has previously eaten a bigger pellet in the maze commonly known as ***Power Pellet***, which strengthens him to the extent that he could eat those ghosts, he would escape from his death. When ***Pac-Man*** eats one of the ***Power Pellets*** in the maze, ***Pac-Man*** is known to be energized for limited time and all ghosts turn simultaneously into blue color, implying they become frightened, and tremble with fear, undecide a right way to escape from being eaten by ***Pac-Man***. However, this feature only happens at some specific low level. At higher levels, these ghosts do not become frightened anymore, instead they just turn 180 degrees backwards, causing ***Pac-Man*** unable to eat them even though he has eaten the ***Power Pellets***.

- The scoring system:
    - ***Pellet***: 10 points
    - ***Power Pellets***: 50 points
    - *Vulnerable Ghosts* (ghosts eaten by Pac-Man): 200, 400, 800, 1600 points for the order of eaten ghosts in succession within per ***PowerPellet***'s effect
    - At each level, in the middle of the game, there is an object appears near the center of the maze, below the ghost house for a short amount of time commonly called ***Fruit*** as for bonus points. To be more specific, [this](https://static.wikia.nocookie.net/pacman/images/2/25/Fruits_Points.png/revision/latest?cb=20210921001546) will tell you how many points would be acquired according to a particular item

- When all ***pellets*** and ***power pellets*** are cleared, the next level of challenge awaits you.

- To have a better understanding how the game actually works, [The Pac-Man Dossier](https://pacman.holenet.info/) will take you into deep explanation.

- If you are interested in how the ghosts decide their own ways to traverse the maze, then I think [this video](https://www.youtube.com/watch?v=ataGotQ7ir8&t=1s) has the most captivating ***Ghosts' behaviors visualization*** and amazing explanation of ***Ghost AI*** I have seen so far.

# Game Modes
## **1. Play Mode**
- This is where the game actually challenges you to control ***Pac-Man*** wisely and set the highest score record to the current gameplay. Note that I develop it to store the records neither locally nor in any databases, so all records you have set will be completely reset every time you shut down the game.

## **2. Practice Mode**
**Play Mode** is not easy to conquer, is it?

- **Practice Mode** is here to help you practice and conquer the game rules. Not only does it grant **Pac-Man** infinite lives, but it also allows you to experience some special features (`Ghost Visualizers`, `Invincibility of Pac-Man`, `PAUSE GAME`, `RESTART LEVEL`, `SKIP LEVEL`)

## **3. Learn Mode**
- This mode provides an opportunity of visualizing one of the ghosts' behaviors

## **4. Coffee Breaks**
- This is not actually a game mode, but it is a small part of the gameplay. In the original **Pac-Man arcade version**, **Coffee Breaks** (also referred to as *Intermissions* or *Cutscenes*) are small, humorous interactions that appear between levels in the Pac-Man maze games. I tried re-implementing the animation of these cutscenes as a separate game feature apart from the gameplay.

# Game Controls
- Use arrow keys &uarr;, &larr; ,&darr; &rarr; to move **Pac-Man**
- Use Left-Click mouse button to click buttons

# Testing platform
- Windows 10

#  IDE
- Microsoft Visual Studio 2022

# Gravity Lander

## Overview

Gravity Lander is a simple 2D game where your mission is to take a ship from point A to point B. The game is located in the moon and the are two landing platforms at any given moment: The start platform and the finish platform. you can move the ship by using a small propulsion force that always points up from the perspective of the ship.

At the beginning of each level you always start a few meters above a platform (start platform). Your job is to take the ship to the finish platform, which is the other platform located in a random point of the moon. You officially land the ship when the bottom of it (not the top nor any of the sides) touches the finish platform at any point of the platform. Immediately after you touch the bottom ship with the platform, you advance to the next level.

![Gameplay](C:\Users\nicol\Desktop\Gameplay.png)

Every time you use the propeller, a small force will be imprinted on the ship. If you run out of fuel and you haven't reached the finish platform, a countdown of 5 seconds starts, at the end of which, you will lose and the score will reset to 0.

Although the finish platform is always placed on random locations on the moon every level, the distance between locations increases the more you advance on the game.

## Controls

* Press the `Space` in order to use the propeller and move forward.
* Press the `Left(⬅️)` and `Right(➡️)` keys to rotate the ship left and right respectively.

## Added features

There is one added feature, and it's the existence of boosters. Boosters are blue boxes that are randomly spawned so that when the player collides with one of them, they will increase the force excerted by the propeller.

![Booster](C:\Users\nicol\Desktop\Booster.png)
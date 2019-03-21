# C++ Salute game
=========================

## Usage
This game is based on the engine of the "Playrix" company.

## Description
The application is a salute shooter in which the salute gun fires rockets at intervals of 5 seconds. Every rocket flies a certain distance up, and then explodes in the form of a salute. Player can forcefully shoot a salute gun by pressing the "Space" bar. Salute can also be launched by clicking the left mouse button on the background area.
The salute gun can be moved to the bottom of the screen left and right using the "left" and "right" keys on the keyboard.

The service buttons are located in the lower left corner of the screen: the pause/play button, the salute stop button, and the settings menu button.
1. "Pause/play" button pauses or continues salute launch.
2. "Stop" button interrupts all active salutes.
3. "Settings" button shows or hides the menu bar. The menu can also be shown by pressing the "Esc" button.

There are 3 settings switches and 2 buttons on the menu panel
1. "Background" switch allows player to change the background of the game. Available backgrounds: Sidney, New York, London.
2. "Difficulty" switch allows player to change the power of the salute. The power of the salute is estimated by the degree of the chain reaction after the rocket explosion. Available mode: Simple, Middle, Hard, Apocalypse.
3. "Salute type" switch allows player to change the type of the salute. Available types: Stars scatter, Rain fall, Rainbow glow, Salute mixed. With the "Salute mixed" type, each charge explodes with one of the 3 previous effects.
4. "Continue" button allows player to hide the menu panel.
5. "Exit" button close the application.

The following modules are implemented in this application:
1. SaluteDelegate. Connecting widgets.
2. SaluteWidget. The main widget of the application.
3. ObjectPool class. Base class for components. Required to manage different components: their destruction from memory, adding new ones, drawing etc.
4. ButtonPool class. Required to manage buttons.
5. Menu class. Required to manage switches and buttons on the menu panel.
6. Button class. Button description class. The base class, which implements the animation of buttons and the actions performed by clicking on them.
7. Switcher class. Switcher description class. The base class, which implements the animation of switches and the actions performed by clicking on them.
8. SaluteGun class. Required to control rockets: adding them to the store, moving rockets into the container, as well as destroying existing ones. Through this class, the lifetime of the rocket to the very effect of the salute.
9. Rocket class. Rocket description class. The base class, which implements the mechanics of the movement of rockets, their animation and salute effect at the end of their lifetime.
10. Cursor class. Class description of the mouse cursor in this game.

README for ‘A Day in Darkness’

Overview

‘A Day in Darkness’ is a 2D side-scrolling game developed using the SFML (Simple and Fast Multimedia Library) framework. The game combines immersive themes, interactive gameplay, and strategic challenges where players navigate through ancient and modern scenarios, collecting items, avoiding obstacles, and surviving with limited resources. The game integrates OpenCV for video playback and utilizes audio-visual assets for an enriched experience.

Features

1. Game Modes
	•	Ancient Theme: Players collect cotton and avoid obstacles while progressing through levels.
	•	Modern Theme: Features a more futuristic setting with KFC collectibles and modern obstacles like police officers and babies.

2. Dynamic Gameplay
	•	Items such as cotton, KFC, watermelons, and whips fall from the top of the screen, and the player character must navigate to collect or avoid them.
	•	Levels become progressively harder, with faster falling objects and more frequent obstacles.

3. Health and Score System
	•	Players have a health bar representing their vitality.
	•	Collectible items increase scores, and collisions with certain obstacles (e.g., whips or police) decrease health.

4. Sound Effects and Music
	•	Background music changes based on the theme (Ancient or Modern).
	•	Sound effects for interactions like collecting watermelons or colliding with obstacles.

5. User Interaction
	•	Player inputs their name before starting the game.
	•	A leaderboard displays high scores to foster competitive gameplay.

6. Settings
	•	Adjustable volume and brightness.
	•	Ability to toggle sound effects on/off.

7. Intro Video
	•	OpenCV integration for playing an introductory video before the game begins.

Controls
	•	Arrow Keys: Move the character left or right.
	•	A: Select Ancient Theme.
	•	M: Select Modern Theme.
	•	TAB: Start the game.
	•	ESC: Access the settings menu or exit to the main menu.
	•	L: View the leaderboard.
	•	T: Toggle sound effects on/off.
	•	R: Restart the game.

Gameplay Rules
	1.	Collect items to increase your score and progress through levels:
	•	Cotton (Ancient Theme) or KFC (Modern Theme) increases score.
	•	Watermelons or babies add extra time or health.
	2.	Avoid harmful items:
	•	Whips and police decrease your health.
	3.	Survive until the time runs out to complete the level and advance to the next.

Setup and Requirements

Dependencies
	1.	Libraries:
	•	SFML: For graphics, window management, and audio.
	•	OpenCV: For playing intro videos.
	•	FFmpeg: For video decoding.
	2.	Resources:
	•	Fonts, textures, and audio files stored in the resources folder.

Installation
	1.	Ensure you have a C++ compiler and an IDE capable of building SFML projects. Examples include Visual Studio, Code::Blocks, or CLion.
	2.	Install the required libraries:
	•	SFML (2.5.1 or later):
	•	Download from the official SFML website.
	•	Follow the installation guide to link SFML to your project. Ensure the include and lib paths are correctly set in your IDE.
	•	OpenCV:
	•	Download OpenCV from the official OpenCV website.
	•	Extract the files and add the OpenCV include directory and lib paths to your project settings.
	•	FFmpeg:
	•	Download FFmpeg from FFmpeg’s official website.
	•	Install the binaries and add the FFmpeg bin folder to your system’s PATH environment variable.
	3.	Clone or download the project files and place the assets in the appropriate directories:
	•	Place all image files in the resources/images/ folder.
	•	Place all audio files in the resources/audio/ folder.
	•	Place the intro video in the resources/videos/ folder.
	4.	Correct the path locations in your code:
Ensure the paths to your resources are correct. For example:

// Example for loading an image
sf::Texture texture;
if (!texture.loadFromFile("resources/images/example_image.png")) {
    // Handle the error
}

// Example for loading a video
cv::VideoCapture video("resources/videos/intro.mp4");
if (!video.isOpened()) {
    // Handle the error
}


	5.	Build and run the main.cpp file using your chosen IDE.

Code Structure
	1.	Main Class: Game
	•	Handles game states, rendering, input processing, and updates.
	2.	Game States:
	•	NAME_INPUT
	•	THEME_SELECTION
	•	MENU
	•	PLAYING
	•	MODERNPLAY
	•	SETTINGS
	•	LEADERBOARD
	3.	Items:
	•	Defined as structs (Cotton, KFC, Watermelon, etc.).
	4.	Sound and Music:
	•	Managed through SFML’s audio modules.
	5.	Rendering:
	•	Handles brightness, health bar, and in-game text updates.

Challenges and Rewards
	•	Survive falling objects while maintaining your health and progressing through levels.
	•	Balance time, health, and score to maximize your leaderboard ranking.

Future Improvements
	•	Add more themes and levels.
	•	Introduce multiplayer support.
	•	Include power-ups and new obstacles.

Credits
	•	Developer: Mujtaba, Sheharyar
	•	Libraries Used: SFML, OpenCV, FFmpeg
	•	Team Assistants: Syed Ahmad, Abdullah Shafaqat, Hamza Memon

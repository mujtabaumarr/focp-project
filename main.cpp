#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}


using namespace std;

class Game {
public:
    struct Cotton {
        sf::Sprite sprite;
        float timeOnGround;
    };

    struct Watermelon {
        sf::Sprite sprite;
        float timeOnGround;
    };

    struct Whip {
        sf::Sprite sprite;
        float timeOnGround;
    };

    struct KFC {
        sf::Sprite sprite;
        float timeOnGround;
    };

    struct Babies {
        sf::Sprite sprite;
        float timeOnGround;
    };

    struct Police {
        sf::Sprite sprite;
        float timeOnGround;
    };



    enum class GameState { NAME_INPUT, THEME_SELECTION, MENU, PLAYING, MODERNPLAY, GAME_OVER, SETTINGS, PLAYER_NAME, LEADERBOARD };

    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBar;
    sf::Font font;
    sf::Text healthText;
    sf::Text modernhealthText;
    sf::RectangleShape volumeBarBackground; // Background of the volume bar
    sf::RectangleShape volumeBar;          // Foreground of the volume bar
    int volumeLevel;                     // Current volume level (0 to 100)
    int health;
    int whipSpawnFrequency;
    bool isSoundEnabled = true; // Global toggle for all sounds
    float brightnessLevel; // Brightness level (default is 1.0)
    string playerName;                         // To store the player's name
    vector<pair<string, int>> leaderboard; // Leaderboard: player name and highest level
    bool enteringName = false;                     // To track if the user is entering their name
    sf::Text namePrompt;                           // Text prompt for entering the name
    sf::Text leaderboardText;                      // Text to display the leaderboard
            // To store the player's name
    sf::Text namePromptText;        // Prompt text
    sf::Text playerNameDisplayText; // Text to show entered name


    Game()

        : window(sf::VideoMode(832, 448), "A day in Darkness"),
          score(0), level(1), target(5), timeRemaining(20.0f), isGameOver(false),brightnessLevel(1.0f),
          playerVelocity(0.0f, 0.0f), cottonFallSpeed(5.0f),whipSpawnFrequency(200), gameState(GameState::MENU),
          congratsMessageDuration(1.0f), playerHealth(50),health(200), volumeLevel(40.0f), currency(0), whipCollided(false){  // Initial health is 100
        window.setFramerateLimit(60);


        namePromptText.setFont(font);
        namePromptText.setCharacterSize(24);
        namePromptText.setFillColor(sf::Color::White);
        namePromptText.setString("Enter your name: ");
        namePromptText.setPosition(100.0f, 150.0f);

        // Initialize player name display text
        playerNameDisplayText.setFont(font);
        playerNameDisplayText.setCharacterSize(24);
        playerNameDisplayText.setFillColor(sf::Color::White);
        playerNameDisplayText.setPosition(300.0f, 150.0f);




        for (int i = 0; i <= 3; i++) { // Adjust the number based on how many textures you have
            sf::Texture texture;
            if (!texture.loadFromFile("/Users/apple/CLionProjects/untitled26/resources/mod" + to_string(i + 1) + ".jpg")) {
                cerr << "Failed to load modern background " << i + 1 << endl;
                exit(1);
            }
            modernBackgroundTexture.push_back(texture);

        }

        if (!modernCharacterTexture.loadFromFile("/Users/apple/CLionProjects/untitled26/resources/charmod.png") ||
            !KFCTexture.loadFromFile("//Users/apple/CLionProjects/untitled26/resources/kfc.png") ||
            !BabyTexture.loadFromFile("//Users/apple/CLionProjects/untitled26/resources/baby.png") ||
            !PoliceTexture.loadFromFile("/Users/apple/CLionProjects/untitled26/resources/police.png")) {
            cerr << "Failed to load modern theme assets!" << endl;
            exit(1);
    }

        applyChosenTheme();

        modernBackground.setTexture(modernBackgroundTexture[rand() % 3]);
        modernCharacter.setTexture(modernCharacterTexture);
        modernCharacter.setScale(0.3f, 0.3f); // Adjust scaling as needed



        // Play intro video before starting the game
        if (!playIntroVideo("/Users/apple/CLionProjects/untitled26/resources/brainrot.mp4")) {
            cerr << "Error playing intro video. Continuing to the game." << endl;
        }

        // Load backgrounds
        if (!backgroundTextures[0].loadFromFile("/Users/apple/CLionProjects/untitled26/resources/background.png") ||
            !backgroundTextures[1].loadFromFile("/Users/apple/CLionProjects/untitled26/resources/background1.png") ||
            !backgroundTextures[2].loadFromFile("/Users/apple/CLionProjects/untitled26/resources/background2.png")) {
            cerr << "Failed to load background image!" << endl;
            exit(1);
        }
        background.setTexture(backgroundTextures[0]);

        // Load character textures
        if (!characterTextures[0].loadFromFile("/Users/apple/CLionProjects/untitled26/resources/ch1.png") ||
            !characterTextures[1].loadFromFile("/Users/apple/CLionProjects/untitled26/resources/chr2.png") ||
            !characterTextures[2].loadFromFile("/Users/apple/CLionProjects/untitled26/resources/ch3.png")||
            !characterTextures[3].loadFromFile("/Users/apple/CLionProjects/untitled26/resources/ch4.png")
            ||
            !characterTextures[4].loadFromFile("/Users/apple/CLionProjects/untitled26/resources/ch5.png")
            ||
            !characterTextures[5].loadFromFile("/Users/apple/CLionProjects/untitled26/resources/ch6.png")) {
            cerr << "Failed to load character textures!" << endl;
            exit(1);
        }

        // Initially set the character texture for level 1
        character.setTexture(characterTextures[0]);
        character.setScale(0.3f, 0.3f);  // Adjust scaling if necessary
        centerCharacter();

        // Load fonts
        if (!font.loadFromFile("/Users/apple/CLionProjects/untitled26/resources/Avenir.ttc")) {
            cerr << "Failed to load font!" << endl;
            exit(1);
        }

        {
            // Initialize volume bar background
            volumeBarBackground.setSize(sf::Vector2f(200.0f, 20.0f)); // Width: 200px, Height: 20px
            volumeBarBackground.setFillColor(sf::Color(50, 50, 50));  // Dark gray
            volumeBarBackground.setPosition(window.getSize().x / 2 - 100.0f, window.getSize().y / 2 - 10.0f);

            // Initialize volume bar (foreground)
            volumeBar.setSize(sf::Vector2f(volumeLevel * 2.0f, 20.0f)); // Width reflects volume level
            volumeBar.setFillColor(sf::Color::Green);                   // Green color
            volumeBar.setPosition(volumeBarBackground.getPosition());   // Same position as background
        }

        // Configure menu text
        menuText.setFont(font);
        menuText.setCharacterSize(36);
        menuText.setFillColor(sf::Color::White);
        menuText.setString("A day in Darkness \n\nPress TAB to Start\nPress Esc for settings\nPress L for LeaderBoard");
        menuText.setPosition(200.0f, 100.0f);

        namePrompt.setFont(font);
        namePrompt.setCharacterSize(24);
        namePrompt.setFillColor(sf::Color::White);

        leaderboardText.setFont(font);
        leaderboardText.setCharacterSize(24);
        leaderboardText.setFillColor(sf::Color::White);

        // Configure score text
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10.0f, 10.0f);

        // Configure timer text
        timeText.setFont(font);
        timeText.setCharacterSize(24);
        timeText.setFillColor(sf::Color::White);
        timeText.setPosition(650.0f, 10.0f);

        // Load cotton texture
        if (!oldCottonTexture.loadFromFile("/Users/apple/CLionProjects/untitled26/resources/cotton.png")) {
            cerr << "Failed to load cotton image!" << endl;
            exit(1);
        }

        // Load watermelon texture
        if (!oldWatermelonTexture.loadFromFile("/Users/apple/CLionProjects/untitled26/resources/ability.png")) {
            cerr << "Failed to load watermelon image!" << endl;
            exit(1);
        }

        // Load whip texture
        if (!oldWhipTexture.loadFromFile("/Users/apple/CLionProjects/untitled26/resources/whip.png")) {
            cerr << "Failed to load whip image!" << endl;
            exit(1);
        }

        // Load whip sound
        if (!whipSoundBuffer.loadFromFile("/Users/apple/CLionProjects/untitled26/resources/whip.mp3")) {
            cerr << "Failed to load whip sound!" << endl;
            exit(1);
        }
        whipSound.setBuffer(whipSoundBuffer);

        // Load watermelon sound
        if (!watermelonSoundBuffer.loadFromFile("/Users/apple/CLionProjects/untitled26/resources/watermelon.mp3")) {
            cerr << "Failed to load watermelon sound!" << endl;
            exit(1);
        }
        watermelonSound.setBuffer(watermelonSoundBuffer);


        // Load baby sound
        if (!babySoundBuffer.loadFromFile("/Users/apple/CLionProjects/untitled26/resources/baby.mp3")) {
            cerr << "Failed to load watermelon sound!" << endl;
            exit(1);
        }
        babySound.setBuffer(babySoundBuffer);



        // Load whip sound
        if (!policeSoundBuffer.loadFromFile("/Users/apple/CLionProjects/untitled26/resources/police.mp3")) {
            cerr << "Failed to load police sound!" << endl;
            exit(1);
        }
        policeSound.setBuffer(policeSoundBuffer);

        if (!ancientThemeMusic.openFromFile("/Users/apple/CLionProjects/untitled26/resources/background_music.mp3")) {
            std::cerr << "Failed to load ancient theme music!" << std::endl;
            exit(1);
        }

        if (!modernThemeMusic.openFromFile("/Users/apple/CLionProjects/untitled26/resources/modmusic.mp3")) {
            std::cerr << "Failed to load modern theme music!" << std::endl;
            exit(1);
        }

        // Set loop to true for continuous playback
        ancientThemeMusic.setLoop(true);
        modernThemeMusic.setLoop(true);

        // Set default theme to Ancient
        currentBackgroundMusic = &ancientThemeMusic;
        currentBackgroundMusic->play();


        // Configure congratulatory message text
        congratulatoryText.setFont(font);
        congratulatoryText.setCharacterSize(24);
        congratulatoryText.setFillColor(sf::Color::Green);


        leaderboard.clear();
        srand(static_cast<unsigned>(time(0)));

        {
            // Initialize health bar
            healthBar.setSize(sf::Vector2f(playerHealth * 8.0f, 40.0f));  // Width of health bar based on initial health
            healthBar.setFillColor(sf::Color::Green);
            healthBar.setPosition(window.getSize().x / 2 - 100.0f, 10.0f);  // Center the health bar
        }

        // Initialize health text ("Rights") inside the health bar
        healthText.setFont(font);
        healthText.setString("Rights");
        healthText.setCharacterSize(16);
        healthText.setFillColor(sf::Color::Black);
        healthText.setPosition(window.getSize().x / 2+50.0f , 10.0f);  // Centered in the health bar

{
            // Initialize health bar background
            healthBarBackground.setSize(sf::Vector2f(400.0f, 20.0f));  // Width: 400px, Height: 30px
            healthBarBackground.setFillColor(sf::Color::White);   // Dark gray background
            healthBarBackground.setPosition((window.getSize().x - healthBarBackground.getSize().x) /2, 10);  // Centered horizontally, 10px from the top

            // Initialize health bar (foreground)
            healthBar.setSize(sf::Vector2f(800.0f, 30.0f));  // Same size as background
            healthBar.setFillColor(sf::Color::Green);        // Start with green (full health)
            healthBar.setPosition(healthBarBackground.getPosition());  // Align with background

            // Initialize health text
            healthText.setFont(font);
            healthText.setCharacterSize(16);
            healthText.setFillColor(sf::Color::Black);
            healthText.setString("Rights");
            healthText.setPosition(
            healthBarBackground.getPosition().x + (healthBarBackground.getSize().x - healthText.getLocalBounds().width) / 3,
            healthBarBackground.getPosition().y + (healthBarBackground.getSize().y - healthText.getLocalBounds().height) / 4);
}


    }

    void run() {
        sf::Clock clock;
        while (window.isOpen()) {
            sf::Time deltaTime = clock.restart();
            processEvents();
            if (gameState == GameState::PLAYING && !isGameOver) {
                handleGameInput(); // Ensure handleGameInput is called for movement
                update(deltaTime.asSeconds());
            }
            if (gameState == GameState::MODERNPLAY && !isGameOver) {
                handleGameInput(); // Ensure handleGameInput is called for movement
                update(deltaTime.asSeconds());
            }
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::Sprite background;
    sf::Texture backgroundTextures[3];
    sf::Sprite character;
    sf::Texture characterTextures[6];  // Array to store textures for different characters
    sf::Text menuText, scoreText, timeText, congratulatoryText;
    sf::Texture cottonTexture, watermelonTexture, whipTexture, KFCTexture, BabyTexture, PoliceTexture;
    sf::Texture oldCottonTexture, oldWatermelonTexture, oldWhipTexture;
    int currentCharacterIndex;
    int currency;         // Tracks the player's currency
    bool whipCollided;    // Tracks if a whip has collided during the level

    // New member variables for theme selection
    string chosenTheme; // To store the selected theme
    sf::Texture BackgroundTexture; // Texture for ancient theme
    sf::Texture modernCharacterTexture;
    sf::Texture modernCottonTexture;
    sf::Texture modernWatermelonTexture;
    sf::Texture modernWhipTexture;
    vector<sf::Texture> modernBackgroundTexture;
    sf::Sprite modernBackground;
    sf::Sprite modernCharacter;



    string healthBarText;
    int score, level, target;
    float timeRemaining;
    bool isGameOver;
    sf::Vector2f playerVelocity;
    const float maxSpeed = 25.0f;
    float cottonFallSpeed;
    vector<Cotton> cottonPieces;
    vector<Watermelon> watermelons;
    vector<Whip> whips;
    vector<KFC> KFCpiece;
    vector<Babies> Babypiece;
    vector<Police> Policeman;
    GameState gameState;
    float congratsMessageDuration;
    sf::Music* currentBackgroundMusic; // Pointer to the currently active background music

    sf::SoundBuffer whipSoundBuffer, watermelonSoundBuffer, babySoundBuffer, policeSoundBuffer;
    sf::Sound whipSound, watermelonSound, babySound, policeSound;
    sf::Music ancientThemeMusic, modernThemeMusic;

    // Health bar variables
    float playerHealth;  // Player's health


    void centerCharacter() {
            // Ensure the character is centered based on the logical resolution (832x448)
            float screenWidth = 832.0f;  // Logical width
            float screenHeight = 448.0f; // Logical height

            character.setPosition(
                (screenWidth - character.getGlobalBounds().width) / 2,  // Center horizontally
                screenHeight - character.getGlobalBounds().height - 10.0f // Position above the bottom edge
            );
        }




    void applyChosenTheme() {


    // For the Ancient theme
    if (chosenTheme == "Ancient") {


        // Set Ancient Background (with level cycling)
        background.setTexture(backgroundTextures[level % 3]);

        // Ensure the correct music is playing for Ancient theme
        if (currentBackgroundMusic != &ancientThemeMusic) {
            if (currentBackgroundMusic) currentBackgroundMusic->stop(); // Stop the current music
            currentBackgroundMusic = &ancientThemeMusic;
            currentBackgroundMusic->play(); // Play Ancient Theme
        }

        // Set the character texture for Ancient theme
        character.setTexture(characterTextures[currentCharacterIndex]);

        // Set text for the Ancient theme on the health bar
        healthBarText = "Rights";

        // Reset other ancient theme-dependent variables (like objects)
        cottonTexture = oldCottonTexture ;           // Set ancient cotton texture
        watermelonTexture = oldWatermelonTexture ;   // Set ancient watermelon texture
        whipTexture = oldWhipTexture ;               // Set ancient whip texture

    }
    // For the Modern theme
    else if (chosenTheme == "Modern") {


        // Set Modern Background (with random cycling)
        background.setTexture(modernBackgroundTexture[rand() % 3]);

        // Set the Modern character
        character.setTexture(modernCharacterTexture);

        // Ensure the correct music is playing for Modern theme
        if (currentBackgroundMusic != &modernThemeMusic) {
            if (currentBackgroundMusic) currentBackgroundMusic->stop(); // Stop the current music
            currentBackgroundMusic = &modernThemeMusic;
            currentBackgroundMusic->play(); // Play Modern Theme
        }

        // Set text for the Modern theme on the health bar
        healthBarText = "Diddy Party";

        // Reset other modern theme-dependent variables (like objects)
        cottonTexture = modernCottonTexture;            // Set modern cotton texture
        watermelonTexture = modernWatermelonTexture;    // Set modern watermelon texture
        whipTexture = modernWhipTexture;                // Set modern whip texture
    }




        // Ensure the character is always centered after the theme is applied
        centerCharacter();


}

    void updateHealthTextPosition() {
        // Calculate the center of the health bar
        float healthBarCenterX = healthBarBackground.getPosition().x + (healthBarBackground.getSize().x / 2);
        float healthBarCenterY = healthBarBackground.getPosition().y + (healthBarBackground.getSize().y / 2);

        // Position healthText at the center of the health bar
        healthText.setPosition(
            healthBarCenterX - (healthText.getLocalBounds().width / 2), // Center horizontally
            healthBarCenterY - (healthText.getLocalBounds().height / 2) // Center vertically
        );
    }

    bool playIntroVideo(const string& videoPath) {
        cv::VideoCapture video(videoPath);

        if (!video.isOpened()) {
            cerr << "Error: Could not open video file!" << endl;
            return false;
        }

        int fps = static_cast<int>(video.get(cv::CAP_PROP_FPS));
        int delay = 1000 / fps;  // Delay per frame in milliseconds

        cv::Mat frame;
        while (true) {
            bool isSuccess = video.read(frame);
            if (!isSuccess) {
                cout << "Video ended!" << endl;
                break;
            }

            cv::imshow("A Day in Darknessxe", frame);
            if (cv::waitKey(delay) == 27) {  // 'Esc' to skip the video
                cout << "Video skipped by user!" << endl;
                break;
            }
        }

        video.release();
        cv::destroyAllWindows();
        return true;
    }

    void updateHealthBar() {
        // Update the width of the health bar based on current health
        healthBar.setSize(sf::Vector2f(400.0f * (health / 100.0f), 30.0f));

        // Update the color of the health bar depending on health percentage
        if (playerHealth > 50) {
            healthBar.setFillColor(sf::Color::Green);
        } else if (playerHealth > 20) {
            healthBar.setFillColor(sf::Color::Yellow);
        }
        else {
            healthBar.setFillColor(sf::Color::Red);
        }



        // Update healthText position dynamically
        updateHealthTextPosition();
    }


    void processEvents() {

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }



        else if (gameState == GameState::THEME_SELECTION) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A) {
                    chosenTheme = "Ancient";
                    applyChosenTheme();
                    gameState = GameState::PLAYING; // Go to menu after choosing a theme
                } else if (event.key.code == sf::Keyboard::M) {
                    chosenTheme = "Modern";
                    applyChosenTheme();
                    gameState = GameState::MODERNPLAY; // Go to menu after choosing a theme
                }
            }
        }

        if (gameState == GameState::MENU) {
            resetGame();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab) {
                    gameState = GameState::NAME_INPUT; // Transition to name input
                    playerName.clear(); // Reset player name for a fresh start
                } else if (event.key.code == sf::Keyboard::Escape) {
                    gameState = GameState::SETTINGS;
                } // Open settings menu
                else if (event.key.code == sf::Keyboard::L) {
                    gameState = GameState::LEADERBOARD; // Open leaderboard
                }
            }
        }

        else if (gameState == GameState::NAME_INPUT) {
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !playerName.empty()) {
                    playerName.pop_back(); // Handle backspace
                } else if (event.text.unicode == '\r' && !playerName.empty()) {
                    gameState = GameState::THEME_SELECTION; // Transition to theme selection
                } else if (event.text.unicode < 128 && event.text.unicode > 31) {
                    playerName += static_cast<char>(event.text.unicode); // Add valid characters
                }
            }


            else if (gameState == GameState::THEME_SELECTION) {
                // Prompt text for theme selection
                sf::Text themePrompt("Choose a Theme:", font, 36);
                themePrompt.setFillColor(sf::Color::White);
                themePrompt.setPosition((window.getSize().x - themePrompt.getLocalBounds().width) / 2, window.getSize().y / 4);
                window.draw(themePrompt);

                // Ancient Theme option
                sf::Text ancientOption("A. Ancient Theme", font, 24);
                ancientOption.setFillColor(sf::Color::Yellow);
                ancientOption.setPosition((window.getSize().x - ancientOption.getLocalBounds().width) / 2, window.getSize().y / 2);
                window.draw(ancientOption);

                // Modern Theme option
                sf::Text modernOption("M. Modern Theme", font, 24);
                modernOption.setFillColor(sf::Color::Cyan);
                modernOption.setPosition((window.getSize().x - modernOption.getLocalBounds().width) / 2, window.getSize().y * 3 / 4);
                window.draw(modernOption);
            }


        // Handle Player Name Inputif (gameState == GameState::PLAYER_NAME) {
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !playerName.empty()) {
                    playerName.pop_back(); // Handle backspace
                } else if (event.text.unicode < 128 && event.text.unicode != '\b' && playerName.size() < 15) {
                    playerName += static_cast<char>(event.text.unicode); // Add character to name
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter && !playerName.empty()) {
                    gameState = GameState::THEME_SELECTION; // Proceed to menu after name is entered
                }
            }
        }

        // Handle Leaderboard Navigation
        else if (gameState == GameState::LEADERBOARD) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    gameState = GameState::MENU; // Return to menu
                }
            }
        }

        // Handle Game Over Events
        else if (gameState == GameState::GAME_OVER) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    leaderboard.push_back({playerName, currency}); // Save to leaderboard
                    resetGame(); // Restart the game
                    gameState = GameState::NAME_INPUT;

                } else if (event.key.code == sf::Keyboard::Escape) {
                    leaderboard.push_back({playerName, currency}); // Save to leaderboard
                    gameState = GameState::MENU; // Return to menu
                    resetGame();
                }
            }
        }

        // Handle Settings Events
        else if (gameState == GameState::SETTINGS) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    brightnessLevel = std::min(1.0f, brightnessLevel + 0.1f); // Increase
                } else if (event.key.code == sf::Keyboard::Down) {
                    brightnessLevel = std::max(0.0f, brightnessLevel - 0.1f);

                } else if (event.key.code == sf::Keyboard::Left) {
                    // Decrease volume
                    volumeLevel = static_cast<int>(std::max(0.0f, volumeLevel - 5.0f));
                    currentBackgroundMusic->setVolume(static_cast<float>(volumeLevel)); // Adjust current music volume
                    volumeBar.setSize(sf::Vector2f(static_cast<float>(volumeLevel) * 2.0f, volumeBar.getSize().y)); // Update volume bar
                } else if (event.key.code == sf::Keyboard::Right) {
                    // Increase volume
                    volumeLevel = static_cast<int>(std::min(100.0f, volumeLevel + 5.0f));
                    currentBackgroundMusic->setVolume(static_cast<float>(volumeLevel)); // Adjust current music volume
                    volumeBar.setSize(sf::Vector2f(static_cast<float>(volumeLevel) * 2.0f, volumeBar.getSize().y)); // Update volume bar
                } else if (event.key.code == sf::Keyboard::Escape) {

                    gameState = GameState::MENU; // Return to the main menu
                } else if (event.key.code == sf::Keyboard::R) {
                    gameState = GameState::PLAYING; // Resume the game
                }
                else if (event.key.code == sf::Keyboard::E) {
                    gameState = GameState::MODERNPLAY; // Resume the game

                }
                else if (event.key.code == sf::Keyboard::T) {
                    isSoundEnabled = !isSoundEnabled; // Toggle global sound
                }
            }
        }

        // Handle Playing Events
        else if (gameState == GameState::PLAYING) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    gameState = GameState::SETTINGS; // Open settings menu
                }
            }
        }
        else if (gameState == GameState::MODERNPLAY) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    gameState = GameState::SETTINGS; // Open settings menu
                }
            }
        }
    }
}






    void handleGameInput() {
        // Get the current width of the character and the screen width
        float characterWidth = character.getGlobalBounds().width;
        float screenWidth = window.getSize().x;

        // Reset velocity if no keys are pressed
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            playerVelocity.x = 0.0f;
        }

        // Move left (ensure the character doesn't move off the screen)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && character.getPosition().x > 0) {
            playerVelocity.x = -maxSpeed;  // Move to the left
        }

        // Move right (ensure the character doesn't move off the screen)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
            character.getPosition().x + characterWidth < screenWidth) {
            playerVelocity.x = maxSpeed;  // Move to the right
        }

        // Apply the movement to the character's position
        character.move(playerVelocity.x, 0.0f);
    }

    void update(float deltaTime) {
        timeRemaining -= deltaTime;
        if (timeRemaining <= 0.0f) {
            isGameOver = true;
            gameState = GameState::GAME_OVER;
            return;
        }
        if (gameState == GameState::PLAYING) {
            spawnancientObjects();
        }
        else if (gameState == GameState::MODERNPLAY) {
            spawnModernObjects();
        }


        // Check health bar
        if (playerHealth <= 0) {
            playerHealth = 0; // Ensure health doesn't go negative
            gameState = GameState::GAME_OVER; // Transition to game over state
            return;
        }

        moveObjects();
        checkCollisions();
        updateText();
        updateHealthBar(); // Update health bar every frame

        if (score >= target) {
            levelUp();
        }

        if (congratsMessageDuration > 0.0f) {
            congratsMessageDuration -= deltaTime;  // Reduce duration
        } else {
            congratulatoryText.setString("");  // Clear the message after duration expires
        }

        // Update the health bar based on the player's health
        healthBar.setSize(sf::Vector2f(playerHealth * 2.0f, 20.0f));  // Health bar width depends on playerHealth
    }

    void spawnancientObjects() {
        if (rand() % 100 == 0) spawnCotton();
        if (rand() % 200 == 0) spawnWatermelon();
        if (rand() % whipSpawnFrequency == 0) spawnWhip();
    }

    void spawnModernObjects() {
        if (rand() % 100 == 0) spawnKFC();
        if (rand() % 200 == 0) spawnBaby();
        if (rand() % whipSpawnFrequency == 0) spawnPolice();
    }

    void spawnCotton() {
        sf::Sprite sprite(cottonTexture);
        sprite.setScale(0.25f, 0.25f);
        sprite.setPosition(rand() % 800, -20.0f);
        cottonPieces.push_back({sprite, -1.0f});
    }

    void spawnKFC() {
        sf::Sprite sprite(KFCTexture);
        sprite.setScale(0.25f, 0.25f);
        sprite.setPosition(rand() % 800, -20.0f);
        KFCpiece.push_back({sprite, -1.0f});
    }

    void spawnBaby() {
        sf::Sprite sprite(BabyTexture);
        sprite.setScale(0.25f, 0.25f);
        sprite.setPosition(rand() % 800, -20.0f);
        Babypiece.push_back({sprite, -1.0f});
    }

    void spawnWatermelon() {
        sf::Sprite sprite(watermelonTexture);
        sprite.setScale(0.1f, 0.1f);
        sprite.setPosition(rand() % 800, -20.0f);
        watermelons.push_back({sprite, -1.0f});
    }

    void spawnWhip() {
        sf::Sprite sprite(whipTexture);
        sprite.setScale(0.05f, 0.05f);
        sprite.setPosition(rand() % 800, -20.0f);
        whips.push_back({sprite, 0.0f});
    }

    void spawnPolice() {
        sf::Sprite sprite(PoliceTexture);
        sprite.setScale(0.05f, 0.05f);
        sprite.setPosition(rand() % 800, -20.0f);
        Policeman.push_back({sprite, 0.0f});
    }

    void moveObjects() {
        for (auto& cotton : cottonPieces) {
            cotton.sprite.move(0.0f, cottonFallSpeed);
        }
        for (auto& watermelon : watermelons) {
            watermelon.sprite.move(0.0f, cottonFallSpeed);
        }
        for (auto& whip : whips) {
            whip.sprite.move(0.0f, cottonFallSpeed);
        }
        for (auto& KFC : KFCpiece) {
            KFC.sprite.move(0.0f, cottonFallSpeed);
        }
        for (auto& Baby : Babypiece) {
            Baby.sprite.move(0.0f, cottonFallSpeed);
        }
        for (auto& Police : Policeman) {
            Police.sprite.move(0.0f, cottonFallSpeed);
        }
    }



    void checkCollisions() {
        checkCottonCollision();
        checkWatermelonCollision();
        checkWhipCollision();
        checkPoliceCollision();
        checkKFCCollision();
        checkBabyCollision();
    }

    void checkCottonCollision() {
        for (auto it = cottonPieces.begin(); it != cottonPieces.end();) {
            if (it->sprite.getGlobalBounds().intersects(character.getGlobalBounds())) {
                score++;
                it = cottonPieces.erase(it);
            } else if (it->sprite.getPosition().y > window.getSize().y) {
                it = cottonPieces.erase(it);
            } else {
                ++it;
            }
        }
    }

    void checkKFCCollision() {
        for (auto it = KFCpiece.begin(); it != KFCpiece.end();) {
            if (it->sprite.getGlobalBounds().intersects(character.getGlobalBounds())) {
                score++;
                it = KFCpiece.erase(it);
            } else if (it->sprite.getPosition().y > window.getSize().y) {
                it = KFCpiece.erase(it);
            } else {
                ++it;
            }
        }
    }
    void checkWatermelonCollision() {
        for (auto it = watermelons.begin(); it != watermelons.end();) {
            if (it->sprite.getGlobalBounds().intersects(character.getGlobalBounds())) {
                timeRemaining += 5.0f;
                if (isSoundEnabled) {
                    watermelonSound.play(); // Play the whip sound only if sounds are enabled
                }

                it = watermelons.erase(it);
            } else if (it->sprite.getPosition().y > window.getSize().y) {
                it = watermelons.erase(it);
            } else {
                ++it;
            }
        }
    }

    void checkBabyCollision() {
        for (auto it = Babypiece.begin(); it != Babypiece.end();) {
            if (it->sprite.getGlobalBounds().intersects(character.getGlobalBounds())) {
                timeRemaining += 5.0f;
                if (isSoundEnabled) {
                    babySound.play(); // Play the whip sound only if sounds are enabled
                }


                it = Babypiece.erase(it);
            } else if (it->sprite.getPosition().y > window.getSize().y) {
                it = Babypiece.erase(it);
            } else {
                ++it;
            }
        }
    }

    void checkPoliceCollision() {
        for (auto it = Policeman.begin(); it != Policeman.end();) {
            sf::FloatRect policeBounds = it->sprite.getGlobalBounds();
            sf::FloatRect characterBounds = character.getGlobalBounds();

            // Adjust the bounding boxes for less sensitive collision detection
            policeBounds.left += policeBounds.width * 0.2f;
            policeBounds.width *= 0.6f;
            policeBounds.top += policeBounds.height * 0.2f;
            policeBounds.height *= 0.6f;

            characterBounds.left += characterBounds.width * 0.2f;
            characterBounds.width *= 0.6f;
            characterBounds.top += characterBounds.height * 0.2f;
            characterBounds.height *= 0.6f;

            if (policeBounds.intersects(characterBounds)) {
                if (isSoundEnabled) {
                    policeSound.play(); // Play the police sound only if sounds are enabled
                }

                playerHealth -= 10; // Decrease player's health
                if (playerHealth < 0) playerHealth = 0; // Ensure health doesn't go below 0
                it = Policeman.erase(it); // Remove the collided policeman from the game
            } else if (it->sprite.getPosition().y > window.getSize().y) {
                it = Policeman.erase(it); // Remove policemen that move off-screen
            } else {
                ++it; // Move to the next policeman
            }
        }
    }




    void updateText() {
        if (chosenTheme == "Ancient") {
            // Text for Ancient Theme
            scoreText.setString("Cottons: " + std::to_string(score) + " / " + std::to_string(target) +
                                "\nLevel: " + std::to_string(level));
            timeText.setString("Time Left: " + std::to_string(static_cast<int>(timeRemaining)) + " s");
        } else if (chosenTheme == "Modern") {
            // Text for Modern Theme
            scoreText.setString("KFC: " + std::to_string(score) + " / " + std::to_string(target) +
                                "\nLevel: " + std::to_string(level));
            timeText.setString("Time Left: " + std::to_string(static_cast<int>(timeRemaining)) + "s");
        }
    }
    void levelUp() {
        level++;
        if (level % 5 == 0) {
            whipSpawnFrequency = 20;
        }
        else {whipSpawnFrequency = 200;}

        target = 5;  // Increase the target score for the next level
        timeRemaining = 20.0f;  // Reset time for the new level
        score = 0;  // Reset the score for the new level
        cottonPieces.clear();  // Clear remaining cotton
        watermelons.clear();  // Clear remaining watermelons
        whips.clear();  // Clear remaining whips
        Babypiece.clear();
        Policeman.clear();
        KFCpiece.clear();




        playerHealth += 10;  // Increase health by 10 on each level-up
        if (playerHealth > 200) {
            playerHealth = 50;
            currency +=100;// Cap health at 100
        }
        healthBar.setSize(sf::Vector2f(playerHealth * 2.0f, 20.0f));


        // Increment currency by 5 if no whip collision occurred
        if (!whipCollided) {
            currency += 5;
        }


        // Add bonus score for completing the level before the target time
        if (timeRemaining > 0) {
            currency += static_cast<int>(timeRemaining / 2);  // Add half of the remaining seconds to the score
        }

        // Reset whip collision flag for the next level
        whipCollided = false;

        // Increment character texture index on level-up



        // Update background based on the level
        if (gameState == GameState::MODERNPLAY) {
            int backgroundIndex1 = level % 3; // Cycle through modern backgrounds
            background.setTexture(modernBackgroundTexture[backgroundIndex1]);

        }
        else if (gameState == GameState::PLAYING) {
            int backgroundIndex = (level % 3); // Cycle through ancient backgrounds
            background.setTexture(backgroundTextures[backgroundIndex]);
            if (currentCharacterIndex < 5) {
                currentCharacterIndex++;
                character.setTexture(characterTextures[currentCharacterIndex]);
                character.setScale(0.3f, 0.3f); // Adjust scaling for consistency
                 // Recenter the character after the texture change

            }

        }


        // Apply the chosen theme to keep the correct assets
        applyChosenTheme();

        // Display congratulatory message
        string congratulatoryMessage = "Congratulations! You've reached Level " + to_string(level) + "!";
        congratulatoryText.setString(congratulatoryMessage);
        congratulatoryText.setPosition(
            (window.getView().getSize().x - congratulatoryText.getLocalBounds().width) / 2,
            (window.getView().getSize().y - congratulatoryText.getLocalBounds().height) / 2);
        congratulatoryText.setFillColor(sf::Color::Green);

        congratsMessageDuration = 2.0f; // Display the message for 2 seconds
    }

    void checkWhipCollision() {
        for (auto it = whips.begin(); it != whips.end();) {
            sf::FloatRect whipBounds = it->sprite.getGlobalBounds();
            sf::FloatRect characterBounds = character.getGlobalBounds();

            // Adjust bounding boxes for collision detection
            whipBounds.left += whipBounds.width * 0.2f;
            whipBounds.width *= 0.6f;
            whipBounds.top += whipBounds.height * 0.2f;
            whipBounds.height *= 0.6f;

            characterBounds.left += characterBounds.width * 0.2f;
            characterBounds.width *= 0.6f;
            characterBounds.top += characterBounds.height * 0.2f;
            characterBounds.height *= 0.6f;

            if (whipBounds.intersects(characterBounds)) {
                playerHealth -= 10; // Decrease health
                if (playerHealth < 0) playerHealth = 0;


                if (gameState == GameState::PLAYING) {
                    // Change to a previous ancient character texture
                    if (currentCharacterIndex > 0) {
                        currentCharacterIndex--;
                        character.setTexture(characterTextures[currentCharacterIndex]);
                    }
                } else if (gameState == GameState::MODERNPLAY) {
                    // Change to a previous modern character texture
                    if (currentCharacterIndex > 0) {
                        character.setTexture(modernCharacterTexture);
                    }
                }

                if (currentCharacterIndex > 0) {
                    currentCharacterIndex--; // Decrement the texture index
                    character.setTexture(characterTextures[currentCharacterIndex]);
                    character.setScale(0.3f, 0.3f); // Adjust scaling for consistency
                    centerCharacter();
                }//
                applyChosenTheme();


                whipCollided = true;  // Set flag to indicate a whip collision
                playerHealth -= 10; // Decrease health
                if (playerHealth < 0) playerHealth = 0;
                if (isSoundEnabled) {
                    whipSound.play();  // Play the police sound only if sounds are enabled
                }

                it = whips.erase(it); // Remove the whip from the screen
            } else if (it->sprite.getPosition().y > window.getSize().y) {
                it = whips.erase(it); // Remove whips that move out of the screen
            } else {
                ++it;
            }
        }
    }
void render() {
    window.clear();

        sf::Color brightnessColor = sf::Color(
        static_cast<sf::Uint8>(255 * brightnessLevel),
        static_cast<sf::Uint8>(255 * brightnessLevel),
        static_cast<sf::Uint8>(255 * brightnessLevel)
    );

        // Apply brightness to the background
        background.setColor(brightnessColor);
        window.draw(background);

        // Apply brightness to the character
        character.setColor(brightnessColor);
        window.draw(character);

        if (gameState == GameState::THEME_SELECTION) {
            window.draw(background); // Draw the background

            // Prompt text for theme selection
            sf::Text themePrompt("Choose Your Theme", font, 36);
            themePrompt.setFillColor(sf::Color::White);

            // Set horizontal position to far left, and position it closer to the top
            themePrompt.setPosition(50.0f, 50.0f); // Adjusted to be near the top
            window.draw(themePrompt);

            // Ancient Theme option
            sf::Text ancientOption("A. Ancient Theme", font, 24);
            ancientOption.setFillColor(sf::Color::Yellow);

            // Set horizontal position to far left, and place it a little lower
            ancientOption.setPosition(50.0f, 120.0f); // Adjusted for spacing below the previous text
            window.draw(ancientOption);

            // Modern Theme option
            sf::Text modernOption("M. Modern Theme", font, 24);
            modernOption.setFillColor(sf::Color::Cyan);

            // Set horizontal position to far left, and place it further down
            modernOption.setPosition(50.0f, 180.0f); // Adjusted for spacing below the previous text
            window.draw(modernOption);

            // Instruction text
            sf::Text instruction("Press A or M to choose your theme", font, 20);
            instruction.setFillColor(sf::Color::White);

            // Set horizontal position to far left, and position it closer to the bottom
            instruction.setPosition(50.0f, 240.0f); // Adjusted to give more room for the theme options
            window.draw(instruction);
        }
    if (gameState == GameState::NAME_INPUT) {
            // Render name input screen
        window.draw(background);
            window.draw(namePromptText);
            playerNameDisplayText.setString(playerName + "_"); // Add cursor effect
            window.draw(playerNameDisplayText);

    }
    else if (gameState == GameState::MENU) {
        window.draw(background);
        window.draw(menuText);
    }
    else if (gameState == GameState::PLAYING) {
        window.draw(background);
        window.draw(character);

        for (const auto& cotton : cottonPieces) {
            auto cottonSprite = cotton.sprite;
            cottonSprite.setColor(brightnessColor);
            window.draw(cotton.sprite);
        }
        for (const auto& watermelon : watermelons) {
            auto watermelonSprite = watermelon.sprite;
            watermelonSprite.setColor(brightnessColor);
            window.draw(watermelon.sprite);
        }
        for (const auto& whip : whips) {
            auto whipSprite = whip.sprite;
            whipSprite.setColor(brightnessColor);
            window.draw(whip.sprite);
        }
        sf::Text currencyText("Score: " + to_string(currency), font, 24);
        currencyText.setFillColor(sf::Color::White);
        currencyText.setPosition(10.0f, 400.0f); // Adjust position as needed
        window.draw(currencyText);
        window.draw(scoreText);
        window.draw(timeText);

        // Draw the health bar
        window.draw(healthBarBackground);
        window.draw(healthBar);
        healthText.setString(healthBarText);
        window.draw(healthText);

        if (congratsMessageDuration > 0.0f) {
            window.draw(congratulatoryText);
        }
    }
    else if (gameState == GameState::MODERNPLAY) {
        window.draw(background);
        window.draw(character);

        for (const auto& KFC : KFCpiece) {
            window.draw(KFC.sprite);
        }
        for (const auto& Baby : Babypiece) {
            window.draw(Baby.sprite);
        }
        for (const auto& Police : Policeman) {
            window.draw(Police.sprite);
        }
        sf::Text currencyText("Score: " + to_string(currency), font, 24);
        currencyText.setFillColor(sf::Color::White);
        currencyText.setPosition(10.0f, 400.0f); // Adjust position as needed
        window.draw(currencyText);
        window.draw(scoreText);
        window.draw(timeText);

        // Draw the health bar
        window.draw(healthBarBackground);
        window.draw(healthBar);
        healthText.setString(healthBarText);
        window.draw(healthText);


        if (congratsMessageDuration > 0.0f) {
            window.draw(congratulatoryText);

        }
    }

    else if (gameState == GameState::GAME_OVER) {
        window.draw(background);
        window.draw(scoreText);

        sf::Text gameOverText("Game Over! Press R to Restart or Esc to Exit", font, 30);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(200.0f, 250.0f);
        window.draw(gameOverText);
    }
    else if (gameState == GameState::SETTINGS) {
        window.draw(background);  // Draw the background

        // Settings title
        float baseX = 20.0f;          // Left margin
    float baseY = 20.0f;          // Top margin
    float verticalSpacing = 50.0f; // Space between each element

    // Title
    sf::Text settingsTitle("Settings", font, 36);
    settingsTitle.setFillColor(sf::Color::White);
    settingsTitle.setPosition(baseX, baseY);  // Position at the top-left
    window.draw(settingsTitle);


        // Sound Toggle


        // Instructions for toggling sound


    // Volume Settings
    sf::Text volumeText("Volume:", font, 24);
    volumeText.setFillColor(sf::Color::White);
    volumeText.setPosition(baseX, baseY + verticalSpacing);  // Below the title
    window.draw(volumeText);

    // Volume bar
    volumeBar.setFillColor(sf::Color::Green);
    volumeBar.setPosition(baseX + 150.0f, baseY + verticalSpacing);  // To the right of the label
    window.draw(volumeBar);



    // Volume percentage text
    sf::Text volumePercentage(to_string(volumeLevel) + "%", font, 20);
    volumePercentage.setFillColor(sf::Color::Black);
    volumePercentage.setPosition(baseX + 250.0f, baseY + verticalSpacing);  // To the right of the bar
    window.draw(volumePercentage);

    // Brightness Settings
    sf::Text brightnessText("Brightness:", font, 24);
    brightnessText.setFillColor(sf::Color::White);
    brightnessText.setPosition(baseX, baseY + 2 * verticalSpacing);  // Below the volume
    window.draw(brightnessText);

    // Brightness bar background
    sf::RectangleShape brightnessBarBackground(sf::Vector2f(200.0f, 20.0f));
    brightnessBarBackground.setFillColor(sf::Color(50, 50, 50));
    brightnessBarBackground.setPosition(baseX + 150.0f, baseY + 2 * verticalSpacing); // To the right of the label
    window.draw(brightnessBarBackground);

    // Brightness bar
    sf::RectangleShape brightnessBar(sf::Vector2f(200.0f * (brightnessLevel ), 20.0f));
    brightnessBar.setFillColor(sf::Color::Yellow);
    brightnessBar.setPosition(baseX + 150.0f, baseY + 2 * verticalSpacing); // Same position as background
    window.draw(brightnessBar);

    // Brightness percentage
    sf::Text brightnessPercentage(to_string(static_cast<int>(brightnessLevel * 100)) + "%", font, 20);
    brightnessPercentage.setFillColor(sf::Color::Black);
    brightnessPercentage.setPosition(baseX + 250.0f, baseY + 2 * verticalSpacing); // To the right of the bar
    window.draw(brightnessPercentage);

    // Resume Button
    sf::Text resumeText("Press R to Resume", font, 24);
    resumeText.setFillColor(sf::Color::White);
    resumeText.setPosition(baseX, baseY + 3 * verticalSpacing);  // Below the brightness settings
    window.draw(resumeText);

    // Exit Option
    sf::Text exitText("Press Esc to Exit to Menu", font, 24);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(baseX, baseY + 4 * verticalSpacing);  // Below the resume text
    window.draw(exitText);

        sf::Text toggleInstruction("Press T to toggle sound", font, 20);
        toggleInstruction.setFillColor(sf::Color::White);
        toggleInstruction.setPosition(baseX, baseY+5* verticalSpacing);
        window.draw(toggleInstruction);

        sf::Text soundToggleText("" + std::string(isSoundEnabled ? "ON" : "OFF"), font, 24);
        soundToggleText.setFillColor(isSoundEnabled ? sf::Color::Green : sf::Color::Red);
        soundToggleText.setPosition(baseX*13, baseY+5* verticalSpacing);
        window.draw(soundToggleText);

    }
    else if (gameState == GameState::LEADERBOARD) {
        window.draw(background);  // Draw the background

        // Leaderboard title
        sf::Text leaderboardTitle("Leaderboard", font, 36);
        leaderboardTitle.setFillColor(sf::Color::White);
        leaderboardTitle.setPosition(10.0f, 10.0f);  // Align to top-left corner
        window.draw(leaderboardTitle);

        // Display leaderboard entries
        float yPosition = 50.0f;  // Start below the title
        for (const auto& entry : leaderboard) {
            sf::Text entryText(entry.first + ": score " + to_string(entry.second), font, 24);
            entryText.setFillColor(sf::Color::Cyan);
            entryText.setPosition(10.0f, yPosition);  // Align to top-left corner with padding
            window.draw(entryText);
            yPosition += 30.0f;  // Space between leaderboard entries
        }

        // Exit option (back to menu)
        sf::Text exitText("Press Esc to Return to Menu", font, 24);
        exitText.setFillColor(sf::Color::White);
        exitText.setPosition(10.0f, yPosition);  // Position it just below the leaderboard entries
        window.draw(exitText);


    }

    window.display();
}

    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) health = 0;  // Health can't go below 0
    }

    void heal(int amount) {
        health += amount;
        if (health > 100) health = 100;  // Health can't go above 100
    }

    void resetGame() {
        score = 0;
        level = 1;
        target = 5;
        currency = 0;
        timeRemaining = 20.0f;
        playerHealth = 50;  // Reset health to 100
        isGameOver = false;
        cottonPieces.clear();
        watermelons.clear();
        whips.clear();
        playerName.clear();
        gameState = GameState::MENU; // Go back to the menu
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}


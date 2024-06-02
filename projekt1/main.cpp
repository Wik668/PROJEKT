#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <sstream>
#include <iomanip>
#include <SFML/System/Vector2.hpp>
#include "Ammo.h"
#include "AnimatedSprite.h"
#include "Boss.h"
#include "Button.h"
#include "Fireball.h"
#include "Zombie.h"
#include "Slime.h"
#include "Bullet.h"
#include "ninja.h"
#include "utils.h"
#include "Menu.h"
#include "Medkit.h"
#include "EndGameMenu.h"
using namespace std;
using namespace sf;

class Game {
    vector<Zombie> zombies;
    sf::Texture zombie_texture;
    vector<Ninja> ninjas;
    sf::Texture ninja_texture;
    vector<Slime> slimes;
    sf::Texture slime_texture;
    vector<Boss> bosses;
    sf::Texture boss_texture;

private:
    float health;
    sf::Text healthText;
    int window_width = 800;
    int window_height = 600;
    sf::RenderWindow window;
    AnimatedSprite hero;
    sf::Texture character_texture;
    sf::Texture background_texture;
    sf::Sprite background_sprite;
    sf::Music gameMusic;
    sf::SoundBuffer buffer;
    sf::Sound sound;
    Menu menu;
    EndGameMenu endGameMenu;
    bool gameStarted;
    bool survivalMode;
    bool gameEnded;
    float move_speed;
    sf::Clock survivalClock;
    sf::Clock MedkitClock;
    sf::Text timerText;
    sf::Font font;
    sf::Texture bullet_texture;
    std::vector<Bullet> bullets;
    float bullet_speed;
    sf::Text killCounterText;
    int zombiesKilled;
    int Ninja_killed;
    int Slime_killed;
    sf::Texture survivalButtonTexture;
    sf::Texture survivalButtonTextureRed;
    sf::Texture stageButtonTexture;
    sf::Texture stageButtonTextureRed;
    sf::Text endGameText;
    float damage = 0.01;
    std::vector<SlimeProjectile> slimeProjectiles;
    std::vector<Fireball> Fireball;
    sf::Texture fireballtexture;
    sf::Texture slimeProjectileTexture;
    sf::Texture medkit_texture;
    std::vector<Medkit> medkits;
    int ammo = 30; // maximum bullets the hero can shoot before reloading
    bool reloading = false; // indicates if the hero is reloading
    sf::Clock reloadClock;
    std::vector<Ammo> ammoPacks; // Vector to store ammo packs
    sf::Texture ammo_texture; // Texture for ammo packs
    sf::Clock ammoRespawnClock; // Clock to manage ammo respawn time
    sf::Clock unlimitedAmmoClock; // Clock to manage unlimited ammo duration
    bool unlimitedAmmo = false;    // clock to measure reloading time
    sf::Text ammoText;
    Button backButton;
    Button playAgainButton;
    Button exitButton;
    bool timeRecorded;
    float survivalTime;
    int roundCounter; // moved from static inside checkBulletCollisions
    int licz;





    bool isFarEnough(const sf::Vector2f& position, float distance) {
        sf::Vector2f heroPosition = hero.getPosition();
        return std::abs(position.x - heroPosition.x) >= distance && std::abs(position.y - heroPosition.y) >= distance;
    }
    void spawnAmmo() {
        const int margin = 50;
        int maxX = window.getSize().x - margin;
        int maxY = window.getSize().y - margin;
        int minX = margin;
        int minY = margin;

        sf::Vector2f newPosition;
        do {
            int posX = rand() % (maxX - minX + 1) + minX;
            int posY = rand() % (maxY - minY + 1) + minY;
            newPosition = sf::Vector2f(static_cast<float>(posX), static_cast<float>(posY));
        } while (!isFarEnough(newPosition, 75.0f));

        ammoPacks.emplace_back(newPosition.x, newPosition.y, ammo_texture);
        ammoRespawnClock.restart();
    }

    void checkHeroAmmoCollisions() {
        for (auto it = ammoPacks.begin(); it != ammoPacks.end();) {
            if (checkCollision(hero, *it)) {
                unlimitedAmmo = true;
                unlimitedAmmoClock.restart();
                it = ammoPacks.erase(it); // Usuń zebrane ammo
                if (reloading) {
                    reloading = false;
                    updateAmmoText();
                }
            } else {
                ++it;
            }
        }
    }



    void loadResources() {
        if (!boss_texture.loadFromFile("boss.png")) {
            std::cout << "Nie udało się wczytać tekstury bossa" << std::endl;
            exit(1);
        }

        if (!ninja_texture.loadFromFile("enemy2.png")){
            cout<< "Nie udało się wczytać tekstur Ninjy" <<endl;
            exit(1);
        }
        if (!ammo_texture.loadFromFile("ammoP.png")) { // Load the texture for ammo packs
            std::cout << "Nie udało się wczytać tekstury amunicji" << std::endl;
            exit(1);
        }
        if (!medkit_texture.loadFromFile("apteczka.png")) {
            std::cout << "Nie udało się wczytać tekstury apteczki" << std::endl;
            exit(1);
        }
        if (!slimeProjectileTexture.loadFromFile("Slimeball.png")) {
            std::cout << "Nie udało się wczytać tekstury pocisku slime" << std::endl;
            exit(1);
        }
        if (!fireballtexture.loadFromFile("fireball.png")) {
            std::cout << "Nie udało się wczytać tekstury pocisku fireball" << std::endl;
            exit(1);
        }

        if (!background_texture.loadFromFile("puste.png")) {
            cout << "Nie udało się wczytać tekstury tła" << endl;
            exit(1);
        }
        if (!character_texture.loadFromFile("Walk.png")) {
            cout << "Nie udało się wczytać tekstury bohatera" << endl;
            exit(1);
        }
        if (!zombie_texture.loadFromFile("Walk_Zombie.png")) {
            cout << "Nie udało się wczytać tekstury zombie" << endl;
            exit(1);
        }
        if (!slime_texture.loadFromFile("Slime.png")) {
            cout << "Nie udało się wczytać tekstury slime" << endl;
            exit(1);
        }
        if (!buffer.loadFromFile("menu_music.wav")) {
            std::cout << "Nie udało się wczytać dźwięku menu" << std::endl;
        }
        if (!bullet_texture.loadFromFile("bullet.png")) {
            std::cout << "Nie udało się wczytać tekstury pocisku" << std::endl;
            exit(1);
        }
        sound.setBuffer(buffer);
        sound.setLoop(true);
        if (!gameMusic.openFromFile("blanka.wav")) {
            std::cout << "Nie udało się wczytać muzyki" << std::endl;
            exit(1);
        }
        background_sprite.setTexture(background_texture);
    }

    void playSound() {
        sound.play();
    }

    void stopSound() {
        sound.stop();
    }
    void initializeMedkits() {
        for (int i = 0; i < 3; ++i) {
            const int margin = 50;
            int maxX = window.getSize().x - margin;
            int maxY = window.getSize().y - margin;
            int minX = margin;
            int minY = margin;

            // Generowanie losowej pozycji z uwzględnieniem marginesu
            int posX = rand() % (maxX - minX + 1) + minX;
            int posY = rand() % (maxY - minY + 1) + minY;

            // Tworzenie apteczki w losowej pozycji
            createMedkit(posX, posY);

        }
    }
    void createMedkit(float x, float y) {
        Medkit medkit(20.0f); // Przykładowa wartość leczenia
        medkit.setTexture(medkit_texture);
        medkit.setPosition(x, y);
        medkit.setScale(0.09, 0.09); // Skalowanie jeśli potrzebne
        medkits.push_back(medkit);
    }
    void checkHeroMedkitCollisions() {
        for (auto it = medkits.begin(); it != medkits.end();) {
            if (checkCollision(hero, *it)) {
                health = std::min(100.0f, health + it->getHealingAmount());
                updateHealthText();
                it = medkits.erase(it); // Usunięcie zebranej apteczki
            } else {
                ++it;
            }
        }
    }

    void initializeHero() {
        hero.setTexture(character_texture);
        hero.add_animation_frame_right(IntRect(9, 70, 14, 25));
        hero.add_animation_frame_right(IntRect(41, 70, 14, 25));
        hero.add_animation_frame_right(IntRect(73, 70, 14, 25));
        hero.add_animation_frame_right(IntRect(105, 70, 14, 25));
        hero.add_standing_frame_right(IntRect(134, 70, 14, 25));

        hero.add_animation_frame_left(IntRect(10, 102, 14, 25));
        hero.add_animation_frame_left(IntRect(41, 102, 14, 25));
        hero.add_animation_frame_left(IntRect(73, 102, 14, 25));
        hero.add_animation_frame_left(IntRect(105, 102, 14, 25));
        hero.add_standing_frame_left(IntRect(134, 102, 14, 25));

        hero.add_animation_frame_up(IntRect(10, 38, 14, 25));
        hero.add_animation_frame_up(IntRect(41, 38, 14, 25));
        hero.add_animation_frame_up(IntRect(73, 38, 14, 25));
        hero.add_animation_frame_up(IntRect(105, 38, 14, 25));
        hero.add_standing_frame_up(IntRect(134, 38, 14, 25));

        hero.add_animation_frame_down(IntRect(9, 6, 14, 25));
        hero.add_animation_frame_down(IntRect(41, 6, 14, 25));
        hero.add_animation_frame_down(IntRect(73, 6, 14, 25));
        hero.add_animation_frame_down(IntRect(105, 6, 14, 25));
        hero.add_standing_frame_down(IntRect(134, 6, 14, 25));

        hero.setScale(2,2);
        hero.setPosition(250, 250);

    }


    void addNinjaAnimationFrames(Ninja& ninja) {
        std::vector<IntRect> frames = {
            IntRect(6, 100, 17, 26),
            IntRect(39, 100, 17, 26),
            IntRect(71, 100, 17, 26),
            IntRect(103, 100, 17, 26),
            IntRect(134, 100, 17, 26),
            IntRect(167, 100, 17, 26),
            IntRect(199, 100, 17, 26),
            IntRect(231, 100, 17, 26)
        };

        for (const auto& frame : frames) {
            ninja.add_animation_frame_right(frame);
        }
    }
    void addSlimeAnimationFrames(Slime& slime) {
        std::vector<IntRect> frames = {
            IntRect(3, 4, 38, 26),
            IntRect(46, 5, 38, 26),
            IntRect(88, 5, 38, 26),
            IntRect(133, 5, 38, 26),
            IntRect(178, 5, 38, 26),
            IntRect(223, 5, 38, 26),
            IntRect(268, 2, 38, 26),
            IntRect(313, 1, 38, 26),
            IntRect(356, 1, 38, 26),
            IntRect(400, 2, 38, 26)
        };

        for (const auto& frame : frames) {
            slime.add_animation_frame_right(frame);
        }
    }

    void addZombieAnimationFrames(Zombie& zombie) {
        std::vector<IntRect> frames = {
            IntRect(11, 69, 14, 26),
            IntRect(42, 69, 14, 26),
            IntRect(74, 69, 14, 26),
            IntRect(107, 69, 14, 26),
            IntRect(140, 69, 14, 26),
            IntRect(173, 69, 14, 26),
            IntRect(204, 69, 14, 26),
            IntRect(231, 100, 17, 26),
            IntRect(269, 69, 14, 26),
            IntRect(300, 69, 14, 26)
        };

        for (const auto& frame : frames) {
            zombie.add_animation_frame_right(frame);
        }
    }
    void addBossAnimationFrames(Boss& boss) {
        std::vector<sf::IntRect> frames = {
            sf::IntRect(108, 219, 64, 100),
            sf::IntRect(397, 221, 64, 100),
            sf::IntRect(685, 219, 64, 100),
            sf::IntRect(973, 220, 64, 100),
            sf::IntRect(1260, 220, 64, 100),
            sf::IntRect(1549, 220, 64, 100),
            sf::IntRect(1834, 220, 64, 100),
            sf::IntRect(2122, 220, 64, 100),
            sf::IntRect(2410, 220, 64, 100),
            sf::IntRect(2700, 220, 64, 100),
            sf::IntRect(2989, 220, 64, 100),
            sf::IntRect(3276, 220, 64, 100)
        };

        for (const auto& frame : frames) {
            boss.add_animation_frame_right(frame);
        }
    }

    void createBoss() {
        const int margin = 50;
        int maxX = window.getSize().x - margin;
        int maxY = window.getSize().y - margin;
        int minX = margin;
        int minY = margin;

        sf::Vector2f newPosition;
        do {
            int posX = rand() % (maxX - minX + 1) + minX;
            int posY = rand() % (maxY - minY + 1) + minY;
            newPosition = sf::Vector2f(static_cast<float>(posX), static_cast<float>(posY));
        } while (!isFarEnough(newPosition, 75.0f));

        Boss boss(5);
        boss.setTexture(boss_texture);
        addBossAnimationFrames(boss); // You need to implement this function similarly to how you did for Slime
        boss.setTextureRect(sf::IntRect(108, 219, 64, 100)); // Set initial frame
        boss.setScale(2, 2);
        boss.setPosition(newPosition);
        boss.step();
        bosses.push_back(boss);
    }

    void createNinja() {
        const int margin = 50;
        int maxX = window.getSize().x - margin;
        int maxY = window.getSize().y - margin;
        int minX = margin;
        int minY = margin;

        sf::Vector2f newPosition;
        do {
            int posX = rand() % (maxX - minX + 1) + minX;
            int posY = rand() % (maxY - minY + 1) + minY;
            newPosition = sf::Vector2f(static_cast<float>(posX), static_cast<float>(posY));
        } while (!isFarEnough(newPosition, 75.0f));

        Ninja ninja(3);
        ninja.setTexture(ninja_texture);
        addNinjaAnimationFrames(ninja);
        ninja.setTextureRect(sf::IntRect(6, 100, 17, 26));
        ninja.setScale(1, 1);
        ninja.setPosition(newPosition);
        ninja.step();
        ninjas.push_back(ninja);
    }
    void createZombie() {
        const int margin = 50;
        int maxX = window.getSize().x - margin;
        int maxY = window.getSize().y - margin;
        int minX = margin;
        int minY = margin;

        sf::Vector2f newPosition;
        do {
            int posX = rand() % (maxX - minX + 1) + minX;
            int posY = rand() % (maxY - minY + 1) + minY;
            newPosition = sf::Vector2f(static_cast<float>(posX), static_cast<float>(posY));
        } while (!isFarEnough(newPosition, 75.0f));

        Zombie zombie(5);
        zombie.setTexture(zombie_texture);
        addZombieAnimationFrames(zombie);
        zombie.setTextureRect(sf::IntRect(11, 69, 14, 26));
        zombie.setScale(2, 2);
        zombie.setPosition(newPosition);
        zombie.step();
        zombies.push_back(zombie);
    }


    void createSlime() {
        const int margin = 50;
        int maxX = window.getSize().x - margin;
        int maxY = window.getSize().y - margin;
        int minX = margin;
        int minY = margin;

        sf::Vector2f newPosition;
        do {
            int posX = rand() % (maxX - minX + 1) + minX;
            int posY = rand() % (maxY - minY + 1) + minY;
            newPosition = sf::Vector2f(static_cast<float>(posX), static_cast<float>(posY));
        } while (!isFarEnough(newPosition, 75.0f));

        Slime slime(5);
        slime.setTexture(slime_texture);
        addSlimeAnimationFrames(slime);
        slime.setTextureRect(sf::IntRect(3, 4, 38, 26)); // Set initial frame
        slime.setScale(2, 2);
        slime.setPosition(newPosition);
        slime.step();
        slimes.push_back(slime);
    }
public:
    Game()
        : health(100), window(sf::VideoMode(window_width, window_height), "Game Window"), hero(5),
        endGameMenu(),
        gameStarted(false),
        gameEnded(false), move_speed(0.1f), bullet_speed(0.5f), // Inicjalizacja zmiennych
        zombiesKilled(0), Ninja_killed(0), Slime_killed(0), backButton(sf::Vector2f(200, 50), font, "Back to menu", sf::Vector2f(300, 300)),
        playAgainButton(sf::Vector2f(200, 50), font, "Play again", sf::Vector2f(300, 400)), // Initialize endGameMenu
        exitButton(sf::Vector2f(200, 50), font, "Exit", sf::Vector2f(300, 500)),
        timeRecorded(false),
        survivalTime(0.0f),
        roundCounter(0), // Initialize counters
        licz(0)    {

        loadResources();
        initializeHero();
        createZombie();
        createNinja();
        createSlime();


        initializeMedkits();
        background_sprite.setTexture(background_texture);

        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Nie udało się wczytać czcionki" << std::endl;
            exit(1);
        }
        timerText.setFont(font);
        timerText.setCharacterSize(24);
        timerText.setFillColor(sf::Color::White);
        timerText.setPosition(10, 10);

        healthText.setFont(font);
        healthText.setCharacterSize(24);
        healthText.setFillColor(sf::Color::White);
        healthText.setPosition(10, window_height - 30);
        updateHealthText();

        endGameText.setFont(font);
        endGameText.setCharacterSize(48);
        endGameText.setFillColor(sf::Color::Red);
        endGameText.setString("Game Over");
        endGameText.setPosition(window_width / 2 - endGameText.getGlobalBounds().width / 2,
                                window_height / 2 - endGameText.getGlobalBounds().height / 2);

        killCounterText.setFont(font);
        killCounterText.setCharacterSize(24);
        killCounterText.setFillColor(sf::Color::White);
        killCounterText.setPosition(window_width - 150, 10);
        updateKillCounterText();

        ammoText.setFont(font);
        ammoText.setCharacterSize(24);
        ammoText.setFillColor(sf::Color::White);
        ammoText.setPosition(10, window_height - 60);
        updateAmmoText();
    }

    void updateHealthText() {
        float displayHealth = std::max(0.0f, health);
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << displayHealth;
        healthText.setString("HP: " + ss.str());
    }
    void spawnMedkit() {

        const int margin = 50;
        int maxX = window.getSize().x - margin;
        int maxY = window.getSize().y - margin;
        int minX = margin;
        int minY = margin;

        sf::Vector2f newPosition;
        do {
            int posX = rand() % (maxX - minX + 1) + minX;
            int posY = rand() % (maxY - minY + 1) + minY;
            newPosition = sf::Vector2f(static_cast<float>(posX), static_cast<float>(posY));
        } while (!isFarEnough(newPosition, 75.0f));
        createMedkit(newPosition.x, newPosition.y);
        MedkitClock.restart();
    }


    void checkHeroZombieCollisions() {
        for (auto& zombie : zombies) {
            if (checkCollision(hero, zombie)) {
                health -= damage;
                updateHealthText();
                if (health <= 0.0) {
                    gameEnded = true;
                    gameMusic.stop();
                }
            }
        }
    }
    void checkHeroNinjaCollisions() {
        for (auto& ninja : ninjas) {
            if (checkCollision(hero, ninja)) {
                health -= damage;
                updateHealthText();
                if (health <= 0.0) {
                    gameEnded = true;
                    gameMusic.stop();
                }
            }
        }
    }
    void checkHeroSlimeCollisions() {
        for (auto& slime : slimes) {
            if (checkCollision(hero, slime)) {
                health -= damage*10;
                updateHealthText();
                if (health <= 0.0) {
                    gameEnded = true;
                    gameMusic.stop();
                }
            }
        }
    }
    void checkHeroBossCollisions() {
        for (auto& boss : bosses) {
            if (checkCollision(hero, boss)) {
                health -= damage*10;
                updateHealthText();
                if (health <= 0.0) {
                    gameEnded = true;
                    gameMusic.stop();
                }
            }
        }
    }

    void run() {
        menu.playSound();
        while (window.isOpen()) {

            handleEvents();
            update();
            render();
        }

        gameMusic.stop();
    }
    void resetGame() {
        // Reset game state variables
        health = 100;
        updateHealthText();
        ammo = 30;
        reloading = false;
        unlimitedAmmo = false;
        bullets.clear();
        zombies.clear();
        ninjas.clear();
        slimes.clear();
        bosses.clear(); // Clear the bosses if you have them
        medkits.clear();
        ammoPacks.clear();

        // Reset kills and time
        zombiesKilled = 0;
        Ninja_killed = 0;
        Slime_killed = 0;
        updateKillCounterText();
        updateAmmoText();

        // Reset flags
        gameEnded = false;
        timeRecorded = false;
        survivalTime = 0.0f;
        survivalClock.restart();
        MedkitClock.restart();

        // Reset round counter
        roundCounter = 0;
        licz = 0;

        // Add initial entities again if needed
        createZombie();
        createNinja();
        createSlime();
        initializeMedkits();
    }
    void shootBullet() {
        if ((ammo > 0 || unlimitedAmmo) && !reloading) {
            sf::Vector2f heroCenter = hero.getPosition() + sf::Vector2f(hero.getGlobalBounds().width / 2, hero.getGlobalBounds().height / 2);
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f target = window.mapPixelToCoords(mousePosition);
            sf::Vector2f direction = normalize(target - heroCenter);
            float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265;
            Bullet bullet(bullet_texture, direction, bullet_speed, angle);
            bullet.setPosition(heroCenter);
            bullets.push_back(bullet);
            if (!unlimitedAmmo) {
                ammo--;
                updateAmmoText();
            }
        }
    }


    void reload() {
        if (!reloading && !unlimitedAmmo) {
            reloading = true;
            reloadClock.restart();
            ammoText.setString("Reloading...");
        }
    }

    void updateAmmoText() {
        std::stringstream ss;
        if (unlimitedAmmo) {
            ss << "Ammo: Unlimited";
        } else {
            ss << "Ammo: " << ammo;
        }
        ammoText.setString(ss.str());
    }



    void checkBulletCollisions() {
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
            bool bulletErased = false;
            for (auto zombieIt = zombies.begin(); zombieIt != zombies.end();) {
                if (checkCollision(*bulletIt, *zombieIt)) {
                    bulletIt = bullets.erase(bulletIt);
                    zombieIt->takeDamage(50);
                    bulletErased = true;
                    if (zombieIt->getHealth() <= 0) {
                        zombieIt = zombies.erase(zombieIt);
                        zombiesKilled++;
                        updateKillCounterText();
                    } else {
                        ++zombieIt;
                    }
                    break;
                } else {
                    ++zombieIt;
                }
            }

            if (!bulletErased) {
                for (auto ninjaIt = ninjas.begin(); ninjaIt != ninjas.end();) {
                    if (checkCollision(*bulletIt, *ninjaIt)) {
                        bulletIt = bullets.erase(bulletIt);
                        ninjaIt->takeDamage(25);
                        bulletErased = true;
                        if (ninjaIt->getHealth() <= 0) {
                            ninjaIt = ninjas.erase(ninjaIt);
                            Ninja_killed++;
                            updateKillCounterText();
                        } else {
                            ++ninjaIt;
                        }
                        break;
                    } else {
                        ++ninjaIt;
                    }
                }
            }

            if (!bulletErased) {
                for (auto slimeIt = slimes.begin(); slimeIt != slimes.end();) {
                    if (checkCollision(*bulletIt, *slimeIt)) {
                        bulletIt = bullets.erase(bulletIt);
                        slimeIt->takeDamage(10);
                        bulletErased = true;
                        if (slimeIt->getHealth() <= 0) {
                            slimeIt = slimes.erase(slimeIt);
                            Slime_killed++;
                            updateKillCounterText();
                        } else {
                            ++slimeIt;
                        }
                        break;
                    } else {
                        ++slimeIt;
                    }
                }
            }
            if (!bulletErased) {
                for (auto bossIt = bosses.begin(); bossIt != bosses.end();) {
                    if (checkCollision(*bulletIt, *bossIt)) {
                        bulletIt = bullets.erase(bulletIt);
                        bossIt->takeDamage(5);
                        bulletErased = true;
                        if (bossIt->getHealth() <= 0) {
                            bossIt = bosses.erase(bossIt);
                            Slime_killed++;
                            updateKillCounterText();
                        } else {
                            ++bossIt;
                        }
                        break;
                    } else {
                        ++bossIt;
                    }
                }
            }

            if (!bulletErased) {
                ++bulletIt;
            }
        }

        bool allZombiesKilled = std::all_of(zombies.begin(), zombies.end(), [](const Zombie& z){ return z.getHealth() <= 0.0; });
        bool allNinjasKilled = std::all_of(ninjas.begin(), ninjas.end(), [](const Ninja& n){ return n.getHealth() <= 0.0; });
        bool allSlimesKilled = std::all_of(slimes.begin(), slimes.end(), [](const Slime& n){ return n.getHealth() <= 0.0; });
        bool allBossesKilled = std::all_of(bosses.begin(), bosses.end(), [](const Boss& n){ return n.getHealth() <= 0.0; });
        if (allZombiesKilled && zombies.empty()&& allBossesKilled && bosses.empty()&& allSlimesKilled && slimes.empty() && allNinjasKilled && ninjas.empty()) {
            roundCounter++;
            spawnMedkit();
            if (roundCounter % 3 != 0) {
                for (int i = 0; i <= roundCounter; i++) {
                    createZombie();
                    createNinja();
                    createSlime();
                }
            }

            if (roundCounter % 3 == 0) {
                createBoss();
                licz++;
                for (int i = 0; i <= licz; i++) {
                    createZombie();
                    createNinja();
                    createSlime();
                }
            }
        }
    }


    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else if (event.key.code == sf::Keyboard::Space) {
                    if (!gameEnded) {
                        shootBullet();
                    }
                } else if (event.key.code == sf::Keyboard::R) {
                    if (!gameEnded) {
                        reload();
                    }
                } else if (gameEnded && event.key.code == sf::Keyboard::Enter) {
                    window.close();
                }
            } else if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (gameEnded) {
                    endGameMenu.update(mousePos);
                } else {
                    updateButtonColors(mousePos);
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (!gameStarted && !gameEnded) {
                        if (menu.isMouseOverButton(menu.survivalButton, mousePos)) {
                            gameStarted = true;
                            survivalMode = true;
                            menu.stopSound();
                            survivalClock.restart();
                            MedkitClock.restart();
                            gameMusic.setLoop(true);
                            gameMusic.play();
                        } else if (menu.isMouseOverButton(menu.stageButton, mousePos)) {
                            gameStarted = true;
                            survivalMode = false;
                            menu.stopSound();
                            survivalClock.restart();
                            MedkitClock.restart();
                            gameMusic.setLoop(true);
                            gameMusic.play();
                        }
                    } else if (gameEnded) {
                        if (endGameMenu.isMouseOverButton(endGameMenu.backToMenuButton, mousePos)) {
                            gameEnded = false;
                            gameStarted = false;
                            menu.playSound();
                            resetGame(); // Reset the game state
                        } else if (endGameMenu.isMouseOverButton(endGameMenu.playAgainButton, mousePos)) {
                            resetGame(); // Reset the game state
                            gameStarted = true;
                            gameMusic.setLoop(true);
                            gameMusic.play();
                        } else if (endGameMenu.isMouseOverButton(endGameMenu.exitButton, mousePos)) {
                            window.close();
                        }
                    }
                }
            }
        }
    }


    void updateButtonColors(const sf::Vector2i& mousePos) {
        if (menu.isMouseOverButton(menu.survivalButton, mousePos)) {
            menu.survivalButton.setTexture(&survivalButtonTextureRed);
        } else {
            menu.survivalButton.setTexture(&survivalButtonTexture);
        }

        if (menu.isMouseOverButton(menu.stageButton, mousePos)) {
            menu.stageButton.setTexture(&stageButtonTextureRed);
        } else {
            menu.stageButton.setTexture(&stageButtonTexture);
        }
    }



    void update() {
        if (gameStarted && !gameEnded) {
            if (reloading) {
                if (reloadClock.getElapsedTime().asSeconds() >= 3.0f) {
                    ammo = 20;
                    reloading = false;
                    updateAmmoText();
                }
            }

            if (unlimitedAmmo) {
                if (unlimitedAmmoClock.getElapsedTime().asSeconds() >= 5.0f) {
                    unlimitedAmmo = false;
                    updateAmmoText();
                } else {
                    ammoText.setString("Ammo: Unlimited");
                }
            } else {
                if (ammoRespawnClock.getElapsedTime().asSeconds() >= 5.0f) {
                    spawnAmmo();
                }
            }

            // Existing update logic...
            FloatRect bounds(0, 0, window_width, window_height);
            checkHeroMedkitCollisions();
            checkHeroAmmoCollisions(); // Check for ammo collisions

            if (Keyboard::isKeyPressed(Keyboard::A)) {
                hero.moveWithCollision(bounds, -move_speed, 0);
            } else if (Keyboard::isKeyPressed(Keyboard::D)) {
                hero.moveWithCollision(bounds, move_speed, 0);
            } else if (Keyboard::isKeyPressed(Keyboard::W)) {
                hero.moveWithCollision(bounds, 0, -move_speed);
            } else if (Keyboard::isKeyPressed(Keyboard::S)) {
                hero.moveWithCollision(bounds, 0, move_speed);
            }
            hero.step();

            Vector2f playerPosition = hero.getPosition();

            for (auto& slime : slimes) {
                slime.shoot(slimeProjectiles, slimeProjectileTexture, hero.getPosition());
            }

            for (auto& projectile : slimeProjectiles) {
                projectile.update();
                if (checkCollision(hero, projectile)) {
                    health -= damage * 500;
                    updateHealthText();
                    projectile = slimeProjectiles.back();
                    slimeProjectiles.pop_back();
                    if (health <= 0.0) {
                        gameEnded = true;
                        gameMusic.stop();
                        survivalTime = survivalClock.getElapsedTime().asSeconds();
                        int totalKills = zombiesKilled + Ninja_killed + Slime_killed;
                        endGameMenu.updateStats(survivalTime, totalKills);
                    }
                }
            }
            slimeProjectiles.erase(std::remove_if(slimeProjectiles.begin(), slimeProjectiles.end(),
                                                  [&bounds](const SlimeProjectile& p) {
                                                      return !bounds.intersects(p.getGlobalBounds());
                                                  }), slimeProjectiles.end());

            for (auto& boss : bosses) {
                boss.shoot(Fireball, fireballtexture, hero.getPosition());
                boss.step(); // Dodanie kroku animacji
            }

            for (auto& projectile : Fireball) {
                projectile.update();
                if (checkCollision(hero, projectile)) {
                    health -= damage * 500;
                    updateHealthText();
                    projectile = Fireball.back();
                    Fireball.pop_back();
                    if (health <= 0.0) {
                        gameEnded = true;
                        gameMusic.stop();
                        survivalTime = survivalClock.getElapsedTime().asSeconds();
                        int totalKills = zombiesKilled + Ninja_killed + Slime_killed;
                        endGameMenu.updateStats(survivalTime, totalKills);
                    }
                }
            }
            Fireball.erase(std::remove_if(Fireball.begin(), Fireball.end(),
                                          [&bounds](const class Fireball& p) {
                                              return !bounds.intersects(p.getGlobalBounds());
                                          }), Fireball.end());

            for (auto& zombie : zombies) {
                Vector2f zombiePosition = zombie.getPosition();
                Vector2f direction = playerPosition - zombiePosition;
                direction = normalize(direction);

                float moveX = direction.x * move_speed * 0.2;
                float moveY = direction.y * move_speed * 0.2;
                zombie.moveWithCollision(bounds, moveX, moveY);
                zombie.step();
            }

            for (auto& ninja : ninjas) {
                Vector2f ninjaPosition = ninja.getPosition();
                Vector2f direction = playerPosition - ninjaPosition;
                direction = normalize(direction);

                float moveX = direction.x * move_speed * 0.5;
                float moveY = direction.y * move_speed * 0.5;
                ninja.moveWithCollision(bounds, moveX, moveY);
                ninja.step();
            }

            for (auto& slime : slimes) {
                Vector2f slimePosition = slime.getPosition();
                Vector2f direction = playerPosition - slimePosition;
                direction = normalize(direction);

                float moveX = direction.x * move_speed * 0.05;
                float moveY = direction.y * move_speed * 0.05;
                slime.moveWithCollision(bounds, moveX, moveY);
                slime.step();
            }
            for (auto& boss : bosses) {
                Vector2f bossPosition = boss.getPosition();
                Vector2f direction = playerPosition - bossPosition;
                direction = normalize(direction);

                float moveX = direction.x * move_speed * 0.05;
                float moveY = direction.y * move_speed * 0.05;
                boss.moveWithCollision(bounds, moveX, moveY);
            }

            checkHeroZombieCollisions();
            checkHeroNinjaCollisions();
            checkHeroSlimeCollisions();
            checkHeroBossCollisions();

            if (survivalMode) {
                Time elapsed = survivalClock.getElapsedTime();
                std::stringstream ss;
                ss << "Survival Time: " << std::fixed << std::setprecision(1) << elapsed.asSeconds() << " seconds";
                timerText.setString(ss.str());
            }

            for (auto& bullet : bullets) {
                bullet.update();
            }

            checkBulletCollisions();
        } else if (!gameStarted && !gameEnded) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            menu.update(mousePos);
        } else if (gameEnded) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            endGameMenu.update(mousePos);

            if (!timeRecorded) {
                survivalTime = survivalClock.getElapsedTime().asSeconds();
                timeRecorded = true;
            }

            int totalKills = zombiesKilled + Ninja_killed + Slime_killed;
            endGameMenu.updateStats(survivalTime, totalKills);
        }
    }


    void render() {
        window.clear();
        if (gameStarted) {
            for (int x = 0; x < window_width; x += background_texture.getSize().x) {
                for (int y = 0; y < window_height; y += background_texture.getSize().y) {
                    background_sprite.setPosition(x, y);
                    window.draw(background_sprite);
                }
            }
            window.draw(hero);

            for (const auto& zombie : zombies) {
                window.draw(zombie);
            }
            for (const auto& medkit : medkits) {
                window.draw(medkit);
            }
            for (const auto& ninja : ninjas) {
                window.draw(ninja);
            }
            for (const auto& slime : slimes) {
                window.draw(slime);
            }
            for (const auto& boss : bosses) {
                window.draw(boss);
            }
            for (const auto& ammo : ammoPacks) {
                window.draw(ammo);
            }
            for (const auto& bullet : bullets) {
                window.draw(bullet);
            }
            for (const auto& projectile : slimeProjectiles) {
                window.draw(projectile);
            }
            for (const auto& projectile : Fireball) {
                window.draw(projectile);
            }

            if (survivalMode) {
                window.draw(timerText);
            }
            window.draw(healthText);
            window.draw(killCounterText);
            window.draw(ammoText);

            if (gameEnded) {
                endGameMenu.draw(window);
            }
        } else {
            menu.draw(window);
        }
        window.display();
    }





    void updateKillCounterText() {
        std::stringstream ss;
        ss << "Kills: " << zombiesKilled + Ninja_killed + Slime_killed;
        killCounterText.setString(ss.str());
    }
    bool checkCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
        return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
    }
    void resolveCollision(sf::Sprite& sprite1, sf::Sprite& sprite2) {
        sf::FloatRect bounds1 = sprite1.getGlobalBounds();
        sf::FloatRect bounds2 = sprite2.getGlobalBounds();
        float overlapLeft = bounds1.left + bounds1.width - bounds2.left;
        float overlapRight = bounds2.left + bounds2.width - bounds1.left;
        float overlapTop = bounds1.top + bounds1.height - bounds2.top;
        float overlapBottom = bounds2.top + bounds2.height - bounds1.top;
        bool fromLeft = abs(overlapLeft) < abs(overlapRight);
        bool fromTop = abs(overlapTop) < abs(overlapBottom);
        float overlapX = fromLeft ? overlapLeft : overlapRight;
        float overlapY = fromTop ? overlapTop : overlapBottom;

        if (abs(overlapX) < abs(overlapY)) {
            sprite1.move(-overlapX / 2, 0);
            sprite2.move(overlapX / 2, 0);
        } else {
            sprite1.move(0, -overlapY / 2);
            sprite2.move(0, overlapY / 2);
        }
    }
};
int main() {
    Game game;
    game.run();
    return 0;
};

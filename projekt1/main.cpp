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
#include "Syringe.h"
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
    std::vector<std::unique_ptr<Zombie>> zombies;
    sf::Texture zombie_texture;
    vector<Ninja> ninjas;
    sf::Texture ninja_texture;
    vector<Slime> slimes;
    sf::Texture slime_texture;
    vector<Boss> bosses;
    sf::Texture boss_texture;
    std::vector<Syringe> syringes;
    sf::Texture syringe_texture;
    sf::Clock syringeClock;
    sf::Clock shootClock;
    bool invulnerable = false;
private:

    bool tripleShotActive = false;
    sf::Clock tripleShotClock;
    float health;
    float orgmove_speed = 0.45;
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
    float damage = 1.0;
    std::vector<SlimeProjectile> slimeProjectiles;
    std::vector<Fireball> Fireball;
    sf::Texture fireballtexture;
    sf::Texture slimeProjectileTexture;
    sf::Texture medkit_texture;
    std::vector<Medkit> medkits;
    int ammo = 30;
    bool reloading = false;
    sf::Clock reloadClock;
    std::vector<Ammo> ammoPacks;
    sf::Texture ammo_texture;
    sf::Clock ammoRespawnClock;
    sf::Clock unlimitedAmmoClock;
    bool unlimitedAmmo = false;
    sf::Text ammoText;
    Button backButton;
    Button playAgainButton;
    Button exitButton;
    bool timeRecorded;
    float survivalTime;
    int roundCounter;
    int licz;
    sf::Clock medkitRespawnClock;
    sf::Clock syringeRespawnClock;
    sf::Text roundText;
    sf::Clock roundTextClock;
    bool showingRoundText;
    bool roundStarted;

    void updateTimerText() {
        sf::Time elapsed = survivalClock.getElapsedTime();
        std::stringstream ss;
        ss << "Time: " << std::fixed << std::setprecision(1) << elapsed.asSeconds() <<"s";
        timerText.setString(ss.str());
    }

    bool isFarEnough(const sf::Vector2f& position, float distance) {
        sf::Vector2f heroPosition = hero.getPosition();
        return std::abs(position.x - heroPosition.x) >= distance && std::abs(position.y - heroPosition.y) >= distance;
    }

    void exitWithError(const std::string& errorMessage) {
        std::cout << errorMessage << std::endl;
        exit(1);
    }

    void loadResources() {
        std::vector<std::pair<std::string, sf::Texture*>> textures = {
            {"boss.png", &boss_texture},
            {"food.png", &syringe_texture},
            {"enemy2.png", &ninja_texture},
            {"ammoP.png", &ammo_texture},
            {"apteczka.png", &medkit_texture},
            {"Slimeball.png", &slimeProjectileTexture},
            {"fireball.png", &fireballtexture},
            {"puste.png", &background_texture},
            {"Walk.png", &character_texture},
            {"Walk_Zombie.png", &zombie_texture},
            {"Slime.png", &slime_texture},
            {"bullet.png", &bullet_texture}
        };
        for (const auto& [file, texture] : textures) {
            if (!texture->loadFromFile(file)) {
                exitWithError("Nie udało się wczytać tekstury: " + file);
            }
        }
        if (!buffer.loadFromFile("menu_music.wav")) {
            exitWithError("Nie udało się wczytać dźwięku menu");
        }
        if (!gameMusic.openFromFile("A Little R & R.ogg")) {
            exitWithError("Nie udało się wczytać muzyki");
        }
        sound.setBuffer(buffer);
        sound.setLoop(true);
        background_sprite.setTexture(background_texture);
    }

    void playSound() {
        sound.play();
    }

    void stopSound() {
        sound.stop();
    }
public:
    Game()
        : health(100),
        window(sf::VideoMode(window_width, window_height), "Jerry_The_Killer"),
        hero(5),
        endGameMenu(),
        gameStarted(false),
        gameEnded(false),
        move_speed(0.45f),
        bullet_speed(2.25f),
        zombiesKilled(0),
        Ninja_killed(0),
        Slime_killed(0),
        backButton(sf::Vector2f(200, 50), font, "Back to menu", sf::Vector2f(300, 300)),
        playAgainButton(sf::Vector2f(200, 50), font, "Play again", sf::Vector2f(300, 400)),
        exitButton(sf::Vector2f(200, 50), font, "Exit", sf::Vector2f(300, 500)),
        timeRecorded(false),
        survivalTime(0.0f),
        roundCounter(0),
        licz(0),
        showingRoundText(false),
        roundStarted(false)
    {

        loadResources();
        hero.initializeHero(character_texture);
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

        roundText.setFont(font);
        roundText.setCharacterSize(48);
        roundText.setFillColor(sf::Color::Yellow);
        roundText.setPosition(window_width / 2 - roundText.getGlobalBounds().width / 2,
                              window_height / 2 - roundText.getGlobalBounds().height / 2);
    }

    void updateHealthText() {
        float displayHealth = std::max(0.0f, health);
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << displayHealth;
        healthText.setString("HP: " + ss.str());
    }

    void run() {
        menu.playSound();
        //endGameMenu.playSound();
        sf::Clock clock;
        float timeSinceLastUpdate = 0.0f;
        float timePerFrame = 1.f / 300.f; // około 0.03333 sekundy

        while (window.isOpen()) {
            sf::Time elapsedTime = clock.restart();
            timeSinceLastUpdate += elapsedTime.asSeconds();

            handleEvents(); // Obsługa zdarzeń może być poza kontrolą czasu, aby szybko reagować na akcje użytkownika

            while (timeSinceLastUpdate > timePerFrame) {
                timeSinceLastUpdate -= timePerFrame;
                update(); // aktualizuj logikę gry
            }

            render(); // rysuj stan gry na ekranie
        }

        gameMusic.stop(); // zatrzymaj muzykę na koniec gry
    }
    void resetGame() {
        health = 100;
        updateHealthText();
        ammo = 30;
        reloading = false;
        unlimitedAmmo = false;
        bullets.clear();
        zombies.clear();
        ninjas.clear();
        slimes.clear();
        bosses.clear();
        medkits.clear();
        ammoPacks.clear();
        Fireball.clear();
        slimeProjectiles.clear();
        syringes.clear();
        zombiesKilled = 0;
        Ninja_killed = 0;
        Slime_killed = 0;
        updateKillCounterText();
        updateAmmoText();
        gameEnded = false;
        timeRecorded = false;
        survivalTime = 0.0f;
        survivalClock.restart();
        MedkitClock.restart();
        medkitRespawnClock.restart();
        ammoRespawnClock.restart();
        roundCounter = 0;
        licz = 0;
        showingRoundText = false;
        roundStarted = false;
        roundText.setString("");
        if (gameStarted && !survivalMode) {
            startRound(1);
        }
    }
    void startRound(int round) {
        roundCounter = round;
        showingRoundText = true;
        roundStarted = false;
        roundTextClock.restart();
        std::string roundString;
        if (round == 1) {
            roundString = "Round 1";
        } else if (round == 2) {
            roundString = "Round 2";
        } else if (round == 3) {
            roundString = "Round 3";
        } else if (round == 4) {
            roundString = "Final Round";
        }
        roundText.setString(roundString);
        roundText.setPosition(window_width / 2 - roundText.getGlobalBounds().width / 2,
                              window_height / 2 - roundText.getGlobalBounds().height / 2);
        zombies.clear();
        ninjas.clear();
        slimes.clear();
        bosses.clear();

    }
    void shootBullet() {
        if ((ammo > 0 || unlimitedAmmo) && !reloading) {
            if (unlimitedAmmo || shootClock.getElapsedTime().asSeconds() >= 0.5f) { // Check if unlimited ammo or 0.5 seconds have passed since the last shot
                sf::Vector2f heroCenter = hero.getPosition() + sf::Vector2f(hero.getGlobalBounds().width / 2, hero.getGlobalBounds().height / 2);
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f target = window.mapPixelToCoords(mousePosition);
                sf::Vector2f direction = normalize(target - heroCenter);

                std::vector<Bullet> newBullets;
                if (tripleShotActive) {
                    // Create three bullets
                    newBullets.push_back(Bullet(bullet_texture, direction, bullet_speed, std::atan2(direction.y, direction.x) * 180 / 3.14159265));

                    // Calculate the left and right directions
                    float angleLeft = std::atan2(direction.y, direction.x) - 30 * 3.14159265 / 180;
                    float angleRight = std::atan2(direction.y, direction.x) + 30 * 3.14159265 / 180;

                    sf::Vector2f directionLeft(std::cos(angleLeft), std::sin(angleLeft));
                    sf::Vector2f directionRight(std::cos(angleRight), std::sin(angleRight));

                    newBullets.push_back(Bullet(bullet_texture, directionLeft, bullet_speed, angleLeft * 180 / 3.14159265));
                    newBullets.push_back(Bullet(bullet_texture, directionRight, bullet_speed, angleRight * 180 / 3.14159265));
                } else {
                    newBullets.push_back(Bullet(bullet_texture, direction, bullet_speed, std::atan2(direction.y, direction.x) * 180 / 3.14159265));
                }

                for (auto& bullet : newBullets) {
                    bullet.setPosition(heroCenter);
                    bullets.push_back(bullet);
                }

                if (!unlimitedAmmo) {
                    ammo--;
                    updateAmmoText();
                }
                shootClock.restart(); // Reset the shoot clock after firing a bullet if not unlimited ammo
            }
        }
    }

    void startGame(bool survival) {
        gameStarted = true;
        gameEnded = false;
        reloading = false;
        unlimitedAmmo = false;
        invulnerable = false;
        survivalMode = survival;
        survivalClock.restart();
        reloadClock.restart();
        unlimitedAmmoClock.restart();
        syringeClock.restart();
        medkitRespawnClock.restart();
        syringeRespawnClock.restart();
        ammoRespawnClock.restart();
        // Additional reset logic if necessary
        gameMusic.setLoop(true);
        gameMusic.play();
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

    void reload() {
        if (!reloading && !unlimitedAmmo) {
            reloading = true;
            reloadClock.restart();
            ammoText.setString("Reloading...");
        }
    }

    void checkBulletCollisions() {
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
            bool bulletErased = false;
            for (auto zombieIt = zombies.begin(); zombieIt != zombies.end();) {
                if (checkCollision(*bulletIt, *(*zombieIt))) { // Dereference unique_ptr to get Zombie reference
                    bulletIt = bullets.erase(bulletIt);
                    (*zombieIt)->takeDamage(50); // Use -> to access Zombie methods
                    bulletErased = true;
                    if ((*zombieIt)->getHealth() <= 0) {
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
                        bossIt->takeDamage(10);
                        bulletErased = true;
                        if (bossIt->getHealth() <= 0 && !bossIt->isDying()) {
                            bossIt->dieAnimation();
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

        // Remove bosses that should be removed
        bosses.erase(std::remove_if(bosses.begin(), bosses.end(),
                                    [](const Boss& boss) { return boss.shouldBeRemoved(); }),
                     bosses.end());

        Vector2f playerPosition = hero.getPosition();
        bool allBossesDeadOrInPostDeathDelay = std::all_of(bosses.begin(), bosses.end(), [](const Boss& boss) {
            return boss.isDying() || boss.isDead() || !boss.canSpawnNewEnemies();
        });

        if (survivalMode) {
            updateTimerText();
            if (allBossesDeadOrInPostDeathDelay && zombies.empty() && ninjas.empty() && slimes.empty()) {
                roundCounter++;
                if (roundCounter % 3 != 0) {
                    for (int i = 0; i < roundCounter; i++) {
                        Zombie::createZombie(zombies, zombie_texture, window);
                        Ninja::createNinja(ninjas, ninja_texture, window, playerPosition);
                        Slime::createSlime(slimes, slime_texture, window, playerPosition);
                    }
                }
                if (roundCounter % 3 == 0) {
                    Boss::createBoss(bosses, boss_texture, window);
                    licz++;
                }
            }
        } else {
            if (roundStarted) {
                if (allBossesDeadOrInPostDeathDelay && zombies.empty() && ninjas.empty() && slimes.empty()) {
                    if (roundCounter < 4) {
                        startRound(roundCounter + 1);
                    } else {
                        gameEnded = true;
                        gameMusic.stop();
                        survivalTime = survivalClock.getElapsedTime().asSeconds();
                        int totalKills = zombiesKilled + Ninja_killed + Slime_killed;
                        endGameMenu.showEndGameMenu(true, survivalTime, totalKills, window, gameStarted, gameEnded, survivalMode, gameMusic, survivalClock, menu, [this]() { resetGame(); }, [this](int round) { startRound(round); });
                    }
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
                            startGame(true);
                            menu.stopSound();
                        } else if (menu.isMouseOverButton(menu.stageButton, mousePos)) {
                            gameStarted = true;
                            startGame(false);
                            menu.stopSound();
                            startRound(1); // Start the first round
                        }
                    } else if (gameEnded) {
                        if (endGameMenu.isMouseOverButton(endGameMenu.backToMenuButton, mousePos)) {
                            gameEnded = false;
                            gameStarted = false;
                            menu.playSound();
                            resetGame(); // Reset the game state
                        } else if (endGameMenu.isMouseOverButton(endGameMenu.playAgainButton, mousePos)) {
                            resetGame(); // Reset the game state
                            startGame(survivalMode);
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
                    ammo = 30;
                    reloading = false;
                    updateAmmoText();
                }
            }
            if (tripleShotActive && tripleShotClock.getElapsedTime().asSeconds() >= 5.0f) {
                tripleShotActive = false;
            }
            if (unlimitedAmmo) {
                if (unlimitedAmmoClock.getElapsedTime().asSeconds() >= 5.0f) {
                    unlimitedAmmo = false;
                    updateAmmoText();
                } else {
                    ammoText.setString("Ammo: Unlimited");
                }
            } else {
                if (ammoRespawnClock.getElapsedTime().asSeconds() >= 15.0f) {
                    Ammo::spawnAmmo(ammoPacks, ammo_texture, window, ammoRespawnClock, hero, 75.0f);
                }
            }

            Syringe::checkHeroSyringeCollisions(syringes, hero, syringeClock, move_speed, orgmove_speed, invulnerable);

            if (invulnerable) {
                if (syringeClock.getElapsedTime().asSeconds() >= 5.0f) {
                    move_speed = orgmove_speed; // Reset movement speed
                    invulnerable = false; // End invulnerability
                }
            }
            if (gameStarted) {
                if (syringeRespawnClock.getElapsedTime().asSeconds() >= 20.0f) {
                    Syringe::spawnSyringe(syringes, syringe_texture, window, syringeRespawnClock, hero, 75.0f);
                    syringeRespawnClock.restart();
                }
                if (medkitRespawnClock.getElapsedTime().asSeconds() >= 10.0f) {
                    Medkit::createMedkit(medkits, medkit_texture, window, hero, 75.0f);
                    medkitRespawnClock.restart();
                }
            }

            FloatRect bounds(0, 0, window_width, window_height);
            Medkit::checkHeroMedkitCollisions(medkits, hero, health, [this]() { updateHealthText(); });
            Ammo::checkHeroAmmoCollisions(ammoPacks, hero, unlimitedAmmo, unlimitedAmmoClock, reloading, [this]() { updateAmmoText(); }, tripleShotActive, tripleShotClock);

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
                    if (!invulnerable) {
                        health -= damage * 10;
                        updateHealthText();
                        projectile = slimeProjectiles.back();
                        slimeProjectiles.pop_back();
                        if (health <= 0.0) {
                            gameEnded = true;
                            gameMusic.stop();
                            survivalTime = survivalClock.getElapsedTime().asSeconds();
                            int totalKills = zombiesKilled + Ninja_killed + Slime_killed;
                            endGameMenu.showEndGameMenu(false, survivalTime, totalKills, window, gameStarted, gameEnded, survivalMode, gameMusic, survivalClock, menu, [this]() { resetGame(); }, [this](int round) { startRound(round); });
                        }
                    }
                }
            }
            slimeProjectiles.erase(std::remove_if(slimeProjectiles.begin(), slimeProjectiles.end(),
                                                  [&bounds](const SlimeProjectile& p) {
                                                      return !bounds.intersects(p.getGlobalBounds());
                                                  }), slimeProjectiles.end());

            for (auto& boss : bosses) {
                boss.shoot(Fireball, fireballtexture, hero.getPosition());
                boss.step(); // Animation step for the boss
            }

            for (auto& projectile : Fireball) {
                projectile.update();
                if (checkCollision(hero, projectile)) {
                    if (!invulnerable) {
                        health -= damage * 15;
                        updateHealthText();
                        projectile = Fireball.back();
                        Fireball.pop_back();
                        if (health <= 0.0) {
                            gameEnded = true;
                            gameMusic.stop();
                            survivalTime = survivalClock.getElapsedTime().asSeconds();
                            int totalKills = zombiesKilled + Ninja_killed + Slime_killed;
                            endGameMenu.showEndGameMenu(false, survivalTime, totalKills, window, gameStarted, gameEnded, survivalMode, gameMusic, survivalClock, menu, [this]() { resetGame(); }, [this](int round) { startRound(round); });
                        }
                    }
                }
            }

            Fireball.erase(std::remove_if(Fireball.begin(), Fireball.end(),
                                          [&bounds](const class Fireball& p) {
                                              return !bounds.intersects(p.getGlobalBounds());
                                          }), Fireball.end());

            // Update positions and resolve collisions for all zombies
            for (auto& zombie : zombies) {
                Vector2f zombiePosition = zombie->getPosition(); // Dereference unique_ptr to get position
                Vector2f direction = playerPosition - zombiePosition;
                direction = normalize(direction);

                float moveX = direction.x * 0.16;
                float moveY = direction.y * 0.16;
                zombie->moveWithCollision(bounds, moveX, moveY); // Dereference unique_ptr to call method
                zombie->step(); // Dereference unique_ptr to call method

                // Resolve collisions with other zombies
                for (auto& otherZombie : zombies) {
                    if (zombie != otherZombie && checkCollision(*zombie, *otherZombie)) { // Dereference unique_ptr to check collision
                        resolveCollision(*zombie, *otherZombie); // Dereference unique_ptr to resolve collision
                    }
                }
            }

            // Update positions and resolve collisions for all ninjas
            for (auto& ninja : ninjas) {
                Vector2f ninjaPosition = ninja.getPosition();
                Vector2f direction = playerPosition - ninjaPosition;
                direction = normalize(direction);

                float moveX = direction.x * 0.35;
                float moveY = direction.y * 0.35;
                ninja.moveWithCollision(bounds, moveX, moveY);
                ninja.step(playerPosition, bounds); // Pass arguments

                // Resolve collisions with other ninjas
                for (auto& otherNinja : ninjas) {
                    if (&ninja != &otherNinja && checkCollision(ninja, otherNinja)) {
                        resolveCollision(ninja, otherNinja);
                    }
                }
            }

            // Update positions and resolve collisions for all slimes
            for (auto& slime : slimes) {
                Vector2f slimePosition = slime.getPosition();
                Vector2f direction = playerPosition - slimePosition;
                direction = normalize(direction);

                float moveX = direction.x * 0.05;
                float moveY = direction.y * 0.05;
                slime.moveWithCollision(bounds, moveX, moveY);
                slime.step();

                // Resolve collisions with other slimes
                for (auto& otherSlime : slimes) {
                    if (&slime != &otherSlime && checkCollision(slime, otherSlime)) {
                        resolveCollision(slime, otherSlime);
                    }
                }
            }

            // Update positions and resolve collisions for all bosses
            for (auto& boss : bosses) {
                Vector2f bossPosition = boss.getPosition();
                Vector2f direction = playerPosition - bossPosition;
                direction = normalize(direction);

                float moveX = direction.x * 0.1;
                float moveY = direction.y * 0.1;
                boss.moveWithCollision(bounds, moveX, moveY);

                // Resolve collisions with other bosses
                for (auto& otherBoss : bosses) {
                    if (&boss != &otherBoss && checkCollision(boss, otherBoss)) {
                        resolveCollision(boss, otherBoss);
                    }
                }
            }

            Zombie::checkHeroZombieCollisions(zombies, hero, health, invulnerable, damage, gameEnded, gameMusic, [this]() { updateHealthText(); });
            Ninja::checkHeroNinjaCollisions(ninjas, hero, health, invulnerable, damage, gameEnded, gameMusic, [this]() { updateHealthText(); });
            Slime::checkHeroSlimeCollisions(slimes, hero, health, invulnerable, damage, gameEnded, gameMusic, [this]() { updateHealthText(); });
            Boss::checkHeroBossCollisions(bosses, hero, health, invulnerable, damage, gameEnded, gameMusic, [this]() { updateHealthText(); });
            for (auto& bullet : bullets) {
                bullet.update();
            }

            checkBulletCollisions();

            if (showingRoundText && roundTextClock.getElapsedTime().asSeconds() >= 3.0f) {
                showingRoundText = false;
                roundStarted = true;
                if (roundCounter == 1) {
                    for (int i = 0; i < 10; ++i) {
                        Zombie::createZombie(zombies, zombie_texture, window);
                    }
                } else if (roundCounter == 2) {
                    for (int i = 0; i < 10; ++i) {
                        Zombie::createZombie(zombies, zombie_texture, window);
                    }
                    for (int i = 0; i < 5; ++i) {
                        Ninja::createNinja(ninjas, ninja_texture, window, playerPosition);
                    }
                } else if (roundCounter == 3) {
                    for (int i = 0; i < 5; ++i) {
                        Zombie::createZombie(zombies, zombie_texture, window);
                    }
                    for (int i = 0; i < 5; ++i) {
                        Ninja::createNinja(ninjas, ninja_texture, window, playerPosition);
                    }
                    for (int i = 0; i < 5; ++i) {
                        Slime::createSlime(slimes, slime_texture, window, playerPosition);
                    }
                } else if (roundCounter == 4) {
                    Boss::createBoss(bosses, boss_texture, window);
                    for (int i = 0; i < 2; ++i) {
                        Slime::createSlime(slimes, slime_texture, window, playerPosition);
                    }
                }
            }

            bool anyBossDyingOrInPostDeathDelay = std::any_of(bosses.begin(), bosses.end(), [](const Boss& boss) {
                return boss.isDying() || !boss.canSpawnNewEnemies();
            });
            if (!anyBossDyingOrInPostDeathDelay) {
                if (roundStarted) {
                    bool allZombiesKilled = std::all_of(zombies.begin(), zombies.end(), [](const std::unique_ptr<Zombie>& z){ return z->getHealth() <= 0.0; });
                    bool allNinjasKilled = std::all_of(ninjas.begin(), ninjas.end(), [](const Ninja& n){ return n.getHealth() <= 0.0; });
                    bool allSlimesKilled = std::all_of(slimes.begin(), slimes.end(), [](const Slime& n){ return n.getHealth() <= 0.0; });
                    bool allBossesKilled = std::all_of(bosses.begin(), bosses.end(), [](const Boss& n){ return n.getHealth() <= 0.0; });

                    if (allZombiesKilled && zombies.empty() && allBossesKilled && bosses.empty() && allSlimesKilled && slimes.empty() && allNinjasKilled && ninjas.empty()) {
                        if (roundCounter < 4) {
                            startRound(roundCounter + 1);
                        } else {
                            gameEnded = true;
                            gameMusic.stop();
                            survivalTime = survivalClock.getElapsedTime().asSeconds();
                            int totalKills = zombiesKilled + Ninja_killed + Slime_killed;
                            endGameMenu.showEndGameMenu(true, survivalTime, totalKills, window, gameStarted, gameEnded, survivalMode, gameMusic, survivalClock, menu, [this]() { resetGame(); }, [this](int round) { startRound(round); });
                        }
                    }
                }
            }
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
            // Draw game elements...

            sf::FloatRect windowBounds(0, 0, window_width, window_height); // Declare windowBounds here

            for (int x = 0; x < window_width; x += background_texture.getSize().x) {
                for (int y = 0; y < window_height; y += background_texture.getSize().y) {
                    background_sprite.setPosition(x, y);
                    window.draw(background_sprite);
                }
            }
            window.draw(hero);

            for (const auto& zombie : zombies) {
                window.draw(*zombie);  // Dereference unique_ptr to draw the zombie
            }
            for (const auto& medkit : medkits) {
                window.draw(medkit);
            }
            for (const auto& ninja : ninjas) {
                window.draw(ninja);
                for (const auto& clone : ninja.getClones()) {
                    window.draw(clone);
                }
            }
            for (const auto& slime : slimes) {
                window.draw(slime);
            }
            for (const auto& boss : bosses) {
                window.draw(boss);
            }
            for (const auto& syringe : syringes) {
                window.draw(syringe);
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

            if (showingRoundText) {
                window.draw(roundText);
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
}

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <sstream>
#include <iomanip>
#include <SFML/System/Vector2.hpp>
#include "AnimatedSprite.h"
#include "Zombie.h"
#include "Bullet.h"
#include "ninja.h"
#include "utils.h"
#include "Menu.h"
using namespace std;
using namespace sf;

    class Game {
        vector<Zombie> zombies;
        sf::Texture zombie_texture;
        vector<Ninja> ninjas;
        sf::Texture ninja_texture;
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
        bool gameStarted;
        bool survivalMode;
        bool gameEnded;
        float move_speed;
        sf::Clock survivalClock;
        sf::Text timerText;
        sf::Font font;
        sf::Texture bullet_texture;
        std::vector<Bullet> bullets;
        float bullet_speed;
        sf::Text killCounterText;
        int zombiesKilled;
        int Ninja_killed;
        sf::Texture survivalButtonTexture;
        sf::Texture survivalButtonTextureRed;
        sf::Texture stageButtonTexture;
        sf::Texture stageButtonTextureRed;
        sf::Text endGameText;
        float damage = 0.01;

        void loadResources() {
            if (!ninja_texture.loadFromFile("enemy2.png")){
                cout<< "Nie udało się wczytać tekstur Ninjy" <<endl;
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
        void initializeZombies() {
            for (int i = 0; i < 5; ++i) {
                createZombie(i * 100, 200);
            }
        }

        void initializeNinjas()
        {
            for (int i = 0; i < 3; i++){
               createNinja(i*150, 400); //czeka na funkcje
            }
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

        void createNinja(float x, float y) {
            Ninja ninja(3);
            ninja.setTexture(ninja_texture);
            addNinjaAnimationFrames(ninja);
            ninja.setTextureRect(IntRect(6, 100, 17, 26)); // Set initial frame
            ninja.setScale(1, 1);
            ninja.setPosition(x, y);
            ninja.step();
            ninjas.push_back(ninja);
        }

        void createZombie(float x, float y) {
            Zombie zombie(5);
            zombie.setTexture(zombie_texture);
            addZombieAnimationFrames(zombie);
            zombie.setTextureRect(IntRect(11, 69, 14, 26)); // Set initial frame
            zombie.setScale(1, 1);
            zombie.setPosition(x, y);
            zombie.step();
            zombies.push_back(zombie);
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



    public:
        Game()
            : health(100), window(sf::VideoMode(window_width, window_height), "Game Window"),
            hero(5), gameStarted(false), gameEnded(false), move_speed(0.1f), bullet_speed(0.5f), zombiesKilled(0) ,Ninja_killed(0) {
            loadResources();
            initializeHero();
            initializeZombies();
            initializeNinjas();
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
            endGameText.setPosition(window_width / 2 - endGameText.getGlobalBounds().width / 2, window_height / 2 - endGameText.getGlobalBounds().height / 2);

            killCounterText.setFont(font);
            killCounterText.setCharacterSize(24);
            killCounterText.setFillColor(sf::Color::White);
            killCounterText.setPosition(window_width - 150, 10);
            updateKillCounterText();
        }

        void updateHealthText() {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(1) << health;
            healthText.setString("HP: " + ss.str());
        }


        void checkHeroZombieCollisions() {
            for (auto& zombie : zombies) {
                if (checkCollision(hero, zombie)) {
                    health -= damage;
                    updateHealthText();
                    if (health <= 0) {
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
                    if (health <= 0) {
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

        void shootBullet() {
            sf::Vector2f heroCenter = hero.getPosition() + sf::Vector2f(hero.getGlobalBounds().width / 2, hero.getGlobalBounds().height / 2);
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f target = window.mapPixelToCoords(mousePosition);
            sf::Vector2f direction = normalize(target - heroCenter);
            float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265;
            Bullet bullet(bullet_texture, direction, bullet_speed, angle);
            bullet.setPosition(heroCenter);
            bullets.push_back(bullet);
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
                    ++bulletIt;
                }
            }

            bool allZombiesKilled = std::all_of(zombies.begin(), zombies.end(), [](const Zombie& z){ return z.getHealth() <= 0; });
            bool allNinjasKilled = std::all_of(ninjas.begin(), ninjas.end(), [](const Ninja& n){ return n.getHealth() <= 0; });
            if (allZombiesKilled && zombies.empty() && allNinjasKilled && ninjas.empty()) {
                    createZombie(rand() % window_width, rand() % window_height*2);
                    createNinja(rand() % window_width, rand() % window_height);
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
                        shootBullet();
                    } else if (gameEnded && event.key.code == sf::Keyboard::Enter) {
                        window.close(); // Optionally restart the game or close the window
                    }
                } else if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    updateButtonColors(mousePos);
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        if (!gameStarted && !gameEnded) {
                            if (menu.isMouseOverButton(menu.survivalButton, mousePos)) {
                                gameStarted = true;
                                survivalMode = true;
                                menu.stopSound();
                                survivalClock.restart();
                                gameMusic.setLoop(true);
                                gameMusic.play();
                            } else if (menu.isMouseOverButton(menu.stageButton, mousePos)) {
                                gameStarted = true;
                                survivalMode = false;
                                menu.stopSound();
                                survivalClock.restart();
                                gameMusic.setLoop(true);
                                gameMusic.play();
                            }
                        }
                    }
                }
            }
        }
        void updateButtonColors(const sf::Vector2i& mousePos) {
            if (menu.isMouseOverButton(menu.survivalButton, mousePos)) {
                menu.survivalButton.setTexture(&survivalButtonTextureRed); // Highlighted version
            } else {
                menu.survivalButton.setTexture(&survivalButtonTexture); // Default version
            }

            if (menu.isMouseOverButton(menu.stageButton, mousePos)) {
                menu.stageButton.setTexture(&stageButtonTextureRed); // Highlighted version
            } else {
                menu.stageButton.setTexture(&stageButtonTexture); // Default version
            }
        }

        void update() {
            if (gameStarted && !gameEnded) {
                FloatRect bounds(0, 0, window_width, window_height);

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

                    float moveX = direction.x * move_speed * 0.2;
                    float moveY = direction.y * move_speed * 0.2;
                    ninja.moveWithCollision(bounds, moveX, moveY);
                    ninja.step();
                }

                checkHeroZombieCollisions();
                checkHeroNinjaCollisions();

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

                for (const auto& ninja : ninjas) {
                    window.draw(ninja);
                }

                for (const auto& bullet : bullets) {
                    window.draw(bullet);
                }

                if (survivalMode) {
                    window.draw(timerText);
                }
                window.draw(healthText);
                window.draw(killCounterText);

                if (gameEnded) {
                    window.draw(endGameText);
                }
            } else {
                menu.draw(window);
            }
            window.display();
        }

        void updateKillCounterText() {
            std::stringstream ss;
            ss << "Kills: " << zombiesKilled + Ninja_killed;
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

// Advanced C++ - Mini Text Adventure Game
// Combines inheritance, polymorphism, STL, templates and smart pointers in a compact example

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include <functional>
#include <map>

/*
 * TEXT ADVENTURE GAME
 * 
 * A compact example combining:
 * 1. Classes & Inheritance: GameEntity hierarchy
 * 2. Operator overloading: + for health potions, == for entity comparison
 * 3. STL containers: vector for entities, map for items
 * 4. Smart pointers: managing entities with unique_ptr
 * 5. Templates: Inventory template that works with any entity type
 * 6. Polymorphism: different entities behaving according to their type
 */

// Forward declarations
class Player;

//=============================================================================
// ENTITY SYSTEM
//=============================================================================

// Base class for all game entities
class GameEntity {
protected:
    std::string name;
    int health;
    
public:
    GameEntity(std::string name, int health) : name(std::move(name)), health(health) {}
    
    virtual void interact(Player& player) = 0;
    
    virtual std::string getDescription() const {
        return name + " (Health: " + std::to_string(health) + ")";
    }
    
    std::string getName() const { return name; }
    int getHealth() const { return health; }
    void setHealth(int newHealth) { health = newHealth; }
    
    // Operator overloading for comparison
    bool operator==(const GameEntity& other) const {
        return name == other.name;
    }
    
    virtual ~GameEntity() = default;
};

// Item class - inherits from GameEntity
class Item : public GameEntity {
protected:
    int value;
    
public:
    Item(std::string name, int value) 
        : GameEntity(std::move(name), 1), value(value) {}
    
    int getValue() const { return value; }
    
    std::string getDescription() const override {
        return name + " (Value: " + std::to_string(value) + " gold)";
    }
};

// Weapon class - inherits from Item
class Weapon : public Item {
private:
    int damage;
    
public:
    Weapon(std::string name, int value, int damage)
        : Item(std::move(name), value), damage(damage) {}
    
    int getDamage() const { return damage; }
    
    std::string getDescription() const override {
        return name + " (Damage: " + std::to_string(damage) + ", Value: " + std::to_string(value) + " gold)";
    }
    
    void interact(Player& player) override;  // Implemented after Player class definition
};

// Potion class - inherits from Item
class Potion : public Item {
private:
    int potency;
    
public:
    Potion(std::string name, int value, int potency)
        : Item(std::move(name), value), potency(potency) {}
    
    int getPotency() const { return potency; }
    
    std::string getDescription() const override {
        return name + " (Heals: " + std::to_string(potency) + ", Value: " + std::to_string(value) + " gold)";
    }
    
    void interact(Player& player) override;  // Implemented after Player class definition
    
    // Operator overloading - combine potions
    Potion operator+(const Potion& other) const {
        return Potion(
            "Strong " + name,
            value + other.value,
            potency + other.potency
        );
    }
};

// Monster class - inherits from GameEntity
class Monster : public GameEntity {
private:
    int damage;
    int gold;
    
public:
    Monster(std::string name, int health, int damage, int gold)
        : GameEntity(std::move(name), health), damage(damage), gold(gold) {}
    
    int getDamage() const { return damage; }
    int getGold() const { return gold; }
    
    std::string getDescription() const override {
        return name + " (Health: " + std::to_string(health) + 
               ", Damage: " + std::to_string(damage) + ")";
    }
    
    void interact(Player& player) override;  // Implemented after Player class definition
};

//=============================================================================
// INVENTORY SYSTEM
//=============================================================================

// Template inventory class
template <typename T>
class Inventory {
private:
    std::vector<std::unique_ptr<T>> items;
    
public:
    bool addItem(std::unique_ptr<T> item) {
        items.push_back(std::move(item));
        return true;
    }
    
    bool removeItem(size_t index) {
        if (index < items.size()) {
            items.erase(items.begin() + index);
            return true;
        }
        return false;
    }
    
    void displayItems() const {
        if (items.empty()) {
            std::cout << "Empty inventory!" << std::endl;
            return;
        }
        
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << i << ": " << items[i]->getDescription() << std::endl;
        }
    }
    
    size_t size() const { return items.size(); }
    
    T* getItem(size_t index) {
        if (index < items.size()) {
            return items[index].get();
        }
        return nullptr;
    }
    
    std::unique_ptr<T> takeItem(size_t index) {
        if (index < items.size()) {
            std::unique_ptr<T> item = std::move(items[index]);
            items.erase(items.begin() + index);
            return item;
        }
        return nullptr;
    }
};

//=============================================================================
// PLAYER CLASS
//=============================================================================

class Player : public GameEntity {
private:
    int gold;
    int maxHealth;
    Inventory<Item> inventory;
    Weapon* equippedWeapon;
    
public:
    Player(std::string name, int health, int gold)
        : GameEntity(std::move(name), health), gold(gold), maxHealth(health), equippedWeapon(nullptr) {}
    
    void addGold(int amount) {
        gold += amount;
        std::cout << "You gained " << amount << " gold. Total: " << gold << std::endl;
    }
    
    int getGold() const { return gold; }
    
    bool spendGold(int amount) {
        if (gold >= amount) {
            gold -= amount;
            std::cout << "You spent " << amount << " gold. Remaining: " << gold << std::endl;
            return true;
        }
        std::cout << "Not enough gold!" << std::endl;
        return false;
    }
    
    void heal(int amount) {
        int oldHealth = health;
        health = std::min(health + amount, maxHealth);
        int actualHeal = health - oldHealth;
        std::cout << "You healed for " << actualHeal << " health. Current health: " << health << std::endl;
    }
    
    void takeDamage(int amount) {
        health -= amount;
        std::cout << "You took " << amount << " damage! Health: " << health << std::endl;
        
        if (health <= 0) {
            std::cout << "You have been defeated!" << std::endl;
        }
    }
    
    bool isAlive() const { return health > 0; }
    
    void equipWeapon(Weapon* weapon) {
        equippedWeapon = weapon;
        std::cout << "Equipped " << weapon->getName() << " (Damage: " << weapon->getDamage() << ")" << std::endl;
    }
    
    int getAttackDamage() const {
        int baseDamage = 1;
        return equippedWeapon ? equippedWeapon->getDamage() : baseDamage;
    }
    
    void interact(Player& player) override {
        std::cout << "You cannot interact with yourself!" << std::endl;
    }
    
    void displayStatus() const {
        std::cout << "\n----- " << name << " -----" << std::endl;
        std::cout << "Health: " << health << "/" << maxHealth << std::endl;
        std::cout << "Gold: " << gold << std::endl;
        std::cout << "Weapon: " << (equippedWeapon ? equippedWeapon->getName() : "None") << std::endl;
        
        std::cout << "\nInventory:" << std::endl;
        inventory.displayItems();
        std::cout << std::endl;
    }
    
    Inventory<Item>& getInventory() { return inventory; }
    
    void attackMonster(Monster& monster) {
        int damage = getAttackDamage();
        std::cout << "You attack " << monster.getName() << " for " << damage << " damage!" << std::endl;
        monster.setHealth(monster.getHealth() - damage);
        
        if (monster.getHealth() <= 0) {
            std::cout << "You defeated " << monster.getName() << "!" << std::endl;
            addGold(monster.getGold());
        }
    }
};

// Now that Player is defined, implement the deferred methods
void Weapon::interact(Player& player) {
    player.equipWeapon(this);
}

void Potion::interact(Player& player) {
    player.heal(potency);
}

void Monster::interact(Player& player) {
    std::cout << name << " attacks you!" << std::endl;
    player.takeDamage(damage);
}

//=============================================================================
// GAME WORLD
//=============================================================================

class Room {
private:
    std::string name;
    std::string description;
    std::vector<std::unique_ptr<GameEntity>> entities;
    std::map<std::string, Room*> exits;
    
public:
    Room(std::string name, std::string description)
        : name(std::move(name)), description(std::move(description)) {}
    
    void addEntity(std::unique_ptr<GameEntity> entity) {
        entities.push_back(std::move(entity));
    }
    
    void addExit(const std::string& direction, Room* room) {
        exits[direction] = room;
    }
    
    Room* getExit(const std::string& direction) {
        auto it = exits.find(direction);
        if (it != exits.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    void displayRoom() const {
        std::cout << "\n===== " << name << " =====" << std::endl;
        std::cout << description << std::endl;
        
        if (!entities.empty()) {
            std::cout << "\nYou see:" << std::endl;
            for (size_t i = 0; i < entities.size(); ++i) {
                std::cout << i << ": " << entities[i]->getDescription() << std::endl;
            }
        }
        
        std::cout << "\nExits: ";
        for (const auto& exit : exits) {
            std::cout << exit.first << " ";
        }
        std::cout << std::endl;
    }
    
    void interact(size_t index, Player& player) {
        if (index < entities.size()) {
            entities[index]->interact(player);
            
            // If the entity is a monster and it's defeated, remove it
            auto* monster = dynamic_cast<Monster*>(entities[index].get());
            if (monster && monster->getHealth() <= 0) {
                std::cout << monster->getName() << " disappeared." << std::endl;
                entities.erase(entities.begin() + index);
            }
            
            // If the entity is an item, move it to player's inventory
            auto* item = dynamic_cast<Item*>(entities[index].get());
            if (item) {
                std::cout << "You picked up " << item->getName() << std::endl;
                player.getInventory().addItem(std::unique_ptr<Item>(
                    static_cast<Item*>(entities[index].release())));
                entities.erase(entities.begin() + index);
            }
        } else {
            std::cout << "Invalid selection!" << std::endl;
        }
    }
};

//=============================================================================
// GAME ENGINE
//=============================================================================

class Game {
private:
    Player player;
    std::vector<std::unique_ptr<Room>> rooms;
    Room* currentRoom;
    bool gameOver;
    
    // Helper to create random numbers
    std::mt19937 rng;
    
    int getRandomInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }
    
public:
    Game() : player("Hero", 100, 50), gameOver(false) {
        rng.seed(std::random_device()());
        initializeGame();
    }
    
    void initializeGame() {
        // Create rooms
        rooms.push_back(std::make_unique<Room>("Forest Entrance", 
            "A dark forest surrounds you. A narrow path leads deeper into the trees."));
        
        rooms.push_back(std::make_unique<Room>("Dark Cave", 
            "A damp cave with glittering minerals in the walls. It smells musty."));
        
        rooms.push_back(std::make_unique<Room>("Mountain Pass", 
            "A narrow path between tall rocky cliffs. The wind howls through the pass."));
        
        rooms.push_back(std::make_unique<Room>("Ancient Ruins", 
            "Crumbling stone structures. Once a grand city, now home to monsters."));
        
        // Set up exits
        rooms[0]->addExit("north", rooms[1].get());
        rooms[0]->addExit("east", rooms[2].get());
        
        rooms[1]->addExit("south", rooms[0].get());
        rooms[1]->addExit("east", rooms[3].get());
        
        rooms[2]->addExit("west", rooms[0].get());
        rooms[2]->addExit("north", rooms[3].get());
        
        rooms[3]->addExit("west", rooms[1].get());
        rooms[3]->addExit("south", rooms[2].get());
        
        // Add entities to rooms
        rooms[0]->addEntity(std::make_unique<Weapon>("Rusty Sword", 5, 3));
        rooms[0]->addEntity(std::make_unique<Potion>("Small Health Potion", 10, 20));
        
        rooms[1]->addEntity(std::make_unique<Monster>("Cave Bat", 15, 2, 10));
        rooms[1]->addEntity(std::make_unique<Potion>("Medium Health Potion", 20, 40));
        
        rooms[2]->addEntity(std::make_unique<Monster>("Mountain Wolf", 25, 4, 20));
        rooms[2]->addEntity(std::make_unique<Weapon>("Hunter's Bow", 25, 5));
        
        rooms[3]->addEntity(std::make_unique<Monster>("Ancient Guardian", 50, 8, 100));
        rooms[3]->addEntity(std::make_unique<Weapon>("Enchanted Sword", 50, 10));
        
        // Start in the first room
        currentRoom = rooms[0].get();
    }
    
    void play() {
        std::cout << "Welcome to the Mini Text Adventure!" << std::endl;
        std::cout << "Commands: look, go [direction], take [item], use [item], attack [monster], status, quit" << std::endl;
        
        while (!gameOver) {
            if (!player.isAlive()) {
                std::cout << "\nGAME OVER - You have been defeated!" << std::endl;
                std::cout << "Thanks for playing!" << std::endl;
                return;
            }
            
            currentRoom->displayRoom();
            
            std::string command;
            std::cout << "\nWhat will you do? ";
            std::cin >> command;
            
            if (command == "look") {
                // Room is already displayed
            } 
            else if (command == "go") {
                std::string direction;
                std::cin >> direction;
                Room* nextRoom = currentRoom->getExit(direction);
                
                if (nextRoom) {
                    currentRoom = nextRoom;
                    std::cout << "You go " << direction << "." << std::endl;
                } else {
                    std::cout << "You can't go that way!" << std::endl;
                }
            }
            else if (command == "take" || command == "attack" || command == "interact") {
                int index;
                std::cin >> index;
                currentRoom->interact(index, player);
            }
            else if (command == "use") {
                int index;
                std::cin >> index;
                
                Item* item = player.getInventory().getItem(index);
                if (item) {
                    item->interact(player);
                    // For consumables like potions, remove after use
                    if (dynamic_cast<Potion*>(item)) {
                        player.getInventory().removeItem(index);
                    }
                } else {
                    std::cout << "Invalid item!" << std::endl;
                }
            }
            else if (command == "status") {
                player.displayStatus();
            }
            else if (command == "quit") {
                gameOver = true;
                std::cout << "Thanks for playing!" << std::endl;
            }
            else {
                std::cout << "Unknown command. Try: look, go, take, use, attack, status, quit" << std::endl;
            }
        }
    }
};

//=============================================================================
// DEMO APPLICATION
//=============================================================================

int main() {
    std::cout << "===== Advanced C++ Mini Text Adventure =====\n" << std::endl;
    
    Game game;
    game.play();
    
    return 0;
} 
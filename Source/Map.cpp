#include "../header/Map.h"
#include <iostream>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cmath>

Map::Map(sf::Vector2u windowSize)
    : windowSize_(windowSize)
{
    background_.setFillColor(sf::Color(70, 50, 30));
    globalButtonStates_.assign(4, false);

    if (!shovelTexture_.loadFromFile("textures/Lopata.png") && !shovelTexture_.loadFromFile("../textures/Lopata.png")) {
        std::cerr << "Eroare: Nu s-a putut incarca Lopata.png" << std::endl;
    }
    if (!wallTexture_.loadFromFile("textures/Wall.png") && !wallTexture_.loadFromFile("../textures/Wall.png")) {
        std::cerr << "Eroare: Nu s-a putut incarca Wall.png" << std::endl;
    }
    if (!floorTexture_.loadFromFile("textures/Floor.png") && !floorTexture_.loadFromFile("../textures/Floor.png")) {
        std::cerr << "Eroare: Nu s-a putut incarca Floor.png" << std::endl;
    }
    if (!portalTexture_.loadFromFile("textures/Portal.png") && !portalTexture_.loadFromFile("../textures/Portal.png")) {
        std::cerr << "Eroare: Nu s-a putut incarca Portal.png" << std::endl;
    }
    if (!generatorTexture_.loadFromFile("textures/Generator.png") && !generatorTexture_.loadFromFile("../textures/Generator.png")) {
        std::cerr << "Eroare: Nu s-a putut incarca Generator.png" << std::endl;
    }
    if (!doorTexture_.loadFromFile("textures/Door.png") && !doorTexture_.loadFromFile("../textures/Door.png")) {
        std::cerr << "Eroare: Nu s-a putut incarca Door.png" << std::endl;
    }
    if (!closetTexture_.loadFromFile("textures/Closet.png") && !closetTexture_.loadFromFile("../textures/Closet.png")) {
        std::cerr << "Eroare: Nu s-a putut incarca Closet.png" << std::endl;
    }
    if (!rubbleTexture_.loadFromFile("textures/Rubble.png") && !rubbleTexture_.loadFromFile("../textures/Rubble.png")) {
        std::cerr << "Eroare: Nu s-a putut incarca Rubble.png" << std::endl;
    }
    if (!dustOverlayTexture_.loadFromFile("textures/UpsideDownDust.png") && !dustOverlayTexture_.loadFromFile("../textures/UpsideDownDust.png")) {
        std::cerr << "Eroare: Nu s-a putut incarca UpsideDownDust.png" << std::endl;
    }
    if (!exitDoorTexture_.loadFromFile("textures/ExitDoor.png") && !exitDoorTexture_.loadFromFile("../textures/ExitDoor.png")) {
        std::cerr << "Eroare: Nu s-a putut incarca ExitDoor.png" << std::endl;
    }

    bool genLoaded = false;
    if (generatorBuffer_.loadFromFile("sounds/GeneratorStart.wav")) genLoaded = true;
    else if (generatorBuffer_.loadFromFile("../sounds/GeneratorStart.wav")) genLoaded = true;
    else if (generatorBuffer_.loadFromFile("GeneratorStart.wav")) genLoaded = true;

    if (genLoaded) {
        generatorSound_.setBuffer(generatorBuffer_);
    } else {
        std::cerr << "Nu s-a gasit 'GeneratorStart.wav'" << std::endl;
    }

    bool doorLoaded = false;
    if (doorBuzzerBuffer_.loadFromFile("sounds/DoorBuzzer.wav")) doorLoaded = true;
    else if (doorBuzzerBuffer_.loadFromFile("../sounds/DoorBuzzer.wav")) doorLoaded = true;
    else if (doorBuzzerBuffer_.loadFromFile("DoorBuzzer.wav")) doorLoaded = true;

    if (doorLoaded) {
        doorBuzzerSound_.setBuffer(doorBuzzerBuffer_);
    } else {
        std::cerr << "Nu s-a gasit 'DoorBuzzer.mp3'!" << std::endl;
    }

    std::vector<std::string> level1 = {
        "WWWWWWWWWWWWWTWWWWWWWWWWWW",
        "W.P......W...............W",
        "W.WWWW.W.W.WWWWWW.WWWWWW.W",
        "W.W....W.W......W......W.W",
        "W.W.WWWW.W.WCWW.W.WWW..W.W",
        "W.W......W.W....W...W..W.W",
        "W.WWWWWWWW.W.WWWW.W.WWWW.W",
        "W..........W......W......W",
        "W.WWWWWWWWWWWWWWWWWWWWWW.W",
        "W.W......................T",
        "W.W.WWWWWWWWWWWWWWWWWWW..T",
        "W.W.W..............W.....T",
        "W.W.W.WWWWWWWWWW.W.W.WWW.W",
        "W.W.W.W..........W.W....DW",
        "W.W.W.W.WWWWWWWWWW.W.WWWW.",
        "W.W.....W...........W....W",
        "W.CWWW.W.WWWWWWWWWWWW.WW.W",
        "W......W..............W..W",
        "W.E....WWWWWWWWWWWWWWWW..W",
        "WWWWWWWWWWWWWWWWWWWWWWWWWW"
    };

    std::vector<std::string> level2 = {
        "WWWWWWWWWWWWWTWWWWWWWWWWWW",
        "W...............W......P.W",
        "W.WWWWWW.WWWWWW.W.W.WWWW.W",
        "W.W......W......W.W....W.W",
        "W.W..WWW.W.WCWW.W.WWW..W.W",
        "W.W..W...W....W.W......W.W",
        "W.WWWW.W.WWWW.W.WWWWWWWW.W",
        "W......W......W..........W",
        "W.WWWWWWWWWWWWWWWWWWWWWW.W",
        "T......................W.W",
        "T..WWWWWWWWWWWWWWWWWWW.W.W",
        "T.....W..............W.W.W",
        "W.WWW.W.W.WWWWWWWWWW.W.W.W",
        "W.....W.W..........W.W.W.W",
        ".WWWW.W.WWWWWWWWWW.W.W.W.W",
        "W....W...........W.....W.W",
        "W.WW.WWWWWWWWWWWW.W.WWWC.W",
        "W..W..............W......W",
        "W..WWWWWWWWWWWWWWWW....E.W",
        "WWWWWWWWWWWWWWWWWWWWWWWWWW"
    };

    std::vector<std::string> level3 = {
        "WWWWWWWWWWWWWWWWWWWWWWWWWW",
        "W.S.......W...C........B.W",
        "W.WWWWW.W.W.WWWWWWW.WWWW.W",
        "W.W.....W.W.W.....R......W",
        "W.W.....W.W.W..G..R......W",
        "W.W.....W.W.WWWWWWW......W",
        "W.WWWWWWWWWWWWWWWWWW.WWWWW",
        "W........................W",
        "W.WWWWWWWWWWWWWWWWWWWWWW.W",
        "W.W.........E..........W.W",
        "W.W.WWWWWWWWWWWWWWWWWW.W.W",
        "W.W.W................W.W.W",
        "W.W.W.WWWWWWWWWWWWWW.W.W.W",
        "W.W.W.W............W.W.W.W",
        "W.W.W.W.WWWWWWWWWW.W.W.W.W",
        "W.W...W..........W.W.W.W.W",
        "W.WWWWW.WWWWWWWW.W.W.W.W.W",
        "W.................B..W...W",
        "WWWWWWWWWWWWWTWWWWWWWWWWWW",
        "WWWWWWWWWWWWWWWWWWWWWWWWWW"
    };

    std::vector<std::string> level4 = {
        "WWWWWWWWWWWWWWWWWWWWWWWWWW",
        "W.B........C...W.........W",
        "W.WWWW.WWWWWWW.W.W.WWWWW.W",
        "W..............W.W.....W.W",
        "W..............W.W.....W.W",
        "W......WWWWWWW.W.W.....W.W",
        "WWWWW.WWWWWWWWWWWWWWWWWW.W",
        "W........................W",
        "W.WWWWWWWWWWWWWWWWWWWWWW.W",
        "W.W.........E..........W.W",
        "W.W.WWWWWWWWWWWWWWWWWW.W.W",
        "W.W.W................W.W.W",
        "W.W.W.WWWWWWWWWWWWWW.W.W.W",
        "W.W.W.W............W.W.W.W",
        "W.W.W.W.WWWWWWWWWW.W.W.W.W",
        "W.W.W.W.W..........W...W.W",
        "W.W.W.W.W.WWWWWWWW.WWWWW.W",
        "W...W..B.................W",
        "WWWWWWWWWWWWWTWWWWWWWWWWWW",
        "WWWWWWWWWWWWWWWWWWWWWWWWWW"
    };

    levels_.push_back(level1);
    levels_.push_back(level2);
    levels_.push_back(level3);
    levels_.push_back(level4);

    loadLevel(0);
}

Map::~Map() = default;

void Map::buildMap() {
    walls_.clear();
    floorTiles_.clear();
    transitionShapes_.clear();
    closets_.clear();
    closetVisited_.clear();
    rubbleShapes_.clear();
    currentLevelButtons_.clear();

    winDoorBounds_ = sf::FloatRect(0,0,0,0);
    winDoorShape_.setPosition(-1000, -1000);

    shovelShape_.setPosition(-1000, -1000);
    generatorShape_.setPosition(-1000, -1000);

    float maxX = 0;
    float maxY = 0;

    int buttonCounter = 0;
    int rubbleCounter = 0;

    for (size_t r = 0; r < grid_.size(); r++) {
        for (size_t c = 0; c < grid_[r].size(); c++) {
            char tile = grid_[r][c];
            float x = c * TILE;
            float y = r * TILE;

            if (x + TILE > maxX) maxX = x + TILE;
            if (y + TILE > maxY) maxY = y + TILE;

            if (tile != 'W') {
                sf::RectangleShape f({TILE, TILE});
                f.setPosition(x, y);
                f.setTexture(&floorTexture_);
                floorTiles_.push_back(f);
            }

            if (tile == 'W') {
                sf::RectangleShape w({TILE, TILE});
                w.setPosition(x, y);
                w.setTexture(&wallTexture_);
                walls_.push_back(w);
            }
            else if (tile == 'P') {
                playerSpawn_ = {x + TILE/4, y + TILE/4};
            }
            else if (tile == 'E') {
                enemySpawn_ = {x + TILE/4, y + TILE/4};
            }
            else if (tile == 'D') {
                winDoorShape_.setSize({TILE, TILE});
                winDoorShape_.setTexture(&exitDoorTexture_);
                winDoorShape_.setPosition(x, y);
                winDoorBounds_ = winDoorShape_.getGlobalBounds();
            }
            else if (tile == 'C') {
                sf::RectangleShape cShape({TILE, TILE});
                cShape.setPosition(x, y);
                cShape.setTexture(&closetTexture_);
                closets_.push_back(cShape);
                closetVisited_.push_back(false);
            }
            else if (tile == 'T') {
                sf::RectangleShape t({TILE, TILE});
                t.setPosition(x, y);

                if ((currentLevelIndex_ == 0 && c > 20) || (currentLevelIndex_ == 1 && c == 0)) {
                    t.setTexture(&portalTexture_);

                    sf::Vector2u pSz = portalTexture_.getSize();
                    int partH = pSz.y / 3;
                    int rowOffset = static_cast<int>(r) - 9;

                    if (rowOffset >= 0 && rowOffset < 3) {
                        t.setTextureRect(sf::IntRect(0, rowOffset * partH, pSz.x, partH));
                    }
                } else {
                    t.setTexture(&doorTexture_);
                }

                transitionShapes_.push_back(t);
            }
            else if (tile == 'S') {
                if (!shovelTaken_) {
                    shovelShape_.setSize({TILE, TILE});
                    shovelShape_.setTexture(&shovelTexture_);
                    shovelShape_.setPosition(x, y);
                }
            }
            else if (tile == 'R') {
                sf::RectangleShape rShape({TILE, TILE});
                rShape.setPosition(x, y);
                rShape.setTexture(&rubbleTexture_);

                if (currentLevelIndex_ == 2) {
                    if (rubbleCounter >= static_cast<int>(rubbleActive_.size())) {
                        rubbleActive_.push_back(true);
                    }
                    if (rubbleActive_[rubbleCounter]) {
                        rubbleShapes_.push_back(rShape);
                    }
                    rubbleCounter++;
                } else {
                    rubbleShapes_.push_back(rShape);
                }
            }
            else if (tile == 'G') {
                generatorShape_.setSize({TILE, TILE});
                generatorShape_.setTexture(&generatorTexture_);
                if (generatorOn_) {
                    generatorShape_.setFillColor(sf::Color::White);
                } else {
                    generatorShape_.setFillColor(sf::Color(150, 150, 150));
                }
                generatorShape_.setPosition(x, y);
            }
            else if (tile == 'B') {
                sf::RectangleShape bShape({TILE/2, TILE/2});
                bShape.setPosition(x + TILE/4, y + TILE/4);

                int globalId = 0;
                if (currentLevelIndex_ == 2) {
                    if (buttonCounter == 0) globalId = 0;
                    else globalId = 1;
                } else if (currentLevelIndex_ == 3) {
                    if (buttonCounter == 0) globalId = 2;
                    else globalId = 3;
                }

                bool active = globalButtonStates_[globalId];
                bShape.setFillColor(active ? sf::Color::Green : sf::Color::Red);

                currentLevelButtons_.push_back({bShape, globalId, active});
                buttonCounter++;
            }
        }
    }

    background_.setSize(sf::Vector2f(maxX, maxY));
    dustOverlayShape_.setSize(sf::Vector2f(maxX, maxY));
    dustOverlayShape_.setTexture(&dustOverlayTexture_);
    dustOverlayShape_.setPosition(0.f, 0.f);
}

void Map::loadLevel(int index) {
    if (index >= 0 && index < static_cast<int>(levels_.size())) {
        currentLevelIndex_ = index;
        grid_ = levels_[currentLevelIndex_];
        if (currentLevelIndex_ == 2 && rubbleActive_.empty()) {
             rubbleActive_.assign(10, true);
        }
        buildMap();
    }
}

void Map::resetToFirstLevel() {
    shovelTaken_ = false;
    generatorOn_ = false;
    globalButtonStates_.assign(4, false);
    rubbleActive_.clear();
    doorSoundPlayed_ = false;
    loadLevel(0);
}

int Map::getCurrentLevel() const {
    return currentLevelIndex_;
}

const std::vector<sf::RectangleShape>& Map::getClosets() const {
    return closets_;
}

bool Map::isClosetVisited(size_t index) const {
    if (index < closetVisited_.size()) return closetVisited_[index];
    return true;
}

void Map::markClosetVisited(size_t index) {
    if (index < closetVisited_.size()) closetVisited_[index] = true;
}

sf::Vector2f Map::getMapSize() const {
    if (grid_.empty()) return {0.f, 0.f};
    float h = grid_.size() * TILE;
    float w = grid_[0].size() * TILE;
    return {w, h};
}

bool Map::collidesWithWall(const sf::FloatRect& box, bool ignoreClosets) const {
    for (const auto& w : walls_) {
        if (box.intersects(w.getGlobalBounds())) return true;
    }

    if (!ignoreClosets) {
        for (const auto& c : closets_) {
            if (box.intersects(c.getGlobalBounds())) return true;
        }
    }

    for (const auto& r : rubbleShapes_) {
        if (box.intersects(r.getGlobalBounds())) return true;
    }

    if (currentLevelIndex_ == 2 && box.intersects(generatorShape_.getGlobalBounds())) return true;

    return false;
}

bool Map::reachedWinDoor(const sf::FloatRect& bounds) const {
    if (bounds.intersects(winDoorBounds_)) {
        if (generatorOn_ && checkButtonCondition()) return true;
    }
    return false;
}

bool Map::reachedTransitionDoor(const sf::FloatRect& bounds) const {
    for (const auto& t : transitionShapes_) {
        if (bounds.intersects(t.getGlobalBounds())) return true;
    }
    return false;
}

sf::Vector2f Map::getPlayerSpawn() const { return playerSpawn_; }
sf::Vector2f Map::getEnemySpawn() const { return enemySpawn_; }
sf::Vector2f Map::getTransitionSpawn() const { return {0.f, 0.f}; }

void Map::draw(sf::RenderWindow& window) const {
    for (const auto& f : floorTiles_) window.draw(f);
    for (const auto& w : walls_) window.draw(w);
    for (const auto& c : closets_) window.draw(c);

    if (winDoorBounds_.width > 0) {
        sf::RectangleShape d = winDoorShape_;
        if (generatorOn_ && checkButtonCondition()) d.setFillColor(sf::Color::White);
        else d.setFillColor(sf::Color(100, 100, 100));
        window.draw(d);
    }

    for (const auto& t : transitionShapes_) window.draw(t);

    if (currentLevelIndex_ == 2 || currentLevelIndex_ == 3) {
        if (currentLevelIndex_ == 2 && !shovelTaken_) window.draw(shovelShape_);

        for (const auto& r : rubbleShapes_) window.draw(r);

        if (currentLevelIndex_ == 2) window.draw(generatorShape_);

        for (const auto& btn : currentLevelButtons_) {
            window.draw(btn.shape);
        }
    }

    if (currentLevelIndex_ == 1 || currentLevelIndex_ == 2) {
        window.draw(dustOverlayShape_);
    }
}

std::vector<sf::Vector2f> Map::findPath(const sf::Vector2f& start, const sf::Vector2f& target) const {
    int startX = static_cast<int>(start.x / TILE);
    int startY = static_cast<int>(start.y / TILE);
    int targetX = static_cast<int>(target.x / TILE);
    int targetY = static_cast<int>(target.y / TILE);

    int rows = static_cast<int>(grid_.size());
    int cols = static_cast<int>(grid_[0].size());

    if (startX < 0 || startX >= cols || startY < 0 || startY >= rows ||
        targetX < 0 || targetX >= cols || targetY < 0 || targetY >= rows) {
        return {};
    }

    struct Node { int x, y; };
    std::queue<Node> q;
    q.push({startX, startY});

    std::vector<std::vector<Node>> parent(rows, std::vector<Node>(cols, {-1, -1}));
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    visited[startY][startX] = true;

    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};
    bool found = false;

    while (!q.empty()) {
        Node current = q.front();
        q.pop();

        if (current.x == targetX && current.y == targetY) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
                if (!visited[ny][nx] && grid_[ny][nx] != 'W' && grid_[ny][nx] != 'C') {
                    if (grid_[ny][nx] == 'G') continue;

                    bool blockedByRubble = false;
                    for (const auto& r : rubbleShapes_) {
                        sf::Vector2f rPos = r.getPosition();
                        int rx = static_cast<int>(rPos.x / TILE);
                        int ry = static_cast<int>(rPos.y / TILE);
                        if (rx == nx && ry == ny) {
                            blockedByRubble = true;
                            break;
                        }
                    }
                    if (blockedByRubble) continue;

                    bool blockedByButton = false;
                    for (const auto& btn : currentLevelButtons_) {
                        sf::Vector2f bPos = btn.shape.getPosition();
                        int bx = static_cast<int>(bPos.x / TILE);
                        int by = static_cast<int>(bPos.y / TILE);
                        if (bx == nx && by == ny) {
                            blockedByButton = true;
                            break;
                        }
                    }
                    if (blockedByButton) continue;

                    visited[ny][nx] = true;
                    parent[ny][nx] = {current.x, current.y};
                    q.push({nx, ny});
                }
            }
        }
    }

    std::vector<sf::Vector2f> path;
    if (found) {
        int currX = targetX;
        int currY = targetY;
        while (currX != startX || currY != startY) {
            path.push_back({currX * TILE + TILE / 2.f, currY * TILE + TILE / 2.f});
            Node p = parent[currY][currX];
            currX = p.x;
            currY = p.y;
        }
        std::reverse(path.begin(), path.end());
    }

    return path;
}

bool Map::isShovelTaken() const { return shovelTaken_; }
void Map::takeShovel() { shovelTaken_ = true; }

bool Map::isRubbleCleared(int index) const {
    if (currentLevelIndex_ != 2) return true;
    if (index >= 0 && index < (int)rubbleActive_.size()) return !rubbleActive_[index];
    return true;
}

void Map::clearRubble(int index) {
    if (currentLevelIndex_ == 2 && index >= 0 && index < (int)rubbleActive_.size()) {
        rubbleActive_[index] = false;
        buildMap();
    }
}

int Map::getIntersectingRubbleIndex(const sf::FloatRect& bounds) const {
    if (currentLevelIndex_ != 2) return -1;

    int counter = 0;
    for (size_t r = 0; r < grid_.size(); r++) {
        for (size_t c = 0; c < grid_[r].size(); c++) {
            if (grid_[r][c] == 'R') {
                if (counter < (int)rubbleActive_.size() && rubbleActive_[counter]) {
                    sf::FloatRect rBounds(c * TILE, r * TILE, TILE, TILE);
                    if (bounds.intersects(rBounds)) return counter;
                }
                counter++;
            }
        }
    }
    return -1;
}

bool Map::isGeneratorOn() const { return generatorOn_; }
void Map::turnOnGenerator() {
    generatorOn_ = true;
    generatorSound_.play();
    checkDoorSound();
    buildMap();
}

void Map::toggleButton(int id) {
    if (id >= 0 && id < 4) {
        globalButtonStates_[id] = !globalButtonStates_[id];
        checkDoorSound();
        buildMap();
    }
}

void Map::checkDoorSound() {
    if (generatorOn_ && checkButtonCondition() && !doorSoundPlayed_) {
        doorBuzzerSound_.play();
        doorSoundPlayed_ = true;
    }
}

bool Map::checkButtonCondition() const {
    return (globalButtonStates_[0] == true && globalButtonStates_[1] == false &&
            globalButtonStates_[2] == false && globalButtonStates_[3] == true);
}

int Map::getIntersectingButtonIndex(const sf::FloatRect& bounds) const {
    for (const auto& btn : currentLevelButtons_) {
        if (bounds.intersects(btn.shape.getGlobalBounds())) return btn.id;
    }
    return -1;
}

sf::FloatRect Map::getShovelBounds() const { return shovelShape_.getGlobalBounds(); }
sf::FloatRect Map::getGeneratorBounds() const { return generatorShape_.getGlobalBounds(); }
const std::vector<sf::RectangleShape>& Map::getRubbleShapes() const { return rubbleShapes_; }
#pragma once
#include "Snake.hpp"
#include <queue>
#include <engine/Engine.h>
typedef std::function<void()> Delegate;

enum class MapPartType : int { empty = 0, snake_part = 1, food = 2 };

class Map;
class Configuration 
{
public:
    glm::vec3 vMapColor   = Helper::HexToRGB(0x2f2f2f);
    glm::vec3 vSnakeColor = Helper::HexToRGB(0xffffff);
    glm::vec3 vFoodColor  = Helper::HexToRGB(0xff0000);
    glm::vec3 vBlinkColor = Helper::HexToRGB(0xff0000);
    glm::ivec2 vSpacing = glm::ivec2(4);    // Spacing between parts in pixels.
    glm::ivec2 vMapOffset = glm::ivec2(30);  // Map offset in pixels.
    glm::ivec2 vPartSize = glm::ivec2(20);  // Size of a single part in pixels.
    int nSnakeMoveSpeed = 123;  // Move interval in milliseconds. NOTE: Use SET function for immidiate effect.
    int nInitSpawnFood = 5;
    bool bBufferMovementCommands = true;

    void SetSnakeGrow(int n) { nSnakeGrow = *pSnakeGrow = n; }
    const int& GetSnakeGrow() { return nSnakeGrow; }

private:
    int nSnakeGrow = 1;
    int* pSnakeGrow = nullptr;  // How much will snake grow when food is eaten.

    friend class Map;
};

class Map : public IObserver
{
    glm::ivec2 vMapSize = glm::ivec2(10, 10);
    std::queue<Utils::Event> qEvents;
    MapPartType* pMap = nullptr;
    Snake* pSnake = nullptr;
    Timer timer = Timer();
    int nScore = 0;
    bool blinkState = false;    // State of blinking. Used for visualizing bit part when GameOver.
    float fTime = 0.0f;

    // Buffer movements, if more are inputed in single tick (before snake moved)
    std::queue<Delegate> qMovementCommands;
    bool bCanMove = true;

    inline MapPartType& at(const glm::ivec2& pos) { return pMap[pos.y * vMapSize.x + pos.x]; }
    inline int posToIndex(const glm::ivec2& pos) { return pos.y * vMapSize.x + pos.x; }
    inline glm::ivec2 indexToPos(int index) { return { index % vMapSize.x, index / vMapSize.x }; }

    void onSnakeTmEnd(Timer* t)
    {
        pSnake->Move();
        bCanMove = true;
    }
    void processEvents()
    {
        while (!qEvents.empty())
        {
            Utils::Event e = qEvents.front();
            switch (e.message)
            {
            case MSG_SNAKE_MOVED:
            {
                MoveData data = std::any_cast<MoveData>(e.args);
                if (at(data.vNewPos) == MapPartType::food)
                {
                    pSnake->Eat();
                    nScore++;
                    respawnFood();
                }
                at(data.vNewPos) = MapPartType::snake_part;
                at(data.vRemovedPos) = MapPartType::empty;
            } break;
            case MSG_SNAKE_BIT_SELF:
            {
                timer.Stop();
                timer.Start(0.5f, [&blinkState = this->blinkState](Timer* t) { blinkState = !blinkState; });
                bGameOver = true;
                MoveData data = std::any_cast<MoveData>(e.args);
                vGameOverPos = data.vNewPos;

            } break;
            default:
                break;
            }
            qEvents.pop();
        }
    }
    // Create snake parts in map according to snake positions.
    void blitSnake()
    {
        for (auto&& part : pSnake->Parts)
            at(part) = MapPartType::snake_part;
    }
    void respawnFood(int n = 1)
    {
        int nSpawned = 0;
        while (nSpawned != n)
        {
            glm::ivec2 vRandPos(Helper::RandomInt(0, vMapSize.x - 1), Helper::RandomInt(0, vMapSize.y - 1));
            if (at(vRandPos) != MapPartType::empty)
                continue;
            
            at(vRandPos) = MapPartType::food;
            nSpawned++;
        }
    }

public:
    Configuration MapConf;

    bool bGameOver = false;
    glm::ivec2 vGameOverPos = glm::ivec2(0);

    Map(glm::ivec2 size)
        : vMapSize(size)
    {
        pMap = new MapPartType[size.x * size.y];
        memset(pMap, int(MapPartType::empty), (size.x * size.y) * sizeof(MapPartType));
    }
    ~Map()
    {
        delete[] pMap;
        delete pSnake;
    }
    void Init()
    {
        glm::ivec2 vSnakePos = vMapSize / 2;
        pSnake = new Snake(vSnakePos, vMapSize);
        pSnake->AddObserver(this);
        MapConf.pSnakeGrow = &pSnake->nEatGrow;
        *MapConf.pSnakeGrow = MapConf.nSnakeGrow;
        blitSnake();

        respawnFood(MapConf.nInitSpawnFood);
        timer.Repeat(true).Start(MapConf.nSnakeMoveSpeed * 0.001f, std::bind(&Map::onSnakeTmEnd, this, std::placeholders::_1));
    }

    void SetSnakeDirection(glm::ivec2 dir)
    {
        if (bCanMove)
        {
            pSnake->SetDirection(dir);
            bCanMove = false;
        }
        else if (MapConf.bBufferMovementCommands)
        {
            qMovementCommands.push([&, dir](){ 
                pSnake->SetDirection(dir);
                bCanMove = false;
            });
        }
    }
    void ProcessInput(InputInterface* input)
    {
        if (MapConf.bBufferMovementCommands && bCanMove && !qMovementCommands.empty())
        {
            qMovementCommands.front()();
            qMovementCommands.pop();
        }
        if (input->Pressed(Key::UP))
            SetSnakeDirection({  0, -1 });
        else if (input->Pressed(Key::DOWN))
            SetSnakeDirection({  0,  1 });
        else if (input->Pressed(Key::LEFT))
            SetSnakeDirection({ -1,  0 });
        else if (input->Pressed(Key::RIGHT))
            SetSnakeDirection({  1,  0 });
    }
    void Update(float dt)
    {
        if (!bGameOver)
            fTime += dt;
        timer.Update(dt);

        processEvents();
    }
    void Render(BasicRenderer* pBasicRenderer, SpriteRenderer* pSpriteRenderer)
    {
        const auto GetPixPos = [&](const glm::ivec2& pos) { return MapConf.vMapOffset + pos * (MapConf.vPartSize + MapConf.vSpacing); };

        int nLen = vMapSize.x * vMapSize.y;
        for (int i = 0; i < nLen; i++)
        {
            // Select tile color.
            glm::vec3 vColor(0.0f);
            switch (pMap[i])
            {
                case MapPartType::empty: vColor = MapConf.vMapColor; break;
                case MapPartType::food: vColor = MapConf.vFoodColor; break;
                case MapPartType::snake_part: vColor = MapConf.vSnakeColor; break;
            }
            if (bGameOver && indexToPos(i) == vGameOverPos && blinkState)
                vColor = MapConf.vBlinkColor;

            if (indexToPos(i) == pSnake->Parts.front())
                pSpriteRenderer->RenderSprite(ResourceManager::GetTexture("face"), GetPixPos(indexToPos(i)), MapConf.vPartSize, 0.0f, vColor);
            else
                pBasicRenderer->RenderShape(br_Shape::circle, GetPixPos(indexToPos(i)), MapConf.vPartSize, 0.0f, vColor);
        }
    }

    inline const int& GetScore() const { return nScore; }
    inline const float& GetTime() const { return fTime; }
    void SetSnakeMovementSpeed(int interval_millisec)
    {
        MapConf.nSnakeMoveSpeed = interval_millisec;
        if (!bGameOver)
            timer.SetDuration(interval_millisec * 0.001f);
    }

    // Observer implementation
    void OnNotify(IObserverSubject* obj, int message, std::any args = nullptr)
    {
        // Logger::LogI("Event: " + std::string(Utils::GetMessageName(message)), "src/include/Map.hpp", __LINE__);
        qEvents.push({ obj, message, args });
    }
};

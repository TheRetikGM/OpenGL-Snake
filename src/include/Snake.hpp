#pragma once
#include <glm/glm.hpp>
#include <memory.h>
#include <list>
#include <initializer_list>
#include <engine/BasicObserverSubject.hpp>
#include "messages.h"

struct MoveData
{
    glm::ivec2 vNewPos;
    glm::ivec2 vRemovedPos;
};

class Snake : public BasicObserverSubject
{
    int nLength = 1;
    glm::ivec2 vDirection = glm::ivec2(1, 0);
    glm::ivec2 vMapSize = glm::ivec2(0, 0);

public:
    std::list<glm::ivec2> Parts;
    int nEatGrow = 1;

    Snake(std::initializer_list<glm::ivec2> positions, glm::ivec2 mapSize)
        : vMapSize(mapSize)
    {
        Parts.assign(positions);
        nLength = Parts.size();
    }
    Snake(glm::ivec2 pos, glm::ivec2 mapSize)
        : vMapSize(mapSize)
    {
        Parts.push_back(pos);
    }
    ~Snake() {}

    // Clamp position to the map borders. Also connect the left-side to right-side and up-side to down-side
    glm::ivec2 ClampPos(glm::ivec2 pos)
    {
        if (pos.x < 0)
            pos.x = vMapSize.x + pos.x;
        if (pos.y < 0)
            pos.y = vMapSize.y + pos.y;
        if (pos.x >= vMapSize.x)
            pos.x = pos.x - vMapSize.x;
        if (pos.y >= vMapSize.y)
            pos.y = pos.y - vMapSize.y;
        return pos;
    }
    bool SnakeIsOn(glm::ivec2 pos)
    {
        for (auto&& p : Parts)
            if (p == pos)
                return true;
        return false;
    }

    void Move()
    {
        int msg = MSG_SNAKE_MOVED;

        glm::ivec2 vNewPosition = ClampPos(Parts.front() + vDirection);

        if (SnakeIsOn(vNewPosition))
            msg = MSG_SNAKE_BIT_SELF;

        MoveData data { vNewPosition, Parts.back() };
        Parts.pop_back();
        Parts.push_front(vNewPosition);

        notify(msg, data);
    }
    // Return false if chaging direction was unsuccessful.
    bool SetDirection(glm::ivec2 direction)
    {   
        // Only normalized direction vectors are allowed.
        if (std::abs(direction.x) != 1 && std::abs(direction.y) != 1)
            return false;

        // == Cannot bite self, when direction == -vDirection.
        // We cannot use (direction == -vDirection), because if the movement is not buffered
        // then it's possible to change the direction multiple times before the movement tick, 
        // which could lead to bitting self.
        // Instead we check if the direction results in bitting 2. snake part.
        if (Parts.front() + direction == *(++Parts.begin()))
            return false;
        vDirection = direction;

        notify(MSG_SNAKE_DIR_CHANGED, direction);
        return true;
    }

    void Eat()
    {
        nLength += nEatGrow;
        // Append nodes to the end of the snake. Positions of the nodes will be equal to the last node.
        for (int i = 0; i < nEatGrow; i++)
            Parts.emplace_back(Parts.back());
        
        notify(MSG_SNAKE_ATE_FOOD, nEatGrow);
    }
};
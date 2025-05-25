#pragma once

#include <vector>
#include <unordered_map>
#include "particle.hpp"

class SpatialGrid {
private:
    float cellSize;
    int gridWidth, gridHeight;
    std::unordered_map<int, std::vector<particle*>> grid;

public:
    SpatialGrid(float cellSize, int width, int height) 
        : cellSize(cellSize), gridWidth(width / cellSize + 1), gridHeight(height / cellSize + 1) {}

    void clear() {
        grid.clear();
    }

    void insert(particle& p) {
        int cellX = static_cast<int>(p.position.x / cellSize);
        int cellY = static_cast<int>(p.position.y / cellSize);
        int cellKey = cellY * gridWidth + cellX;
        grid[cellKey].push_back(&p);
    }

    std::vector<particle*> getNearbyParticles(const particle& p) {
        std::vector<particle*> nearby;
        int cellX = static_cast<int>(p.position.x / cellSize);
        int cellY = static_cast<int>(p.position.y / cellSize);

        for (int y = cellY - 1; y <= cellY + 1; ++y) {
            for (int x = cellX - 1; x <= cellX + 1; ++x) {
                if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
                    int cellKey = y * gridWidth + x;
                    auto it = grid.find(cellKey);
                    if (it != grid.end()) {
                        nearby.insert(nearby.end(), it->second.begin(), it->second.end());
                    }
                }
            }
        }
        return nearby;
    }
};
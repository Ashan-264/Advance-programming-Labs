#pragma once

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int BASE_SIZE = 64;
constexpr int PLAYER_MOVE_SPEED = 5;
constexpr int SCREEN = SCREEN_WIDTH - BASE_SIZE;

enum class GameState {
    Start,
    Playing,
    GameOver,
    Victory
};
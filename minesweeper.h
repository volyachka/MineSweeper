#pragma once

#include <string>
#include <vector>
#include <random>
#include <string>
#include <vector>
#include <random>
#include <time.h>
#include <set>
#include <deque>

class Minesweeper {
public:
    struct Cell {
        int x = 0;
        int y = 0;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;
    Minesweeper(size_t width, size_t height, size_t mines_count) {
        NewGame(width, height, mines_count);
    }
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
        NewGame(width, height, cells_with_mines);
    }

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

private:
    std::vector<std::vector<int>> matrix_;
    std::vector<std::vector<char>> play_field_;
    std::deque<struct Cell> neighbourhood_;
    size_t width_;
    size_t height_;
    GameStatus game_status_ = GameStatus::NOT_STARTED;
    time_t start_;
    mutable time_t end_;
    int flag_ = 0;
    size_t count_of_open_cells_ = 0;
    size_t count_of_flags_ = 0;
    size_t count_of_mines_ = 0;
};

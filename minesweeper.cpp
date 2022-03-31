#include "minesweeper.h"

void Minesweeper::OpenCell(const Cell &cell) {
    if (flag_ == 0) {
        flag_ = 1;
        start_ = time(nullptr);
    }
    if (matrix_[cell.y][cell.x] == -1) {
        end_ = time(nullptr);
        game_status_ = GameStatus::DEFEAT;
        for (auto i = 0; i < matrix_.size(); ++i) {
            for (auto j = 0; j < matrix_[0].size(); ++j) {
                if (matrix_[i][j] == 0) {
                    play_field_[i][j] = '.';
                } else if (matrix_[i][j] == -1) {
                    play_field_[i][j] = '*';
                } else {
                    play_field_[i][j] = matrix_[i][j] + '0';
                }
            }
        }
    } else if (matrix_[cell.y][cell.x] != 0) {
        play_field_[cell.y][cell.x] = matrix_[cell.y][cell.x] + '0';
        ++count_of_open_cells_;
    } else {
        neighbourhood_.push_front(cell);
        while (!neighbourhood_.empty()) {
            Cell current_cell = neighbourhood_[0];
            for (int k = -1; k != 2; ++k) {
                for (int l = -1; l != 2; ++l) {
                    auto coordinate_x_of_neighbours = current_cell.x + k;
                    auto coordinate_y_of_neighbours = current_cell.y + l;
                    if (coordinate_x_of_neighbours < width_ && coordinate_x_of_neighbours > -1 &&
                        coordinate_y_of_neighbours < height_ && coordinate_y_of_neighbours > -1) {
                        if (matrix_[coordinate_y_of_neighbours][coordinate_x_of_neighbours] == 0 &&
                            play_field_[coordinate_y_of_neighbours][coordinate_x_of_neighbours] == '-') {
                            ++count_of_open_cells_;
                            play_field_[coordinate_y_of_neighbours][coordinate_x_of_neighbours] = '.';
                            neighbourhood_.push_back(
                                Cell{.x = coordinate_x_of_neighbours, .y = coordinate_y_of_neighbours});
                        } else if (play_field_[coordinate_y_of_neighbours][coordinate_x_of_neighbours] == '-') {
                            ++count_of_open_cells_;
                            play_field_[coordinate_y_of_neighbours][coordinate_x_of_neighbours] =
                                matrix_[coordinate_y_of_neighbours][coordinate_x_of_neighbours] + '0';
                        }
                    }
                }
            }
            neighbourhood_.pop_front();
        }
    }
    if (count_of_open_cells_ == width_ * height_ - count_of_mines_) {
        end_ = time(nullptr);
        game_status_ = GameStatus::VICTORY;
    }
}

time_t Minesweeper::GetGameTime() const {
    end_ = time(nullptr);
    return end_ - start_;
};
void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell> &cells_with_mines) {
    count_of_mines_ = cells_with_mines.size();
    matrix_.clear();
    play_field_.clear();
    width_ = width;
    height_ = height;
    for (auto i = 0; i < height; ++i) {
        matrix_.emplace_back();
        play_field_.emplace_back();
        for (auto j = 0; j < width; ++j) {
            matrix_[i].push_back(0);
            play_field_[i].push_back('-');
        }
    }
    std::vector<int> number_of_mine;
    for (auto coordinates : cells_with_mines) {
        matrix_[coordinates.y][coordinates.x] = -1;
        for (int k = -1; k != 2; ++k) {
            for (int l = -1; l != 2; ++l) {
                auto coordinate_x_of_neighbours = coordinates.x + k;
                auto coordinate_y_of_neighbours = coordinates.y + l;
                if (coordinate_x_of_neighbours < width && coordinate_x_of_neighbours > -1 &&
                    coordinate_y_of_neighbours < height && coordinate_y_of_neighbours > -1 &&
                    matrix_[coordinate_y_of_neighbours][coordinate_x_of_neighbours] != -1) {
                    ++matrix_[coordinate_y_of_neighbours][coordinate_x_of_neighbours];
                }
            }
        }
    }
}
void Minesweeper::MarkCell(const Cell &cell) {
    if (flag_ == 0) {
        flag_ = 1;
        start_ = time(nullptr);
    }
    if (game_status_ == GameStatus::DEFEAT || game_status_ == GameStatus::VICTORY) {
    } else {
        if (play_field_[cell.y][cell.x] == '?') {
            --count_of_flags_;
            play_field_[cell.y][cell.x] = '-';
        } else {
            play_field_[cell.y][cell.x] = '?';
            ++count_of_flags_;
        }
        if (count_of_open_cells_ == width_ * height_ - count_of_mines_) {
            time(&end_);
            game_status_ = GameStatus::VICTORY;
        }
    }
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    count_of_mines_ = mines_count;
    matrix_.clear();
    play_field_.clear();
    width_ = width;
    height_ = height;
    std::random_device random_device;
    std::mt19937 rng(random_device());
    for (auto i = 0; i < height; ++i) {
        matrix_.emplace_back();
        play_field_.emplace_back();
        for (auto j = 0; j < width; ++j) {
            matrix_[i].push_back(0);
            play_field_[i].push_back('-');
        }
    }
    std::vector<int> number_of_mine;
    for (auto i = 0; i < width * height; ++i) {
        number_of_mine.push_back(i + 1);
    }
    size_t index_of_row;
    size_t index_of_column;
    std::shuffle(number_of_mine.begin(), number_of_mine.end(), random_device);
    for (auto i = 0; i < mines_count; ++i) {
        index_of_row = ceil(double(number_of_mine[i]) / width) - 1;
        index_of_column = number_of_mine[i] - index_of_row * width - 1;
        matrix_[index_of_row][index_of_column] = -1;
        for (int k = -1; k != 2; ++k) {
            for (int l = -1; l != 2; ++l) {
                auto coordinate_x_of_neighbours = index_of_column + k;
                auto coordinate_y_of_neighbours = index_of_row + l;
                if (coordinate_x_of_neighbours < width && coordinate_x_of_neighbours >= 0 &&
                    coordinate_y_of_neighbours < height && coordinate_y_of_neighbours >= 0 &&
                    matrix_[coordinate_y_of_neighbours][coordinate_x_of_neighbours] != -1) {
                    ++matrix_[coordinate_y_of_neighbours][coordinate_x_of_neighbours];
                }
            }
        }
    }
}

Minesweeper::RenderedField Minesweeper::RenderField() const {
    Minesweeper::RenderedField showing;
    for (auto &row : play_field_) {
        std::string str;
        for (auto &elem : row) {
            str += elem;
        }
        showing.push_back(str);
    }
    return showing;
}

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return game_status_;
}

#include <iostream>
#include <array>
#include <memory>

// Forward declaration
enum class Color { White, Black };
struct Position { int row, col; };
class ChessBoard;

// Abstract base class for pieces
enum class Color;
struct Position;
class Piece {
protected:
    Color color;

public:
    Piece(Color c) : color(c) {}
    virtual ~Piece() = default;

    Color getColor() const { return color; }

    // Check if moving from `from` to `to` is valid, given the board state
    virtual bool isMoveValid(const Position& from, const Position& to, const ChessBoard& board) const = 0;
};

// Concrete Pawn piece
class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c) {}

    bool isMoveValid(const Position& from, const Position& to, const ChessBoard& board) const override;
};

// Chess board class holding 8x8 grid of pieces
class ChessBoard {
    // nullptr means empty square
    std::array<std::array<Piece*, 8>, 8> squares{};

public:
    ChessBoard() { 
        for (auto& row : squares)
            row.fill(nullptr);
    }

    ~ChessBoard() {
        // Clean up owned pieces
        for (auto& row : squares)
            for (auto* p : row)
                delete p;
    }

    Piece* getPieceAt(const Position& pos) const {
        return squares[pos.row][pos.col];
    }

    void placePiece(Piece* piece, const Position& pos) {
        delete squares[pos.row][pos.col];  // remove existing piece, if any
        squares[pos.row][pos.col] = piece;
    }

    bool movePiece(const Position& from, const Position& to) {
        Piece* p = getPieceAt(from);
        if (!p) return false;

        if (!p->isMoveValid(from, to, *this))
            return false;

        // Execute move
        delete squares[to.row][to.col];          // capture if needed
        squares[to.row][to.col] = p;
        squares[from.row][from.col] = nullptr;
        return true;
    }
};

// Pawn movement: moves forward one square; no capturing logic here
bool Pawn::isMoveValid(const Position& from, const Position& to, const ChessBoard& board) const {
    int direction = (color == Color::White ? -1 : +1);
    // simple forward move
    if (to.col == from.col && to.row == from.row + direction
        && board.getPieceAt(to) == nullptr) {
        return true;
    }
    return false;
}

int main() {
    ChessBoard board;
    board.placePiece(new Pawn(Color::White), {6, 0});  // white pawn at row 6, col 0

    Position from{6, 0}, to{5, 0};
    if (board.movePiece(from, to))
        std::cout << "Move succeeded\n";
    else
        std::cout << "Move invalid\n";

    return 0;
}

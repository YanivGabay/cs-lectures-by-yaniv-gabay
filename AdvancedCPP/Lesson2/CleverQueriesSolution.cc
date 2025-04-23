// Piece.h
#pragma once
#include <functional>
#include <vector>

class Piece {
public:
    // The context holds *every* board-dependent query your pieces may need.
    struct BoardContext {
        // “What piece is on that square?”
        std::function<const Piece*(int x,int y)>  getPieceAt;
        // “Is the straight-line path clear?”
        std::function<bool(int x1,int y1,int x2,int y2)>  isPathClear;
        // “What colour is this piece?” (if you need to check friendly blockers)
        std::function<bool(int x,int y, bool white)>   isOccupiedBy;
        // you can add anything else: castling rights, en-passant, move history…
    };

    virtual bool isMoveValid(int fromX, int fromY,
                             int toX,   int toY,
                             const BoardContext& ctx) const = 0;
    virtual ~Piece() = default;
};
// Pawn.h
#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
    bool isMoveValid(int fx,int fy,int tx,int ty,
                     const BoardContext& ctx) const override
    {
        // one step forward if empty
        if (tx==fx && ty==fy+1 && ctx.getPieceAt(tx,ty)==nullptr)
            return true;

        // two-step on first move, path must be clear
        if (fy==1 && tx==fx && ty==fy+2 &&
            ctx.getPieceAt(fx,fy+1)==nullptr &&
            ctx.getPieceAt(fx,fy+2)==nullptr)
            return true;

        // diagonal capture
        if (abs(tx-fx)==1 && ty==fy+1 &&
            ctx.isOccupiedBy(tx,ty,false)) // occupied by black
            return true;

        return false;
    }
};
// ChessBoard.h
#pragma once
#include <array>
#include "Piece.h"

class ChessBoard {
    std::array<std::array<Piece*,8>,8> grid_;

  public:
    const Piece* get(int x,int y) const {
        if (x<0||x>=8||y<0||y>=8) return nullptr;
        return grid_[y][x];
    }

    // check that all squares between (x1,y1) → (x2,y2) are empty
    bool pathClear(int x1,int y1,int x2,int y2) const {
        int dx = (x2>x1) - (x2<x1);
        int dy = (y2>y1) - (y2<y1);
        int cx = x1+dx, cy = y1+dy;
        while (cx!=x2 || cy!=y2) {
            if (get(cx,cy)) return false;
            cx += dx; cy += dy;
        }
        return true;
    }

    bool movePiece(int fx,int fy,int tx,int ty) {
        Piece* p = grid_[fy][fx];
        if (!p) return false;

        // build the context with lambdas that capture *this*
        Piece::BoardContext ctx {
            /*getPieceAt=*/  [this](int x,int y){ return this->get(x,y); },
            /*isPathClear=*/ [this](int x1,int y1,int x2,int y2){
                                  return this->pathClear(x1,y1,x2,y2);
                              },
            /*isOccupiedBy=*/
                              [this](int x,int y,bool white){
                                  auto *q=this->get(x,y);
                                  return q && (q->isWhite()==white);
                              }
        };

        if (p->isMoveValid(fx,fy,tx,ty, ctx)) {
            std::swap(grid_[fy][fx], grid_[ty][tx]);
            return true;
        }
        return false;
    }
    // … your other board logic …

    bool isWhiteMoving() const;       // for context if you need turn info
    // etc.
};


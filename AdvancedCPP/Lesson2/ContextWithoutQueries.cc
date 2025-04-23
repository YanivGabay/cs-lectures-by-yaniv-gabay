// Piece.h
#pragma once

#include <vector>

class Piece;  // only to refer to Piece* inside MoveContext

struct MoveContext {
    int  fromX, fromY;
    int  toX,   toY;

    // What’s on the destination square (if anything)?
    const Piece*  targetPiece;

    // For straight‐line movers (rook, bishop, queen): 
    // the intervening squares along the path (exclusive)
    std::vector<const Piece*>  path;

    // If you need turn info or special flags, add them here:
    bool  whiteToMove;
    bool  canCastleKingside;
    bool  canEnPassant;
    // …whatever else your rules require…
};

class Piece {
public:
    virtual bool
    isMoveValid(const MoveContext& ctx) const = 0;

    virtual bool isWhite() const = 0;
    virtual ~Piece() = default;
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

    // Build only the info this move needs:
    MoveContext makeContext(int fx,int fy,int tx,int ty) const {
        MoveContext ctx;
        ctx.fromX = fx;  ctx.fromY = fy;
        ctx.toX   = tx;  ctx.toY   = ty;
        ctx.targetPiece = get(tx,ty);
        ctx.whiteToMove = /* your turn-tracker */;
        ctx.canCastleKingside = /* … */;
        ctx.canEnPassant      = /* … */;

        // if it’s a straight move, fill the path vector:
        int dx = (tx>fx) - (tx<fx),
            dy = (ty>fy) - (ty<fy);
        int cx = fx + dx, cy = fy + dy;
        while ((cx!=tx || cy!=ty) && (dx||dy)) {
            ctx.path.push_back(get(cx,cy));
            cx += dx; cy += dy;
        }

        return ctx;
    }

    bool movePiece(int fx,int fy,int tx,int ty) {
        Piece* p = grid_[fy][fx];
        if (!p) return false;

        auto ctx = makeContext(fx,fy,tx,ty);
        if (p->isMoveValid(ctx)) {
            std::swap(grid_[fy][fx], grid_[ty][tx]);
            return true;
        }
        return false;
    }

    // …other board logic (turns, castling rights, etc.)…
};
// Pawn.h
#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
    bool isWhite() const override { return true; /* or stored member */ }

    bool isMoveValid(const MoveContext& ctx) const override {
        int fx = ctx.fromX, fy = ctx.fromY;
        int tx = ctx.toX,   ty = ctx.toY;

        // single step
        if (tx==fx && ty==fy + (isWhite()?1:-1)
            && ctx.targetPiece==nullptr
            && ctx.path.empty())
            return true;

        // double step on first move
        if ((isWhite() && fy==1) || (!isWhite() && fy==6)) {
            if (tx==fx && (ty==fy + 2*(isWhite()?1:-1))
                && ctx.targetPiece==nullptr
                && ctx.path.size()==2
                && ctx.path[0]==nullptr)
                return true;
        }

        // capture
        if (std::abs(tx-fx)==1 && ty==fy + (isWhite()?1:-1)
            && ctx.targetPiece
            && ctx.targetPiece->isWhite()!=isWhite())
            return true;

        // en passant, promotions, etc. use ctx.canEnPassant, etc.

        return false;
    }
};

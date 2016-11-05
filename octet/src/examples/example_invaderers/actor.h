#pragma once

#ifndef actor_h
#define actor_h

namespace octet {

  class Actor {
  public: enum ActorType {
      PLAYER,
      GUARD
    };


  private:
    ActorType actor_type_;

    sprite sprite_;

    MapCell* occupied_cell_;  // Cell the actor is currently standing in.
    
    // Stores the player and NPCs.
    static std::vector<Actor>& actors_() {
      static std::vector<Actor> actors_;
      return actors_;
    };

    // Function Pointers
    void(*PlaySoundfx)(SoundManager::Sounds sound);  // TODO Don't need this now?


  public:

    Actor() {
      actors_().push_back(*this);  // When actors are created, add them to the actors list. TODO This doesn't work right now? Is a duplicate being pushed into the vector?
    }

    static std::vector<Actor>& Actors() {
      return actors_();
    }

    static Actor& Player() {
      return Actors().at(0);
    }

    // Slightly shorter than having to go through Actors() first.
    static Actor& GetActor(int actor_index) {
      return actors_().at(actor_index);
    }

    void Init(ActorType type,
              float x, float y,
              float w, float h) {
      // Actor textures
      GLuint player_texture = resource_dict::get_texture_handle(GL_RGBA,
      "assets/invaderers/player.gif");
      GLuint guard_texture = resource_dict::get_texture_handle(GL_RGBA,
      "assets/invaderers/guard.gif");
      
      int _texture;
      if (type == PLAYER) {
        _texture = player_texture;
        actor_type_ = PLAYER;
      }
      else if (type == GUARD) {
        _texture = guard_texture;
        actor_type_ = GUARD;
      }
      sprite_.init(_texture, x, y, w, h);
    }

    sprite& GetSprite() {
      return sprite_;
    }

    MapCell& OccupiedCell(MapCell* set_cell = NULL) {
      if (set_cell != NULL) {
        occupied_cell_ = set_cell;
      }
      return *occupied_cell_;
    }

    int Update() {
      // Else ifs because player is not allowed to move diagonally.
      if (actor_type_ == PLAYER) {
        if (OccupiedCell().GetType() == MapCell::GOAL) {
          return 10;
        }
        if (app_common::is_key_down(key_W)) {
          MapCell& destination_cell = occupied_cell_->GetAdjacentCell(NORTH);
          MoveToCell(destination_cell);
          GetSprite().SetLocalRotation(90);
          return NORTH;
        }
        else if (app_common::is_key_down(key_S)) {
          MapCell& destination_cell = occupied_cell_->GetAdjacentCell(SOUTH);
          MoveToCell(destination_cell);
          GetSprite().SetLocalRotation(270);
          return SOUTH;
        }
        else if (app_common::is_key_down(key_A)) {
          MapCell& destination_cell = occupied_cell_->GetAdjacentCell(WEST);
          MoveToCell(destination_cell);
          GetSprite().SetLocalRotation(180);
          return WEST;
        }
        else if (app_common::is_key_down(key_D)) {
          MapCell& destination_cell = occupied_cell_->GetAdjacentCell(EAST);
          MoveToCell(destination_cell);
          GetSprite().SetLocalRotation(0);
          return EAST;
        }
      }
      return 4;
    }

    // Moves the actor to the new cell, so long as cell is walkable.
    int MoveToCell(MapCell& destination_cell) {
      if (destination_cell.IsWalkable()) {
        sprite_.set_relative(destination_cell.GetSprite(), 0, 0);
        OccupiedCell(&destination_cell);
        return 1;
      }
      else {
        SoundManager::GameSound().PlaySoundfx(SoundManager::SFX_WRONG);
        printf("Cant walk there.\n");
        return 0;
      }
    }

  };


}

#endif // !actor_h

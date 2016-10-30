#pragma once

#ifndef actor_h
#define actor_h

namespace octet {

  class Actor {
 
    //Actor* Actor::player_ = NULL;  // Can't define a static member var in a .h file, but without initialising the static pointer player_ it creates a compalation linking error.
    // In order to have static class variables, they need to be initialised, which can only be done in the .cpp file as you can't double declare in a header. As we're working with 'header only' I needed a workaround.
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4424.pdf
    // http://stackoverflow.com/questions/18860895/how-to-initialize-static-members-in-the-header
    // Suggested answer is to use a static function which returns it's own static variable of the type desired. I have modified the example to enable Set and Get behaviour within the same funtion.
    static Actor*& player_(Actor* player_object = NULL) {
      static Actor* player_ = NULL;
      if (player_object != NULL) {
        player_ = player_object;
      }
      return player_;
    }

    sprite sprite_;

    MapCell* occupied_cell_;  // Cell the actor is currently standing in.
    
    static std::vector<Actor>& actors_() { static std::vector<Actor> actors_; return actors_; };  // Stores the player and NPCs.

  public:
    enum ActorType {
      PLAYER,
      GUARD
    };

    Actor() {
      actors_().push_back(*this);  // When actors are created, add them to the actors list. TODO This doesnt work right now? Is a duplicate being pushed into the vector?
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
        player_(this);
      }
      else if (type == GUARD) {
        _texture = guard_texture;
      }
      sprite_.init(_texture, x, y, w, h);
    }

    static Actor& Player() {
      return *player_();
    }

    sprite& GetSprite() {
      return sprite_;
    }

    static std::vector<Actor>& Actors() {
      return actors_();
    }

    static Actor& GetActor(int actor_index) {
      return actors_().at(actor_index);
    }

    MapCell& OccupiedCell(MapCell* set_cell = NULL) {
      if (set_cell != NULL) {
        occupied_cell_ = set_cell;
      }
      return *occupied_cell_;
    }

    int Update() {
      // Else ifs because player is not aloud to move diagonally.
      if (app_common::is_key_down(key_W)) {
        MapCell& destination_cell = occupied_cell_->GetAdjacentCell(NORTH);
        return MoveToCell(destination_cell);
      }
      else if (app_common::is_key_down(key_S)) {
        MapCell& destination_cell = occupied_cell_->GetAdjacentCell(SOUTH);
        return MoveToCell(destination_cell);
      }
      else if (app_common::is_key_down(key_A)) {
        MapCell& destination_cell = occupied_cell_->GetAdjacentCell(WEST);
        return MoveToCell(destination_cell);
      }
      else if (app_common::is_key_down(key_D)) {
        MapCell& destination_cell = occupied_cell_->GetAdjacentCell(EAST);
        return MoveToCell(destination_cell);
      }
      return 0;
    }

    // Moves the actor to the new cell, so long as cell is walkable.
    int MoveToCell(MapCell& destination_cell) {
      if (destination_cell.IsWalkable()) {
        sprite_.set_relative(destination_cell.GetSprite(), 0, 0);
        OccupiedCell(&destination_cell);
        return 1;
      }
      else {
        printf("Cant walk there.\n");
        return 0;
      }
    }

  };


}

#endif // !actor_h

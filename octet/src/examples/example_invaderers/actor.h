#pragma once

#ifndef actor_h
#define actor_h

namespace octet {

  class Actor {
 
    static Actor* player_;

    sprite sprite_;
    MapCell cell_occupied_;

    std::vector<Actor> actors_;  // Stores the player and NPCs.

    const GLuint player_texture = resource_dict::get_texture_handle(GL_RGBA,
      "assets/invaderers/player.gif");
    const GLuint guard_texture = resource_dict::get_texture_handle(GL_RGBA,
      "assets/invaderers/guard.gif");

  public:
    enum ActorType {
      PLAYER,
      GUARD
    };

    Actor() {
      actors_.push_back(*this);
    }

    sprite& GetSprite() {
      return sprite_;
    }

    void Init(ActorType type,
              float x, float y,
              float w, float h) {
      int _texture;
      if (type == PLAYER) {
        _texture = player_texture;
        player_ = this;
      }
      else if (type == GUARD) {
        _texture = guard_texture;
      }
      sprite_.init(_texture, x, y, w, h);
    }

    void Update() {
      if (invaderers_app::GameApp().is_key_down(key_W)) {
        MapCell destination_cell = cell_occupied_.Above();
        sprite_.set_relative(destination_cell.Sprite(), 0, 0);
      }
      if (invaderers_app::GameApp().is_key_down(key_S)) {
        MapCell destination_cell = cell_occupied_.Below();
        sprite_.set_relative(destination_cell.Sprite(), 0, 0);
      }
      if (invaderers_app::GameApp().is_key_down(key_A)) {
        MapCell destination_cell = cell_occupied_.Left();
        sprite_.set_relative(destination_cell.Sprite(), 0, 0);
      }
      if (invaderers_app::GameApp().is_key_down(key_D)) {
        MapCell destination_cell = cell_occupied_.Right();
        sprite_.set_relative(destination_cell.Sprite(), 0, 0);
      }
    }

    static Actor& Player() {
      return *player_;
    }

    Actor& Actors(int actor_index) {
      return actors_.at(actor_index);
    }

    // Adds the given actor object to the end of the Actors array.
    int AddActor(Actor actor) {
      actors_.push_back(actor);
      return actors_.size() - 1; // Return index of added actor.
    }
  };


}

#endif // !actor_h

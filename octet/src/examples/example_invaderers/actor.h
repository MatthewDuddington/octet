#pragma once

#ifndef actor_h
#define actor_h

namespace octet {

  class Actor {
    sprite sprite_;
    MapCell cell_occupied_;

  public:
    Actor() {
      Level::CurrentLevel().AddActor(*(this));
    }

    const sprite &Sprite() {
      return sprite_;
    }

    void Init(int _texture,
      MapCell::CellType CellType,
      float x, float y,
      float w, float h) {
      sprite_.init(_texture, x, y, w, h);
    }

    void Update() {
      if (invaderers_app::GameApp().is_key_down(app_common::key_W)) {
        MapCell other_cell = cell_occupied_.Above();
        sprite_.set_relative(other_cell.Sprite(), 0, 0);
      }
      if (invaderers_app::GameApp().is_key_down(app_common::key_S)) {
        MapCell other_cell = cell_occupied_.Below();
        sprite_.set_relative(other_cell.Sprite(), 0, 0);
      }
      if (invaderers_app::GameApp().is_key_down(app_common::key_A)) {
        MapCell other_cell = cell_occupied_.Left();
        sprite_.set_relative(other_cell.Sprite(), 0, 0);
      }
      if (invaderers_app::GameApp().is_key_down(app_common::key_D)) {
        MapCell other_cell = cell_occupied_.Right();
        sprite_.set_relative(other_cell.Sprite(), 0, 0);
      }
    }
  };


}

#endif // !actor_h

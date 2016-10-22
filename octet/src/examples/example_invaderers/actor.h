#pragma once

#ifndef actor_h
#define actor_h

namespace octet {

  class Actor {
    sprite sprite_;

  public:
    const sprite &Sprite() {
      return sprite_;
    }

    void Init(int _texture,
      MapCell::CellType CellType,
      float x, float y,
      float w, float h) {
      sprite_.init(_texture, x, y, w, h);
    }
  };

}

#endif // !actor_h

#pragma once

#ifndef input_h
#define input_h

namespace octet {

  class Input {
    
    key key_down_;

  public:
    
    bool IsKeyDown(key key_code) {
      return key_code == key_down_ ? true : false;
    }

    void SetKeyDown(key key_code) {
      key_down_ = key_code;
    }

  };

}

#endif // !input_h
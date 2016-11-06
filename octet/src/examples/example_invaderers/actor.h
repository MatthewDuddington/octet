////////////////////////////////////////////////////////////////////////////////
//
//  Octet Framework (c) Andy Thomason 2012-2014.
//  Modular Framework for OpenGLES2 rendering on multiple platforms.
//
//  (c) Matthew Duddington 2016.
//

#ifndef actor_h
#define actor_h

namespace octet {

  class Actor {

    enum ActorType;
    ActorType actor_type_;

    sprite sprite_;
    MapCell* occupied_cell_;  // Cell the actor is currently standing in.
    
    // Stores the player and NPCs.
    static std::vector<Actor>& actors_()
    {
      static std::vector<Actor> actors_;
      return actors_;
    };


  public:
    enum ActorType {
      PLAYER,
      GUARD
    };

    enum UpdateReturnCodes {
      PRESSED_EAST = EAST,
      PRESSED_NORTH = NORTH,
      PRESSED_WEST = WEST,
      PRESSED_SOUTH = SOUTH,
      ACTOR_IDLE = 4,
      CAUGHT_BY_GUARD = 9,
      GOAL_REACHED = 10
    };

    Actor() { Actor::Actors().push_back(*this); }

    static std::vector<Actor>& Actors() { return actors_(); }

    // Slightly shorter than having to go through Actors() first.
    static Actor& GetActor( int actor_index ) { return actors_().at(actor_index); }

    static Actor& Player() { return Actors().at(0); }

    void Init( ActorType type
      ,        float x, float y
      ,        float s )  // Width and height are the same.
    {
      // Actor textures
      GLuint player_texture =
        resource_dict::get_texture_handle( GL_RGBA, "assets/invaderers/player.gif" );
      GLuint guard_texture =
        resource_dict::get_texture_handle( GL_RGBA, "assets/invaderers/guard.gif" );
      
      int _texture;
      if ( type == PLAYER )
      {
        _texture = player_texture;
        actor_type_ = PLAYER;
      }
      else if ( type == GUARD )
      {
        _texture = guard_texture;
        actor_type_ = GUARD;
      }

      sprite_.init( _texture, x, y, s, s );
    }

    // Return a reference to the Actor's sprite.
    sprite& GetSprite() { return sprite_; }

    // Update or Get the map cell the Actor is currently standing in.
    MapCell& OccupiedCell( MapCell* set_cell = NULL )
    {
      if (set_cell != NULL) { occupied_cell_ = set_cell; }
      return *occupied_cell_;
    }

    int Update() {
      if (actor_type_ == PLAYER)
      {
        // Check for standing on goal.
        if (OccupiedCell().GetType() == MapCell::GOAL) { return GOAL_REACHED; }

        // Check for player pressing movement keys.
        // (Else ifs because player is not allowed to move diagonally)
        if (app_common::is_key_down(key_D))
        {
          MapCell& destination_cell = occupied_cell_->GetAdjacentCell(EAST);
          MoveToCell(destination_cell);
          GetSprite().LocalRotation(R_EAST);
          return PRESSED_EAST;
        }
        else if (app_common::is_key_down(key_W))
        {
          MapCell& destination_cell = occupied_cell_->GetAdjacentCell(NORTH);
          MoveToCell(destination_cell);
          GetSprite().LocalRotation(R_NORTH);
          return PRESSED_NORTH;
        }
        else if (app_common::is_key_down(key_A))
        {
          MapCell& destination_cell = occupied_cell_->GetAdjacentCell(WEST);
          MoveToCell(destination_cell);
          GetSprite().LocalRotation(R_WEST);
          return PRESSED_WEST;
        }
        else if (app_common::is_key_down(key_S))
        {
          MapCell& destination_cell = occupied_cell_->GetAdjacentCell(SOUTH);
          MoveToCell(destination_cell);
          GetSprite().LocalRotation(R_SOUTH);
          return PRESSED_SOUTH;
        }
        // else Do nothing.
        return ACTOR_IDLE;
      }

      // else Actor == Guard
      // If player moves into a cell in line of sight distance of a guard...
      if ( OccupiedCell().IsInLineWithMe( Actor::Player().OccupiedCell()
                                        , GetSprite().LocalRotationDirection()
                                        , 2 )
        || &OccupiedCell() == &Actor::Player().OccupiedCell() )
        // ...or actually moves into the same cell as the guard. TODO Optionally this second condition could be ignored and instead allow the player to 'knock out' guards when approaching from the side or behind.
      { return CAUGHT_BY_GUARD; }  // Main app will reset level.
      // else Do nothing.
      return ACTOR_IDLE;
    }

    // Moves the actor to the new cell, so long as cell is walkable.
    int MoveToCell( MapCell& destination_cell )
    {
      if (destination_cell.IsWalkable())
      {
        sprite_.set_relative( destination_cell.GetSprite(), 0, 0 );
        OccupiedCell(&destination_cell);
        return 1;
      }
      // else
      SoundManager::GameSound().PlaySoundfx(SoundManager::SFX_WRONG);
      printf( "Cant walk there.\n" );
      return 0;
    }

  };

}

#endif // !actor_h

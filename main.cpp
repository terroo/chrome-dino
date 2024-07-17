#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include <array>
#include "spritesheet.hpp"

int main(){

  srand(static_cast<unsigned>(time(nullptr)));

  sf::RenderWindow window(
      sf::VideoMode(1280,720), 
      "chrome://dino",
      sf::Style::Titlebar | sf::Style::Close
  ); 
  window.setPosition(sf::Vector2i(100, 100));
  window.setFramerateLimit(60);

  sf::Texture texture;
  //texture.loadFromFile("./spritesheet.png");
  texture.loadFromMemory(data.data(), data.size());

  sf::Sprite ground(texture), ground_back(texture);
  ground.setTextureRect(sf::IntRect(2, 104, 2440, 26));
  ground.setPosition(0, 520);
  ground_back = ground;

  constexpr int gap = {24};
  sf::Sprite dino(texture);
  dino.setTextureRect(sf::IntRect(1678, 2, 88, 94));
  dino.setPosition(150.f, ground.getPosition().y - (dino.getTextureRect().height - gap));

  float frame_ground = 0.f;
  float frame_dino   = 0.f;

  float gravity = ground.getPosition().y - (dino.getTextureRect().height - gap);
  float velocity = 0.f;
  float jump = -20.f;

  float frame_down = 0.f;
  bool crouch = {false};
  bool is_crouching = {false};

  std::vector<sf::IntRect> objs = {
    sf::IntRect(260, 14, 92, 68), // frame 1 do pássaro(2 frames)
    sf::IntRect(446, 2, 68, 70), // 2 cactus pequenos
    sf::IntRect(752, 2, 50, 96), // 1 cactu grande
    sf::IntRect(652, 2, 98, 96), // 2 cactus grandes
    sf::IntRect(848, 2, 104, 98), // 3 cactus
  };
  std::vector<sf::Sprite> sprites = {};
  int count = {0};
  float frame_bird = 0.f;

  sf::Sprite hi(texture);
  hi.setTextureRect(sf::IntRect(1494, 2, 38, 21));
  hi.setPosition(955, 167);
  float hi_start_x = 1020.f;
  int num_hi = 0;
  float spacing = 20.f;
  constexpr int total = {5};

  std::unordered_map<char, sf::IntRect> digits = {
    {'0', sf::IntRect(1294, 2, 18, 21)},
    {'1', sf::IntRect(1316, 2, 18, 21)}, 
    {'2', sf::IntRect(1334, 2, 18, 21)}, 
    {'3', sf::IntRect(1354, 2, 18, 21)}, 
    {'4', sf::IntRect(1374, 2, 18, 21)}, 
    {'5', sf::IntRect(1394, 2, 18, 21)}, 
    {'6', sf::IntRect(1414, 2, 18, 21)}, 
    {'7', sf::IntRect(1434, 2, 18, 21)}, 
    {'8', sf::IntRect(1454, 2, 18, 21)}, 
    {'9', sf::IntRect(1474, 2, 18, 21)} 
  };
  float start_x = 1140;
  float start_y = hi.getPosition().y;

  sf::RectangleShape rect_dino(sf::Vector2f(40.f, 70.f));
  //rect_dino.setFillColor(sf::Color::Red);
  rect_dino.setFillColor(sf::Color(255, 255, 255, 0));
  std::vector<sf::RectangleShape> rect_objs = {};
  bool gameover = {false};

  sf::Sprite sprite_gameover(texture);
  sprite_gameover.setTextureRect(sf::IntRect(1295, 29, 380, 21));
  sprite_gameover.setPosition(
    window.getSize().x / 2.f - sprite_gameover.getTextureRect().width / 2.f,
    window.getSize().y / 2.f - sprite_gameover.getTextureRect().height / 2.f
  ); 
  sf::Sprite icon_restart(texture);
  icon_restart.setTextureRect(sf::IntRect(506, 130, 72, 64));
  icon_restart.setPosition(
    window.getSize().x / 2.f - icon_restart.getTextureRect().width / 2.f,
    window.getSize().y / 2.f - icon_restart.getTextureRect().height / 2.f + 70.f
  ); 

  int color = {255};
  bool day {true}, change_day {false};

  sf::Sprite sun(texture);
  sf::Sprite cloud(texture);
  sf::Sprite star1(texture);
  sf::Sprite star2(texture);
  std::vector<sf::Sprite> clouds = {};


  sun.setTextureRect(sf::IntRect(1074, 2, 80, 80));
  sun.setPosition(
    window.getSize().x / 2.f - sun.getTextureRect().width / 2.f,
    window.getSize().y / 2.f - sun.getTextureRect().height / 2.f - 150.f
  ); 
  sun.setColor(sf::Color(255, 255, 255, 100));

  cloud.setTextureRect(sf::IntRect(166, 2, 92, 27));
  cloud.setColor(sf::Color(255, 255, 255, 170));
  for (size_t i {}; i < 6; ++i){
    clouds.push_back(cloud);
  }
  clouds[0].setPosition(100.f, 100.f);
  clouds[1].setPosition(300.f, 300.f);
  clouds[2].setPosition(1280.f, 200.f);
  clouds[3].setPosition(650.f, 60.f);
  clouds[4].setPosition(830.f, 150.f);
  clouds[5].setPosition(1000.f, 400.f);

  star1.setTextureRect(sf::IntRect(1274, 39, 18, 17));
  star1.setColor(sf::Color(255, 255, 255, 70));
  star2 = star1;
  star1.setPosition(200.f, 200.f);
  star2.setPosition(800.f, 300.f);

  float speed = 0.2;

  while( window.isOpen() ){
    sf::Event event;
    while( window.pollEvent(event)){
      if( event.type == sf::Event::Closed ){
        window.close();
      }

      if(event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::Space && gravity == ground.getPosition().y - (dino.getTextureRect().height - gap)){
          velocity = jump;

        }

        if(event.key.code == sf::Keyboard::Down && !is_crouching && gravity == ground.getPosition().y - (dino.getTextureRect().height - gap)){
          crouch = true;
          is_crouching = true;

        }

      }

      if(event.type == sf::Event::KeyReleased){
        if(event.key.code == sf::Keyboard::Down){
          crouch = false;
          is_crouching = false;
        }
      }
    }

    if(!gameover){
      int rand_distance = std::rand() % 2 + 1;
      //if(count % (50 * rand_distance - (int)speed) == 0){ HARD MODE
      if(count % (50 * rand_distance) == 0){
        int rand_obj = std::rand() % objs.size();
        sf::Sprite sprite(texture);
        sprite.setTextureRect(objs[rand_obj]);

        if(sprite.getTextureRect().height == 68){
          sprite.setPosition(
              window.getSize().x,
              410
              );
        }else{
          sprite.setPosition(
              window.getSize().x,
              520 - sprite.getTextureRect().height + gap
              );
        }

        sf::RectangleShape rect_obj(sf::Vector2f(
              sprite.getTextureRect().width,
              sprite.getTextureRect().height
              ));
        //rect_obj.setFillColor(sf::Color::Blue);
        rect_obj.setFillColor(sf::Color(255, 255, 255, 0));
        rect_obj.setPosition(sprite.getPosition());
        sprites.push_back(sprite);
        rect_objs.push_back(rect_obj);
      }

      if(!crouch){
        velocity += 1.f;
        if(velocity < jump){
          velocity = jump;
        }
      }

      gravity += velocity;
      if(gravity > ground.getPosition().y - (dino.getTextureRect().height - gap)){
        gravity = ground.getPosition().y - (dino.getTextureRect().height - gap);
        velocity = 0.f;
      }

      frame_dino += 0.2;
      if(frame_dino > 4.f){
        frame_dino -= 4.f;
      }

      frame_down += 0.1f;
      if(frame_down > 1.5f){
        frame_down -= 1.5f;
      }

      frame_ground -= 8.f;
      if(frame_ground < -ground.getTextureRect().width){
        frame_ground = 0.f;
      }
      ground.setPosition(frame_ground - speed, ground.getPosition().y);
      ground_back.setPosition(frame_ground - speed + (ground.getTextureRect().width - 40), ground.getPosition().y);

      if(crouch){
        dino.setTextureRect(sf::IntRect(2206 + 118 * static_cast<int>(frame_down), 36, 118, 60));
        dino.setPosition(dino.getPosition().x, gravity + 30);    
        rect_dino.setSize(sf::Vector2f(100.f, 40.f));
        rect_dino.setPosition(dino.getPosition().x + 20.f, gravity + 40.f);
      }else{
        dino.setTextureRect(sf::IntRect(1678 + 88 * static_cast<int>(frame_dino), 2, 88, 94));
        dino.setPosition(dino.getPosition().x, gravity);
        rect_dino.setSize(sf::Vector2f(40.f, 70.f));
        rect_dino.setPosition(dino.getPosition().x + 20.f, gravity + 10.f);
      }

      for (size_t i {}; i < sprites.size(); ++i){
        if(sprites[i].getTextureRect().height == 68){
          frame_bird += 0.08f;
          if(frame_bird > 1.5f){
            frame_bird -= 1.5f;
          }
          sprites[i].setTextureRect(sf::IntRect(260 + 92 * static_cast<int>(frame_bird), 14, 92, 68));
        }
        sprites[i].move(-10.f - speed, 0);
        rect_objs[i].move(-10.f - speed, 0);

        if(rect_dino.getGlobalBounds().intersects(rect_objs[i].getGlobalBounds())){
          gameover = true;
        }

        if(sprites[i].getPosition().x < -sprites[i].getTextureRect().width){
          sprites.erase(sprites.begin() + i);
          rect_objs.erase(rect_objs.begin() + i);
        }
      }

      for (size_t i {}; i < clouds.size(); ++i){
        clouds[i].setPosition(
            clouds[i].getPosition().x - 0.2f,
            clouds[i].getPosition().y 
            ); 
        if(clouds[i].getPosition().x < -clouds[i].getTextureRect().width){
          clouds[i].setPosition(1280, clouds[i].getPosition().y);
        }
      }

      if(color < 200){
        sun.setTextureRect(sf::IntRect(1034, 2, 40, 80));
      }else{
        sun.setTextureRect(sf::IntRect(1074, 2, 80, 80));
      }

    } // GAME OVER


    if(count % 250 == 0){
      speed += 1.f;
    }

    if(count % 1000 == 0 && count > 1){
      change_day = true;
    }

    if(change_day){
      if(day){
        color -= 3;
        if(color <= 30){
          color = 30;
          day = false;
          change_day = false;
        }
      }else{
        color += 3;
        if(color >= 255){
          color = 255;
          day = true;
          change_day = false;
        }
      }
    }

    window.clear(sf::Color(color, color, color));

    window.draw(sun);
    for (size_t i {}; i < clouds.size(); ++i){
      window.draw(clouds[i]);
    }
    window.draw(star1);
    window.draw(star2);

    window.draw(ground);
    window.draw(ground_back);

    for (size_t i {}; i < sprites.size(); ++i){
      window.draw(rect_objs[i]);
      window.draw(sprites[i]);
    }

    window.draw(rect_dino);
    window.draw(dino);

    std::stringstream ss;
    ss << std::setw(total) << std::setfill('0') << count;
    std::string count_str = ss.str();
    float x = start_x;
    for(char digit : count_str){
      sf::Sprite sprite(texture);
      sprite.setTextureRect(digits[digit]);
      sprite.setPosition(x, start_y);
      window.draw(sprite);
      x += spacing;
    }

    window.draw(hi);

    std::stringstream hi_ss;
    hi_ss << std::setw(total) << std::setfill('0') << num_hi;
    std::string hi_str = hi_ss.str();
    float pos_x_hi = hi_start_x;
    for(char digit : hi_str){
      sf::Sprite sprite(texture);
      sprite.setTextureRect(digits[digit]);
      sprite.setPosition(pos_x_hi, start_y);
      window.draw(sprite);
      pos_x_hi += spacing;
    }

    if(gameover){
      window.draw(sprite_gameover);
      window.draw(icon_restart);
      dino.setTextureRect(sf::IntRect(2122, 6, 80, 86));
      if(count > num_hi){
        num_hi = count;
      }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
        count = 0;
        speed = 0.2f;
        rect_objs.clear();
        sprites.clear();
        gravity = ground.getPosition().y - (dino.getTextureRect().height - gap);
        velocity = 0.f;
        day = true;
        color = 255;
        change_day = false;
        gameover = false;
      }
    }else{
      ++count;
      if(count >= 99999){
        count = 99999;
      }
    }


    window.display();
  }

  return EXIT_SUCCESS;
}
// g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system
// g++ -g -Wall -Wextra -fsanitize=address main.cpp -lsfml-graphics -lsfml-window -lsfml-system && ./a.out

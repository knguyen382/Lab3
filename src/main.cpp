#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <ctime>
#include <string>
#include <random>
#include <SFML/Graphics.hpp>

#define WIDTH 29
#define HEIGHT 32
#define PACMAN_SPEED 3
#define GHOST_SPEED 4

using namespace sf;

unsigned int score = 0;
unsigned int lives = 3;
unsigned int pellet_count = 230;


unsigned int pellet_limiter = 0;
unsigned int power_limiter = 0;
unsigned int wall_limiter = 0;


//self explainatory variables
bool red_death, blue_death, orange_death, pink_death;
bool paused;
bool power = 0;
bool left,right,up,down{false};
bool start = false;
bool acceptInput = false;
bool game_over;


// vectors that store map elements
std::vector <RectangleShape> wall_vector;
std::vector <CircleShape> pellet_vector;
std::vector <CircleShape> power_vector;


// PROTOTYPE FUNCTIONS

bool wall_collision(Sprite &pac, Vector2f &pos, char direction);
void ghost_collision(Sprite &pac, Sprite &ghost, char color);
void edible_collision(Sprite &pac);


//BIT MAP
std::vector <std::string> game_map
	{
		"-----------------------------",
		"-............ -............ -",
		"-.    .     . -.     .    . -",
		"-0 ---. ----. -. ----. ---0 -",
		"-. ---. ----. -. ----. ---. -",
		"-.......................... -",
		"-.    .              .    . -",
		"-. ---. -------------. ---. -",
		"-...... -     -     -...... -",
		"-     . -     -     -.      -",
		"------. ----  -  ----. ------",
		"------. -     rbop  -. ------",
		"------. -           -. ------",
		"------. -  -------  -. ------",
		"i     .    -------   .      o",
		"      .    -------   .       ",
		"------. -  -------  -. ------",
		"------. -           -. ------",
		"------. -           -. ------",
		"------. -  -------  -. ------",
		"-............ -............ -",
		"-.    .     . -.     .    . -",
		"-. ---. ----. -. ----. ---. -",
		"-0.. -................ -..0 -",
		"-  . -.  .        .  . -.   -",
		"---. -. -. -------. -. -. ---",
		"-...... -.... -.... -...... -",
		"-.      -   . -.    -     . -",
		"-. ---------. -. ---------. -",
		"-.......................... -",
		"-                           -",
		"-----------------------------"

	};

int main()
{
	// Seed that helps determine ghost's direction
	srand ( time(NULL) );
	// Create a video mode pac
	VideoMode vm(641, 728);
	
	// Create and open a window for the game
	RenderWindow window(vm, "PACMAN");
	window.setFramerateLimit(60);
	window.clear();

	// Load background
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/maze.bmp");
	Sprite spriteBackground; 
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	// Press ENTER to start the game prompt
	sf::Text messageText;
	sf::Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	messageText.setFont(font);
	messageText.setString("Press Enter to start!");
	messageText.setCharacterSize(20);
	messageText.setFillColor(Color::White);
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);
	messageText.setPosition(400,400);

	// SCORE
	


	// Ghosts' sprites
	Texture orange_ghost;
	orange_ghost.loadFromFile("graphics/orange_ghost.bmp");
	Sprite orange;
	orange.setTexture(orange_ghost);
	orange.setPosition(307, 251);

	Texture pink_ghost;
	pink_ghost.loadFromFile("graphics/pink_ghost.bmp");
	Sprite pink;
	pink.setTexture(pink_ghost);
	pink.setPosition(307, 251);

	Texture blue_ghost;
	blue_ghost.loadFromFile("graphics/blue_ghost.bmp");
	Sprite blue;
	blue.setTexture(blue_ghost);
	blue.setPosition(307, 251);

	Texture red_ghost;
	red_ghost.loadFromFile("graphics/red_ghost.bmp");
	Sprite red;
	red.setTexture(red_ghost);
	red.setPosition(307, 251);

	// PACMAN Sprite
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/pacman.bmp");
	Sprite PAC_MAN;
	PAC_MAN.setTexture(texturePlayer);
	PAC_MAN.setPosition(306,492);

	// Portals

	RectangleShape portal_in (Vector2f(19,19));
	portal_in.setPosition(32,309);
	portal_in.setFillColor(Color::Transparent);
	RectangleShape portal_out (Vector2f(19,19));
	portal_out.setPosition(574,309);
	portal_out.setFillColor(Color::Transparent);

	paused = true;
	int ghost_direction = rand() % 2;

	
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			
			if (event.type == Event::KeyReleased && !paused)
			{
				// Listen for key presses again
				acceptInput = true;
			}

		}

		/*
		****************************************
		Handle the players input
		****************************************
		*/
		
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			window.clear();

			paused = false;

			score = 0;

			acceptInput = true;

			start = true;

		}
		if(!paused)
		{
			if(Keyboard::isKeyPressed(Keyboard::Right))
			{
					
					left = false;
					down = false;
					up = false;
					right = true;
			}
			if(Keyboard::isKeyPressed(Keyboard::Left))
			{
					up = false;
					right = false;
					down = false;
					left = true;				
			}
			if(Keyboard::isKeyPressed(Keyboard::Up))
			{
					left = false;
					right = false;
					down = false;
					up = true;
			}
			if(Keyboard::isKeyPressed(Keyboard::Down))
			{
					left = false;
					right = false;
					up = false;
					down = true;
			}
		}
		
		// END HANDLING INPUTS


		/*
		****************************************
		// START DRAWING SPRITES
		****************************************
		*/

		//SCORES
		std::ostringstream score_text;
		score_text << score;
		Text scores;
		scores.setString(score_text.str());
		scores.setPosition (168,680);
		scores.setFont(font);
		scores.setCharacterSize(30);
		scores.setFillColor(Color::White);

		//PACMAN LIVES
		std::ostringstream lives_text;
		lives_text << lives;
		Text live_;
		live_.setString(lives_text.str());
		live_.setPosition (445,680);
		live_.setFont(font);
		live_.setCharacterSize(30);
		live_.setFillColor(Color::White);


		window.draw(spriteBackground);
		window.draw(PAC_MAN);
		window.draw(portal_in);	
		window.draw(portal_out);
		window.draw(scores);
		window.draw(live_);
		
		if(!blue_death)
			window.draw(blue);
		if(!orange_death)
			window.draw(orange);
		if(!red_death)
			window.draw(red);
		if(!pink_death)
			window.draw(pink);

		// START DRAWING STATIONARY OBJECTS
		for (int i = 0; i < WIDTH; i ++)
		{
			
			for (int j = 0; j < HEIGHT; j++)
			{
				
				switch (game_map[j][i])
				{
					case '-': // Invisible walls
					{	
						RectangleShape wall (Vector2f(18,18));
						wall.setPosition(28 + 20 *i,23 + 20*j);
						wall.setFillColor(Color::Transparent);
						window.draw(wall);
						if (wall_limiter < 370)
						{
							wall_vector.push_back(wall);
							wall_limiter++;
						}
						break;
					}
					case '.': // pellets
					{
						
						CircleShape pellet;
						pellet.setRadius(21/8);
						pellet.setPosition(45 + 20*i, 38 + 20*j);
						pellet.setFillColor(Color(255,255,255));
						window.draw(pellet);	
						if (pellet_limiter < 230)
						{
							pellet_vector.push_back(pellet);
							pellet_limiter ++;
						}
						break;
					}
					case '0': // power-ups
					{
						CircleShape power;
						power.setRadius(21/4);
						power.setPosition(40 + 20*i, 35 + 20*j);
						power.setFillColor(Color(255,255,255));
						window.draw(power);
						if(power_limiter < 4)
						{
							power_limiter++;
							power_vector.push_back(power);
						}
						break;
					}
					default:
					{
						break;
					}
						
				}
			}
		}
		
		// END DRAWING

		//std::cout << pellet_vector.size() << std::endl;
		/*
		****************************************
		// START HANDLING GAME'S LOGIC
		****************************************
		*/

		// TURNING LEFT RIGHT UP DOWN
		Vector2f pos = PAC_MAN.getPosition();
		if(right)
		{	
			PAC_MAN.setPosition(pos.x + PACMAN_SPEED, pos.y + 0);
			wall_collision(PAC_MAN, pos, 'r');		
		}
		if(left)
		{
				
			PAC_MAN.setPosition(pos.x - PACMAN_SPEED, pos.y + 0);
			wall_collision(PAC_MAN, pos, 'l');
		}
		if(up)
		{
			PAC_MAN.setPosition(pos.x + 0, pos.y - PACMAN_SPEED);
			wall_collision(PAC_MAN, pos, 'u');
			
		}
		if(down)
		{
			PAC_MAN.setPosition(pos.x + 0, pos.y + PACMAN_SPEED);
			wall_collision(PAC_MAN, pos, 'd');	
		}
		
			//PORTAL IN AND OUT
			if (PAC_MAN.getGlobalBounds().intersects(portal_in.getGlobalBounds()))
			{
				PAC_MAN.setPosition(530,309);
			}
			if (PAC_MAN.getGlobalBounds().intersects(portal_out.getGlobalBounds()))
			{
				PAC_MAN.setPosition(78,309);
			}
		

		//std::cout << ghost_direction << std::endl;

		
		// ghost collision handler
		ghost_collision(PAC_MAN, red, 'r');
		ghost_collision(PAC_MAN, pink, 'p');
		ghost_collision(PAC_MAN, orange, 'o');
		ghost_collision(PAC_MAN, blue, 'b');

		//edible handler
		if (!paused)
			edible_collision(PAC_MAN);

		Vector2f red_position = red.getPosition();
		
		
		if(paused)
			window.draw(messageText);

		if(pellet_count == 0 || lives == 0)
			game_over = true;
			//acceptInput = false;
			//paused = true;

		window.display();

	}
		
}
/*
****************************************
// END MAIN FUNCTION, START HELPER FUNCTIONS
****************************************
*/

// Wall collision handler function

bool wall_collision(Sprite &pac, Vector2f &pos, char direction)
{
	
	for (int i = 0; i < wall_vector.size(); i ++)
	{
		
		if (pac.getGlobalBounds().intersects(wall_vector[i].getGlobalBounds()))
		{
			switch (direction)
			{
				case 'r':
					//right = false;
					pac.setPosition(pac.getPosition().x - PACMAN_SPEED, pac.getPosition().y);
					break;
				case 'l':
					//left = false;
					pac.setPosition(pac.getPosition().x + PACMAN_SPEED, pac.getPosition().y);
					break;
				case 'u':
					//up = false;
					pac.setPosition(pac.getPosition().x, pac.getPosition().y  + PACMAN_SPEED );
					break;
				case 'd':
					//down = false;
					pac.setPosition(pac.getPosition().x, pac.getPosition().y  - PACMAN_SPEED);
					break;
				default:
					break;
			}
			return true;
		}
	
	}
	
	return false;
	
	}

//EDIBLE HANDLER FUNCTION ( POWER PELLETS AND PELLETS)
//the basic idea is to reverse engineer of the placement of the consumables and rewrite the bitmap

void edible_collision(Sprite &pac)
{
	for (int i = 0; i < pellet_vector.size(); i ++)
	{
		if (pac.getGlobalBounds().intersects(pellet_vector[i].getGlobalBounds()))
		{

			game_map[(pellet_vector[i].getPosition().y - 38)/20][(pellet_vector[i].getPosition().x - 45 )/20] = ' ';
			pellet_vector.erase(pellet_vector.begin() + i);
			pellet_count --;
			score++;
		}
		
	}
	for (int i = 0; i < power_vector.size(); i ++)
	{
		if (pac.getGlobalBounds().intersects(power_vector[i].getGlobalBounds()))
			{
				game_map[(power_vector[i].getPosition().y - 35)/20][(power_vector[i].getPosition().x - 40 )/20] = ' ';
				power = true;
			}
	}
	
}


void ghost_collision(Sprite &pac, Sprite &ghost, char color)
{
	if (pac.getGlobalBounds().intersects(ghost.getGlobalBounds()) && !power)
	{
		pac.setPosition(306,492);
		lives --;
	}
	if (pac.getGlobalBounds().intersects(ghost.getGlobalBounds()) && power)
	{
		if (color == 'r')
			red_death = 1;
		if (color == 'p')
			pink_death = 1;
		if (color == 'o')
			orange_death = 1;
		if (color == 'b')
			blue_death = 1;
		ghost.setPosition(52,696);
	}	
}


			

		
	










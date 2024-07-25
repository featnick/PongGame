#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const std::string RESOURCES_PATH = "Resources/";
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 500;
const float PLATFORM_HEIGHT = 80;
const float PLATFORM_WIDTH = 20;
const float INITIAL_SPEED = 200;
const float INITIAL_BALL_SPEED = 600;
const float BALL_SIZE = 20;

int main()
{
	int seed = (int)time(nullptr);
	srand(seed);

	//Init window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "PongGame");

	//Init player state
	float playerX = 0 + PLATFORM_WIDTH / 2.f;
	float playerY = SCREEN_HEIGHT / 2.f;
	float playerSpeed = INITIAL_SPEED;
	int playerDirection = 0; // 1 - Up, 2 - Down

	//Init AI state
	float aiX = SCREEN_WIDTH - PLATFORM_WIDTH / 2.f;
	float aiY = SCREEN_HEIGHT / 2.f;
	float aiSpeed = INITIAL_SPEED;
	int aiDirection = 1; //1-up, 2-down

	//Init player shape 
	sf::RectangleShape playerShape;
	playerShape.setSize(sf::Vector2f(PLATFORM_WIDTH, PLATFORM_HEIGHT));
	playerShape.setFillColor(sf::Color::White);
	playerShape.setOrigin(PLATFORM_WIDTH / 2.f, PLATFORM_HEIGHT / 2.f);
	playerShape.setPosition(playerX, playerY);

	//Init AI shape
	sf::RectangleShape aiShape;
	aiShape.setSize(sf::Vector2f(PLATFORM_WIDTH, PLATFORM_HEIGHT));
	aiShape.setFillColor(sf::Color::White);
	aiShape.setOrigin(PLATFORM_WIDTH / 2.f, PLATFORM_HEIGHT / 2.f);
	aiShape.setPosition(aiX, aiY);

	//Init ball state
	float ballX = SCREEN_WIDTH / 2.f;
	float ballY = SCREEN_HEIGHT / 2.f;
	float ballSpeed = INITIAL_BALL_SPEED;
	float ballSpeedY = 0+rand() % 61;
	int ballDirection = 1 + rand() % 2;; // 1-left, 2-right

	//Init ball shape
	sf::CircleShape ballShape;
	ballShape.setRadius(BALL_SIZE / 2.f);
	ballShape.setFillColor(sf::Color::White);
	ballShape.setOrigin(BALL_SIZE / 2.f, BALL_SIZE / 2.f);
	ballShape.setPosition(ballX, ballY);

	//Init game clocks
	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	//Init score text
	int playerScore = 0;
	int aiScore = 0;
	sf::Font font;
	font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Light.ttf");
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::Yellow);
	scoreText.setPosition(388, 6);

	//Init player text
	sf::Text playerText;
	playerText.setFont(font);
	playerText.setCharacterSize(24);
	playerText.setFillColor(sf::Color::Magenta);
	playerText.setPosition(288, 6);
	playerText.setString("Player");

	//Init AI text
	sf::Text aiText;
	aiText.setFont(font);
	aiText.setCharacterSize(24);
	aiText.setFillColor(sf::Color::Red);
	aiText.setPosition(488, 6);
	aiText.setString("AI");

	//main loop
	while (window.isOpen())
	{
		//Calculate time delta
		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//Read events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}

		//Handle input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			playerDirection = 1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			playerDirection = 2;
		}

		//Update player state
		if (playerDirection == 1)
		{
			playerY -= playerSpeed * deltaTime;
		}
		else if (playerDirection == 2)
		{
			playerY += playerSpeed * deltaTime;
		}

		//Find player collision with screen borders
		if (playerY + PLATFORM_HEIGHT / 2.f >= SCREEN_HEIGHT)
		{
			if (playerDirection == 2)
			{
				playerSpeed = 0;
			}
			else if (playerDirection == 1)
			{
				playerSpeed = INITIAL_SPEED;
			}
		}
		if (playerY - PLATFORM_HEIGHT / 2.f <= 0)
		{
			if (playerDirection == 1)
			{
				playerSpeed = 0;
			}
			else if (playerDirection == 2)
			{
				playerSpeed = INITIAL_SPEED;
			}
		}

		//Movement AI
		/*if (aiY - PLATFORM_HEIGHT / 2.f <= 0)
		{
			aiDirection = 2;
		}
		else if (aiY + PLATFORM_HEIGHT / 2.f >= SCREEN_HEIGHT)
		{
			aiDirection = 1;
		}*/

		/*if (ballY > aiY && ballX > SCREEN_WIDTH / 2.f - 100)
		{
			aiDirection = 2;
		}
		else if (ballY < aiY && ballX > SCREEN_WIDTH / 2.f - 100)
		{
			aiDirection = 1;
		}
		else
		{
			aiDirection = 0;
		}*/

		if (ballY < aiY - PLATFORM_HEIGHT / 2.f && ballX > SCREEN_WIDTH / 2.f - 100)
		{
			aiDirection = 1;
		}
		else if (ballY > aiY + PLATFORM_HEIGHT / 2.f && ballX > SCREEN_WIDTH / 2.f - 100)
		{
			aiDirection = 2;
		}
		else
		{
			aiDirection = 0;
		}

		//Find ai collision with screen borders
		if (aiY + PLATFORM_HEIGHT / 2.f >= SCREEN_HEIGHT)
		{
			if (aiDirection == 2)
			{
				aiSpeed = 0;
			}
			else if (aiDirection == 1)
			{
				aiSpeed = INITIAL_SPEED;
			}
		}
		if (aiY - PLATFORM_HEIGHT / 2.f <= 0)
		{
			if (aiDirection == 1)
			{
				aiSpeed = 0;
			}
			else if (aiDirection == 2)
			{
				aiSpeed = INITIAL_SPEED;
			}
		}

		//Update ai state
		if (aiDirection == 1)
		{
			aiY -= aiSpeed * deltaTime;
		}
		else if (aiDirection == 2)
		{
			aiY += aiSpeed * deltaTime;
		}

		//Check ball collision with platform
		if (ballX - BALL_SIZE / 2.f <= playerX + PLATFORM_WIDTH / 2.f  && ballX - BALL_SIZE / 2.f >= playerX && //collision with player
			ballY >= playerY - PLATFORM_HEIGHT / 2.f && ballY <= playerY + PLATFORM_HEIGHT / 2.f)
		{
			ballSpeedY = 0;
			ballDirection = 2;

			//Check degree
			if (ballY >= playerY - PLATFORM_HEIGHT / 4.f && ballY <= playerY)
			{
				ballSpeedY -= 30 + rand() % 101;
			}
			else if (ballY <= playerY + PLATFORM_HEIGHT / 4.f && ballY >= playerY)
			{
				ballSpeedY += 30 + rand() % 101;
			}
			else if (ballY >= playerY - PLATFORM_HEIGHT / 2.f && ballY < playerY - PLATFORM_HEIGHT / 4.f)
			{
				ballSpeedY -= 80 + rand() % 281;
			}
			else if (ballY <= playerY + PLATFORM_HEIGHT / 2.f && ballY > playerY + PLATFORM_HEIGHT / 4.f)
			{
				ballSpeedY += 80 + rand() % 281;
			}
		}
		else if (ballX + BALL_SIZE / 2.f >= aiX - PLATFORM_WIDTH / 2.f && ballX + BALL_SIZE / 2.f <= aiX && //collision with ai
			ballY >= aiY - PLATFORM_HEIGHT / 2.f && ballY <= aiY + PLATFORM_HEIGHT / 2.f)
		{
			ballSpeedY = 0;
			ballDirection = 1;

			//Check degree
			if (ballY >= aiY - PLATFORM_HEIGHT / 4.f && ballY <= aiY)
			{
				ballSpeedY -= 30 + rand() % 101;
			}
			else if (ballY <= aiY + PLATFORM_HEIGHT / 4.f && ballY >= aiY)
			{
				ballSpeedY += 30 + rand() % 101;
			}
			else if (ballY >= aiY - PLATFORM_HEIGHT / 2.f && ballY < aiY - PLATFORM_HEIGHT / 4.f)
			{
				ballSpeedY -= 80 + rand() % 281;
			}
			else if (ballY <= aiY + PLATFORM_HEIGHT / 2.f && ballY > aiY + PLATFORM_HEIGHT / 4.f)
			{
				ballSpeedY += 80 + rand() % 281;
			}
		}

		//Check ball collision with wall
		if (ballY + BALL_SIZE / 2.f >= SCREEN_HEIGHT)
		{
			ballSpeedY = -ballSpeedY;
		}
		if (ballY - BALL_SIZE / 2.f <= 0)
		{
			ballSpeedY = -ballSpeedY;
		}

		//Update ball state
		if (ballDirection == 1)
		{
			ballX -= ballSpeed * deltaTime;
			ballY += ballSpeedY * deltaTime;
		}
		else if (ballDirection == 2)
		{
			ballX += ballSpeed * deltaTime;
			ballY += ballSpeedY * deltaTime;
		}

		//Check score and respawning ball
		if (ballX < -200)
		{
			ballX = SCREEN_WIDTH / 2.f;
			ballY = SCREEN_HEIGHT / 2.f;
			ballDirection = 1 + rand() % 2;
			ballSpeedY = 0 + rand() % 61;
			aiScore += 1;
		}
		else if (ballX > SCREEN_WIDTH + 200)
		{
			ballX = SCREEN_WIDTH / 2.f;
			ballY = SCREEN_HEIGHT / 2.f;
			ballDirection = 1 + rand() % 2;
			ballSpeedY = 0 + rand() % 61;
			playerScore += 1;
		}

		scoreText.setString(std::to_string(playerScore)+ " : " + std::to_string(aiScore));

		//Draw everything
		window.clear();

		playerShape.setPosition(playerX, playerY); //Update player position
		aiShape.setPosition(aiX, aiY);
		ballShape.setPosition(ballX, ballY);

		window.draw(playerShape);
		window.draw(aiShape);
		window.draw(ballShape);
		window.draw(scoreText);
		window.draw(playerText);
		window.draw(aiText);

		window.display();
	}
	return 0;
}

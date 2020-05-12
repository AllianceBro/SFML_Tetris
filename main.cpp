#include <SFML/Graphics.hpp>


using namespace sf;

/*
  Here we define coords for each figure.
  That's the pattern for all of them:
    01
    23
    45
    67
    Each number stands for one block, which will be included in one of 7 figures.
  */
    //SET GLOBAL VARIABLES
    const unsigned char width(80), height(160); //Oh, now it's 1 byte for each variable instead of 2!!!!
    int field[width][height] = {0};             //Main game field
    int figures[7][4]=
    {
        1,3,5,7, // I figure
        2,4,5,7, // S figure
        3,5,4,6, // Z figure
        3,5,4,7, // T figure
        2,3,5,7, // L figure
        3,5,7,6, // J figure
        2,3,4,5, // O figure
    };

    struct Point
    {
        int x, y;
    };
    Point coords[4] = {0};

    int signed moveX(0), moveY(0); //X and Y movements
    bool rotateF(false);            //Figure rotation
    float timer(0), delay(0.3);


int main()
{
	RenderWindow window(VideoMode(120, 184), "TETRIS!");

	Texture texture1;                                   //Load objects texture
    texture1.loadFromFile("textures\\texture1.png");

    Sprite sBlock(texture1);
    sBlock.setTextureRect(IntRect(0, 192, 8, 8));       //Create blocks sprite, x=0, y=192, 8x8pp

    Sprite mainFrame(texture1);
    mainFrame.setTextureRect(IntRect(0, 0, 96, 184));

    Clock clock;                                        //Make timer for our figures

	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer +=time;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

            if (event.type == Event::KeyPressed)
            {
               if (event.key.code == Keyboard::Left) moveX = -1;
                    else if (event.key.code == Keyboard::Right) moveX = 1;
                        else if (event.key.code == Keyboard::Down) moveY = 1;
                            else if (event.key.code == Keyboard::Up) rotateF = true;
                                else if (event.key.code == Keyboard::Escape) window.close();

            }
		}

    int nFigure(0);                                             //Figure number

    if (coords[0].x == 0)
    {

        for (int i = 0; i < 4; i++)                             //Set coords for each block
        {
            coords[i].x = figures[nFigure][i] % 2;
            coords[i].y = figures[nFigure][i] / 2;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        if(coords[3].y <= 18)
        {
            coords[i].x += moveX;                               //Move our figure on X or Y coords
            coords[i].y += moveY;
        }
    }
    if (rotateF && (coords[3].y <= 18))                        //Figure rotation
    {
        Point pointRotate = coords[1];                          //Set point of rotation
        for (int i = 0; i < 4; i++)
        {
            int x1 = coords[i].y - pointRotate.y;               //New X coords
            int y1 = coords[i].x - pointRotate.x;               //New Y coords
            coords[i].x = pointRotate.x - x1;
            coords[i].y = pointRotate.y + y1;
        }
    }
    moveX = 0;
    moveY = 0;
    rotateF = false;

    if ((timer > delay) && (coords[3].y <= 18))
    {
        for (int i = 0; i < 4; i++)
            coords[i].y +=1;
        timer = 0;
    }

    window.clear(Color::White);

    window.draw(mainFrame);

    for (int i = 0; i < 4; i++)
    {
        sBlock.setPosition(coords[i].x * 8, coords[i].y * 8);
        sBlock.move(8, 8);
        window.draw(sBlock);
    }

    window.display();
	}

	return 0;
}

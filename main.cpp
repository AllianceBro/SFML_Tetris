#include <SFML/Graphics.hpp>
#include <time.h>

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
    /* SET GLOBAL VARIABLES */
    bool gameBegin(true), gameOver(false);
    int signed moveX(0), moveY(0);              //X and Y movements
    bool rotateF(false);                        //Figure rotation
    float timer(0), delay(0.3);
    char colorNum(1);
    const unsigned char width(10), height(20);
    int field[height][width] = {0};             //Main game matrix
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

    struct Point //New coords and previous coords of the figure
    {
        int x, y;
    };
    Point coords[4], coordsOld[4];

 /* BORDERS CHECKING */
bool Check()
{
    for (int i = 0; i < 4; i++)
        if (coords[i].x < 0 || coords[i].x >= width || coords[i].y >= height) //Check if a figure touches the borders
            return false;
        else if (field[coords[i].y][coords[i].x])                             //Check if smth already exists on the figure's way
            return false;

        return true;
}


int main()
{
	RenderWindow window(VideoMode(120, 184), "TETRIS!");

    /* TEXTURE LOADING */
	Texture texture1;
    texture1.loadFromFile("textures\\texture1.png");

    Sprite sBlock(texture1);
    sBlock.setTextureRect(IntRect(0, 192, 8, 8));       //Create blocks sprite, x=0, y=192, 8x8pp

    Sprite mainFrame(texture1);
    mainFrame.setTextureRect(IntRect(0, 0, 96, 184));   //Game frame texture

    /* TIME AND RANDOM NUMBER OF FIGURES BASEMENT */
    Clock clock;
    srand(time(NULL));

	while (window.isOpen())
	{
        /* TIME SETTINGS */
		float time = clock.getElapsedTime().asSeconds(); //Get time in seconds
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
                        else if (event.key.code == Keyboard::Down) delay = 0.05; //Make figure falling faster
                            else if (event.key.code == Keyboard::Up) rotateF = true;
                                else if (event.key.code == Keyboard::Escape) window.close();

            }
		}
    int nFigure = rand() % 7;                                //Figure random number

    /* INICIATE FIRST APPEREANCE OF FIRST FIGURE */
    if (gameBegin)                                          //Is it the first figure on the screen?
    {
        gameBegin = false;
        for (int i = 0; i < 4; i++)                         //Set coords for each block
        {
            coords[i].x = figures[nFigure][i] % 2 + 4;
            coords[i].y = figures[nFigure][i] / 2;
        }
    }
    /* HORIZONTAL MOVEMENTS */
    for (int i = 0; i < 4; i++)
    {
            coordsOld[i] = coords[i];                           //Write down previous coords
            coords[i].x += moveX;                               //Move our figure on X or Y coords
    }
    if(!Check())
            for (int i = 0; i < 4; i++)
                coords[i] = coordsOld[i];
    /* FIGURE ROTATION */
    if (rotateF)
    {
        Point pointRotate = coords[1];                          //Set point of rotation
        for (int i = 0; i < 4; i++)
        {
            int x1 = coords[i].y - pointRotate.y;               //New X coords
            int y1 = coords[i].x - pointRotate.x;               //New Y coords
            coordsOld[i] = coords[i];
            coords[i].x = pointRotate.x - x1;
            coords[i].y = pointRotate.y + y1;
        }
        if(!Check())
        {
            for (int i = 0; i < 4; i++)
                coords[i] = coordsOld[i];
        }
    }

    /* VERTICAL MOVEMENTS DUE TO TIMER */
    if (timer > delay)              //Figure movements due to timer, goes +1y each 0.3 sec
    {
        for (int i = 0; i < 4; i++)
        {
            coordsOld[i] = coords[i];
            coords[i].y +=1;

        }
        if (!Check())
        {
            /* FIGURE COLOUR AND NUMBER CHANGING */
            for (int i = 0; i < 4; i++)
                    field[coordsOld[i].y][coordsOld[i].x] = colorNum;
            colorNum = 1 + rand() % 7;
            nFigure = rand() % 7;
            /* INICIATE NEXT LOCAION OF NEW FIGURE */
            for (int i = 0; i < 4; i++)
            {
                coords[i].x = figures[nFigure][i] % 2 + 4;
                coords[i].y = figures[nFigure][i] / 2;
            /* CHECK IF THERE IS ENOUGH SPACE FOR NEXT FIGURE ON THE SCREEN */
                if (field[coords[i].y][coords[i].x] == field[coordsOld[i].y][coordsOld[i].x])
                    gameOver = true;
            }
            delay = 0.3;
        }
        timer = 0;
    }
    /* GAME OVER CONDITION */
    if(gameOver)
        window.close();
    moveX = 0;
    moveY = 0;
    rotateF = false;

    /* CHECK BLOCKS LINE WHICH MUST BE DELETED */
    int k(height-1);
    for (int i = height - 1; i > 0; i--) //Check matrix from bottom to upper border
    {
        int blockClear(0);              //Number of blocks in line
        for (int j = 0; j < width; j++)
        {
            if (field[i][j])            //If there is one block, we count it
                blockClear++;
            field[k][j] = field[i][j];  //Assign coords of upper line to lower one, blocks move        }
        }
        if (blockClear < width)         //If there is more than 10 blocks, K doesnt change
            k--;
    }

    /* WINDOW DRAWING SECTION */
    window.clear(Color::White);

    window.draw(mainFrame);

    /* OLD FIGURE DRAWING */
    for (int i = 0; i < height; i++)      //Check main game field matrix
	{
	    for (int j = 0; j < width; j++)
        {
            if (field[i][j] != 0){      //If smth already exists, that means there is a block of the previous figure
                char colorNum2 = field[i][j];
                sBlock.setTextureRect(IntRect(colorNum2 * 8, 192, 8, 8));
                sBlock.setPosition(j * 8, i * 8);
                sBlock.move(8, 8);
                window.draw(sBlock);}
        }
    }

    for (int i = 0; i < 4; i++)
    {
        sBlock.setTextureRect(IntRect(colorNum * 8, 192, 8, 8));
        sBlock.setPosition(coords[i].x * 8, coords[i].y * 8);
        sBlock.move(8, 8);
        window.draw(sBlock);
    }

    window.display();
	}

	return 0;
}

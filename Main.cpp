#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <ctime>
enum directions {right, left, up, down};
class Player
{
private:
    int iter = 1;
    int newx = 0;
    int newy = 0;

public:
    float frame = 0;
    int x = 14;
    int y = 14;

    void update(int dir, int map[][16])
    {
        frame += 0.1;
        if (frame > 4)
            frame -= 4;
        iter += 1;
        if (iter > 17)
            iter -= 17;
        if (iter % 17 == 0)
        {
            if (dir == 0)
            {
                newx = 1;
                newy = 0;
                if (map[x + newx][y] != 1)
                    x += newx;
            }
            else if (dir == 1)
            {
                newx = -1;
                newy = 0;
                if (map[x + newx][y] != 1)
                    x += newx;
            }
            else if (dir == 2)
            {
                newx = 0;
                newy = -1;
                if (map[x][y + newy] != 1)
                    y += newy;
            }
            else if (dir == 3)
            {
                newx = 0;
                newy = 1;
                if (map[x][y + newy] != 1)
                    y += newy;
            }
        }
    }
};
class Ghost
{
private:
    int iter = 0;
    int newx = 0;
    int newy = 0;

public:
    int x;
    int y;
    int dir = 0;

    void update(int map[][16])
    {
        iter += 1;
        if (iter > 128)
            iter -= 128;
        if (iter % 64 == 0)
            dir = rand() % 4;
        if (iter % 16 == 0)
        {
            if (dir == 0)
            {
                newx = 1;
                newy = 0;
                if (map[x + newx][y] != 1)
                    x += newx;
            }
            else if (dir == 1)
            {
                newx = -1;
                newy = 0;
                if (map[x + newx][y] != 1)
                    x += newx;
            }
            else if (dir == 2)
            {
                newx = 0;
                newy = -1;
                if (map[x][y + newy] != 1)
                    y += newy;
            }
            else if (dir == 3)
            {
                newx = 0;
                newy = 1;
                if (map[x][y + newy] != 1)
                    y += newy;
            }
        }
    }
};

void delenie(bool(&newMap)[13][13], int rowsFM, int rowsSM, int colsFM, int colsSM);

int main() {

    srand(time(NULL));

    // Создаём окно
    const int width = 1024;
    const int height = 1024;
    sf::RenderWindow window(sf::VideoMode({ width, height }), "Pac-guy");
    window.setFramerateLimit(60);

    sf::Texture pacmanTexture("Sprites/pacmanSprites.png"); //Создание пакмана
    sf::Sprite pacmanSprite(pacmanTexture);
    int dir = 0;
    pacmanSprite.setOrigin({ 16, 16 });
    pacmanSprite.setScale({ 4.0f, 4.0f });
    pacmanSprite.setPosition({ 64.0f * 15, 64.0f * 15 });

    sf::Texture blockTexture("Sprites/Block.png");  //Создание блоков
    sf::Sprite blockSprite(blockTexture);
    blockSprite.setOrigin({ 16, 16 });
    blockSprite.setScale({ 4.0f, 4.0f });

    sf::Texture pointTexture("Sprites/Point.png");  //Создание точек
    sf::Sprite pointSprite(pointTexture);
    pointSprite.setOrigin({ 16, 16 });
    pointSprite.setScale({ 4.0f, 4.0f });

    sf::Texture ghostTexture("Sprites/ghostSprites.png");   //Создание приведений
    sf::Sprite ghostSprite(ghostTexture);
    ghostSprite.setOrigin({ 16, 16 });
    ghostSprite.setScale({ 4.0f, 4.0f });
    ghostSprite.setPosition({ 64.0f * 3, 64.0f * 3 });

    const sf::Font font("times.ttf");

    sf::RectangleShape bg({ 400.0f, 100.0f });
    bg.setOrigin(bg.getSize() / 2.0f);
    bg.setPosition({ 20.0f, 5.0f });
    bg.setFillColor(sf::Color::Yellow);
    

    // Create a text
    sf::Text scoreText(font);
    scoreText.setCharacterSize(40);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(5);
    scoreText.setPosition({ 8.0f, 8.0f });

    sf::Text lostText(font);
    lostText.setCharacterSize(80);
    lostText.setStyle(sf::Text::Bold);
    lostText.setFillColor(sf::Color::Magenta);
    lostText.setOutlineColor(sf::Color::Black);
    lostText.setOutlineThickness(5);
    lostText.setString("\t\t\tYou lost! :(\nHold SPACE to play again\nor hold ESC to close a game");
    lostText.setPosition({ 50.0f, width / 3.0f });

    sf::Text victoryText(font);
    victoryText.setCharacterSize(80);
    victoryText.setStyle(sf::Text::Bold);
    victoryText.setFillColor(sf::Color::Yellow);
    victoryText.setOutlineColor(sf::Color::Black);
    victoryText.setOutlineThickness(5);
    victoryText.setString("\t\tCongrats! You won! :)\nHold SPACE to play again\nor hold ESC to close a game");
    victoryText.setPosition({ 50.0f, width / 3.0f });
    
    sf::Music music("HavaNagila.ogg");
    sf::Music beep("Beep.ogg");
    music.setVolume(50);
    music.setLooping(true);
    music.play();
    PA:

    Player p;
    Ghost g[5];
    g[0].x = 1;
    g[0].y = 1;
    g[1].x = 14;
    g[1].y = 1;
    g[2].x = 1;
    g[2].y = 14;
    g[3].x = 7;
    g[3].y = 1;
    g[4].x = 1;
    g[4].y = 7;
    int pointCounter = 0;
    int count = 0;
    std::string scoreStr;

    //Создание массива отвечающего за карту
    const int r = 16, c = 16;
    int map[r][c] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},//0
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//1
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//2
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//3
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//4
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//5
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//6
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//7
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//8
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//9
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//10
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//11
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//12
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},//13
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1},//14
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} //15
    };

    //Создание внутриннего массива
    const int newR = 13, newC = 13;
    bool newMap[newR][newC];
    for (int i = 0; i < newR; i++)
        for (int j = 0; j < newC; j++)
            newMap[i][j] = 0;

    delenie(newMap, 0, 12, 0, 12);

    for (int i = 0; i < 13; i++)
        for (int j = 0; j < 13; j++)
            //map[i + 2][j + 2] = int(newMap[i][j]);
        {
            if (map[i + 1][j + 2] == 1 && i % 2 == 0)
            {
                map[i + 2][j + 1] = 0;
                continue;
            }
            else if (map[i + 1][j + 2] == 1 && i % 2 != 0)
                continue;
            else
                map[i + 2][j + 2] = int(newMap[i][j]);
        }


    // Главный цикл программы
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        //Изменение вектора движения пакмана
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
        {
            dir = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
        {
            dir = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
        {
            dir = 2;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
        {
            dir = 3;
        }

        // Отрисовка
        window.clear(); // Очищаем экран
        
        //////////
        //Рисуем//
        //////////
        
        //Отрисовка карты
        count = 0;
        for (int i = 0; i < 16; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                if (map[i][j] == 1)
                {
                    blockSprite.setPosition({ 64.0f * (i + 1), 64.0f * (j + 1) });
                    window.draw(blockSprite);
                }
                if (map[i][j] == 0)
                {
                    pointSprite.setPosition({ 64.0f * (i + 1), 64.0f * (j + 1) });
                    window.draw(pointSprite);
                    count += 1;
                }
                if (map[i][j] == 2)
                {
                    continue;
                }
            }
        }
        if (count == 0)
        {
            window.draw(victoryText);
            window.display();
            while ((!sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)) && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape)))
            {
                window.waitEvent(sf::Time::Zero);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
                goto PA;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape))
                return 0;
        }
        window.draw(scoreText);
        //Отрисовка, анимация и движение пакмана
        p.update(dir, map);
        pacmanSprite.setTextureRect(sf::IntRect({ 16 * int(p.frame), 16 * dir }, { 16, 16 }));
        pacmanSprite.setPosition({ 64.0f * (p.x + 1), 64.0f * (p.y + 1) });
        window.draw(pacmanSprite);

        for (int i = 0; i < 5; i++)
        {
            g[i].update(map);
            ghostSprite.setTextureRect(sf::IntRect({ 16 * g[i].dir , 0 }, { 16, 16 }));
            ghostSprite.setPosition({ 64.0f * (g[i].x + 1), 64.0f * (g[i].y + 1) });
            window.draw(ghostSprite);
        }

        for (int i = 0; i < 5; i++)
        {
            if (((p.x == g[i].x) && (p.y == g[i].y)))
            {
                std::cerr << "You lost!" << std::endl;
                window.draw(lostText);
                window.display();
                while ((!sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)) && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape)))
                {
                    window.waitEvent(sf::Time::Zero);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
                    goto PA;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Escape))
                    return 0;
            }
        }
        
        if (map[p.x][p.y] == 0)
        {
            map[p.x][p.y] = 2;
            pointCounter += 1;
            scoreStr = std::to_string(pointCounter);
            system("cls");
            std::cout << scoreStr << std::endl;
            scoreText.setString("Score: " + sf::String(scoreStr));
            beep.play();
        }
        
        window.display(); // Отображаем всё на экране
    }

    return 0;
}
void delenie(bool(&newMap)[13][13], int x1, int x2, int y1, int y2)
{
    if (x2 - x1 < 3 || y2 - y1 < 3)
        return;
    if (x2 >= y2)						//Если рядков больше чем столбцов
    {
        int x = rand() % (x2 - x1) + x1;
        for (int i = y1; i < y2; i++)
        {
            newMap[i][x] = 1;			//Создание линии
        }
        int o1;
        o1 = rand() % y2;				//Создание проходов
        newMap[o1][x] = 0;
        if (x > 0)
            newMap[o1][x - 1] = 0;

        delenie(newMap, x1, x, y1, y2);
        delenie(newMap, x, x2, y1, y2);
    }

    else if (x2 < y2)					//Если столбцов больше чем рядков
    {
        int y = rand() % (y2 - y1) + y1;
        for (int i = x1; i < x2; i++)
        {
            newMap[y][i] = 1;			//Создание линии
        }
        int o1;
        o1 = rand() % x2;				//Создание проходов
        newMap[y][o1] = 0;
        if (y > 0)
            newMap[y - 1][o1] = 0;

        delenie(newMap, x1, x2, y1, y);
        delenie(newMap, x1, x2, y, y2);
    }
}
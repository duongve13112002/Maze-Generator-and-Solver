#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include "Menu.h"
#include "Infor.h"
#include "option.h"
#include <sstream>
#include <stack>
#include <string>

#define MAX_COLS 50
#define MAX_ROWS 40

int value_cols = MAX_COLS;
int value_rows = MAX_ROWS;
int end_value_c = value_cols - 1;
int end_value_r = value_rows - 1;

const int SCR_WIDTH = 1080;
const int SCR_HEIGHT = 650;

const int end_c = end_value_c;
const int end_r = end_value_r;

const int* cols = &value_cols;     // how many cells horizontally
const int* rows = &value_rows;     // how many cells vertically

bool m_previous = false;
bool s_previous = false;

enum states {
    PREMAZE,
    Change_start_create_maze,
    complete_change_create_maze,
    GENMAZE,
    PRESOLVE,
    add_wall,
    complete_wall,
    Change_start_end,
    complete_change,
    add_start,
    SOLVING,
    SOLVED
};


int state = PREMAZE;

class Spot : public sf::Drawable
{
private:
    sf::Color col = sf::Color::White;
public:
    bool visited = false;

    float f = 0;
    float g = 0;
    float h = 0;

    float i = 0;
    float j = 0;

    std::vector<bool> walls = { true, true, true, true };

    std::vector<Spot*> neighbors;
    Spot* previous = 0;


    void addNeighbors(std::vector<std::vector<Spot>>& cells) {
        neighbors.clear();
        if (i < *cols - 1) {
            if (!cells[i + 1][j].visited)
                neighbors.push_back(&cells[i + 1][j]);
        }
        if (i > 0) {
            if (!cells[i - 1][j].visited)
                neighbors.push_back(&cells[i - 1][j]);
        }
        if (j < *rows - 1) {
            if (!cells[i][j + 1].visited)
                neighbors.push_back(&cells[i][j + 1]);
        }
        if (j > 0) {
            if (!cells[i][j - 1].visited)
                neighbors.push_back(&cells[i][j - 1]);
        }
    }

    void addMazeNeighbors(std::vector<std::vector<Spot>>& cells) {
        neighbors.clear();
        if (i < *cols - 1 && !walls[1])
            neighbors.push_back(&cells[i + 1][j]);
        if (i > 0 && !walls[3])
            neighbors.push_back(&cells[i - 1][j]);
        if (j < *rows - 1 && !walls[2])
            neighbors.push_back(&cells[i][j + 1]);
        if (j > 0 && !walls[0])
            neighbors.push_back(&cells[i][j - 1]);
    }

    void setij(int x, int y) {
        i = x;
        j = y;
    }

    void setCol(sf::Color new_col) {
        col = new_col;
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        int cell_w = SCR_WIDTH / *cols;
        int cell_h = SCR_HEIGHT / *rows;
        sf::RectangleShape shape(sf::Vector2f(cell_w, cell_h));
        shape.move(sf::Vector2f(i * cell_w, j * cell_h));
        shape.setFillColor(col);
        target.draw(shape);


        if (walls[0]) { // top wall
            sf::VertexArray wall(sf::Lines, 2);
            wall[0].position = sf::Vector2f(i * cell_w, j * cell_h);
            wall[0].color = sf::Color::Black;
            wall[1].position = sf::Vector2f((i + 1) * cell_w, j * cell_h);
            wall[1].color = sf::Color::Black;
            target.draw(wall);
        }
        if (walls[1]) { // right wall
            sf::VertexArray wall(sf::Lines, 2);
            wall[0].position = sf::Vector2f((i + 1) * cell_w, j * cell_h);
            wall[0].color = sf::Color::Black;
            wall[1].position = sf::Vector2f((i + 1) * cell_w, (j + 1) * cell_h);
            wall[1].color = sf::Color::Black;
            target.draw(wall);
        }
        if (walls[2]) { // bottom wall
            sf::VertexArray wall(sf::Lines, 2);
            wall[0].position = sf::Vector2f(i * cell_w, (j + 1) * cell_h);
            wall[0].color = sf::Color::Black;
            wall[1].position = sf::Vector2f((i + 1) * cell_w, (j + 1) * cell_h);
            wall[1].color = sf::Color::Black;
            target.draw(wall);
        }
        if (walls[3]) { // left wall
            sf::VertexArray wall(sf::Lines, 2);
            wall[0].position = sf::Vector2f(i * cell_w, j * cell_h);
            wall[0].color = sf::Color::Black;
            wall[1].position = sf::Vector2f(i * cell_w, (j + 1) * cell_h);
            wall[1].color = sf::Color::Black;
            target.draw(wall);
        }
    }
};

std::vector<Spot> tmp(*rows);
std::vector<std::vector<Spot>> grid(*cols, tmp);

std::vector<Spot*> openSet;
std::vector<Spot*> closedSet;

std::vector<Spot*> cell_stack;
int start_x = 0;
int start_y = 0;
Spot* start = &grid[start_x][start_y];

//int reset_maze = 0;
void addToOpenSet(Spot* cell) {
    openSet.push_back(cell);
    cell->setCol(sf::Color(180, 255, 180));
}

void addToClosedSet(Spot* cell) {
    closedSet.push_back(cell);
    cell->setCol(sf::Color::Red);
}


void removeFromArray(Spot* cell, std::vector<Spot*>& vect) {
    for (int i = 0; i < (int)vect.size(); i++) {
        if (vect[i] == cell) {
            vect.erase(vect.begin() + i);
        }
    }
}

bool arrayContainsElem(Spot* cell, std::vector<Spot*>& vect) {
    for (int i = 0; i < (int)vect.size(); i++) {
        if (vect[i] == cell) {
            return true;
        }
    }
    return false;
}

void removeWall(Spot* a, Spot* b) {
    int x = a->i - b->i;
    int y = a->j - b->j;
    if (x == 1) {
        a->walls[3] = false;
        b->walls[1] = false;
    }
    if (x == -1) {
        a->walls[1] = false;
        b->walls[3] = false;
    }
    if (y == 1) {
        a->walls[0] = false;
        b->walls[2] = false;
    }
    if (y == -1) {
        a->walls[2] = false;
        b->walls[0] = false;
    }
}

bool checkWall(Spot* a, Spot* b) {
    int x = a->i - b->i;
    int y = a->j - b->j;
    if (x == 1) {
        return a->walls[3];
    }
    if (x == -1) {
        return a->walls[1];
    }
    if (y == 1) {
        return a->walls[0];
    }
    return a->walls[2];
}

float heuristic(Spot* a, Spot* b) {
    return std::sqrt((a->i - b->i) * (a->i - b->i) + (a->j - b->j) * (a->j - b->j));    //return euclidean distance between two spots
}

void checkInput() {
    bool p_current = sf::Keyboard::isKeyPressed(sf::Keyboard::P);
    bool m_current = sf::Keyboard::isKeyPressed(sf::Keyboard::M);
    bool c_current = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
    bool f_current = sf::Keyboard::isKeyPressed(sf::Keyboard::F);
    bool s_current = sf::Keyboard::isKeyPressed(sf::Keyboard::S);

    if (p_current)
    {
        if (state == PREMAZE)
            state = Change_start_create_maze;
    }
    if (!m_previous && m_current) {
        // M key was pressed, generate maze
        if (state == PREMAZE || state == complete_change_create_maze)
            state = GENMAZE;
    }
    if (c_current)
    {
        if (state == PRESOLVE)
        {
            state = add_wall;
        }
    }
    if (f_current)
    {
        if (state == complete_wall || state == PRESOLVE)
            state = Change_start_end;
    }
    if (!s_previous && s_current) {
        // S key was pressed, solve maze
        if (state == PRESOLVE || state == complete_wall || state == complete_change) {

            state = add_start;
        }
    }
}
std::stack<Spot>  cell_pre;
int x_cools = 0;
int x_row = 0;
int start_change_x = 0;
int start_change_y = 0;
int support_pre_cell = 0;
//void remove_add_wall_from_user(sf::RenderWindow& window, sf::Event& event)
//{
//   
//}
int end_x = *cols - 1;
int end_y = *rows - 1;
//Hai cái này là giúp chuyển đổi các ô bắt đầu và kết thúc 
int change_start_time = 0;
int change_end_time = 0;
void playMaze()
{

    sf::RenderWindow window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "maze solver");
    window.setFramerateLimit(15);


    std::srand(std::time(nullptr));

    for (int i = 0; i < *cols; i++) {
        for (int j = 0; j < *rows; j++) {
            grid[i][j].setij(i, j);
        }
    }

    Spot* current = &grid[start_change_x][start_change_y];
    Spot* next = nullptr;
    Spot* end = &grid[end_x][end_y];
    int flagzz = 0;
    int flag_start = 0;
    while (window.isOpen())
    {



        if (flagzz == 0)
        {
            start_x = 0;
            start_y = 0;
            end_x = *cols - 1;
            end_y = *rows - 1;
            flagzz = 1;
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)))
            {
                window.close();
                std::vector<Spot> tmp1(MAX_ROWS);
                std::vector<std::vector<Spot>> grid_r(MAX_COLS, tmp1);
                /*std::vector<Spot*> openSet1;
                std::vector<Spot*> closedSet1;*/
                openSet.clear();
                cell_stack.clear();
                closedSet.clear();

                //std::vector<Spot*> cell_stack1;

                //Spot* start1 = &grid[0][0];
                grid = grid_r;
                /*openSet = openSet1;
                closedSet = closedSet1;
                cell_stack = cell_stack1;*/
                while (!cell_pre.empty())
                {
                    cell_pre.pop();
                }
                /*std::stack<Spot>  cell_pre1;
                cell_pre = cell_pre1;*/
                //start = start1;
                state = PREMAZE;
                start_x = 0;
                start_y = 0;
                end_x = *cols - 1;
                end_y = *rows - 1;
                start_change_x = start_change_y = 0;
                flagzz = 0;
            }
        }


        window.clear();
        //*****************************************************************
        //std::cout << state << std::endl;
       // int flagx = 0;

        start = &grid[start_x][start_y];
        end = &grid[end_x][end_y];

        checkInput();
        //flagx = 1;
        if (state == add_start) //Cho start vao để nó giải maze
        {
            addToOpenSet(start); // add start to open set
            if ((end->walls[0] == true) && (end->walls[1] == true) && (end->walls[2] == true) && (end->walls[3] == true))
            {
                end_x = *cols;
                end_y = *rows;
                // end = &grid[*cols][*rows];

            }
            state = SOLVING;
        }
        //current->setCol(sf::Color::Blue);
        if (state == Change_start_create_maze)
        {
            current->setCol(sf::Color::Blue);
            if (event.type == sf::Event::MouseButtonPressed)
            {

                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    current->setCol(sf::Color::White);
                    start_change_x = event.mouseButton.x / (SCR_WIDTH / *cols);
                    start_change_y = event.mouseButton.y / (SCR_HEIGHT / *rows);
                    //current->setCol(sf::Color::White);
                    current = &grid[start_change_x][start_change_y];
                    //current->visited = true;
                    current->setCol(sf::Color::Blue);

                    //flag_start = 1;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                state = complete_change_create_maze;
            }
        }

        if (state == GENMAZE) {

            if (flag_start == 0)
            {
                current->visited = true;
                flag_start = 1;
            }
            current->addNeighbors(grid);
            if (current->neighbors.size() > 0) {
                float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
                int i = std::floor(r * current->neighbors.size());
                next = current->neighbors[i];
                current->neighbors.erase(current->neighbors.begin() + i);
            }
            if (next != nullptr) {
                next->visited = true;

                cell_stack.push_back(current);

                removeWall(current, next);
                current = next;
                next = nullptr;
            }
            else {
                if (cell_stack.size() == 0) {
                    // done!
                    state = PRESOLVE;
                    for (int i = 0; i < *cols; i++) {
                        for (int j = 0; j < *rows; j++) {
                            grid[i][j].addMazeNeighbors(grid);
                        }
                    }
                }
                else {
                    current = cell_stack[cell_stack.size() - 1];
                    cell_stack.pop_back();
                }
            }
        }
        if (state == SOLVING) {
            if (openSet.size() > 0) {
                // we can keep going
                int winner = 0;
                for (int i = 0; i < (int)openSet.size(); i++) {
                    if (openSet[i]->f < openSet[winner]->f) {
                        winner = i;
                    }
                }

                current = openSet[winner];

                if (openSet[winner] == end) {
                    std::cout << "Done!" << std::endl;
                    state = SOLVED;
                }

                //remove current from open set
                removeFromArray(current, openSet);
                addToClosedSet(current);

                for (int i = 0; i < (int)current->neighbors.size(); i++) {          // iterate through all neighbors of current
                    if (!arrayContainsElem(current->neighbors[i], closedSet) && !checkWall(current, current->neighbors[i])/* && !current->neighbors[i]->wall */) {     // if current neighbor is not in the closed set and neighbor is not a wall
                        float tempG = current->g + 1;

                        if (arrayContainsElem(current->neighbors[i], openSet)) {    // if current neighbor is in the open set (already has a G value)
                            if (tempG < current->neighbors[i]->g) {
                                current->neighbors[i]->g = tempG;                   // if tentative G score is better than current, set new G value
                                current->neighbors[i]->previous = current;
                            }
                        }
                        else {
                            current->neighbors[i]->g = tempG;                       // if current neighbor is not in open set, add to open set and set new G value
                            addToOpenSet(current->neighbors[i]);
                            current->neighbors[i]->h = heuristic(current->neighbors[i], end); // If we need to find the shortest way change g to h 
                            current->neighbors[i]->previous = current;
                        }
                        current->neighbors[i]->f = current->neighbors[i]->g + current->neighbors[i]->h;
                    }
                }

            }
            else {
                // no solution
                std::cout << "no solution" << std::endl;
                state = SOLVED;
            }

            for (int i = 0; i < (int)closedSet.size(); i++) {
                closedSet[i]->setCol(sf::Color(255, 180, 180));
            }
        }

        if (state == SOLVING || state == SOLVED) {
            while (current->previous != 0) {
                current->setCol(sf::Color(180, 180, 255));
                current = current->previous;
            }
            current->setCol(sf::Color(180, 180, 255));
        }



        for (int i = 0; i < *cols; i++) {
            for (int j = 0; j < *rows; j++) {
                if (state == GENMAZE) {
                    if (grid[i][j].visited)
                        //grid[i][j].setCol(sf::Color::Magenta);
                        grid[i][j].setCol(sf::Color(255, 180, 255));
                    current->setCol(sf::Color::Blue);
                }
                else if (state == PRESOLVE) {
                    grid[i][j].setCol(sf::Color::White);
                }
                window.draw(grid[i][j]);
            }
        }

        if (state == add_wall) // Check add_tuongvo
        {
            //remove_add_wall_from_user(window, event);
            sf::Vector2i MousePosn = sf::Mouse::getPosition(window);

            if (event.type == sf::Event::MouseButtonPressed)
            {
                int flag = 0;
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    x_cools = event.mouseButton.x / (SCR_WIDTH / *cols);
                    x_row = event.mouseButton.y / (SCR_HEIGHT / *rows);
                    //std::cout << x_cools << " " << x_row << std::endl;
                    //cell_pre.push(grid[x_cools][x_row]);

                    if ((grid[x_cools][x_row].walls[0] == false) || (grid[x_cools][x_row].walls[1] == false) || (grid[x_cools][x_row].walls[2] == false) || (grid[x_cools][x_row].walls[3] == false))
                    {
                        flag = 1;
                        cell_pre.push(grid[x_cools][x_row]);
                        grid[x_cools][x_row].walls = { true,true,true,true };
                        grid[x_cools][x_row].setCol(sf::Color::White);
                    }
                }
                if (flag == 1)
                {
                    window.draw(grid[x_cools][x_row]);
                }

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSpace))
            {
                if (!cell_pre.empty())
                {
                    if (support_pre_cell == 0)
                    {
                        Spot cell_pre_stack = cell_pre.top();
                        cell_pre.pop();
                        grid[cell_pre_stack.i][cell_pre_stack.j] = cell_pre_stack;
                        //std::cout << "do" << std::endl;
                        support_pre_cell = 1;
                    }
                    else
                        support_pre_cell = 0;
                }

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                state = complete_wall;
                //std::cout << "do" << std::endl;
            }
        }

        if (state == Change_start_end)
        {
            start->setCol(sf::Color::Yellow);
            end->setCol(sf::Color(255, 127, 80));
            change_end_time = change_start_time = 1;
            if (event.type == sf::Event::MouseButtonPressed)
            {

                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (change_start_time == 1)
                    {
                        grid[start_x][start_y].setCol(sf::Color::White);
                        change_start_time = 0;
                    }
                    start_x = event.mouseButton.x / (SCR_WIDTH / *cols);
                    start_y = event.mouseButton.y / (SCR_HEIGHT / *rows);

                    grid[start_x][start_y].setCol(sf::Color::Yellow);
                    change_start_time = 1;
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (change_end_time == 1)
                    {
                        grid[end_x][end_y].setCol(sf::Color::White);
                        change_end_time = 0;
                    }
                    end_x = event.mouseButton.x / (SCR_WIDTH / *cols);
                    end_y = event.mouseButton.y / (SCR_HEIGHT / *rows);
                    grid[end_x][end_y].setCol(sf::Color(255, 127, 80));
                    change_end_time = 1;
                }


            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                state = complete_change;
            }
        }
        //checkInput();

        //*****************************************************************
        window.display();
    }
}

sf::Texture nameInputTexture;
sf::Texture nameInputTexture1;
sf::Font font;
sf::Text data_coll;
sf::Text data_row;
sf::RectangleShape nameInput(sf::Vector2f(400.0f, 200.0f));
sf::RectangleShape nameInput1(sf::Vector2f(400.0f, 200.0f));

void dosomthing()
{
    nameInput.setPosition(355, 260);
    nameInput1.setPosition(355, 260);
    nameInputTexture.loadFromFile("name.png");
    nameInputTexture1.loadFromFile("name1.png");
    nameInput.setPosition(355, 260);
    nameInput1.setPosition(355, 260);

    nameInput.setTexture(&nameInputTexture);
    nameInput1.setTexture(&nameInputTexture1);

    font.loadFromFile("arial.ttf");
    data_coll.setFont(font);
    data_row.setFont(font);

    data_coll.setPosition(430, 370);

    data_row.setPosition(430, 370);

}

//nameInputText.setFont(nameInputFont);
//nameInputText.setPosition(430, 370);
std::string set_coll;
std::string set_row;
int page_number = 3;
int selection_option = 2;
int get_out = 0;
int gets_out = 0;
int delete_support_cols = 0;
int delete_support_rows = 0;
void change_coll()
{
    dosomthing();
    sf::RenderWindow windowe(sf::VideoMode(1024, 640), "Columns", sf::Style::Close);

    sf::Texture texture;

    if (!texture.loadFromFile("nameground.jpg"))
    {
        //handle error
    }
    sf::Vector2i MousePosn = sf::Mouse::getPosition(windowe);
    sf::Sprite background;
    background.setTexture(texture);
    while (windowe.isOpen())
    {
        sf::Event event;
        while (windowe.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || get_out == 1)
            {

                windowe.close();

            }

            if (event.TextEntered)
                if (event.text.unicode > 47 && event.text.unicode < 58)
                    set_coll += event.text.unicode;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSpace))
            {
                if (delete_support_cols == 0)
                {
                    if (set_coll.size() > 0)  set_coll.resize(set_coll.size() - 1);
                    delete_support_cols = 1;
                }
                else
                    delete_support_cols = 0;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                if (set_coll[0] != '\0')
                {
                    std::stringstream geek(set_coll);
                    int temp_cols = 0;
                    geek >> temp_cols;
                    if ((temp_cols <= MAX_COLS) && (temp_cols > 0)) // Được cấp phát tối đa 50 cols là MAX
                    {
                        value_cols = temp_cols;
                    }
                    else
                    {
                        std::stringstream ss;
                        ss << value_cols;
                        set_coll = ss.str();
                    }


                }
                get_out = 1;

            }
            data_coll.setString(set_coll);

            //std::cout << cols << std::endl;
        }
        windowe.clear();
        windowe.draw(background);

        windowe.draw(data_coll);
        windowe.draw(nameInput);

        windowe.display();
    }
}

void change_rows()
{
    sf::RenderWindow windowne(sf::VideoMode(1024, 640), "Rows", sf::Style::Close);
    dosomthing();
    sf::Texture texture;

    if (!texture.loadFromFile("nameground.jpg"))
    {
        //handle error
    }
    sf::Vector2i MousePosn = sf::Mouse::getPosition(windowne);
    sf::Sprite background;
    background.setTexture(texture);
    while (windowne.isOpen())
    {
        sf::Event event;
        while (windowne.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || gets_out == 1)
            {

                windowne.close();

            }

            if (event.TextEntered)
                if (event.text.unicode > 47 && event.text.unicode < 58)
                    set_row += event.text.unicode;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSpace))
            {
                if (delete_support_rows == 0)
                {
                    if (set_row.size() > 0)  set_row.resize(set_row.size() - 1);
                    delete_support_rows = 1;
                }
                else
                    delete_support_rows = 0;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                if (set_row[0] != '\0')
                {
                    std::stringstream geek(set_row);
                    int temp_rows = 0;
                    geek >> temp_rows;
                    if ((temp_rows <= MAX_ROWS) && (temp_rows > 0))// Được cấp phát tối đa 40 rows là MAX
                    {
                        value_rows = temp_rows;
                    }
                    else
                    {
                        std::stringstream ss;
                        ss << value_rows;
                        set_row = ss.str();
                    }


                }

                gets_out = 1;

            }
            data_row.setString(set_row);

            //std::cout << cols << std::endl;
        }
        windowne.clear();
        windowne.draw(background);

        windowne.draw(data_row);
        windowne.draw(nameInput1);

        windowne.display();
    }
}
void option_menu()
{
    while (true)
    {

        sf::RenderWindow windows(sf::VideoMode(1024, 640), "Option", sf::Style::Close);
        option options(1024, 640);
        sf::Texture texture;
        if (!texture.loadFromFile("options.jpg"))
        {
            //handle error
        }
        sf::Sprite background;
        background.setTexture(texture);

        while (windows.isOpen())
        {
            sf::Event event;
            while (windows.pollEvent(event))
            {
                //std::cout << *rows - 1 << " " << *cols - 1 << std::endl;
                if (event.type == sf::Event::Closed)
                {
                    selection_option = 2;
                    windows.close();

                }
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::Up)
                    {
                        options.moveUp();
                        break;
                    }
                    if (event.key.code == sf::Keyboard::Down)
                    {
                        options.moveDown();
                        break;
                    }

                    //choose page
                    if (event.key.code == sf::Keyboard::Return) //enter
                    {
                        if (options.optionpreesed() == 0)
                        {
                            windows.close();
                            selection_option = 0;
                        }
                        if (options.optionpreesed() == 1)
                        {
                            windows.close();
                            selection_option = 1;
                        }
                        if (options.optionpreesed() == 2)
                        {
                            windows.close();
                            selection_option = 2;
                        }

                    }


                }

            }
            windows.clear();

            windows.draw(background);
            options.draw(windows);
            windows.display();
        }

        if (selection_option == 0)  //  Change Columns
        {
            get_out = 0;
            change_coll();
        }
        if (selection_option == 1)//change rows
        {
            gets_out = 0;
            change_rows();
        }

        if (selection_option == 2)
        {
            break;
        }
    }
}


void infor_menu()
{
    sf::RenderWindow window(sf::VideoMode(1024, 640), "Infor", sf::Style::Close);
    Infor inforr(1024, 640);
    sf::Texture texture;
    if (!texture.loadFromFile("infor.jpg"))
    {
        //handle error
    }
    sf::Sprite background;
    background.setTexture(texture);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)))
                window.close();


        }
        window.clear();
        window.draw(background);
        inforr.draw(window);

        window.display();
    }
}

int main()
{

    while (true)
    {


        sf::RenderWindow window(sf::VideoMode(1024, 640), "Menu", sf::Style::Close);
        Menu menuu(1024, 640);
        sf::Texture texture;
        if (!texture.loadFromFile("maze.jpg"))
        {
            //handle error
        }
        sf::Sprite background;
        background.setTexture(texture);
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                //std::cout << *rows - 1 << " " << *cols - 1 << std::endl;
                if (event.type == sf::Event::Closed)
                {
                    page_number = 3;
                    window.close();

                }
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::Up)
                    {
                        menuu.moveUp();
                        break;
                    }
                    if (event.key.code == sf::Keyboard::Down)
                    {
                        menuu.moveDown();
                        break;
                    }

                    //choose page
                    if (event.key.code == sf::Keyboard::Return) //enter
                    {

                        if (menuu.Mainmenupreesed() == 0)
                        {
                            window.close();
                            page_number = 0;
                        }
                        if (menuu.Mainmenupreesed() == 1)
                        {
                            window.close();
                            page_number = 1;
                        }
                        if (menuu.Mainmenupreesed() == 2)
                        {
                            window.close();
                            page_number = 2;
                        }
                        if (menuu.Mainmenupreesed() == 3)
                        {
                            window.close();
                            page_number = 3;
                        }
                    }


                }

            }
            window.clear();
            window.draw(background);
            menuu.draw(window);
            window.display();
        }
        if (page_number == 0) // Run maze
        {
            playMaze();


        }
        if (page_number == 1)
        {
            //option

            option_menu();


        }
        if (page_number == 2)
        {
            //infor
            infor_menu();
            //reset_maze = 1;
        }
        if (page_number == 3)
        {
            //exit
            break;
        }
        /*if (reset_maze == 1)
        {



            reset_maze = 0;
        }*/

    }

    return 0;
}
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include<windows.h>
#include <thread>
using namespace std;
//Global variables
bool resumeG = false;
const int EMPTY = 0;
const int Player1 = 1;
const int Player2 = -1;
const int col = 5;
const int cellsize = 100;
int currentPlayer = Player2;
int moveMade = 0;
bool actualgame = false;
bool animeselected = false;
bool player1HasBeads = false;
bool player2HasBeads = false;
bool batmanselected = false;
int score1 = 0;
int score2 = 0;
//BOARD 
int board[5][5] = { {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 0, -1, -1}, {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1} };
//TEXT GLOBAL VARIABLES
const string SAVE_FILE_PATH = "game_save.txt";
sf::Text exitText;
sf::Text anime;
sf::Sound sound;
sf::Text batman;
sf::Text resume;
sf::Text rulesText;
sf::Text save;
sf::Text theme;
sf::SoundBuffer buffer;
//Music for game 
sf::Music music;
bool initializeMusic(const std::string& filePath) {
    if (!music.openFromFile(filePath)) {
        // Error handling
        return false;
    }
    return true;
}
void playMusic() {
    music.play();
}
// Save game state to a file FILE HANDLING
void SaveGameToFile()
{
    ofstream saveFile(SAVE_FILE_PATH);

    if (saveFile.is_open())
    {
        // Save the board state
        for (int i = 0; i < col; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                saveFile << board[i][j] << " ";
            }
            saveFile << endl;
        }

        // Save the current player
        saveFile << currentPlayer;

        saveFile.close();
        //cout << "Game saved successfully." << endl;
    }
    else
    {
        cout << "Unable to open save file." << endl;
    }
}

// Load game state from a file
void LoadGameFromFile()
{
    ifstream loadFile(SAVE_FILE_PATH);

    if (loadFile.is_open())
    {
        // Load the board state
        for (int i = 0; i < col; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                loadFile >> board[i][j];
            }
        }

        // Load the current player
        loadFile >> currentPlayer;

        loadFile.close();
        //cout << "Game loaded successfully." << endl;
    }
    else
    {
        cout << "Unable to open save file. Starting a new game." << endl;

    }
}
// FUNCTIONS FOR MOVEMENT 
void up(int board[][col], int sel_row, int sel_col)
{
    if ((board[sel_row - 1][sel_col] == 0))
    {
        board[sel_row - 1][sel_col] = board[sel_row][sel_col];
        board[sel_row][sel_col] = EMPTY;
    }
    if ((board[sel_row - 2][sel_col] == 0) && (board[sel_row - 1][sel_col] * -1 == (board[sel_row][sel_col])))
    {
        board[sel_row - 2][sel_col] = board[sel_row][sel_col];
        board[sel_row][sel_col] = EMPTY;
        board[sel_row - 1][sel_col] = EMPTY;

    }
}
void down(int board[][col], int sel_row, int sel_col)
{
    if (board[sel_row + 1][sel_col] == 0)
    {
        board[sel_row + 1][sel_col] = board[sel_row][sel_col];
        board[sel_row][sel_col] = EMPTY;
    }
    if ((board[sel_row + 2][sel_col] == 0) && (board[sel_row + 1][sel_col] * -1 == (board[sel_row][sel_col])))
    {
        board[sel_row + 2][sel_col] = board[sel_row][sel_col];
        board[sel_row][sel_col] = EMPTY;
        board[sel_row + 1][sel_col] = EMPTY;

    }
}
void right(int board[][col], int sel_row, int sel_col)
{
    if (board[sel_row][sel_col + 1] == 0)
    {
        board[sel_row][sel_col + 1] = board[sel_row][sel_col];
        board[sel_row][sel_col] = EMPTY;
    }
    if ((board[sel_row][sel_col + 2] == 0) && (board[sel_row][sel_col + 1] * -1 == (board[sel_row][sel_col])))
    {
        board[sel_row][sel_col + 2] = board[sel_row][sel_col];
        board[sel_row][sel_col] = EMPTY;
        board[sel_row][sel_col + 1] = EMPTY;
    }
}
void left(int board[][col], int sel_row, int sel_col)
{
    if (board[sel_row][sel_col - 1] == 0)
    {
        board[sel_row][sel_col - 1] = board[sel_row][sel_col];
        board[sel_row][sel_col] = EMPTY;
    }
    if ((board[sel_row][sel_col - 2] == 0) && (board[sel_row][sel_col - 1] * -1 == (board[sel_row][sel_col])))
    {
        board[sel_row][sel_col - 2] = board[sel_row][sel_col];
        board[sel_row][sel_col] = EMPTY;
        board[sel_row][sel_col - 1] = EMPTY;
    }
}
void diagonal_up_right(int board[][col], int sel_row, int sel_col)
{
    if (board[sel_row - 1][sel_col + 1] == 0)
    {
        if ((sel_row + sel_col) % 2 == 0)
        {
            board[sel_row - 1][sel_col + 1] = board[sel_row][sel_col];
            board[sel_row][sel_col] = EMPTY;
        }
    }
    if ((board[sel_row - 2][sel_col + 2] == 0) && (board[sel_row - 1][sel_col + 1] * -1 == (board[sel_row][sel_col])))
    {
        if ((sel_row + sel_col) % 2 == 0)
        {
            board[sel_row - 2][sel_col + 2] = board[sel_row][sel_col];
            board[sel_row][sel_col] = EMPTY;
            board[sel_row - 1][sel_col + 1] = EMPTY;
        }
    }
}
void diagonal_down_right(int board[][col], int sel_row, int sel_col)
{
    if (board[sel_row + 1][sel_col + 1] == 0)
    {
        if ((sel_col + sel_row) % 2 == 0)
        {
            board[sel_row + 1][sel_col + 1] = board[sel_row][sel_col];
            board[sel_row][sel_col] = EMPTY;
        }
    }
    if ((board[sel_row + 2][sel_col + 2] == 0) && (board[sel_row + 1][sel_col + 1] * -1 == (board[sel_row][sel_col])))
    {
        if ((sel_row + sel_col) % 2 == 0)
        {
            board[sel_row + 2][sel_col + 2] = board[sel_row][sel_col];
            board[sel_row][sel_col] = EMPTY;
            board[sel_row + 1][sel_col + 1] = EMPTY;
        }
    }
}
void diagonal_up_left( int board[][col],int sel_row, int sel_col)
{
    if (board[sel_row - 1][sel_col - 1] == 0)
    {
        if ((sel_col + sel_row) % 2 == 0)
        {
            board[sel_row - 1][sel_col - 1] = board[sel_row][sel_col];
            board[sel_row][sel_col] = EMPTY;
        }
    }
    if ((board[sel_row - 2][sel_col - 2] == 0) && (board[sel_row - 1][sel_col - 1] * -1 == (board[sel_row][sel_col])))
    {
        if ((sel_row + sel_col) % 2 == 0)
        {
            board[sel_row - 2][sel_col - 2] = board[sel_row][sel_col];
            board[sel_row][sel_col] = EMPTY;
            board[sel_row - 1][sel_col - 1] = EMPTY;
        }
    }
}
void diagonal_down_left(int board[][col], int sel_row, int sel_col)
{

    if (board[sel_row + 1][sel_col - 1] == 0)
    {
        if ((sel_col + sel_row) % 2 == 0)
        {
            board[sel_row + 1][sel_col - 1] = board[sel_row][sel_col];
            board[sel_row][sel_col] = EMPTY;
        }
    }
    if ((board[sel_row + 2][sel_col - 2] == 0) && (board[sel_row + 1][sel_col - 1] * -1 == (board[sel_row][sel_col])))
    {
        if ((sel_row + sel_col) % 2 == 0)
        {
            board[sel_row + 2][sel_col - 2] = board[sel_row][sel_col];
            board[sel_row][sel_col] = EMPTY;
            board[sel_row + 1][sel_col - 1] = EMPTY;
        }
    }
}
void botMove()              // BOT MOVEMENT FUNCTIONS 
{
    int moveMade = 0;
    for (int i = 1; i < col-1; i++)
    {
        for (int j = 1; j < col-1; j++)
        {
            if (board[i][j] == 0)
            {
                // elimination bot
                // diagonal right down
                if ((board[i - 1][j - 1] == -1) && (board[i - 2][j - 2] == 1))
                {
                    if ((i + j) % 2 == 0)
                    {
                        board[i][j] = board[i - 2][j - 2];
                        board[i - 2][j - 2] = EMPTY;
                        board[i - 1][j - 1] = EMPTY;
                        moveMade = 1;
                        
                        break;
                    }
                }
                // diagonal left up
                else if ((board[i + 1][j + 1] == -1) && (board[i + 2][j + 2] == 1))
                {
                    if ((i + j) % 2 == 0)
                    {
                        board[i][j] = board[i + 2][j + 2];
                        board[i + 2][j + 2] = EMPTY;
                        board[i + 1][j + 1] = EMPTY;
                        moveMade = 1;
                        
                        break;
                    }
                }
                // diagonal right up
                else if ((board[i - 1][j + 1] == -1) && (board[i - 2][j + 2] == 1))
                {
                    if ((i + j) % 2 == 0)
                    {
                        board[i][j] = board[i - 2][j + 2];
                        board[i - 2][j + 2] = EMPTY;
                        board[i - 1][j + 1] = EMPTY;
                        moveMade = 1;
                        
                        break;
                    }
                }
                // diagonal left down
                else if (i>=2&&(board[i + 1][j - 1] == -1) && (board[i + 2][j - 2] == 1))
                {
                    if ((i + j) % 2 == 0)
                    {
                        board[i][j] = board[i + 2][j - 2];
                        board[i + 2][j - 2] = EMPTY;
                        board[i + 1][j - 1] = EMPTY;
                        moveMade = 1;
                        
                        break;
                    }
                }
                // down elimination
                else if ((board[i - 2][j] == 1) && (board[i - 1][j] == -1))
                {
                    board[i][j] = board[i - 2][j];
                    board[i - 2][j] = EMPTY;
                    board[i - 1][j] = EMPTY;
                    moveMade = 1;
                    
                    break;
                }
                // up elimination
                else if ((board[i + 2][j] == 1) && (board[i + 1][j] == -1))
                {
                    board[i][j] = board[i + 2][j];
                    board[i + 2][j] = EMPTY;
                    board[i + 1][j] = EMPTY;
                    moveMade = 1;
                    
                    break;
                }
                // left elimination
                else if ((board[i][j + 2] == 1) && (board[i][j + 1] == -1))
                {
                    board[i][j] = board[i][j + 2];
                    board[i][j + 2] = EMPTY;
                    board[i][j + 1] = EMPTY;
                    moveMade = 1;
                   
                    break;
                }
                // right elimination
                else if (j >= 2 && (board[i][j - 2] == 1) && (board[i][j - 1] == -1))
                {
                    board[i][j] = board[i][j - 2];
                    board[i][j - 2] = EMPTY;
                    board[i][j - 1] = EMPTY;
                    moveMade = 1;
                    
                    break;
                }

                // movement
                //  down
                else if (board[i - 1][j] == 1)
                {
                    board[i][j] = board[i - 1][j];
                    board[i - 1][j] = EMPTY;
                    moveMade = 1;
                    
                    break;
                }
                // up
                else if (board[i + 1][j] == 1)
                {
                    board[i][j] = board[i + 1][j];
                    board[i + 1][j] = EMPTY;
                    moveMade = 1;
                    
                    break;
                }
                // right
                else if (board[i][j - 1] == 1)
                {
                    board[i][j] = board[i][j - 1];
                    board[i][j - 1] = EMPTY;
                    moveMade = 1;
                    
                    break;
                }
                // left
                else if (board[i][j + 1] == 1)
                {
                    board[i][j] = board[i][j + 1];
                    board[i][j + 1] = EMPTY;
                    moveMade = 1;
                    
                    break;
                }
                // diagonal down left
                else if (board[i + 1][j - 1] == 1)
                {
                    if ((i + j) % 2 == 0)
                    {
                        board[i][j] = board[i + 1][j - 1];
                        board[i + 1][j - 1] = EMPTY;
                        moveMade = 1;
                        
                        break;
                    }
                }
                // diagonal up right
                else if (board[i - 1][j + 1] == 1)
                {
                    if ((i + j) % 2 == 0)
                    {
                        board[i][j] = board[i - 1][j + 1];
                        board[i - 1][j + 1] = EMPTY;
                        moveMade = 1;
                        
                        break;
                    }
                }
                // diagonal up left
                else if (board[i - 1][j - 1] == 1)
                {
                    if ((i + j) % 2 == 0)
                    {
                        board[i][j] = board[i - 1][j - 1];
                        board[i - 1][j - 1] = EMPTY;
                        moveMade = 1;
                        
                        break;
                    }
                }
                // diagonal down right
                else if (board[i + 1][j + 1] == 1)
                {
                    if ((i + j) % 2 == 0)
                    {
                        board[i][j] = board[i + 1][j + 1];
                        board[i + 1][j + 1] = EMPTY;
                        moveMade = 1;
                        
                        break;
                    }
                }
            }
        }
        if (moveMade == 1)
        {
            break;
        }
    }
}
//WINNER CONDITION AND DISPLAY 
void displayWinner(sf::RenderWindow& window, const std::string& winner)
{
    // Clear the window
    window.clear();

    // Display the winning message
    sf::Font font;
    if (!font.loadFromFile("final.ttf"))
    {
        cerr << "Error loading font" << endl;
        return;
    }
    window.clear(sf:: Color::Black);
    sf::Text winnerText;
    winnerText.setFont(font);
    winnerText.setString(winner + " \n wins!");
    winnerText.setCharacterSize(50);
    winnerText.setFillColor(sf::Color::White);
    winnerText.setPosition(250, 205);
    //float effect
    sf::Clock clock1;
    // Update positions based on sine function for floating effect
    float time = static_cast<float>(clock1.getElapsedTime().asMilliseconds());
    float floatOffset = 5.f * std::sin(time / 200.f);

    winnerText.setPosition(50, 50 + floatOffset);
    

    // Draw the winning message
    window.draw(winnerText);

    // Update the window
    window.display();

    // Wait for a moment
    Sleep(30000);

    // Close the window 
    window.close();
}
// Function to start the first BATMAN GAME WITH BOT
void startGame()
{
    int xOffset = 25;
    int yOffset = 25;
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(cellsize * col, cellsize * col), "SFML window");

    // Load textures
    sf::Texture boardTexture, bead1Texture, bead2Texture;
    if (!boardTexture.loadFromFile("batmansupermanboard.jpg") ||
        !bead1Texture.loadFromFile("batmansayan1.png") ||
        !bead2Texture.loadFromFile("supermanshayan1.png"))
    {
        cerr << "Error loading textures" << endl;
        return;
    }
    sf::Font font;
    if (!font.loadFromFile("font.ttf"))
    {
        cerr << "Error loading font" << endl;
        return;
    }
    sf::Text playerTurnText1;
    playerTurnText1.setFont(font);
    playerTurnText1.setString("Player1 Turn");
    playerTurnText1.setFillColor(sf::Color::White);
    playerTurnText1.setPosition(5, 5);

    sf::Text playerTurnText2;
    playerTurnText2.setFont(font);
    playerTurnText2.setString("Player2 Turn");
    playerTurnText2.setFillColor(sf::Color::White);
    playerTurnText2.setPosition(5, 5);

    // Create sprites
    sf::Sprite boardSprite(boardTexture);
    sf::Sprite bead1Sprite(bead1Texture);
    sf::Sprite bead2Sprite(bead2Texture);
    boardSprite.setScale(sf::Vector2f(1.01f, 1.01f));
    bead1Sprite.setScale(sf::Vector2f(0.6f, 0.6f));
    bead2Sprite.setScale(sf::Vector2f(0.6f, 0.6f));

    bool beadSelected = false;
    int selectedRow = -1, selectedCol = -1;

    // Start the game loop
    while (window.isOpen())
    {
        if (resumeG == true)
        {
            LoadGameFromFile();
            resumeG = false;
        }
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int clickedRow = (event.mouseButton.y - yOffset) / cellsize;
                    int clickedCol = (event.mouseButton.x - xOffset) / cellsize;
                    if (clickedRow >= 0 && clickedRow < 5 && clickedCol >= 0 && clickedCol < 5)
                    {
                        if ((!beadSelected && board[clickedRow][clickedCol] != EMPTY) && (board[clickedRow][clickedCol] == currentPlayer))
                        {
                            beadSelected = true;
                            selectedRow = clickedRow;
                            selectedCol = clickedCol;
                        }
                        else if (beadSelected)
                        {

                            if ((clickedRow == selectedRow - 1 && clickedCol == selectedCol) || (clickedRow == selectedRow - 2 && clickedCol == selectedCol))
                            {
                                up(board, selectedRow, selectedCol);
                            }
                            else if ((clickedRow == selectedRow + 1 && clickedCol == selectedCol) || (clickedRow == selectedRow + 2 && clickedCol == selectedCol))
                            {
                                down(board, selectedRow, selectedCol);

                            }
                            else if ((clickedRow == selectedRow && clickedCol == selectedCol - 1) || (clickedRow == selectedRow && clickedCol == selectedCol - 2))
                            {
                                left(board, selectedRow, selectedCol);
                            }
                            else if ((clickedRow == selectedRow && clickedCol == selectedCol + 1) || (clickedRow == selectedRow && clickedCol == selectedCol + 2))
                            {
                                right(board, selectedRow, selectedCol);
                            }
                            else if ((clickedRow == selectedRow - 1 && clickedCol == selectedCol + 1) || (clickedRow == selectedRow - 2 && clickedCol == selectedCol + 2))
                            {
                                diagonal_up_right(board, selectedRow, selectedCol);
                            }
                            else if ((clickedRow == selectedRow + 1 && clickedCol == selectedCol + 1) || (clickedRow == selectedRow + 2 && clickedCol == selectedCol + 2))
                            {
                                diagonal_down_right(board, selectedRow, selectedCol);
                            }
                            else if ((clickedRow == selectedRow - 1 && clickedCol == selectedCol - 1) || (clickedRow == selectedRow - 2 && clickedCol == selectedCol - 2))
                            {
                                diagonal_up_left(board, selectedRow, selectedCol);
                            }
                            else if ((clickedRow == selectedRow + 1 && clickedCol == selectedCol - 1) || (clickedRow == selectedRow + 2 && clickedCol == selectedCol - 2))
                            {
                                diagonal_down_left(board, selectedRow, selectedCol);
                            }

                            currentPlayer = (currentPlayer == Player1) ? Player2 : Player1;
                            if (currentPlayer == Player1) {
                                botMove();
                                currentPlayer = Player2;
                            }
                            beadSelected = false;
                        }
                    }
                }
            }
        }
        // Clear screen
        window.clear();


        // Draw the board
        window.draw(boardSprite);

        // Draw the beads
        for (int i = 0; i < col; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                if (board[i][j] == Player1)
                {
                    bead1Sprite.setPosition(j * cellsize + xOffset, i * cellsize + yOffset);
                    window.draw(bead1Sprite);
                }
                else if (board[i][j] == Player2)
                {
                    bead2Sprite.setPosition(j * cellsize + xOffset, i * cellsize + yOffset);
                    window.draw(bead2Sprite);
                }
            }
        }
        // Draw player turn text
        if (currentPlayer == Player2)
        {
            window.draw(playerTurnText2);
        }
        else
        {
            window.draw(playerTurnText1);
        }
        //save button logic 

        sf::Text save;
        save.setFont(font);
        save.setString("SAVE");
        save.setFillColor(sf::Color::White);
        save.setPosition(400, 5);
        window.draw(save);
        // Handle button clicks
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (save.getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                //save the current state
                SaveGameToFile();

            }
        }
        // GLOW HOVER EFFECT
        sf::Vector2i mousePoss = sf::Mouse::getPosition(window);

        if (save.getGlobalBounds().contains(mousePoss.x, mousePoss.y))
        {
            save.setOutlineColor(sf::Color::Red);
            save.setOutlineThickness(2.1f);
            sound.play();
        }
        else
        {
            save.setOutlineColor(sf::Color::Transparent);
        }


        player1HasBeads = false;
        player2HasBeads = false;
        for (int i = 0; i < col; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                if (board[i][j] == Player1)
                {
                    player1HasBeads = true;
                }
                else if (board[i][j] == Player2)
                {
                    player2HasBeads = true;
                }
            }
        }

        if (player1HasBeads == false)
        {
            displayWinner(window, "Player2");
            cout << "Player 2 wins! Player 1 has no beads left." << endl;
            window.close(); // Close the window 
        }
        else if (player2HasBeads == false)
        {
            displayWinner(window, "Player1");
            cout << "Player 1 wins! Player 2 has no beads left." << endl;
            window.close(); // Close the window 
        }
        // Update the window
        window.display();
    }
}
// Function to start the Madara GAME theme
void startGameMadara()
{
    int xOffset = 25;
    int yOffset = 25;
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(cellsize * col, cellsize * col), "SFML window");
    
    // Load textures
    sf::Texture boardTexture, bead1Texture, bead2Texture,board1tex;
    if (!boardTexture.loadFromFile("madback.jpeg") || !board1tex.loadFromFile("madboard2.png")||
        !bead1Texture.loadFromFile("Madara.png") ||
        !bead2Texture.loadFromFile("hasirama.png"))
    {
        cerr << "Error loading textures" << endl;
        return;
    }
    sf::Font font;
    if (!font.loadFromFile("final.ttf"))
    {
        cerr << "Error loading font" << endl;
        return;
    }
    

    sf::Text playerTurnText1;
    playerTurnText1.setFont(font);
    playerTurnText1.setString("PlayerOne Turn");
    playerTurnText1.setCharacterSize(20); // Set a larger character size
    playerTurnText1.setFillColor(sf::Color::White);
    playerTurnText1.setPosition(0, 0);

    sf::Text playerTurnText2;
    playerTurnText2.setFont(font);
    playerTurnText2.setString("Player2 Turn");
    playerTurnText2.setCharacterSize(20); // Set a larger character size
    playerTurnText2.setFillColor(sf::Color::White);
    playerTurnText2.setPosition(0, 0);

    // Create sprites
    sf::Sprite boardSprite(boardTexture);
    sf::Sprite bead1Sprite(bead1Texture);
    sf::Sprite bead2Sprite(bead2Texture);
    sf::Sprite board12(board1tex);
    boardSprite.setScale(sf::Vector2f(2.4f, 2.4f));
    bead1Sprite.setScale(sf::Vector2f(0.6f, 0.6f));
    bead2Sprite.setScale(sf::Vector2f(0.6f, 0.6f));
    board12.setScale(sf::Vector2f(1.01f, 1.01f));

    bool beadSelected = false;
    int selectedRow = -1, selectedCol = -1;

    // Start the game loop
    while (window.isOpen())
    {
        if (resumeG == true)
        {
            LoadGameFromFile();
            resumeG = false;
        }
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int clickedRow = (event.mouseButton.y - yOffset) / cellsize;
                    int clickedCol = (event.mouseButton.x - xOffset) / cellsize;
                    if (clickedRow >= 0 && clickedRow < 5 && clickedCol >= 0 && clickedCol < 5)
                    {
                        if ((!beadSelected && board[clickedRow][clickedCol] != EMPTY) && (board[clickedRow][clickedCol] == currentPlayer))
                        {
                            beadSelected = true;
                            selectedRow = clickedRow;
                            selectedCol = clickedCol;
                        }
                        else if (beadSelected)
                        {

                            if ((clickedRow == selectedRow - 1 && clickedCol == selectedCol) || (clickedRow == selectedRow - 2 && clickedCol == selectedCol))
                            {
                                up(board, selectedRow, selectedCol);
                            }
                            else if ((clickedRow == selectedRow + 1 && clickedCol == selectedCol) || (clickedRow == selectedRow + 2 && clickedCol == selectedCol))
                            {
                                down(board, selectedRow, selectedCol);

                            }
                            else if ((clickedRow == selectedRow && clickedCol == selectedCol - 1) || (clickedRow == selectedRow && clickedCol == selectedCol - 2))
                            {
                                left(board, selectedRow, selectedCol);
                            }
                            else if ((clickedRow == selectedRow && clickedCol == selectedCol + 1) || (clickedRow == selectedRow && clickedCol == selectedCol + 2))
                            {
                                right(board, selectedRow, selectedCol);
                            }
                            else if ((clickedRow == selectedRow - 1 && clickedCol == selectedCol + 1) || (clickedRow == selectedRow - 2 && clickedCol == selectedCol + 2))
                            {
                                diagonal_up_right(board, selectedRow, selectedCol);
                            }
                            else if ((clickedRow == selectedRow + 1 && clickedCol == selectedCol + 1) || (clickedRow == selectedRow + 2 && clickedCol == selectedCol + 2))
                            {
                                diagonal_down_right(board, selectedRow, selectedCol);
                            }
                            else if ((clickedRow == selectedRow - 1 && clickedCol == selectedCol - 1) || (clickedRow == selectedRow - 2 && clickedCol == selectedCol - 2))
                            {
                                diagonal_up_left(board, selectedRow, selectedCol);
                            }
                            else if ((clickedRow == selectedRow + 1 && clickedCol == selectedCol - 1) || (clickedRow == selectedRow + 2 && clickedCol == selectedCol - 2))
                            {
                                diagonal_down_left(board, selectedRow, selectedCol);
                            }

                            currentPlayer = (currentPlayer == Player1) ? Player2 : Player1;
                            beadSelected = false;
                        }
                    }
                }
            }
        }
        // Glow hover
        sf::Vector2i mousePoss = sf::Mouse::getPosition(window);

        if (save.getGlobalBounds().contains(mousePoss.x, mousePoss.y))
        {
            save.setOutlineColor(sf::Color::Red);
            save.setOutlineThickness(2.1f);
            sound.play();
        }
        else
        {
            save.setOutlineColor(sf::Color::Transparent);
        }
        // Clear screen
        window.clear();
        // Draw the board
        window.draw(boardSprite);
        window.draw(board12);
        // Draw the beads
        for (int i = 0; i < col; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                if (board[i][j] == Player1)
                {
                    bead1Sprite.setPosition(j * cellsize + xOffset, i * cellsize + yOffset);
                    window.draw(bead1Sprite);
                }
                else if (board[i][j] == Player2)
                {
                    bead2Sprite.setPosition(j * cellsize + xOffset, i * cellsize + yOffset);
                    window.draw(bead2Sprite);
                }
            }
        }
        // Handle button clicks
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (save.getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                //save the current state
                SaveGameToFile();
                
            }
        }
        player1HasBeads = false;
        player2HasBeads = false;
        for (int i = 0; i < col; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                if (board[i][j] == Player1)
                {
                    player1HasBeads = true;
                }
                else if (board[i][j] == Player2)
                {
                    player2HasBeads = true;
                }
            }
        }
        if (player1HasBeads==false)
        {
            displayWinner(window, "Player2");
            cout << "Player 2 wins! Player 1 has no beads left." << endl;
            window.close(); // Close the window 
        }
        else if (player2HasBeads==false)
        {
            displayWinner(window, "Player1");
            cout << "Player 1 wins! Player 2 has no beads left." << endl;
            window.close(); // Close the window 
        }
        // Update the window
         // Draw player turn text
        if (currentPlayer == Player2)
        {
            window.draw(playerTurnText2);
        }
        else
        {
            window.draw(playerTurnText1);
        }
        //save button logic 

        save.setFont(font);
        save.setString("SAVE");
        save.setCharacterSize(22); // SETTING a larger character size
        save.setFillColor(sf::Color::White);
        save.setPosition(400, 5);
        window.draw(save);

        window.display();
    }
}
//drawing the main menu
void drawButtons(sf::RenderWindow& window, sf::Font& font, bool& gameStarted, sf::Text& startGameText)
{  
    // Setting up the START GAME button
    startGameText.setFont(font);
    startGameText.setCharacterSize(150); // SETTING a larger character size
    startGameText.setString("S T A R T G A M E");
    startGameText.setCharacterSize(30); // to adjust the character size
    startGameText.setFillColor(sf::Color::White);
    startGameText.setPosition(window.getSize().x / 2 - startGameText.getGlobalBounds().width / 2, window.getSize().y / 2 - 20);
    window.draw(startGameText);
    // Setting up the EXIT button    
    exitText.setFont(font);
    exitText.setString("EXIT");
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(50, 60);
    window.draw(exitText);
    // Set up the RESUME button    
    resume.setFont(font);
    resume.setString("RESUME");
    resume.setFillColor(sf::Color::White);
    resume.setPosition(50, 100);
    window.draw(resume);
    // Glow hover
    sf::Vector2i mousePosh = sf::Mouse::getPosition(window);

    if (startGameText.getGlobalBounds().contains(mousePosh.x, mousePosh.y))
    {
        startGameText.setOutlineColor(sf::Color::Blue);
        startGameText.setOutlineThickness(2.1f);
        sound.play();
    }
    else
    {
        startGameText.setOutlineColor(sf::Color::Transparent);
    }

    if (exitText.getGlobalBounds().contains(mousePosh.x, mousePosh.y))
    {
        exitText.setOutlineColor(sf::Color::Blue);
        exitText.setOutlineThickness(2.1f);
        sound.play();
    }
    else
    {
        exitText.setOutlineColor(sf::Color::Transparent);
    }

    if (rulesText.getGlobalBounds().contains(mousePosh.x, mousePosh.y))
    {
        rulesText.setOutlineColor(sf::Color::Blue);
        rulesText.setOutlineThickness(2.1f);
        sound.play();
    }
    else
    {
        rulesText.setOutlineColor(sf::Color::Transparent);
    }

    if (resume.getGlobalBounds().contains(mousePosh.x, mousePosh.y))
    {
        resume.setOutlineColor(sf::Color::Blue);
        resume.setOutlineThickness(2.1f);
        sound.play();
    }
    else
    {
        resume.setOutlineColor(sf::Color::Transparent);
    }
    // Handle button clicks
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (!gameStarted && startGameText.getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            // Start game when "START GAME" is clicked
            gameStarted = true;
            exitText.setPosition(-1, -1);
        }
        else if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            window.close();
        }
        else if (!gameStarted && resume.getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            resumeG = true;
        }
    }
}
void selecttheme(sf::RenderWindow& window, sf::Font& font, bool& gameStarted)
{
    if (gameStarted) {
        window.clear();
        
        // Set up the START GAME button
        theme.setFont(font);
        theme.setString("T H E M E S");
        theme.setFillColor(sf::Color::White);
        theme.setPosition(window.getSize().x / 2 - theme.getGlobalBounds().width / 2, window.getSize().y / 2 - 20);
        window.draw(theme);
        // Glow hover
        sf::Vector2i mousePosh = sf::Mouse::getPosition(window);

        if (theme.getGlobalBounds().contains(mousePosh.x, mousePosh.y))
        {
            theme.setOutlineColor(sf::Color::Red);
            theme.setOutlineThickness(2.1f);
        }
        else
        {
            theme.setOutlineColor(sf::Color::Transparent);
        }
        // Handle button clicks
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (theme.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                actualgame = true;                               
            }

        }
    }
}
void whichtheme(sf::RenderWindow& window, sf::Font& font, bool& gameStarted, bool& showback)//theme options
{
    // Define global variables for floating animation
    static sf::Text anime;
    static sf::Text batman;
    static bool initialized = false;
    static sf::Clock clock;
    if (!initialized)
    {
        // Set up the START GAME button
        anime.setFont(font);
        anime.setString(">Anime Style");
        anime.setCharacterSize(40);
        anime.setFillColor(sf::Color::White);
        anime.setPosition(50, 50);

        // Set up the START GAME button
        batman.setFont(font);
        batman.setString(">BATMAN VS SUPERMAN\n\twith bot");
        batman.setFillColor(sf::Color::White);
        batman.setPosition(50, 150);

        initialized = true;
    }

    if (actualgame)
    {
        window.clear();

        // Update positions based on sine function for floating effect
        float time = static_cast<float>(clock.getElapsedTime().asMilliseconds());
        float floatOffset = 5.f * std::sin(time / 200.f);

        anime.setPosition(50, 50 + floatOffset);
        batman.setPosition(50, 150 + floatOffset);

        // Draw the animated texts
        window.draw(anime);
        window.draw(batman);

        // Glow hover
        sf::Vector2i mousePosh = sf::Mouse::getPosition(window);

        if (anime.getGlobalBounds().contains(mousePosh.x, mousePosh.y))
        {
            anime.setOutlineColor(sf::Color::Red);
            anime.setOutlineThickness(2.1f);
            sound.play();
        }
        else
        {
            anime.setOutlineColor(sf::Color::Transparent);
        }
        if (batman.getGlobalBounds().contains(mousePosh.x, mousePosh.y))
        {
            batman.setOutlineColor(sf::Color::Red);
            batman.setOutlineThickness(2.1f);
            sound.play();
        }
        else
        {
            batman.setOutlineColor(sf::Color::Transparent);
        }
        //click handle
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (anime.getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                animeselected = true;
                

            }
            else if (batman.getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                batmanselected = true;
                
            }
        }
    }
}
int main()
{
    if (!initializeMusic("GameMusic.ogg")) {
        // Handle initialization error
        return EXIT_FAILURE;
    }

    sf::RenderWindow window(sf::VideoMode(cellsize * col, cellsize * col), "SFML window");
    sf::Text startGameText;
    if (!buffer.loadFromFile("clicksound.ogg"))
    {
        std::cout << "Error buffer" << std::endl;
    }
    
    sound.setBuffer(buffer);

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    bool showback = true;
    bool gameStarted = false;

    // Declare the font variable
    sf::Font font;
    if (!font.loadFromFile("final.ttf"))
    {
        std::cerr << "Error loading font" << std::endl;
        return 1; // Return an error code
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(); // Clear the window only once

        
            if (!backgroundTexture.loadFromFile("bgforthemes.jpg"))
            {
                cerr << "Error loading background texture" << endl;
                return 0;
            }
            
            backgroundSprite.setTexture(backgroundTexture);
            backgroundSprite.setScale(0.4f,0.5f);
            window.draw(backgroundSprite);
        

        drawButtons(window, font, gameStarted, startGameText);
        selecttheme(window, font, gameStarted);
       
        whichtheme(window, font, gameStarted, showback);
        window.display();

        if (animeselected)
        {
            std::thread musicThread(playMusic);
            startGameMadara();
            musicThread.join();
        }
        else if (batmanselected)
        {
            std::thread musicThread1(playMusic);
            startGame();
            musicThread1.join();
        }
    }    
    return 0;
}
 

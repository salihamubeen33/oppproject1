#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const string RESET   = "\033[0m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";
const string BG_DARK = "\033[40m";

// ==========================================
// 1. Player Class (Abstract) 
// ==========================================
class Player {
private:
    string name; 
    int score;   

public:
    Player(string name) : name(name), score(0) {}
    virtual ~Player() {}

    string getName() const { return name; }
    void setName(string name) { this->name = name; }
    
    int getScore() const { return score; }
    void setScore(int score) { this->score = score; }
    void incrementScore() { score++; }
    void resetScore() { score = 0; }
 
    virtual char makeChoice(char opponentsLastMove = ' ') = 0;
};

// ==========================================
// 2. HumanPlayer Class 
// ==========================================
class HumanPlayer : public Player {
public:
    HumanPlayer(string name) : Player(name) {}
    char makeChoice(char opponentsLastMove = ' ') override {
        char choice;
        while (true) {
            cout << CYAN << "Your move (R/P/S): " << RESET;
            cin >> choice;
            choice = toupper(choice);

            if (choice == 'R' || choice == 'P' || choice == 'S') {
                return choice;
            }
            cout << RED << "Invalid choice! Please enter R, P, or S." << RESET << endl;
        }
    }
};

// ==========================================
// 3. ComputerPlayer Class (Abstract) 
// ==========================================
class ComputerPlayer : public Player {
public:
    ComputerPlayer(string name) : Player(name) {}
    virtual ~ComputerPlayer() {}
    virtual char makeChoice(char opponentsLastMove = ' ') override = 0;
};

// ==========================================
// 4. EasyComputer Class 
// ==========================================
class EasyComputer : public ComputerPlayer {
public:
    EasyComputer() : ComputerPlayer("Easy Computer") {}
    char makeChoice(char opponentsLastMove = ' ') override {
        return 'R';
    }
};

// ==========================================
// 5. MediumComputer Class 
// ==========================================
class MediumComputer : public ComputerPlayer {
public:
    MediumComputer() : ComputerPlayer("Medium Computer") {}
    char makeChoice(char opponentsLastMove = ' ') override {
        int randNum = rand() % 3;
        if (randNum == 0) return 'R';
        if (randNum == 1) return 'P';
        return 'S';
    }
};

// ==========================================
// 6. HardComputer Class 
// ==========================================
class HardComputer : public ComputerPlayer {
public:
    HardComputer() : ComputerPlayer("Hard Computer") {}
    char makeChoice(char opponentsLastMove = ' ') override {
        if (opponentsLastMove == ' ') {
            int randNum = rand() % 3;
            if (randNum == 0) return 'R';
            if (randNum == 1) return 'P';
            return 'S';
        }
        if (opponentsLastMove == 'R') return 'P'; 
        if (opponentsLastMove == 'P') return 'S'; 
        return 'R';                                
    }
};

// ==========================================
// 7. Game Class 
// ==========================================
class Game {
private:
    HumanPlayer* human;
    ComputerPlayer* computer;

    string choiceToString(char move) {
        if (move == 'R') return "Rock";
        if (move == 'P') return "Paper";
        return "Scissors";
    }

public:
    Game() : human(nullptr), computer(nullptr) {
        srand(time(0));  
        human = new HumanPlayer("You");
    }

    ~Game() {
        delete human;
        if (computer != nullptr) {
            delete computer;
        }
    }

    void showMenu() {
        cout << BG_DARK << MAGENTA << "=========================================" << RESET << endl;
        cout << BG_DARK << YELLOW << "  ROCK PAPER SCISSORS - STRICT 3 ROUNDS  " << RESET << endl;
        cout << BG_DARK << MAGENTA << "=========================================" << RESET << endl;
        cout << CYAN << "Choose Computer Level:" << RESET << endl;
        cout << "1. Easy (Always Rock)" << endl;
        cout << "2. Medium (Random)" << endl;
        cout << "3. Hard (Beats your last move)" << endl;
    }

    void play() {
        char playAgain = 'Y';

        while (toupper(playAgain) == 'Y') { 
            showMenu();
            
            int choice;
            while (true) {
                cout << CYAN << "Your choice: " << RESET;
                cin >> choice;
                if (cin.fail() || choice < 1 || choice > 3) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << RED << "Invalid choice! Select level 1, 2, or 3." << RESET << endl;
                } else {
                    break;
                }
            }

            if (computer != nullptr) delete computer;
            if (choice == 1) computer = new EasyComputer();
            else if (choice == 2) computer = new MediumComputer();
            else computer = new HardComputer();

            cout << GREEN << "\nLevel selected: " << computer->getName() << RESET << endl;

            human->resetScore();
            computer->resetScore();
            
            char humanLastMove = ' '; 

            // CHANGED: Fixed 3 Rounds Loop
            for (int roundNum = 1; roundNum <= 3; roundNum++) {
                cout << YELLOW << "\nRound " << roundNum << " of 3" << RESET << endl;
                
                char humanMove = human->makeChoice();
                char computerMove = computer->makeChoice(humanLastMove); 

                humanLastMove = humanMove;

                cout << "Computer chose: " << choiceToString(computerMove) << endl;
                if (humanMove == computerMove) {
                    cout << YELLOW << "IT'S A TIE THIS ROUND!" << RESET << endl;
                } 
                else if ((humanMove == 'R' && computerMove == 'S') ||
                         (humanMove == 'P' && computerMove == 'R') ||
                         (humanMove == 'S' && computerMove == 'P')) {
                    cout << GREEN << "YOU WIN THIS ROUND!" << RESET << endl;
                    human->incrementScore();
                } 
                else {
                    cout << RED << "COMPUTER WINS THIS ROUND!" << RESET << endl;
                    computer->incrementScore();
                }

                cout << "Current Round Score: You " << human->getScore() << " - Computer " << computer->getScore() << endl;
            }
      
            // ADDED: Bonus Logic for 2 wins out of 3
            if (human->getScore() >= 2) {
                cout << MAGENTA << "BONUS! You won 2 or more rounds! +1 Bonus Point!" << RESET << endl;
                human->incrementScore();
            }
            if (computer->getScore() >= 2) {
                cout << MAGENTA << "BONUS! Computer won 2 or more rounds! +1 Bonus Point!" << RESET << endl;
                computer->incrementScore();
            }

            // FINAL MATCH RESULT
            cout << "\n=========================================" << endl;
            if (human->getScore() > computer->getScore()) {
                cout << GREEN << "CONGRATULATIONS! YOU WIN THE MATCH " << human->getScore() << "-" << computer->getScore() << "!" << RESET << endl;
            } else if (computer->getScore() > human->getScore()) {
                cout << RED << "GAME OVER! COMPUTER WINS THE MATCH " << computer->getScore() << "-" << human->getScore() << "." << RESET << endl;
            } else {
                cout << YELLOW << "THE MATCH ENDED IN A DRAW!" << RESET << endl;
            }
            cout << "=========================================" << endl;

            cout << CYAN << "Play again? (Y/N): " << RESET;
            cin >> playAgain;
            cout << endl;
        }

        cout << YELLOW << "Thanks for playing!" << RESET << endl; 
    }
};

int main() {
    Game rpsGame;
    rpsGame.play(); 
    return 0;
}
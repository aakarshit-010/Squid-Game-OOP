#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <windows.h>
using namespace std;

// ================= Utility Functions =================
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void pauseScreen(int ms) {
    Sleep(ms);
}

// ================= Base Class =================
class Game {
public:
    virtual void showRules() = 0;
    virtual bool startGame(string playerName) = 0;
    virtual ~Game() {}
};

// ================= Player Class =================
class Player {
private:
    string name;
    bool alive;
public:
    Player(string n) { name = n; alive = true; }
    string getName() { return name; }
    bool isAlive() { return alive; }
    void eliminate() { alive = false; }
};

// ================= Game 1: Red Light Green Light =================
class RedLightGreenLight : public Game {
public:
    void showRules() override {
        cout << "\n--- Red Light Green Light ---\n";
        cout << "Survive 4 steps.\n";
        cout << "Type 'move' to advance, 'stay' to wait.\n";
        cout << "If you move on RED - eliminated!\n\n";
    }

    bool startGame(string playerName) override {
        int finishLine = 4;  
        int position = 0;
        int turns = 0, maxTurns = 15;  

        while (position < finishLine && turns < maxTurns) {
            string light = (rand() % 100 < 70) ? "GREEN" : "RED";

            system("cls");
            cout << "--- Red Light Green Light ---\nTrack: ";
            for (int j = 0; j <= finishLine; j++) {
                if (j == position) cout << "O ";
                else cout << ". ";
            }
            cout << "\n";

            string action;
            do {
                cout << playerName << " - choose action (move/stay): ";
                cin >> action;
                action = toLower(action);
                if (action != "move" && action != "stay") {
                    cout << "Invalid action! Please type 'move' or 'stay' only.\n";
                }
            } while (action != "move" && action != "stay");

            cout << "Light: ";
            if (light == "RED") { setColor(12); cout << "RED\n"; setColor(7); }
            else { setColor(10); cout << "GREEN\n"; setColor(7); }

            if (action == "move") {
                if (light == "RED") {
                    cout << "X " << playerName << " moved on RED! Eliminated.\n";
                    pauseScreen(1500);
                    return false;
                } else {
                    position++;
                    cout << " Safe move! Position: " << position << "/" << finishLine << endl;
                }
            } else {
                cout << playerName << " stayed.\n";
            }

            turns++;
            pauseScreen(1000);
        }

        if (position >= finishLine) {
            cout << "** " << playerName << " reached the finish line! **\n";
            return true;
        }
        cout << " " << playerName << " ran out of time! Eliminated.\n";
        return false;
    }
};

// ================= Game 2: Glass Bridge =================
class GlassBridge : public Game {
public:
    static bool pathInitialized;
    static bool isRightSafe[3];  

    void showRules() override {
        cout << "\n--- Glass Bridge ---\n";
        cout << "Choose LEFT or RIGHT. One is safe, the other breaks!\n\n";
    }

    bool startGame(string playerName) override {
        int steps = 3;
        
        if (!pathInitialized) {
            for (int i = 0; i < steps; i++) {
                isRightSafe[i] = (rand() % 2 == 0);
            }
            pathInitialized = true;
        }

        cout << "\n" << playerName << " starts crossing the bridge...\n";
        for (int i = 0; i < steps; i++) {
            system("cls");
            cout << "--- Glass Bridge ---\n";
            for (int j = 0; j <= i; j++) cout << "    ";
            cout << "O\n"; 
            cout << "[ ?? ]   [ ?? ]\n\n";

            string choice;
            do {
                cout << "Step " << i+1 << " - " << playerName << ", choose (left/right): ";
                cin >> choice;
                choice = toLower(choice);
                if (choice != "left" && choice != "right") {
                    cout << "Invalid choice! Please type 'left' or 'right' only.\n";
                }
            } while (choice != "left" && choice != "right");

            bool choseRight = (choice == "right");
            if (choseRight == isRightSafe[i]) {
                setColor(10); cout << "Safe step!\n"; setColor(7);
                pauseScreen(1000);
            } else {
                setColor(12); cout << "Glass broke! " << playerName << " falls.\n"; setColor(7);
                pauseScreen(1500);
                return false;
            }
        }
        setColor(10); cout << " " << playerName << " crossed the Glass Bridge!\n"; setColor(7);
        return true;
    }
};
bool GlassBridge::pathInitialized = false;
bool GlassBridge::isRightSafe[3];

// ================= Game 3: Tug of War =================
class TugOfWar : public Game {
public:
    void showRules() override {
        cout << "\n--- Tug of War ---\n";
        cout << "Type 'pull' for 5 turns. Each pull adds 1-3 strength.\n";
        cout << "Opponent has random strength. Higher wins!\n\n";
    }

    bool startGame(string playerName) override {
        int playerStrength = 0;
        for (int i = 1; i <= 5; i++) {
            system("cls");
            cout << "--- Tug of War ---\n";
            cout << "|====O----|----O====|\n\n"; 
            string action;
            do {
                cout << "Turn " << i << " - Type 'pull': ";
                cin >> action;
                if (action != "pull") {
                    cout << "Invalid action! Please type 'pull' only.\n";
                }
            } while (action != "pull");
            if (action == "pull") {
                int gain = rand() % 3 + 1;
                playerStrength += gain;
                cout << " +" << gain << " strength (Total = " << playerStrength << ")\n";
            }
            pauseScreen(1000);
        }
        int opponentStrength = rand() % 10 + 8;
        cout << "\nFinal Strength - " << playerName << " = " << playerStrength
             << " | Opponent = " << opponentStrength << endl;

        if (playerStrength >= opponentStrength) {
            setColor(10); cout << " " << playerName << " wins the Tug of War!\n"; setColor(7);
            return true;
        }
        setColor(12); cout << " " << playerName << " lost the Tug of War.\n"; setColor(7);
        return false;
    }
};

// ================= Game Manager =================
class GameManager {
private:
    vector<Player> players;
    vector<Game*> games;
public:
    void addPlayer(string name) { players.push_back(Player(name)); }
    void addGame(Game* g) { games.push_back(g); }

    void start() {
        for (auto g : games) {
            g->showRules();
            pauseScreen(2000);
            for (auto &p : players) {
                if (p.isAlive()) {
                    bool survived = g->startGame(p.getName());
                    if (!survived) p.eliminate();
                }
            }
        }
        cout << "\n=== Final Results ===\n";
        bool winner = false;
        for (auto &p : players) {
            if (p.isAlive()) {
                cout << " " << p.getName() << " survived all games!\n";
                winner = true;
            }
        }
        if (!winner) cout << "No one survived...\n";
    }

    ~GameManager() {
        for (auto g : games) delete g;
    }
};

// ================= Main =================
int main() {
    srand(time(0));

    GameManager manager;

    int n;
    do {
        cout << "Enter number of players (1-10): ";
        cin >> n;
    } while (n < 1 || n > 10);
    for (int i = 0; i < n; i++) {
        string name;
        cout << "Enter player " << i+1 << " name: ";
        cin >> name;
        manager.addPlayer(name);
    }

    manager.addGame(new RedLightGreenLight());
    manager.addGame(new GlassBridge());
    manager.addGame(new TugOfWar());

    manager.start();
    return 0;
}

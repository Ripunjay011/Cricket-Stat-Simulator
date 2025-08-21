#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;

// Abstract base class
class Player
{
protected:
  string name;
  int age;
  int totalMatches;

public:
  Player(string name = "", int age = 0, int matches = 0)
      : name(name), age(age), totalMatches(matches) {}

  virtual void display() const = 0;
  virtual string getType() const = 0;
  virtual void updateStats(int &remainingWickets) = 0;
  virtual string serialize() const = 0;
  string getName() const { return name; }
  virtual ~Player() {}
};

// Batsman class
class Batsman : public Player
{
protected:
  int runs;

public:
  Batsman(string name = "", int age = 0, int matches = 0, int runs = 0)
      : Player(name, age, matches), runs(runs) {}

  void display() const override
  {
    cout << "[Batsman] Name: " << name << ", Age: " << age
         << ", Matches: " << totalMatches << ", Runs: " << runs << endl;
  }

  string getType() const override { return "Batsman"; }

  void updateStats(int &remainingWickets) override
  {
    int matchRuns;
    totalMatches++;
    do
    {
      cout << "Enter runs scored by " << name << ": ";
      cin >> matchRuns;
      if (matchRuns < 0)
        cout << "Runs cannot be negative.\n";
    } while (matchRuns < 0);
    runs += matchRuns;
  }

  string serialize() const override
  {
    return "Batsman," + name + "," + to_string(age) + "," +
           to_string(totalMatches) + "," + to_string(runs);
  }
};

// Bowler class
class Bowler : public Player
{
protected:
  int runs;
  int wickets;

public:
  Bowler(string name = "", int age = 0, int matches = 0, int runs = 0, int wickets = 0)
      : Player(name, age, matches), runs(runs), wickets(wickets) {}

  void display() const override
  {
    cout << "[Bowler] Name: " << name << ", Age: " << age
         << ", Matches: " << totalMatches << ", Runs: " << runs
         << ", Wickets: " << wickets << endl;
  }

  string getType() const override { return "Bowler"; }

  void updateStats(int &remainingWickets) override
  {
    int matchRuns, matchWickets;
    totalMatches++;
    do
    {
      cout << "Enter runs scored by " << name << ": ";
      cin >> matchRuns;
      if (matchRuns < 0)
        cout << "Runs cannot be negative.\n";
    } while (matchRuns < 0);

    do
    {
      cout << "Enter wickets taken by " << name << " [0-" << min(10, remainingWickets) << "]: ";
      cin >> matchWickets;
      if (matchWickets < 0 || matchWickets > 10 || matchWickets > remainingWickets)
        cout << "Invalid. Wickets should be in [0-" << min(10, remainingWickets) << "]\n";
    } while (matchWickets < 0 || matchWickets > 10 || matchWickets > remainingWickets);

    runs += matchRuns;
    wickets += matchWickets;
    remainingWickets -= matchWickets;
  }

  string serialize() const override
  {
    return "Bowler," + name + "," + to_string(age) + "," + to_string(totalMatches) + "," +
           to_string(runs) + "," + to_string(wickets);
  }
};

// AllRounder class
class AllRounder : public Player
{
protected:
  int runs;
  int wickets;

public:
  AllRounder(string name = "", int age = 0, int matches = 0, int runs = 0, int wickets = 0)
      : Player(name, age, matches), runs(runs), wickets(wickets) {}

  void display() const override
  {
    cout << "[AllRounder] Name: " << name << ", Age: " << age
         << ", Matches: " << totalMatches << ", Runs: " << runs
         << ", Wickets: " << wickets << endl;
  }

  string getType() const override { return "AllRounder"; }

  void updateStats(int &remainingWickets) override
  {
    int matchRuns, matchWickets;
    totalMatches++;

    do
    {
      cout << "Enter runs scored by " << name << ": ";
      cin >> matchRuns;
      if (matchRuns < 0)
        cout << "Runs cannot be negative.\n";
    } while (matchRuns < 0);

    do
    {
      cout << "Enter wickets taken by " << name << " [0-" << min(10, remainingWickets) << "]: ";
      cin >> matchWickets;
      if (matchWickets < 0 || matchWickets > 10 || matchWickets > remainingWickets)
        cout << "Invalid. Wickets should be in [0-" << min(10, remainingWickets) << "]\n";
    } while (matchWickets < 0 || matchWickets > 10 || matchWickets > remainingWickets);

    runs += matchRuns;
    wickets += matchWickets;
    remainingWickets -= matchWickets;
  }

  string serialize() const override
  {
    return "AllRounder," + name + "," + to_string(age) + "," + to_string(totalMatches) + "," +
           to_string(runs) + "," + to_string(wickets);
  }
};

// Main system class
class CricketerSystem
{
private:
  vector<Player *> playerList;
  vector<Player *> currentTeam;

  void saveToFile()
  {
    ofstream file("data.txt");
    for (auto &p : playerList)
      file << p->serialize() << endl;
    file.close();
  }

  void saveTeamToFile()
  {
    ofstream file("team.txt");
    for (auto &p : currentTeam)
      file << p->getName() << endl;
    file.close();
  }

  bool isInTeam(const string &name)
  {
    return any_of(currentTeam.begin(), currentTeam.end(),
                  [&](Player *p)
                  { return p->getName() == name; });
  }

  void loadFromFile()
  {
    ifstream file("data.txt");
    string line;
    while (getline(file, line))
    {
      stringstream ss(line);
      string type, name, ageStr, matchesStr, runsStr, wicketsStr;
      getline(ss, type, ','); // ',' is delimiters (Separator)
      getline(ss, name, ',');
      getline(ss, ageStr, ',');
      getline(ss, matchesStr, ',');
      getline(ss, runsStr, ',');
      int age = stoi(ageStr), matches = stoi(matchesStr), runs = stoi(runsStr);
      if (type == "Batsman")
      {
        playerList.push_back(new Batsman(name, age, matches, runs));
      }
      else
      {
        getline(ss, wicketsStr, ',');
        int wickets = stoi(wicketsStr);
        if (type == "Bowler")
          playerList.push_back(new Bowler(name, age, matches, runs, wickets));
        else if (type == "AllRounder")
          playerList.push_back(new AllRounder(name, age, matches, runs, wickets));
      }
    }
    file.close();
  }

  void loadTeamFromFile()
  {
    ifstream file("team.txt");
    string line;
    while (getline(file, line))
    {
      auto it = find_if(playerList.begin(), playerList.end(),
                        [&](Player *p)
                        { return p->getName() == line; });
      if (it != playerList.end())
      {
        currentTeam.push_back(*it);
      }
    }
    file.close();
  }

public:
  CricketerSystem()
  {
    loadFromFile();
    loadTeamFromFile();
  }

  ~CricketerSystem()
  {
    for (auto p : playerList)
      delete p;
  }

  void addPlayer()
  {
    string type, name;
    int age, matches, runs, wickets;

    cin.ignore();
    cout << "Enter type (Batsman/Bowler/AllRounder): ";
    getline(cin, type);
    cout << "Enter full name: ";
    getline(cin, name);

    do
    {
      cout << "Enter age: ";
      cin >> age;
    } while (age < 0);

    do
    {
      cout << "Enter total matches played: ";
      cin >> matches;
    } while (matches < 0);

    if (type == "Batsman")
    {
      do
      {
        cout << "Enter total runs: ";
        cin >> runs;
      } while (runs < 0);
      playerList.push_back(new Batsman(name, age, matches, runs));
    }
    else if (type == "Bowler")
    {
      do
      {
        cout << "Enter runs scored: ";
        cin >> runs;
      } while (runs < 0);
      do
      {
        cout << "Enter total wickets: ";
        cin >> wickets;
      } while (wickets < 0);
      playerList.push_back(new Bowler(name, age, matches, runs, wickets));
    }
    else if (type == "AllRounder")
    {
      do
      {
        cout << "Enter total runs: ";
        cin >> runs;
      } while (runs < 0);
      do
      {
        cout << "Enter total wickets: ";
        cin >> wickets;
      } while (wickets < 0);
      playerList.push_back(new AllRounder(name, age, matches, runs, wickets));
    }
    else
    {
      cout << "Invalid type.\n";
      return;
    }

    saveToFile();
    cout << "Player added.\n";
  }

  void removePlayer()
  {
    if (playerList.size() <= 11)
    {
      cout << "Cannot remove. Minimum 11 players required.\n";
      return;
    }

    cin.ignore();
    string name;
    cout << "Enter player name to remove: ";
    getline(cin, name);
    auto it = find_if(playerList.begin(), playerList.end(),
                      [&](Player *p)
                      { return p->getName() == name; });

    if (it != playerList.end())
    {
      delete *it;
      playerList.erase(it);
      saveToFile();
      cout << "Player removed.\n";
    }
    else
    {
      cout << "Player not found.\n";
    }
  }

  void displayPlayers() const
  {
    cout << "\n--- Batsmen ---\n";
    for (auto p : playerList)
      if (p->getType() == "Batsman")
        p->display();
    cout << "\n--- Bowlers ---\n";
    for (auto p : playerList)
      if (p->getType() == "Bowler")
        p->display();
    cout << "\n--- AllRounders ---\n";
    for (auto p : playerList)
      if (p->getType() == "AllRounder")
        p->display();
  }

  void makeTeam()
  {
    currentTeam.clear();
    cin.ignore();
    cout << "Select 11 players by full name:\n";
    for (int i = 0; i < 11; ++i)
    {
      string name;
      cout << "Player " << (i + 1) << ": ";
      getline(cin, name);
      auto it = find_if(playerList.begin(), playerList.end(),
                        [&](Player *p)
                        { return p->getName() == name; });

      if (it != playerList.end() && !isInTeam(name))
      {
        currentTeam.push_back(*it);
      }
      else
      {
        cout << "Invalid or duplicate player. Try again.\n";
        --i;
      }
    }
    saveTeamToFile();
  }

  void replacePlayerInTeam()
  {
    cin.ignore();
    string name;
    cout << "Enter name of player to replace: ";
    getline(cin, name);

    auto it = find_if(currentTeam.begin(), currentTeam.end(),
                      [&](Player *p)
                      { return p->getName() == name; });

    if (it != currentTeam.end())
    {
      string newName;
      cout << "Enter new player name: ";
      getline(cin, newName);
      auto found = find_if(playerList.begin(), playerList.end(),
                           [&](Player *p)
                           { return p->getName() == newName; });
      if (found != playerList.end() && !isInTeam(newName))
      {
        *it = *found;
        saveTeamToFile();
        cout << "Player replaced successfully.\n";
      }
      else
      {
        cout << "Replacement failed. Player not found or already in team.\n";
      }
    }
    else
    {
      cout << "Player not found in current team.\n";
    }
  }

  void updateTeamStats()
  {
    int remainingWickets = 10;
    for (auto &p : currentTeam)
    {
      cout << "\nUpdating stats for: " << p->getName() << endl;
      p->updateStats(remainingWickets);
    }
    saveToFile();
  }
};

// Entry point
int main()
{
  CricketerSystem system;
  int choice;
  while (true)
  {
    cout << "\n--- Indian Cricketer Career Info System ---\n";
    cout << "1. Display all players\n2. Add player\n3. Remove player\n";
    cout << "4. Make Team\n5. Update Team Stats\n6. Replace Player in Team\n7. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
      system.displayPlayers();
      break;
    case 2:
      system.addPlayer();
      break;
    case 3:
      system.removePlayer();
      break;
    case 4:
      system.makeTeam();
      break;
    case 5:
      system.updateTeamStats();
      break;
    case 6:
      system.replacePlayerInTeam();
      break;
    case 7:
      return 0;
    default:
      cout << "Invalid choice.\n";
    }
  }
}
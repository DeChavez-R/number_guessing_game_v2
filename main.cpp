#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// ============================================================================
// CONSTANTS & ENUMS
// ============================================================================

enum class Difficulty { EASY, MEDIUM, HARD, EXPERT };

constexpr int EASY_MIN = 1;
constexpr int EASY_MAX = 50;
constexpr int EASY_CHANCES = 10;

constexpr int MEDIUM_MIN = 1;
constexpr int MEDIUM_MAX = 100;
constexpr int MEDIUM_CHANCES = 7;

constexpr int HARD_MIN = 1;
constexpr int HARD_MAX = 200;
constexpr int HARD_CHANCES = 6;

constexpr int EXPERT_MIN = 1;
constexpr int EXPERT_MAX = 500;
constexpr int EXPERT_CHANCES = 8;

constexpr int BASE_SCORE = 1000;
constexpr int PERFECT_BONUS = 500;

// ============================================================================
// UTILITY CLASSES
// ============================================================================

class RandomGenerator {
private:
  std::mt19937 engine;

public:
  RandomGenerator() {
    auto seed =
        std::chrono::high_resolution_clock::now().time_since_epoch().count();
    engine.seed(static_cast<unsigned int>(seed));
  }

  int generate(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
  }
};

class InputValidator {
public:
  static int getInt(const std::string &prompt) {
    int value;
    while (true) {
      std::cout << prompt;
      std::cin >> value;

      if (std::cin.fail()) {
        std::cout << "❌ Invalid input! Please enter a valid number."
                  << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      } else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
      }
    }
  }

  static std::string getString(const std::string &prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
  }

  static std::string toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
  }

  static std::string trim(const std::string &str) {
    std::size_t start = str.find_first_not_of(" \t\n\r");
    std::size_t end = str.find_last_not_of(" \t\n\r");

    if (start == std::string::npos || end == std::string::npos) {
      return "";
    }

    return str.substr(start, end - start + 1);
  }
};

// ============================================================================
// PLAYER & SCORE CLASSES
// ============================================================================

struct PlayerScore {
  std::string playerName;
  int score;
  Difficulty difficulty;
  int attempts;
  std::string timestamp;

  // Default constructor
  PlayerScore()
      : playerName(""), score(0), difficulty(Difficulty::EASY), attempts(0),
        timestamp("") {}

  // Parameterized constructor
  PlayerScore(const std::string &name, int s, Difficulty diff, int att)
      : playerName(name), score(s), difficulty(diff), attempts(att) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    timestamp = ss.str();
  }

  std::string getDifficultyString() const {
    switch (difficulty) {
    case Difficulty::EASY:
      return "Easy";
    case Difficulty::MEDIUM:
      return "Medium";
    case Difficulty::HARD:
      return "Hard";
    case Difficulty::EXPERT:
      return "Expert";
    default:
      return "Unknown";
    }
  }

  bool operator>(const PlayerScore &other) const { return score > other.score; }
};

class Leaderboard {
private:
  std::vector<PlayerScore> scores;
  std::string filename;
  const std::size_t MAX_ENTRIES = 10;

public:
  Leaderboard(const std::string &file = "leaderboard.txt") : filename(file) {
    loadFromFile();
  }

  void addScore(const PlayerScore &score) {
    scores.push_back(score);
    std::sort(scores.begin(), scores.end(), std::greater<PlayerScore>());

    // Use erase instead of resize to avoid needing default constructor
    if (scores.size() > MAX_ENTRIES) {
      scores.erase(scores.begin() + MAX_ENTRIES, scores.end());
    }

    saveToFile();
  }

  void display() const {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout
        << "🏆                        LEADERBOARD                        🏆"
        << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    if (scores.empty()) {
      std::cout << "No scores yet! Be the first to play!" << std::endl;
    } else {
      std::cout << std::left << std::setw(5) << "Rank" << std::setw(20)
                << "Player" << std::setw(10) << "Score" << std::setw(12)
                << "Difficulty" << std::setw(10) << "Attempts"
                << "Date" << std::endl;
      std::cout << std::string(80, '-') << std::endl;

      for (std::size_t i = 0; i < scores.size(); i++) {
        std::cout << std::left << std::setw(5) << (i + 1) << std::setw(20)
                  << scores[i].playerName << std::setw(10) << scores[i].score
                  << std::setw(12) << scores[i].getDifficultyString()
                  << std::setw(10) << scores[i].attempts << scores[i].timestamp
                  << std::endl;
      }
    }

    std::cout << std::string(80, '=') << std::endl;
  }

  int getHighScore() const { return scores.empty() ? 0 : scores[0].score; }

private:
  void saveToFile() {
    std::ofstream file(filename);
    if (file.is_open()) {
      for (const auto &score : scores) {
        file << score.playerName << "|" << score.score << "|"
             << static_cast<int>(score.difficulty) << "|" << score.attempts
             << "|" << score.timestamp << "\n";
      }
      file.close();
    }
  }

  void loadFromFile() {
    std::ifstream file(filename);
    if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, timestamp;
        int score, diffInt, attempts;

        std::getline(ss, name, '|');
        ss >> score;
        ss.ignore();
        ss >> diffInt;
        ss.ignore();
        ss >> attempts;
        ss.ignore();
        std::getline(ss, timestamp);

        PlayerScore ps(name, score, static_cast<Difficulty>(diffInt), attempts);
        ps.timestamp = timestamp;
        scores.push_back(ps);
      }
      file.close();

      std::sort(scores.begin(), scores.end(), std::greater<PlayerScore>());
      if (scores.size() > MAX_ENTRIES) {
        scores.erase(scores.begin() + MAX_ENTRIES, scores.end());
      }
    }
  }
};

// ============================================================================
// GAME CONFIGURATION
// ============================================================================

class GameConfig {
public:
  int minRange;
  int maxRange;
  int maxChances;
  Difficulty difficulty;

  GameConfig(Difficulty diff) : difficulty(diff) {
    switch (diff) {
    case Difficulty::EASY:
      minRange = EASY_MIN;
      maxRange = EASY_MAX;
      maxChances = EASY_CHANCES;
      break;
    case Difficulty::MEDIUM:
      minRange = MEDIUM_MIN;
      maxRange = MEDIUM_MAX;
      maxChances = MEDIUM_CHANCES;
      break;
    case Difficulty::HARD:
      minRange = HARD_MIN;
      maxRange = HARD_MAX;
      maxChances = HARD_CHANCES;
      break;
    case Difficulty::EXPERT:
      minRange = EXPERT_MIN;
      maxRange = EXPERT_MAX;
      maxChances = EXPERT_CHANCES;
      break;
    }
  }

  std::string getDifficultyName() const {
    switch (difficulty) {
    case Difficulty::EASY:
      return "Easy";
    case Difficulty::MEDIUM:
      return "Medium";
    case Difficulty::HARD:
      return "Hard";
    case Difficulty::EXPERT:
      return "Expert";
    default:
      return "Unknown";
    }
  }

  void display() const {
    std::cout << "📊 Difficulty: " << getDifficultyName() << std::endl;
    std::cout << "🎯 Range: " << minRange << " - " << maxRange << std::endl;
    std::cout << "💪 Max Chances: " << maxChances << std::endl;
  }
};

// ============================================================================
// GAME STATE & STATISTICS
// ============================================================================

class GameStatistics {
private:
  int gamesPlayed;
  int gamesWon;
  int totalAttempts;
  int bestScore;

public:
  GameStatistics()
      : gamesPlayed(0), gamesWon(0), totalAttempts(0), bestScore(0) {}

  void recordGame(bool won, int attempts, int score) {
    gamesPlayed++;
    if (won) {
      gamesWon++;
      totalAttempts += attempts;
      if (score > bestScore) {
        bestScore = score;
      }
    }
  }

  void display() const {
    std::cout << "\n📈 Session Statistics:" << std::endl;
    std::cout << "   Games Played: " << gamesPlayed << std::endl;
    std::cout << "   Games Won: " << gamesWon << std::endl;
    if (gamesPlayed > 0) {
      double winRate = (static_cast<double>(gamesWon) / gamesPlayed) * 100;
      std::cout << "   Win Rate: " << std::fixed << std::setprecision(1)
                << winRate << "%" << std::endl;
    }
    if (gamesWon > 0) {
      double avgAttempts = static_cast<double>(totalAttempts) / gamesWon;
      std::cout << "   Average Attempts (Wins): " << std::fixed
                << std::setprecision(2) << avgAttempts << std::endl;
    }
    std::cout << "   Best Score: " << bestScore << std::endl;
  }
};

// ============================================================================
// CORE GAME ENGINE
// ============================================================================

class GuessingGame {
private:
  std::unique_ptr<GameConfig> config;
  std::unique_ptr<RandomGenerator> rng;
  int targetNumber;
  int remainingChances;
  int attemptsMade;
  bool gameWon;
  std::string playerName;

public:
  GuessingGame(Difficulty difficulty, const std::string &name)
      : config(std::make_unique<GameConfig>(difficulty)),
        rng(std::make_unique<RandomGenerator>()), remainingChances(0),
        attemptsMade(0), gameWon(false), playerName(name) {
    initialize();
  }

  void initialize() {
    targetNumber = rng->generate(config->minRange, config->maxRange);
    remainingChances = config->maxChances;
    attemptsMade = 0;
    gameWon = false;
  }

  void displayGameInfo() const {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "🎮 Player: " << playerName << std::endl;
    config->display();
    std::cout << std::string(60, '=') << std::endl;
  }

  bool playRound() {
    std::cout << "\n🎲 Game started! Guess the number between "
              << config->minRange << " and " << config->maxRange << std::endl;

    while (remainingChances > 0) {
      std::cout << "\n💡 Chances left: " << remainingChances << std::endl;

      std::stringstream prompt;
      prompt << "Choose a number (" << config->minRange << "-"
             << config->maxRange << "): ";
      int guess = InputValidator::getInt(prompt.str());

      attemptsMade++;

      if (guess < config->minRange || guess > config->maxRange) {
        std::cout << "⚠️  Number out of range! Please stay within "
                  << config->minRange << "-" << config->maxRange << std::endl;
        continue;
      }

      if (guess > targetNumber) {
        std::cout << "📈 Too high!" << std::endl;
        provideHint(guess);
      } else if (guess < targetNumber) {
        std::cout << "📉 Too low!" << std::endl;
        provideHint(guess);
      } else {
        gameWon = true;
        std::cout << "\n✅ Congratulations! You guessed it right! 🎉"
                  << std::endl;
        return true;
      }

      remainingChances--;
    }

    std::cout << "\n❌ Game Over! The number was " << targetNumber << std::endl;
    return false;
  }

  int calculateScore() const {
    if (!gameWon)
      return 0;

    int score = BASE_SCORE;

    // Bonus for fewer attempts
    int attemptsBonus = (config->maxChances - attemptsMade) * 100;
    score += attemptsBonus;

    // Perfect game bonus
    if (attemptsMade == 1) {
      score += PERFECT_BONUS;
    }

    // Difficulty multiplier
    double difficultyMultiplier = 1.0;
    switch (config->difficulty) {
    case Difficulty::EASY:
      difficultyMultiplier = 1.0;
      break;
    case Difficulty::MEDIUM:
      difficultyMultiplier = 1.5;
      break;
    case Difficulty::HARD:
      difficultyMultiplier = 2.0;
      break;
    case Difficulty::EXPERT:
      difficultyMultiplier = 2.5;
      break;
    }

    score = static_cast<int>(score * difficultyMultiplier);

    return score;
  }

  void displayScore() const {
    int score = calculateScore();
    std::cout << "\n" << std::string(50, '-') << std::endl;
    std::cout << "🏆 SCORE BREAKDOWN" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Base Score: " << BASE_SCORE << std::endl;
    std::cout << "Attempts Used: " << attemptsMade << " / "
              << config->maxChances << std::endl;

    if (gameWon) {
      int attemptsBonus = (config->maxChances - attemptsMade) * 100;
      std::cout << "Efficiency Bonus: +" << attemptsBonus << std::endl;

      if (attemptsMade == 1) {
        std::cout << "Perfect Game Bonus: +" << PERFECT_BONUS << " 🌟"
                  << std::endl;
      }

      double diffMult = 1.0;
      switch (config->difficulty) {
      case Difficulty::EASY:
        diffMult = 1.0;
        break;
      case Difficulty::MEDIUM:
        diffMult = 1.5;
        break;
      case Difficulty::HARD:
        diffMult = 2.0;
        break;
      case Difficulty::EXPERT:
        diffMult = 2.5;
        break;
      }
      std::cout << "Difficulty Multiplier: x" << std::fixed
                << std::setprecision(1) << diffMult << std::endl;
    }

    std::cout << std::string(50, '-') << std::endl;
    std::cout << "FINAL SCORE: " << score << " 🎯" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
  }

  int getAttempts() const { return attemptsMade; }
  bool isGameWon() const { return gameWon; }
  Difficulty getDifficulty() const { return config->difficulty; }
  std::string getPlayerName() const { return playerName; }

private:
  void provideHint(int guess) const {
    int difference = std::abs(guess - targetNumber);
    int range = config->maxRange - config->minRange;

    if (difference <= range * 0.05) {
      std::cout << "🔥 You're very close!" << std::endl;
    } else if (difference <= range * 0.15) {
      std::cout << "🌡️  Getting warmer..." << std::endl;
    } else if (difference <= range * 0.30) {
      std::cout << "❄️  Getting colder..." << std::endl;
    }
  }
};

// ============================================================================
// GAME ENGINE / FRAMEWORK
// ============================================================================

class GameEngine {
private:
  std::unique_ptr<Leaderboard> leaderboard;
  std::unique_ptr<GameStatistics> stats;
  std::string currentPlayer;

public:
  GameEngine()
      : leaderboard(std::make_unique<Leaderboard>()),
        stats(std::make_unique<GameStatistics>()) {
    currentPlayer = "Guest";
  }

  void run() {
    displayWelcome();
    currentPlayer = InputValidator::getString("Enter your name: ");

    if (currentPlayer.empty()) {
      currentPlayer = "Guest";
    }

    bool running = true;
    while (running) {
      displayMainMenu();
      int choice = InputValidator::getInt("Enter your choice: ");

      switch (choice) {
      case 1:
        playGame();
        break;
      case 2:
        leaderboard->display();
        break;
      case 3:
        stats->display();
        break;
      case 4:
        displayInstructions();
        break;
      case 5:
        std::cout << "\n👋 Thanks for playing! Goodbye, " << currentPlayer
                  << "!" << std::endl;
        running = false;
        break;
      default:
        std::cout << "❌ Invalid choice! Please select 1-5." << std::endl;
      }

      if (running && choice != 5) {
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
      }
    }
  }

private:
  void displayWelcome() const {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << R"(
    ╔═══════════════════════════════════════════════════════════════╗
    ║                                                               ║
    ║        🎮  NUMBER GUESSING GAME ENGINE v2.0  🎮               ║
    ║                                                               ║
    ║              Test your guessing skills!                       ║
    ║                                                               ║
    ╚═══════════════════════════════════════════════════════════════╝
)" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
  }

  void displayMainMenu() const {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "               MAIN MENU" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "1. 🎮 Play Game" << std::endl;
    std::cout << "2. 🏆 View Leaderboard" << std::endl;
    std::cout << "3. 📊 View Session Statistics" << std::endl;
    std::cout << "4. ℹ️  Instructions" << std::endl;
    std::cout << "5. 🚪 Exit" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
  }

  void displayInstructions() const {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "                 INSTRUCTIONS" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "\n📖 How to Play:" << std::endl;
    std::cout << "   • Choose a difficulty level" << std::endl;
    std::cout << "   • The computer picks a random number" << std::endl;
    std::cout << "   • Guess the number within the given chances" << std::endl;
    std::cout << "   • Get hints: 'Too High' or 'Too Low'" << std::endl;
    std::cout << "   • Win by guessing correctly!" << std::endl;

    std::cout << "\n🎯 Difficulty Levels:" << std::endl;
    std::cout << "   Easy   : 1-50   (10 chances)" << std::endl;
    std::cout << "   Medium : 1-100  (7 chances)" << std::endl;
    std::cout << "   Hard   : 1-200  (6 chances)" << std::endl;
    std::cout << "   Expert : 1-500  (8 chances)" << std::endl;

    std::cout << "\n🏆 Scoring System:" << std::endl;
    std::cout << "   • Base Score: 1000 points" << std::endl;
    std::cout << "   • Efficiency Bonus: +100 per unused chance" << std::endl;
    std::cout << "   • Perfect Game: +500 (guess in 1 attempt)" << std::endl;
    std::cout << "   • Difficulty Multipliers:" << std::endl;
    std::cout << "     - Easy: x1.0" << std::endl;
    std::cout << "     - Medium: x1.5" << std::endl;
    std::cout << "     - Hard: x2.0" << std::endl;
    std::cout << "     - Expert: x2.5" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
  }

  Difficulty selectDifficulty() const {
    std::cout << "\n📊 SELECT DIFFICULTY LEVEL:" << std::endl;
    std::cout << "1. 🟢 Easy   (1-50, 10 chances)" << std::endl;
    std::cout << "2. 🟡 Medium (1-100, 7 chances)" << std::endl;
    std::cout << "3. 🔴 Hard   (1-200, 6 chances)" << std::endl;
    std::cout << "4. 💀 Expert (1-500, 8 chances)" << std::endl;

    int choice = InputValidator::getInt("Choose difficulty (1-4): ");

    switch (choice) {
    case 1:
      return Difficulty::EASY;
    case 2:
      return Difficulty::MEDIUM;
    case 3:
      return Difficulty::HARD;
    case 4:
      return Difficulty::EXPERT;
    default:
      std::cout << "Invalid choice! Defaulting to Medium." << std::endl;
      return Difficulty::MEDIUM;
    }
  }

  void playGame() {
    Difficulty difficulty = selectDifficulty();

    auto game = std::make_unique<GuessingGame>(difficulty, currentPlayer);
    game->displayGameInfo();

    bool won = game->playRound();
    game->displayScore();

    int score = game->calculateScore();
    stats->recordGame(won, game->getAttempts(), score);

    if (won) {
      PlayerScore playerScore(game->getPlayerName(), score,
                              game->getDifficulty(), game->getAttempts());
      leaderboard->addScore(playerScore);

      int highScore = leaderboard->getHighScore();
      if (score == highScore) {
        std::cout << "\n🎊 NEW HIGH SCORE! You're #1 on the leaderboard! 🎊"
                  << std::endl;
      }
    }
  }
};

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================

int main() {
  try {
    auto engine = std::make_unique<GameEngine>();
    engine->run();
  } catch (const std::exception &e) {
    std::cerr << "❌ Fatal Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

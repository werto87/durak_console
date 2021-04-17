#include "src/card.hxx"
#include "src/game.hxx"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
//#include <magic_enum.hpp>
#include "src/print.hxx"
#include <map>
#include <random>
#include <string>
#include <sys/types.h>
#include <vector>
int
main ()
{

  int playerCount = 0;
  std::cout << "Input Player Count: ";
  std::cin >> playerCount;
  std::cout << "Player Count: " << playerCount << std::endl;
  auto playerNames = std::vector<std::string>{};
  for (int i = 0; i < playerCount; i++)
    {
      auto playerName = std::string{};
      std::cout << "Input Player Name: ";
      std::cin >> playerName;
      std::cout << "Player Name: " << playerName << std::endl;
      playerNames.emplace_back (std::move (playerName));
    }
  auto playerSelection = std::string{};
  auto game = Game{ std::move (playerNames) };
  while (not game.checkIfGameIsOver ())
    {
      if (game.getAttackStarted ())
        {
          std::cout << tableAsString (game);
        }
      std::cout << attackingPlayerWithNameAndCardIndexValueAndType (game);
      std::cout << "Attacking Player select Card by index to attack: ";
      std::cin >> playerSelection;
      if (game.getAttackStarted ())
        {
          game.playerAssists (PlayerRole::attack, { std::stoull (playerSelection) });
        }
      else
        {
          game.playerStartsAttack ({ std::stoull (playerSelection) });
        }

      std::cout << tableAsString (game);
      std::cout << defendingPlayerWithNameAndCardIndexValueAndType (game);
      std::cout << "Defending Player select Card to beat by index to defend or type 'd' to draw the cards from table: ";
      std::cin >> playerSelection;
      if (playerSelection.at (0) == 'd')
        {
          game.defendingPlayerTakesAllCardsFromTheTable ();
        }
      else
        {
          auto cardToBeatOnTableIndex = std::stoull (playerSelection);
          std::cout << "Defending Player select Card to beat with by index to defend: ";
          std::cin >> playerSelection;
          game.playerDefends (cardToBeatOnTableIndex, game.getDefendingPlayer ().getCards ().at (std::stoull (playerSelection)));
        }
    }
  return 0;
}

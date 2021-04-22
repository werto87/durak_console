#include "durak/card.hxx"
#include "durak/game.hxx"
#include "durak/print.hxx"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <string>
#include <sys/types.h>
#include <vector>
int
main ()
{
  using namespace durak;
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
          std::cout << attackingPlayerWithNameAndCardIndexValueAndType (game);
          std::cout << "Attacking Player select Card to support or 'p' to pass: ";
          std::cin >> playerSelection;
          if (playerSelection.at (0) == 'p')
            {
              if (game.pass (PlayerRole::attack))
                {
                  // if pass was successfull round is over
                  continue;
                }
            }
          else
            {
              game.playerAssists (PlayerRole::attack, sortedCardIndexing (game.getAttackingPlayer ().getCards (), { std::stoull (playerSelection) }));
            }
        }
      else
        {
          std::cout << attackingPlayerWithNameAndCardIndexValueAndType (game);
          std::cout << "Attacking Player select Card by index to attack: ";
          std::cin >> playerSelection;
          game.playerStartsAttack (sortedCardIndexing (game.getAttackingPlayer ().getCards (), { std::stoull (playerSelection) }));
        }
      if (game.getPlayers ().size () >= 3)
        {
          std::cout << tableAsString (game);
          std::cout << assistingPlayerWithNameAndCardIndexValueAndType (game);
          std::cout << "Assisting Player select Card to support or 'p' to pass: ";
          std::cin >> playerSelection;
          if (playerSelection.at (0) == 'p')
            {
              if (game.pass (PlayerRole::assistAttacker))
                {
                  // if pass was successfull round is over
                  continue;
                }
            }
          else
            {
              game.playerAssists (PlayerRole::assistAttacker, sortedCardIndexing (game.getAssistingPlayer ().getCards (), { std::stoull (playerSelection) }));
            }
        }
      std::cout << tableAsString (game);
      std::cout << defendingPlayerWithNameAndCardIndexValueAndType (game);
      std::cout << "Defending Player select Card to beat by index to defend or type 'd' to draw the cards from table: ";
      std::cin >> playerSelection;
      if (playerSelection.at (0) == 'd')
        {
          std::cout << tableAsString (game);
          std::cout << attackingPlayerWithNameAndCardIndexValueAndType (game);
          std::cout << "Defending Player takes Cards from table you can add cards with index or press 'p' to pass: ";
          std::cin >> playerSelection;
          if (playerSelection.at (0) == 'p')
            {
              game.pass (PlayerRole::attack);
            }
          else
            {
              game.playerAssists (PlayerRole::attack, sortedCardIndexing (game.getAttackingPlayer ().getCards (), { std::stoull (playerSelection) }));
            }
          if (game.getPlayers ().size () >= 3)
            {
              std::cout << tableAsString (game);
              std::cout << assistingPlayerWithNameAndCardIndexValueAndType (game);
              std::cout << "Defending Player takes Cards from table you can add cards with index or press 'p' to pass: ";
              std::cin >> playerSelection;
              if (playerSelection.at (0) == 'p')
                {
                  game.pass (PlayerRole::assistAttacker);
                }
              else
                {
                  game.playerAssists (PlayerRole::assistAttacker, sortedCardIndexing (game.getAssistingPlayer ().getCards (), { std::stoull (playerSelection) }));
                }
            }
          game.defendingPlayerTakesAllCardsFromTheTable ();
        }
      else
        {
          auto cardToBeatOnTableIndex = std::stoull (playerSelection);
          std::cout << "Defending Player select Card to beat with by index to defend: ";
          std::cin >> playerSelection;
          game.playerDefends (cardToBeatOnTableIndex, sortedCardIndexing (game.getDefendingPlayer ().getCards (), { std::stoull (playerSelection) }).at (0));
        }
    }
  if (auto durak = game.durak ())
    {
      std::cout << "durak: " << durak.value ().id << std::endl;
    }
  else
    {
      std::cout << "tie" << std::endl;
    }
  return 0;
}

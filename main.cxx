#include "src/card.hxx"
#include "src/game.hxx"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
//#include <magic_enum.hpp>
#include <map>
#include <random>
#include <string>
#include <sys/types.h>
#include <vector>
int
main ()
{
  auto game = Game{ 2, std::vector<Card>{ { 7, Type::clubs }, { 8, Type::clubs }, { 3, Type::hearts }, { 3, Type::clubs }, { 2, Type::diamonds }, { 3, Type::diamonds }, { 2, Type::clubs }, { 5, Type::diamonds }, { 6, Type::diamonds }, { 7, Type::diamonds }, { 8, Type::diamonds }, { 9, Type::diamonds }, { 1, Type::spades }, { 2, Type::spades }, { 3, Type::spades }, { 1, Type::diamonds }, { 5, Type::spades }, { 6, Type::spades }, { 7, Type::spades }, { 8, Type::spades }, { 9, Type::spades }, { 1, Type::hearts }, { 2, Type::hearts }, { 9, Type::clubs }, { 1, Type::clubs }, { 5, Type::hearts }, { 6, Type::clubs }, { 7, Type::hearts }, { 8, Type::hearts }, { 9, Type::hearts }, { 4, Type::hearts }, { 4, Type::diamonds }, { 4, Type::spades }, { 4, Type::clubs }, { 5, Type::clubs }, { 6, Type::hearts } } };
  game.playerStartsAttack ({ 2 });
  game.countOfNotBeatenCardsOnTable ();
  game.playerDefends (0, game.getDefendingPlayer ().getCards ().at (3));
  game.countOfNotBeatenCardsOnTable ();
  game.pass (PlayerRole::attack);
  game.pass (PlayerRole::assistAttacker);
  game.getTable ().size ();
  game.getRound ();
  return 0;
}
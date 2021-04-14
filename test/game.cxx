#include "src/game.hxx"
#include "src/card.hxx"
#include <algorithm>
#include <array>
#include <catch2/catch.hpp>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sys/types.h>

TEST_CASE ("at the start of the game the players have 6 cards", "[game]")
{
  auto game = Game{ 2 };
  auto &player1 = game.getPlayers ().at (0);
  auto &player2 = game.getPlayers ().at (1);
  REQUIRE (player1.getCards ().size () == 6);
  REQUIRE (player2.getCards ().size () == 6);
}

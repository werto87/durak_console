#include "src/game.hxx"
#include "src/card.hxx"
#include "test/constant.hxx"
#include <algorithm>
#include <array>
#include <catch2/catch.hpp>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <pipes/pipes.hpp>
#include <range/v3/range.hpp>
#include <sys/types.h>
#include <tuple>
#include <utility>
#include <vector>
namespace test
{

TEST_CASE ("at the start of the game the players have 6 cards", "[game]")
{
  auto game = Game{ 2 };
  auto &player1 = game.getPlayers ().at (0);
  auto &player2 = game.getPlayers ().at (1);
  REQUIRE (player1.getCards ().size () == 6);
  REQUIRE (player2.getCards ().size () == 6);
}

TEST_CASE ("player starts attack", "[game]")
{
  auto game = Game{ 2, testCardDeck () };
  REQUIRE (game.getPlayers ().at (static_cast<size_t> (PlayerRole::attack)).getCards ().size () == 6);
  REQUIRE (game.playerStartsAttack ({ 2, 3, 4 }));
  REQUIRE (game.getPlayers ().at (static_cast<size_t> (PlayerRole::attack)).getCards ().size () == 3);
}

TEST_CASE ("player assists attack", "[game]")
{
  auto game = Game{ 2, testCardDeck () };
  auto cards = game.getPlayers ().at (static_cast<size_t> (PlayerRole::attack)).getCards ();
  REQUIRE (cards.size () == 6);
  game.playerStartsAttack ({ 2, 3 });
  cards = game.getPlayers ().at (static_cast<size_t> (PlayerRole::attack)).getCards ();
  REQUIRE (cards.size () == 4);
  REQUIRE (game.playerAssists (PlayerRole::attack, { 2, 3 }));
  cards = game.getPlayers ().at (static_cast<size_t> (PlayerRole::attack)).getCards ();
  REQUIRE (cards.size () == 2);
}

TEST_CASE ("cardsNotBeatenOnTable empty table", "[game]")
{
  auto game = Game{ 2, testCardDeck () };
  REQUIRE (game.countOfNotBeatenCardsOnTable () == 0);
}

TEST_CASE ("countOfNotBeatenCardsOnTable table with two cards which are not beaten", "[game]")
{
  auto game = Game{ 2, testCardDeck () };
  game.playerStartsAttack ({ 2, 3 });
  REQUIRE (game.countOfNotBeatenCardsOnTable () == 2);
}

TEST_CASE ("cardsNotBeatenOnTableWithIndex table with two cards which are not beaten", "[game]")
{
  auto game = Game{ 2, testCardDeck () };
  game.playerStartsAttack ({ 2, 3 });
  REQUIRE (game.cardsNotBeatenOnTableWithIndex ().size () == 2);
  auto [cardIndex, card] = game.cardsNotBeatenOnTableWithIndex ().at (0);
  REQUIRE (cardIndex == 0);
  REQUIRE (card.value == 4);
  REQUIRE (card.type == Type::clubs);
}

TEST_CASE ("playerDefends player beats one card of two cards on the table", "[game]")
{
  auto game = Game{ 2, testCardDeck () };
  game.playerStartsAttack ({ 2, 3 });
  REQUIRE (game.playerDefends (0, game.getPlayers ().at (static_cast<size_t> (PlayerRole::defend)).getCards ().at (3)));
}

TEST_CASE ("playerDefends value to low", "[game]")
{
  auto game = Game{ 2, testCardDeck () };
  game.playerStartsAttack ({ 2, 3 });
  REQUIRE_FALSE (game.playerDefends (0, game.getPlayers ().at (static_cast<size_t> (PlayerRole::defend)).getCards ().at (5)));
}

}

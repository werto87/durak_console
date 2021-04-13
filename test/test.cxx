#include "src/card.hxx"
#include <algorithm>
#include <catch2/catch.hpp>
#include <cstddef>
#include <sys/types.h>
namespace test
{

std::optional<Card>
drawSpecificCard (std::vector<Card> &cardDeck, Card const &cardToDraw)
{
  auto result = std::optional<Card>{};
  // try to find and if found return it and remove it from cardDeck
  auto card = std::find (cardDeck.begin (), cardDeck.end (), cardToDraw);
  if (card != cardDeck.end ())
    {
      result = std::move (*card);
      cardDeck.erase (card);
    }
  return result;
}

std::vector<Card>
generateCardDeck ()
{
  const size_t cardValueMax = 9;
  std::vector<Card> cardDeck{};
  for (u_int16_t type = 0; type <= 3; type++)
    {
      for (u_int16_t cardValue = 1; cardValue <= cardValueMax; cardValue++)
        {
          cardDeck.push_back (Card{ .value = cardValue, .type = static_cast<Type> (type) });
        }
    }
  static std::random_device rd;
  static std::mt19937 g (rd ());
  std::shuffle (cardDeck.begin (), cardDeck.end (), g);
  return cardDeck;
}

std::optional<Card>
drawCard (std::vector<Card> &cardDeck)
{
  std::optional<Card> card{};
  if (!cardDeck.empty ())
    {
      card = std::move (cardDeck.back ());
      cardDeck.pop_back ();
    }
  return card;
}

struct Player
{
public:
  void
  putCards (std::vector<Card> const &cardsToPut, std::vector<Card> &target)
  {
    auto cardsToMove = std::stable_partition (cards.begin (), cards.end (), [&cardsToPut] (Card const &card) { return std::find (cardsToPut.begin (), cardsToPut.end (), card) == cardsToPut.end (); });
    std::move (cardsToMove, cards.end (), std::back_inserter (target));
    cards.erase (cardsToMove, cards.end ());
  }

  void
  takeCards (std::vector<Card> &&cardsToTake)
  {
    std::move (cardsToTake.begin (), cardsToTake.end (), std::back_inserter (cards));
    cardsToTake.clear ();
  }

  std::vector<Card> const &
  getCards () const
  {
    return cards;
  }

private:
  std::vector<Card> cards{};
};

class Game
{
public:
  Game (u_int16_t playerCount) : cardDeck{ generateCardDeck () }, players (playerCount)
  {
    std::for_each (players.begin (), players.end (), [this] (Player &player) { playerDrawsCardsFromDeck (player, 6); });
  }

  void
  playerDrawsCardsFromDeck (Player &player, u_int16_t numberOfCards)
  {
    auto result = std::vector<Card>{};
    for (size_t i = 0; i < numberOfCards; i++)
      {
        auto card = drawCard (cardDeck);
        if (card)
          {
            result.push_back (card.value ());
          }
        else
          {
            break;
          }
      }
    player.takeCards (std::move (result));
  }

  void
  playerDrawsCardsFromTable (Player &player)
  {
    player.takeCards (std::move (table));
  }

  void
  playerPutsCardsOnTable (Player &player, std::vector<Card> const &cards)
  {
    player.putCards (cards, table);
  }
  std::vector<Player> const &
  getPlayers ()
  {
    return players;
  }

private:
  std::vector<Card> cardDeck{};
  std::vector<Player> players{};
  std::vector<Card> table{};
};

TEST_CASE ("smallest card value can not beat a card", "[beats]")
{
  auto cardDeck = generateCardDeck ();
  auto cardToCompare = drawSpecificCard (cardDeck, Card{ .value = 1, .type = Type::clubs });
  auto cardsBeaten = 0;
  while (auto card = drawCard (cardDeck))
    {
      if (beats (card.value (), cardToCompare.value (), Type::diamonds))
        {
          cardsBeaten++;
        }
    }
  REQUIRE (cardsBeaten == 0);
  REQUIRE (cardDeck.empty ());
}

TEST_CASE ("smallest card gets beaten by  cards with same symbol or trump", "[beats]")
{
  auto cardDeck = generateCardDeck ();
  auto cardToCompare = drawSpecificCard (cardDeck, Card{ .value = 1, .type = Type::clubs });
  auto cardsBeaten = 0;
  while (auto card = drawCard (cardDeck))
    {
      if (beats (cardToCompare.value (), card.value (), Type::diamonds))
        {
          cardsBeaten++;
        }
    }
  REQUIRE (cardsBeaten == 17);
  REQUIRE (cardDeck.empty ());
}

TEST_CASE ("highest card value and not trump beats every card with same symbol", "[beats]")
{
  auto cardDeck = generateCardDeck ();
  auto cardToCompare = drawSpecificCard (cardDeck, Card{ .value = 9, .type = Type::clubs });
  auto cardsBeaten = 0;
  while (auto card = drawCard (cardDeck))
    {
      if (beats (card.value (), cardToCompare.value (), Type::diamonds))
        {
          cardsBeaten++;
        }
    }
  REQUIRE (cardsBeaten == 8);
  REQUIRE (cardDeck.empty ());
}

TEST_CASE ("highest card value and not trump gets only beaten by trump", "[beats]")
{
  auto cardDeck = generateCardDeck ();
  auto cardToCompare = drawSpecificCard (cardDeck, Card{ .value = 9, .type = Type::clubs });
  auto cardsBeaten = 0;
  while (auto card = drawCard (cardDeck))
    {
      if (beats (cardToCompare.value (), card.value (), Type::diamonds))
        {
          cardsBeaten++;
        }
    }
  REQUIRE (cardsBeaten == 9);
  REQUIRE (cardDeck.empty ());
}

TEST_CASE ("highest card and trump beats every card", "[beats]")
{
  auto cardDeck = generateCardDeck ();
  auto cardToCompare = drawSpecificCard (cardDeck, Card{ .value = 9, .type = Type::clubs });
  auto cardsBeaten = 0;
  while (auto card = drawCard (cardDeck))
    {
      if (beats (card.value (), cardToCompare.value (), Type::clubs))
        {
          cardsBeaten++;
        }
    }
  REQUIRE (cardsBeaten == 35);
  REQUIRE (cardDeck.empty ());
}

TEST_CASE ("highest card and trump cant be beaten", "[beats]")
{
  auto cardDeck = generateCardDeck ();
  auto cardToCompare = drawSpecificCard (cardDeck, Card{ .value = 9, .type = Type::clubs });
  auto cardsBeaten = 0;
  while (auto card = drawCard (cardDeck))
    {
      if (beats (cardToCompare.value (), card.value (), Type::clubs))
        {
          cardsBeaten++;
        }
    }
  REQUIRE (cardsBeaten == 0);
  REQUIRE (cardDeck.empty ());
}

TEST_CASE ("lowest trump card and  beats every card which is not a trump", "[beats]")
{
  auto cardDeck = generateCardDeck ();
  auto cardToCompare = drawSpecificCard (cardDeck, Card{ .value = 1, .type = Type::clubs });
  auto cardsBeaten = 0;
  while (auto card = drawCard (cardDeck))
    {
      if (beats (card.value (), cardToCompare.value (), Type::clubs))
        {
          cardsBeaten++;
        }
    }
  REQUIRE (cardsBeaten == 27);
  REQUIRE (cardDeck.empty ());
}

TEST_CASE ("lowest card and trump gets only beaten by other trump cards", "[beats]")
{
  auto cardDeck = generateCardDeck ();
  auto cardToCompare = drawSpecificCard (cardDeck, Card{ .value = 1, .type = Type::clubs });
  auto cardsBeaten = 0;
  while (auto card = drawCard (cardDeck))
    {
      if (beats (cardToCompare.value (), card.value (), Type::clubs))
        {
          cardsBeaten++;
        }
    }
  REQUIRE (cardsBeaten == 8);
  REQUIRE (cardDeck.empty ());
}

TEST_CASE ("at the start of the game the players have 6 cards", "[game]")
{
  auto game = Game{ 2 };
  REQUIRE (game.getPlayers ().at (0).getCards ().size () == 6);
  REQUIRE (game.getPlayers ().at (1).getCards ().size () == 6);
}

}

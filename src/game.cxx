#include "src/game.hxx"
#include "src/card.hxx"
#include "src/player.hxx"
#include <algorithm>
#include <random>
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

Game::Game (u_int16_t playerCount) : cardDeck{ generateCardDeck () }, players (playerCount)
{
  trump = cardDeck.back ().type;
  std::for_each (players.begin (), players.end (), [this] (Player &player) { playerDrawsCardsFromDeck (player, 6); });
}

void
Game::pass (u_int16_t player)
{
  if (player == attack)
    {
      attackingPlayerPass = true;
    }
  else if (player == assistAttacker)
    {
      assistingPlayerPass = true;
    }
  if (attackingPlayerPass && assistingPlayerPass && players.at (defend).getCards ().empty ())
    {
      nextRound (false);
    }
}

void
Game::rewokePass (u_int16_t player)
{
  if (player == attack)
    {
      attackingPlayerPass = false;
    }
  else if (player == assistAttacker)
    {
      assistingPlayerPass = false;
    }
}

bool
Game::playerStartsAttack (std::vector<size_t> const &index)
{
  auto cards = players.at (attack).getCards ();
  cards.erase (std::remove_if (cards.begin (), cards.end (), [i = int{}, index] (auto) mutable {
    auto test = std::find (index.begin (), index.end (), i);
    i++;
    return test == index.end ();
  }));
  auto valueToCompare = cards.front ().value;
  if (std::find_if (cards.begin (), cards.end (), [valueToCompare] (Card const &card) { return valueToCompare != card.value; }) == cards.end ())
    {
      players.at (attack).putCards (cards, table);
      if (players.at (attack).getCards ().empty ())
        {
          pass (attack);
        }
      return true;
    }
  return false;
}

bool
Game::playerAssists (u_int16_t player, std::vector<size_t> const &index)
{
  auto cards = players.at (attack).getCards ();
  cards.erase (std::remove_if (cards.begin (), cards.end (), [i = int{}, index] (auto) mutable {
    auto test = std::find (index.begin (), index.end (), i);
    i++;
    return test == index.end ();
  }));
  auto result = false;
  if (player == attack || player == assistAttacker)
    {
      auto tableVector = getTableAsVector ();
      std::sort (tableVector.begin (), tableVector.end ());
      std::sort (cards.begin (), cards.end ());
      if (std::includes (tableVector.begin (), tableVector.end (), cards.begin (), cards.end ()))
        {
          result = true;
          players.at (player).putCards (cards, table);
          if (players.at (player).getCards ().empty ())
            {
              pass (player);
            }
        }
    }
  return result;
}

bool
Game::playerDefends (u_int16_t indexFromCardOnTheTable, Card &&card)
{
  auto cardToBeat = table.at (indexFromCardOnTheTable).first;
  if (not table.at (indexFromCardOnTheTable).second && beats (cardToBeat, card, trump))
    {
      table.at (indexFromCardOnTheTable).second = card;
      players.at (defend).putCards ({ card }, table);
      if (players.at (defend).getCards ().empty ())
        {
          nextRound (false);
        }
      else
        {
          rewokePass (attack);
          rewokePass (defend);
        }
      return true;
    }
  else
    {
      return false;
    }
}

void
Game::defendingPlayerTakesAllCardsFromTheTable ()
{
  players.at (defend).takeCards (getTableAsVector ());
  table.clear ();
  nextRound (true);
}

void
Game::playerDrawsCardsFromDeck (Player &player, u_int16_t numberOfCards)
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
Game::playerDrawsCardsFromTable (Player &player)
{
  player.takeCards (getTableAsVector ());
}

std::vector<Player> const &
Game::getPlayers () const
{
  return players;
}

std::vector<std::pair<Card, std::optional<Card> > > const &
Game::getTable () const
{
  return table;
}

std::vector<Card>
Game::getTableAsVector ()
{
  auto result = std::vector<Card>{};
  for (auto &cardPair : table)
    {
      result.push_back (std::move (cardPair.first));
      if (cardPair.second)
        {
          result.push_back (std::move (*cardPair.second));
        }
    }
  return result;
}

void
Game::calculateNextRoles (bool attackSuccess)
{
  players.erase (std::remove_if (players.begin (), players.end (), [] (Player const &player) { return player.getCards ().empty (); }), players.end ());
  if (attackSuccess)
    {
      std::rotate (players.begin (), players.begin () + 1, players.end ());
    }
  else
    {
      std::rotate (players.begin (), players.begin () + 2, players.end ());
    }
}

void
Game::drawCards ()
{
  if (players.size () >= 3)
    {
      if (((6 - players.at (attack).getCards ().size ()) > 0))
        {
          playerDrawsCardsFromDeck (players.at (0), 6u - static_cast<u_int16_t> (players.at (0).getCards ().size ()));
        }
      if (((6 - players.at (assistAttacker).getCards ().size ()) > 0))
        {
          playerDrawsCardsFromDeck (players.at (2), 6u - static_cast<u_int16_t> (players.at (2).getCards ().size ()));
        }
      if (((6 - players.at (defend).getCards ().size ()) > 0))
        {
          playerDrawsCardsFromDeck (players.at (1), 6u - static_cast<u_int16_t> (players.at (1).getCards ().size ()));
        }
    }
  if (players.size () == 2)
    {
      for (auto &player : players)
        {
          if (((6 - player.getCards ().size ()) > 0))
            {
              playerDrawsCardsFromDeck (player, 6u - static_cast<u_int16_t> (player.getCards ().size ()));
            }
        }
    }
}

void
Game::nextRound (bool attackingSuccess)
{
  rewokePass (attack);
  rewokePass (defend);
  drawCards ();
  calculateNextRoles (attackingSuccess);
}

#ifndef B3662CAA_D812_46F7_8DD7_C85FCFAC47A4
#define B3662CAA_D812_46F7_8DD7_C85FCFAC47A4

#include "src/player.hxx"

class Game
{
public:
  Game (u_int16_t playerCount);

  enum PlayerRole
  {
    attack,
    defend,
    assistAttacker
  };

  // attack starts round and can only be used by playr with role attack
  bool playerStartsAttack (std::vector<Card> &cards);

  // after attack is started player with role attack and assistAttacker can add cards with same value which are allready on the table
  bool playerAssists (u_int16_t player, std::vector<Card> &&cards);

  // defending player can try to beat card on the table
  bool playerDefends (u_int16_t indexFromCardOnTheTable, Card &card);

  void defendingPlayerTakesAllCardsFromTheTable ();

  std::vector<Player> const &getPlayers () const;

  std::vector<std::pair<Card, std::optional<Card> > > const &getTable () const;

  // if defending player has no cards round finishes and table gets flushed
  // if cards are on the table defending player can take them and the round finishes
  // If attacking and assisting player waits and all cards on the table are beaten round finishes and talbe gets flushed
  void nextRound (bool attackingSuccess);

private:
  std::vector<Card> getTableAsVector ();

  void playerDrawsCardsFromDeck (Player &player, u_int16_t numberOfCards);

  void playerDrawsCardsFromTable (Player &player);

  void calculateNextRoles (bool attackSuccess);

  void drawCards ();

  std::vector<Card> cardDeck{};
  std::vector<Player> players{};
  std::vector<std::pair<Card, std::optional<Card> > > table{};
  Type trump{};
};

std::vector<Card> generateCardDeck ();

std::optional<Card> drawCard (std::vector<Card> &cardDeck);

std::optional<Card> drawSpecificCard (std::vector<Card> &cardDeck, Card const &cardToDraw);

#endif /* B3662CAA_D812_46F7_8DD7_C85FCFAC47A4 */

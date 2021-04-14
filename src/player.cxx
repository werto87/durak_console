#include "src/player.hxx"

void
Player::putCards (std::vector<Card> const &cardsToPut, std::vector<std::pair<Card, std::optional<Card> > > &target)
{
  auto cardsToMove = std::stable_partition (cards.begin (), cards.end (), [&cardsToPut] (Card const &card) { return std::find (cardsToPut.begin (), cardsToPut.end (), card) == cardsToPut.end (); });
  std::for_each (cardsToMove, cards.end (), [&target] (Card &card) { target.emplace_back (std::pair<Card, Card>{ std::move (card), {} }); });
  cards.erase (cardsToMove, cards.end ());
}

void
Player::takeCards (std::vector<Card> &&cardsToTake)
{
  std::move (cardsToTake.begin (), cardsToTake.end (), std::back_inserter (cards));
  cardsToTake.clear ();
}

std::vector<Card> const &
Player::getCards () const
{
  return cards;
}

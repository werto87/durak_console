#include "src/player.hxx"
#include <stdexcept>
#include <string>

void
Player::putCards (std::vector<Card> const &cardsToPut, std::vector<std::pair<Card, std::optional<Card> > > &target)
{
  auto cardsToMove = std::stable_partition (cards.begin (), cards.end (), [&cardsToPut] (Card const &card) { return std::find (cardsToPut.begin (), cardsToPut.end (), card) == cardsToPut.end (); });
  std::for_each (cardsToMove, cards.end (), [&target] (Card &card) { target.emplace_back (std::pair<Card, std::optional<Card> >{ std::move (card), {} }); });
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

std::vector<Card>
Player::cardsForIndex (std::vector<size_t> const &cardIndex)
{
  auto cardsWithIndex = cards;
  auto count = -1;
  cardsWithIndex.erase (std::remove_if (cardsWithIndex.begin (), cardsWithIndex.end (),
                                        [&count, &cardIndex] (auto) {
                                          count++;
                                          return std::find (cardIndex.begin (), cardIndex.end (), count) == cardIndex.end ();
                                        }),
                        cardsWithIndex.end ());
  if (cardsWithIndex.size () != cardIndex.size ()) throw std::logic_error{ "cardsWithIndex.size() != cardIndex.size() " + std::to_string (cardsWithIndex.size ()) + " != " + std::to_string (cardIndex.size ()) };
  return cardsWithIndex;
}

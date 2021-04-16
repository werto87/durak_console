#include "src/card.hxx"
#include <algorithm>
#include <random>

bool
operator== (Card const &x, Card const &y)
{
  return (x.value == y.value) && (x.type == y.type);
}
bool
operator!= (Card const &x, Card const &y)
{
  return !(x == y);
}

bool
beats (Card const &cardToBeat, Card const &cardWhichTriesTobeat, Type trump)
{
  if (cardToBeat.type == cardWhichTriesTobeat.type && cardToBeat.value < cardWhichTriesTobeat.value)
    {
      return true;
    }
  else if ((cardToBeat.type != trump) && (cardWhichTriesTobeat.type == trump))
    {
      return true;
    }
  else
    {
      return false;
    }
}

bool
hasSameValue (Card const &x, Card const &y)
{
  return x.value == y.value;
}

bool
cardsHaveSameValue (std::vector<Card> const &cards)
{
  return std::find_if (cards.begin (), cards.end (), [valueToCompare = cards.front ().value] (Card const &card) { return valueToCompare != card.value; }) == cards.end ();
}

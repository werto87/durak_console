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

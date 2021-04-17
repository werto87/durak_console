#include "src/print.hxx"
#include "src/game.hxx"
#include <fmt/format.h>
#include <magic_enum.hpp>
#include <pipes/pipes.hpp>
#include <range/v3/range.hpp>
#include <ranges>
#include <sstream>
std::ostream &
operator<< (std::ostream &os, Card const &card)
{
  os << fmt::format ("Card: {{{0} , {1}}}", card.value, magic_enum::enum_name (card.type));
  return os;
}

std::ostream &
operator<< (std::ostream &os, Player const &player)
{
  os << "Player Cards sorted by Value" << std::endl;
  auto cards = player.getCards ();
  std::ranges::sort (cards);
  for (auto const &card : cards)
    os << card << std::endl;
  return os;
}

std::string
cardsSortedByValueWithIndex (std::vector<Card> cards)
{
  auto result = std::vector<std::tuple<int, Card> >{};
  pipes::mux (ranges::to<std::vector> (std::views::iota (size_t{}, cards.size ())), cards) >>= pipes::transform ([] (auto index, auto &&card) { return std::make_tuple (index, card); }) >>= pipes::push_back (result);
  std::ranges::sort (result, [] (std::tuple<int, Card> const &x, std::tuple<int, Card> const &y) { return std::get<1> (x) < std::get<1> (y); });
  auto cardsMessage = std::stringstream{};
  cardsMessage << "Cards sorted by Value with Index, Value and Type " << std::endl;
  for (auto const &card : result)
    cardsMessage << "index: " << std::get<0> (card) << " " << std::get<1> (card) << std::endl;
  return cardsMessage.str ();
}

std::string
attackingPlayerWithNameAndCardIndexValueAndType (Game const &game)
{
  auto result = std::stringstream{};
  result << game.getAttackingPlayer ().id << " Attacking Player: " << cardsSortedByValueWithIndex (game.getAttackingPlayer ().getCards ());
  return result.str ();
}

std::string
defendingPlayerWithNameAndCardIndexValueAndType (Game const &game)
{
  auto result = std::stringstream{};
  result << game.getDefendingPlayer ().id << " Defending Player: " << cardsSortedByValueWithIndex (game.getDefendingPlayer ().getCards ());
  return result.str ();
}

std::string
assistingPlayerWithNameAndCardIndexValueAndType (Game const &game)
{
  auto result = std::stringstream{};
  result << game.getAssistingPlayer ().id << " Assisting Player: " << cardsSortedByValueWithIndex (game.getAssistingPlayer ().getCards ());
  return result.str ();
}

std::string
tableAsString (Game const &game)
{
  // TODO cards should get sorted than we add index. if the player clicks on a card we use the card and not the index to do the rest of the logic

  auto result = std::vector<std::tuple<int, Card, std::optional<Card> > >{};
  pipes::mux (ranges::to<std::vector> (std::views::iota (size_t{}, game.getTable ().size ())), game.getTable ()) >>= pipes::transform ([] (auto index, auto &&card) { return std::make_tuple (index, card.first, card.second); }) >>= pipes::push_back (result);
  std::ranges::sort (result, [] (std::tuple<int, Card, std::optional<Card> > const &x, std::tuple<int, Card, std::optional<Card> > const &y) { return std::get<1> (x) < std::get<1> (y); });
  auto cardsMessage = std::stringstream{};
  cardsMessage << "Cards On Table sorted by Value with Index, Value and Type " << std::endl;
  for (auto const &card : result)
    {
      cardsMessage << "index: " << std::get<0> (card) << " " << std::get<1> (card);
      if (std::get<2> (card).has_value ())
        {
          cardsMessage << " " << std::get<2> (card).value () << std::endl;
        }
      else
        {
          cardsMessage << " Card to Beat" << std::endl;
        }
    }
  return cardsMessage.str ();
}
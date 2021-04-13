#ifndef AE375AF6_DDFE_4D22_AD80_723EB970B8C7
#define AE375AF6_DDFE_4D22_AD80_723EB970B8C7

#include <optional>
#include <sys/types.h>
#include <vector>
enum struct Type
{
  hearts,
  clubs,
  diamonds,
  spades
};

struct Card
{
  friend bool operator== (Card const &x, Card const &y);

  friend bool operator!= (Card const &x, Card const &y);

  u_int16_t value{};
  Type type{};
};

bool beats (Card const &cardToBeat, Card const &cardWhichTriesTobeat, Type trump);

bool operator== (Card const &lhs, Card const &rhs);

#endif /* AE375AF6_DDFE_4D22_AD80_723EB970B8C7 */

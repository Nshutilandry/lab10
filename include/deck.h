#ifndef ARCADE_DECK_H
#define ARCADE_DECK_H

#include <random>

#include "card.h"

/// @file deck.h
/// @brief A deck of 52 cards that owns its memory.

/// The number of cards in a full deck.
inline constexpr int deck_size = 52;

// Class invariant:
//  - m_cards points to a heap array of m_size Cards allocated with new[].
//    This Deck owns that array and no other Deck shares it. m_cards may be
//    nullptr only when m_size is 0.
//  - m_size is the number of cards in the array (deck_size for a full deck).
//  - 0 <= m_next <= m_size. m_next is the index of the next card deal() will
//    return. Cards at indexes m_next to m_size - 1 have not been dealt yet.
//    When m_next == m_size, every card has been dealt and the deck is empty.
//
// Why operator= needs the self-assignment check:
//  In d = d, this and other are the same object, so m_cards and other.m_cards
//  are the same pointer. If operator= frees the old cards first and then
//  copies from other, it is copying from the array it just deleted. That is a
//  use-after-free: the cards are lost or replaced with garbage, and
//  AddressSanitizer reports heap-use-after-free. Checking this == &other and
//  returning early means assigning a deck to itself does nothing, which is
//  the correct result.

/// A deck of playing cards stored in a heap array.
class Deck {
public:
  /// Make a full, ordered deck: one of each rank in each suit.
  Deck();

  /// Free the cards.
  ~Deck();

  /// Make an independent copy of another deck (a deep copy).
  Deck(const Deck &other);

  /// Replace this deck's contents with a copy of another deck.
  /// Safe when assigning a deck to itself.
  Deck &operator=(const Deck &other);

  /// Put all the cards in random order and start dealing from the top again.
  /// @param gen the random number generator to draw from
  void shuffle(std::mt19937 &gen);

  /// Take the top card.
  /// @pre the deck is not empty
  /// @return the card that was on top
  Card deal();

  /// @return true if every card has been dealt
  bool empty() const;

  /// @return how many cards are left to deal
  int remaining() const;

private:
  Card *m_cards;
  int m_size;
  int m_next;
};

#endif // ARCADE_DECK_H

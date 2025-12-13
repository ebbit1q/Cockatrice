#include "gtest/gtest.h"
#include <QDebug>
#include <libcockatrice/deck_list/deck_list.h>

static constexpr int amount = 1e5;

const QString deckStart =
    R"(<?xml version="1.0"?><cockatrice_deck version="1"><deckname></deckname><comments></comments><zone name="main">)";
const QString deckEnd = R"(</zone></cockatrice_deck>)";

TEST(DeckHashTest, RepeatTest)
{
    QString repeatDeck =
        deckStart +
        R"(<card number="1" name="Mountain"/><card number="2" name="Island"/></zone><zone name="side"><card number="3" name="Forest"/>)" +
        deckEnd;
    DeckList decklist(repeatDeck);
    for (int i = 0; i < amount; ++i) {
        decklist.getDeckHash();
        decklist.refreshDeckHash();
    }
    auto hash = decklist.getDeckHash().toStdString();
    ASSERT_EQ(hash, "5cac19qm") << "The hash does not match!";
}

TEST(DeckHashTest, NumberTest)
{
    QString numberDeck = deckStart + QString(R"(<card number="%1" name="Island"/>)").arg(amount) + deckEnd;
    DeckList decklist(numberDeck);
    auto hash = decklist.getDeckHash().toStdString();
    ASSERT_EQ(hash, "e0m38p19") << "The hash does not match!";
}

TEST(DeckHashTest, UniquesTest)
{
    QStringList deckString{deckStart};
    int len = QString::number(amount).length();
    for (int i = 0; i < amount; ++i) {
        // creates already sorted list
        deckString << R"(<card number="1" name="card )" << QString::number(i).rightJustified(len, '0') << R"("/>)";
    }
    deckString << deckEnd;
    QString uniquesDeck = deckString.join("");
    DeckList decklist(uniquesDeck);
    auto hash = decklist.getDeckHash().toStdString();
    ASSERT_EQ(hash, "88prk025") << "The hash does not match!";
}

TEST(DeckHashTest, UniquesTestXor)
{
    QStringList deckStringXor(deckStart);
    for (int i = 0; i < amount; ++i) {
        // xor in order to mess with sorting
        deckStringXor << R"(<card number="1" name="card )" << QString::number(i ^ amount) << R"("/>)";
    }
    deckStringXor << deckEnd;
    QString uniquesXorDeck = deckStringXor.join("");
    DeckList decklist(uniquesXorDeck);
    auto hash = decklist.getDeckHash().toStdString();
    ASSERT_EQ(hash, "hkn6q4pf") << "The hash does not match!";
}

TEST(DeckHashTest, DuplicatesTest)
{
    QString duplicatesDeck = deckStart + QString(R"(<card number="1" name="card"/>)").repeated(amount) + deckEnd;
    DeckList decklist(duplicatesDeck);
    auto hash = decklist.getDeckHash().toStdString();
    ASSERT_EQ(hash, "ekt6tg1h") << "The hash does not match!";
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

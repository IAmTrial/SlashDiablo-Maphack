# Colonini Config
This is the directory containing the Conlonini parser files.

## What is Colonini
Colonini is a config format that was used in BH.

## Name
It was named as such because there wasn't a name that for this config file type, other than "BH Config Format" or "Colon-Delimited Config Format". The file format is basically INI with a colon key-value delimiter, and some extensions for mapped values.

## Syntax
The syntax is quite simple. Key-value pairings come in the form of `key: value`. Each entry can only occupy one line.

```
// This is a comment.

// Number
Answer to life and universe: 42     // Comments can be put at the end of a line.

// Hex value
Horse color: 0xAACCEE00

// Text
Friend: cat (or dog)

// Boolean, is case insensitive (ex: tRuE, FalsE)
Enable secret setting: true
Crash game on startup: false

// Key press
Win the game button: VK_W
Lose the game button: None

// Key toggle
Infravision: false, VK_O   // First value indicates starting value.
Deal no damage: true, None

// List, indexed by number (starting from 0, all values must be the same type)
Food[0]: Carrot
Food[1]: Tomato
Food[2]: Celery
Food[3]: Potato
// Food[4]: 12      // This is invalid!

// Table (basically a list indexed by text)
Level[Bob]: 12
Level[Joe]: 99
Level[Tom]: 87

// Nested (requires all values and subvalues to have the same type)
House[Kitchen][Stove][0]: Grilled asparagus
House[Kitchen][Freezer][0]: Ice
House[Kitchen][Freezer][1]: Frozen broccoli
House[Bedroom][Bed][0]: Blanket

// Keys can also have extra spaces at the start and end that will be ignored
    Hello   :   World!        // Will be interpretted as "Hello: World!"
```

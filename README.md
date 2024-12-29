# QMK Config - BDN9 Rev2 w/ 3 Rotary Encs

This is a repo for the configuration of the firmware for a [keeb.io BDN9 Revision 2 Macropad](https://docs.keeb.io/bdn9-rev2-build-guide), which supports up 3 rotary encoders in the top 3 slots (the top row). I have all 3 installed, so this is to build fw for my specific situation.

### Physical Layout

| rotary encoder 1 | rotary encoder 2 | rotary encoder 3 |
|------------------|------------------|------------------|
| kailh silver     | kailh red        | kailh black      |
| kailh blue       | kailh green      | kailh white      |


### Functions

Order for rotary encoders is as follows:

`clockwise action / counter-clockwise action`

#### Layer 0 (default)

| property | Encoder 0 | Encoder 1 | Encoder 2 |
| -- | -- | -- | -- |
| turning | vol up / down | arrow down / up | pg down / up |
| color normally | none | none | none |
| click   | mute | home | end |
| click color | red while muted | flash blue | flash green |

| property | key 0 | key 1 | key 2 |
| -- | -- | -- | -- |
| tapdance 1x tap | F13 | F14 | F15 |
| color | red | green | blue |
| tapdance 2x tap: one-shot | layer 1 | layer 2 | layer 3 |
| color | whole layer red | whole layer green | whole layer blue |

| property | key 3 | key 4 | key 5 |
| -- | -- | -- | -- |
| tapdance 1x tap | F16 | F17 | F18 |
| color | cyan | magenta | yellow |
| tapdance 2x tap: one-shot | layer 4 | layer 5 | layer 6 |
| color | whole layer cyan | whole layer magenta | whole layer yellow |

#### Layer 1 (meta functions)

\* = F22 used as modifer key in this layer. F22 down will be sent, then the key mentioned, then F22 up/release will be sent.

| property | Encoder 0 | Encoder 1 | Encoder 2 |
| -- | -- | -- | -- |
| turning | \*F19 / \*F20 | arrow down / up | pg down / up |
| color normally | none | none | none |
| click   | \*F21 | home | end |
| click color | red | flash blue | flash green |

| property | key 0 | key 1 | key 2 |
| -- | -- | -- | -- |
| tapdance 1x tap | \*F13 | \*F14 | \*F15 |
| color | red | green | blue |
| tapdance 2x tap: one-shot | TO layer 0 | -- | -- |
| color | off | whole layer green | whole layer blue |

| property | key 3 | key 4 | key 5 |
| -- | -- | -- | -- |
| tapdance 1x tap | \*F16 | \*F17 | \*F18 |
| color | cyan | magenta | yellow |
| tapdance 2x tap: one-shot | -- | -- | -- |
| color | whole layer cyan | whole layer magenta | whole layer yellow |

#### Layer 2 (meta functions)

@ = F23 used as modifer key in this layer. F23 down will be sent, then the key mentioned, then F23 up/release will be sent.

| property | Encoder 0 | Encoder 1 | Encoder 2 |
| -- | -- | -- | -- |
| turning | @F19 / @F20 | arrow down / up | pg down / up |
| color normally | none | none | none |
| click   | @F21 | home | end |
| click color | red | flash blue | flash green |

| property | key 0 | key 1 | key 2 |
| -- | -- | -- | -- |
| tapdance 1x tap | @F13 | @F14 | @F15 |
| color | red | green | blue |
| tapdance 2x tap: one-shot | -- | TO layer 0 | -- |
| color | whole layer red | off | whole layer blue |

| property | key 3 | key 4 | key 5 |
| -- | -- | -- | -- |
| tapdance 1x tap | @F16 | @F17 | @F18 |
| color | cyan | magenta | yellow |
| tapdance 2x tap: one-shot | -- | -- | -- |
| color | whole layer cyan | whole layer magenta | whole layer yellow |

#### Layer 3 (meta functions)

& = F24 used as modifer key in this layer. F24 down will be sent, then the key mentioned, then F24 up/release will be sent.

| property | Encoder 0 | Encoder 1 | Encoder 2 |
| -- | -- | -- | -- |
| turning | &F19 / &F20 | arrow down / up | pg down / up |
| color normally | none | none | none |
| click   | &F21 | home | end |
| click color | red | flash blue | flash green |

| property | key 0 | key 1 | key 2 |
| -- | -- | -- | -- |
| tapdance 1x tap | &F13 | &F14 | &F15 |
| color | red | green | blue |
| tapdance 2x tap: one-shot | -- | -- | layer 0 |
| color | whole layer red | whole layer green | off |

| property | key 3 | key 4 | key 5 |
| -- | -- | -- | -- |
| tapdance 1x tap | &F16 | &F17 | &F18 |
| color | cyan | magenta | yellow |
| tapdance 2x tap: one-shot | -- | -- | -- |
| color | whole layer cyan | whole layer magenta | whole layer yellow |


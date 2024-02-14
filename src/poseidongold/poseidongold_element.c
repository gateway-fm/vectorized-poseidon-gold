#include "poseidongold_element.h"

#include "poseidongold_element_scalar.h"
#include "poseidongold_element_utils.h"

const PoseidongoldElement PoseidongoldElement_ZR = 0x0000000000000000LL;
const PoseidongoldElement PoseidongoldElement_Q = 0xFFFFFFFF00000001LL;
const PoseidongoldElement PoseidongoldElement_MM = 0xFFFFFFFeFFFFFFFFLL;
const PoseidongoldElement PoseidongoldElement_CQ = 0x00000000FFFFFFFFLL;
const PoseidongoldElement PoseidongoldElement_R2 = 0xFFFFFFFe00000001LL;

const PoseidongoldElement PoseidongoldElement_W[33] = {0x1,
                                                       18446744069414584320ULL,
                                                       281474976710656ULL,
                                                       16777216ULL,
                                                       4096ULL,
                                                       64ULL,
                                                       8ULL,
                                                       2198989700608ULL,
                                                       4404853092538523347ULL,
                                                       6434636298004421797ULL,
                                                       4255134452441852017ULL,
                                                       9113133275150391358ULL,
                                                       4355325209153869931ULL,
                                                       4308460244895131701ULL,
                                                       7126024226993609386ULL,
                                                       1873558160482552414ULL,
                                                       8167150655112846419ULL,
                                                       5718075921287398682ULL,
                                                       3411401055030829696ULL,
                                                       8982441859486529725ULL,
                                                       1971462654193939361ULL,
                                                       6553637399136210105ULL,
                                                       8124823329697072476ULL,
                                                       5936499541590631774ULL,
                                                       2709866199236980323ULL,
                                                       8877499657461974390ULL,
                                                       3757607247483852735ULL,
                                                       4969973714567017225ULL,
                                                       2147253751702802259ULL,
                                                       2530564950562219707ULL,
                                                       1905180297017055339ULL,
                                                       3524815499551269279ULL,
                                                       7277203076849721926ULL};

const PoseidongoldElement PoseidongoldElement_ONE = 0x0000000000000001LL;
const PoseidongoldElement PoseidongoldElement_ZERO = 0x0000000000000000LL;
const PoseidongoldElement PoseidongoldElement_NEGONE = 0xFFFFFFFF00000000LL;
const PoseidongoldElement PoseidongoldElement_TWO32 = 0x0000000100000000LL;
const PoseidongoldElement PoseidongoldElement_SHIFT = 7;

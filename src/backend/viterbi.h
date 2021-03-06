#ifndef __VITERBI__
#define __VITERBI__
/*
 *  Viterbi.h according to the SPIRAL project
 */
#include    "DabConstants.h"
#include    "MathHelper.h"

//  For our particular viterbi decoder, we have
#define RATE    4
#define NUMSTATES 64
#define DECISIONTYPE uint32_t
//#define DECISIONTYPE uint8_t
//#define DECISIONTYPE_BITSIZE 8
#define DECISIONTYPE_BITSIZE 32
#define COMPUTETYPE uint32_t

//decision_t is a BIT vector
typedef union {
    DECISIONTYPE t[NUMSTATES/DECISIONTYPE_BITSIZE];
    uint32_t w[NUMSTATES/32];
    uint16_t s[NUMSTATES/16];
    uint8_t c[NUMSTATES/8];
} decision_t __attribute__ ((aligned (16)));

typedef union {
    COMPUTETYPE t[NUMSTATES];
} metric_t __attribute__ ((aligned (16)));

/* State info for instance of Viterbi decoder
*/

struct v {
    /* path metric buffer 1 */
    __attribute__ ((aligned (16))) metric_t metrics1;
    /* path metric buffer 2 */
    __attribute__ ((aligned (16))) metric_t metrics2;
    /* Pointers to path metrics, swapped on every bit */
    metric_t   *old_metrics,*new_metrics;
    decision_t *decisions;   /* decisions */
};

class viterbi
{
    public:
        viterbi(int16_t, bool spiral = true);
        ~viterbi(void);
        void deconvolve(int16_t *input, uint8_t *output);

    private:
        bool        spiral;
        struct v    vp;
        COMPUTETYPE Branchtab   [NUMSTATES / 2 * RATE] __attribute__ ((aligned (16)));
        //  int parityb     (uint8_t);
        int parity(int x);
        void partab_init (void);
        //  uint8_t Partab  [256];
        void init_viterbi(struct v *, int16_t starting_state);

        void update_viterbi_blk_GENERIC( struct v *vp,
                                         COMPUTETYPE *syms,
                                         int16_t nbits);

        void update_viterbi_blk_SPIRAL( struct v *vp,
                                        COMPUTETYPE *syms,
                                        int16_t nbits);

        void chainback_viterbi( struct v *vp,
                                uint8_t *data, /* Decoded output data */
                                int16_t nbits, /* Number of data bits */
                                uint16_t endstate); /*Terminal encoder state */

        void BFLY( int i, int s, COMPUTETYPE * syms, struct v * vp, decision_t * d);

        uint8_t *data;
        COMPUTETYPE *symbols;
        int16_t frameBits;
};

#endif


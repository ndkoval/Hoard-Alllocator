#ifndef HOARD_TEST_RAND_H
#define HOARD_TEST_RAND_H

// simple pseudorandom number generator useful for testing
struct Rand {
    Rand(unsigned a = 179424673, unsigned b = 982451161) : cur(0), a(a), b(b) {
    }

    inline unsigned next24() {
        cur = cur * a + b;
        return cur >> 8;
    }

    inline unsigned next32() {
        const unsigned first = next24();
        const unsigned second = next24();
        return (first << 8) ^ second;
    }

    inline unsigned long long next64() {
        return ((static_cast<unsigned long long>(next32()) << 32) ^ next32());
    }

    inline unsigned operator()() {
        return next32();
    }

private:
    unsigned cur;
    const unsigned a;
    const unsigned b;
};


#endif
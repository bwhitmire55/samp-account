//
// impl-whirlpool.inc
//

#if !defined WHIRLPOOL_LEN
    #tryinclude <whirlpool>
    #if !defined WHIRLPOOL_LEN
        #warning "Could not locate Whirlpool library. Adding definitions manually..."

        // whirlpool.inc contents (credits Y_Less)
        #define WHIRLPOOL_LEN (129)
        native WP_Hash(buffer[], len, const str[]);
    #endif
#endif
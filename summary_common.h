#ifdef WANT_SUMMARY

#ifndef NO_SUCCESS
COUNTER (23, r.success, "success probability");
#endif
AVG(0,r.n, "\naverage n: ")
COUNTER (0, r.n <= 0, "   n <= 0:");
COUNTER (1, r.n <= 4, "n <= 4:");
COUNTER (2, r.n <= 8, "n <= 8:");
COUNTER (3, r.n <= 16, "n <= 16:");
COUNTER (4, r.n <= 24, "n <= 24:");
COUNTER (5, r.n <= 32, "n <= 32:");
COUNTER (6, r.n <= 48, "n <= 48:");
COUNTER (7, r.n <= 64, "n <= 64:");
COUNTER (8, r.s % 4 == 0, "\ns aligned to 4 bytes:");
COUNTER (9, r.s % 8 == 0, " 8 bytes:");
COUNTER (10, r.s % 16 == 0, "16 bytes:");
AVG(17,  r.cache_latency, "\naverage *s access cache latency ")
COUNTER (18,  r.cache_latency <= 8, "   l <= 8:");
COUNTER (19,  r.cache_latency <= 16, "l <= 16:");
COUNTER (20,  r.cache_latency <= 32, "l <= 32:");
COUNTER (21,  r.cache_latency <= 64, "l <= 64:");
COUNTER (22,  r.cache_latency <= 128, "l <= 128:");

#ifdef USES_S2
COUNTER (11, r.s2 % 4 == 0, "\ns2 aligned to 4 bytes:");
COUNTER (12, r.s2 % 8 == 0, " 8 bytes:");
COUNTER (13, r.s2 % 16 == 0, "16 bytes:");
COUNTER (14, (r.s - r.s2) % 4 == 0, "\ns-s2 aligned to 4 bytes:");
COUNTER (15, (r.s - r.s2) % 8 == 0, " 8 bytes:");
COUNTER (16, (r.s - r.s2) % 16 == 0, "16 bytes:");
AVG(31,  r.cache_latency2, "\naverage *s2 access cache latency ")
COUNTER (32,  r.cache_latency2 <= 8, "   l <= 8:");
COUNTER (33,  r.cache_latency2 <= 16, "l <= 16:");
COUNTER (35,  r.cache_latency2 <= 32, "l <= 32:");
COUNTER (37,  r.cache_latency2 <= 64, "l <= 64:");
COUNTER (38,  r.cache_latency2 <= 128, "l <= 128:");
#endif

#ifdef USES_CAPA
AVG(40,r.capa, "\naverage capacity: ")
COUNTER (40, r.capa <= 0, "   c <= 0:");
COUNTER (41, r.capa <= 4, "c <= 4:");
COUNTER (42, r.capa <= 8, "c <= 8:");
COUNTER (43, r.capa <= 16, "c <= 16:");
COUNTER (44, r.capa <= 24, "c <= 24:");
COUNTER (45, r.capa <= 32, "c <= 32:");
COUNTER (46, r.capa <= 48, "c <= 48:");
COUNTER (47, r.capa <= 64, "c <= 64:");
#endif



#endif

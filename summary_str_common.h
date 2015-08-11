#ifdef WANT_SUMMARY

AVG(50,strlen(r.needle), "average needle size: ");
COUNTER (50, strlen(r.needle) <= 0, "   ns <= 0:");
COUNTER (51, strlen(r.needle) <= 4, "ns <= 4:");
COUNTER (52, strlen(r.needle) <= 8, "ns <= 8:");
COUNTER (53, strlen(r.needle) <= 16, "ns <= 16:");
COUNTER (54, strlen(r.needle) <= 24, "ns <= 24:");
COUNTER (55, strlen(r.needle) <= 32, "ns <= 32:");
COUNTER (56, strlen(r.needle) <= 48, "ns <= 48:");
COUNTER (57, strlen(r.needle) <= 64, "ns <= 64:");
AVG(51,r.digraph, "digraph count: ");
AVG(52,r.trigraph, "trigraph count: ");

#endif
#include "summary_common.h"

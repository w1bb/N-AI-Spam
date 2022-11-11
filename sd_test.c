// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

// Include the asscociated header file
#include "sd_test.h"

// This function sets an array according to the emails' true nature if and only
// if SD_LOG_FULL is defined
// - - - - -
// bool  private = Usually false, true only for testing purposes.
// int[]  true_spam = The array that will be returned.
#ifdef SD_LOG_FULL
void get_true_spam(bool is_private,
				   int true_spam[MAX_EMAILS])
{
#ifdef SD_LOG_FULL
	if (is_private) {
		for (int i = 0; i < 1500; ++i)
			true_spam[i] = 1;
		for (int i = 1500; i < 5200; ++i)
			true_spam[i] = 0;
	} else {
		for (int i = 0; i < 155; ++i)
			true_spam[i] = false;
		true_spam[1] = true;
		true_spam[105] = true;
		true_spam[115] = true;
		true_spam[121] = true;
		true_spam[131] = true;
		true_spam[14] = true;
		true_spam[17] = true;
		true_spam[25] = true;
		true_spam[30] = true;
		true_spam[4] = true;
		true_spam[47] = true;
		true_spam[50] = true;
		true_spam[60] = true;
		true_spam[68] = true;
		true_spam[83] = true;
		true_spam[90] = true;
		true_spam[97] = true;
		true_spam[100] = true;
		true_spam[106] = true;
		true_spam[119] = true;
		true_spam[127] = true;
		true_spam[132] = true;
		true_spam[148] = true;
		true_spam[18] = true;
		true_spam[26] = true;
		true_spam[37] = true;
		true_spam[41] = true;
		true_spam[49] = true;
		true_spam[54] = true;
		true_spam[62] = true;
		true_spam[69] = true;
		true_spam[87] = true;
		true_spam[93] = true;
		true_spam[98] = true;
		true_spam[101] = true;
		true_spam[112] = true;
		true_spam[120] = true;
		true_spam[129] = true;
		true_spam[135] = true;
		true_spam[16] = true;
		true_spam[23] = true;
		true_spam[29] = true;
		true_spam[39] = true;
		true_spam[42] = true;
		true_spam[5] = true;
		true_spam[59] = true;
		true_spam[63] = true;
		true_spam[71] = true;
		true_spam[9] = true;
		true_spam[96] = true;
	}
#else
	is_private = is_private; // useless warnings
	for (int i = 0; i < MAX_EMAILS; ++i)
		true_spam[i] = -1;
#endif
}
#endif

//Measures time elapsed from code within checkpoints
struct timeval tvalBefore, tvalAfter, tvalResult;

gettimeofday(&tvalBefore, NULL);

//some code to time, for example:
sleep(1);

gettimeofday(&tvalAfter, NULL);

timersub(&tvalAfter, &tvalBefore, &tvalResult);

printf("Time elapsed: %ld.%06ld\n", (long int)tvalResult.tv_sec, (long int)tvalResult.tv_usec);
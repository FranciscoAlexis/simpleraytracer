long long timeval_diff(struct timeval *difference, struct timeval *end_time, struct timeval *start_time)
{
        struct timeval temp_diff;

        if(difference==NULL)
        {
                difference=&temp_diff;
        }

        difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
        difference->tv_usec=end_time->tv_usec-start_time->tv_usec;

	
        /* Using while instead of if below makes the code slightly more robust. */
        return difference->tv_sec*1000000 + difference->tv_usec;
}

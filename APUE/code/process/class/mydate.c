#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*
 * -y year
 * -m month
 * -d day
 *  -H hour
 *  -M mintue
 * 0S second
 * */
#define TIMESTRSIZE 1024
#define FMTSTRSIZE 1024

int main(int argc, char** argv)
{
    time_t stamp;
    struct tm* tm;
    char timestr[TIMESTRSIZE];
    char fmtstr[FMTSTRSIZE];
    int c = 0;

    stamp = time(NULL);
    tm = localtime(&stamp);
    fmtstr[0] = '\0';

    while (1) {
	c = getopt(argc, argv, "H:MSy:md"); //optstring is a string containing the legitimate option characters
	if (c < 0) {
	    break;
	}
	switch (c) {
	case 'H':
	    if (strcmp(optarg, "12") == 0) {
		strncat(fmtstr, "%I(%P) ", FMTSTRSIZE - 1);
	    } else if (strcmp(optarg, "24") == 0) {
		strncat(fmtstr, "%H ", FMTSTRSIZE - 1);
	    } else
		fprintf(stderr, "不合适的格式..");
	    break;
	case 'M':
	    strncat(fmtstr, "%M ", FMTSTRSIZE - 1);
	    break;
	case 'S':
	    strncat(fmtstr, "%S ", FMTSTRSIZE - 1);
	    break;
	case 'y':
	    if (strcmp(optarg, "2") == 0) {
		strncat(fmtstr, "%y ", FMTSTRSIZE - 1);
	    } else if (strcmp(optarg, "4") == 0) {
		strncat(fmtstr, "%Y ", FMTSTRSIZE - 1);
	    } else
		fprintf(stderr, "不合适的格式..");
	    break;
	case 'm':
	    strncat(fmtstr, "%m ", FMTSTRSIZE - 1);
	    break;
	case 'd':
	    strncat(fmtstr, "%d ", FMTSTRSIZE - 1);
	    break;
	default:
	    break;
	}
    }
    strftime(timestr, TIMESTRSIZE, fmtstr, tm);
    puts(timestr);

    exit(1);
}

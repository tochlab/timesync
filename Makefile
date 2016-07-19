default:
	$(CC) -Wall -std=c99 -o timesyncd tsserver.c tslib.c
	$(CC) -Wall -std=c99 -o timesync tsclient.c tslib.c

clean:
	rm -f timesyncd
	rm -f timesync

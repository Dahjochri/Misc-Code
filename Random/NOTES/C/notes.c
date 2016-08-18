#include <stdio.h>
#include <time.h>

#define note_file "NOTES.TXT"

int main(int argc, char**argv)
{
	FILE *note;
	time_t timer;
	int i;
	char ch;

	if(argc==1)
	{
		//Read NOTES.TXT and print it
		note = fopen(note_file, "r");
		if(note!=NULL)
		{
			while((ch=getc(note))!=EOF)
				putc(ch, stdout);
		}
	}
	else
	{
	    //Write date & time to NOTES.TXT, and then arguments
		note = fopen(note_file, "a");
		if(note!=NULL)
		{
			timer = time(NULL);
			fprintf(note,"%s",asctime(localtime(&timer)));
			putc('\t', note);
			for(i=1;i<argc;i++)
				fprintf(note, "%s ", argv[i]);
			putc('\n', note);
		}
	}

	fclose(note);
	return 0;
}

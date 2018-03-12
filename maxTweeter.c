#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Tweeter tweeter;

struct Tweeter {
   char  *Name;
   int   numberOfTweets;
   struct Tweeter *nextTweeter;
};


void addTweeter(struct Tweeter **tweet, char *tweeterName, int numTweets){
    struct Tweeter *tempTweet = (struct Tweeter*) malloc(sizeof(struct Tweeter));
    tempTweet->Name = tweeterName;
    tempTweet->numberOfTweets = numTweets;
    tempTweet->nextTweeter = *tweet;
    *tweet = tempTweet;
}

bool checkAndAddIfInList(struct Tweeter **tweetList, char* stringToFind){
    struct Tweeter *tempTweet;
    bool isIn = false;
    for(tempTweet = *tweetList; tempTweet != NULL; tempTweet = tempTweet->nextTweeter){
            if(strcmp (tempTweet->Name, stringToFind) == 0){
                tempTweet->numberOfTweets = tempTweet->numberOfTweets + 1;
                isIn =  true;
            }

    }
    return isIn;
}

int getcol(char *line)
{
        if(!line)
                return -1;
        int count=0;
        const char* str;
        char colname[7]="\"name\"\0";
        str=strtok(line,",");
        if(str ==NULL || strcmp(str, "\"\"")!=0 ){
                printf("Input format error\n");
                return -1;
                }
        for(;;str=strtok(NULL, ",\n")){

                if(str == NULL){
                    printf("Input format error\n");
                    exit(-1);
                }
                if(strstr(str,colname)!=NULL){
                    if( strcmp(strstr(str,colname),colname) == 0 && strcmp(str,colname)!=0){
                        printf("Input format error\n");
                        return -1;
                    }
                }
                else if(*str && str[strlen(str + 1)] != '"'){
                    printf("Input format error\n");
                    return -1;
                }
                if(strcmp(str, colname)==0)
                        return count +1;
                else
                        count++;
                }
        return -1;
}


const char * getfield(char * line, int count)
{
        const char *str;
        for(str=strtok(line,",");;str=strtok(NULL, ",\n")){
                if(!--count)
                        return str;
        }
        return NULL;
}


void sortList(struct Tweeter **tweetList){
    struct Tweeter *tempTweet1;
    struct Tweeter *tempTweet2;
    int tempNumTweets;
    char *tempName;
    for(tempTweet1 = *tweetList; tempTweet1 != NULL; tempTweet1  = tempTweet1->nextTweeter){
        for(tempTweet2 = tempTweet1->nextTweeter; tempTweet2 != NULL; tempTweet2 = tempTweet2->nextTweeter){
            if(tempTweet2->numberOfTweets > tempTweet1->numberOfTweets){
                tempNumTweets = tempTweet1->numberOfTweets;
                tempName= tempTweet1->Name;
                tempTweet1->numberOfTweets = tempTweet2->numberOfTweets;
                tempTweet1->Name = tempTweet2->Name;
                tempTweet2->numberOfTweets = tempNumTweets;
                tempTweet2->Name = tempName;
            }
        }
    }
}


int main( int argc, char *argv[] )  {
   if(argc!=2){
        printf("<filename.csv>\n");
        return -1;
    }
    // Needed variables before starting (aka get the file:
    char * filePath = argv[1];
    FILE *fd;
    fd = fopen (filePath,"r");
    // Test if we can read the file.
    if (fd == NULL) {
        printf ("Error creating file, error number: %d\n", errno);
        return -1;
    }

    else{

        fseek (fd, 0, SEEK_END);
        long fileSize = ftell(fd);

        if (0 == fileSize)
        {
            printf("Invalid Input Format\n");
            return -1;
        }
        fseek(fd, 0, SEEK_SET);
        // if we can read the file, get the data.
        struct Tweeter *tweeters= NULL;
        char * line = (char *)malloc(sizeof(char) * 1024);
        // Get the column that contains the name.
        fgets(line, 1024, fd);
        if(line == NULL){
                    printf("Invalid Input Format\n");
                    return -1;
        }
        char * test = line;
        int testlen = strlen(test);
        if(testlen == 0){
            printf("Invalid Input Format\n");
            return -1;
        }
        int name_col = getcol(line);
        if(name_col != -1){
            int numLines = 1;
            int numTweeters = 0;
            // Get the tweeter's name, and add it to the Linked List if it's not already in it.
            // If the tweeter is in the Linked List, increment the counter.
            while(fgets(line, 1024, fd) && numLines < 20000){
                numLines++;
                char * name = (char *)malloc(sizeof(char) * 100);
                int num = strlen(line);

                char * field = getfield(line, name_col);
                if(field == NULL){
                    printf("Invalid Input Format\n");
                    return -1;
                }
                test = line;
                testlen = strlen(test);
                if(testlen == 0){
                    printf("Invalid Input Format\n");
                    return -1;
                }
                if(sizeof(field) > sizeof(char) * 100 || numLines > 20000)
                    return -1;
                //printf(field);
                strcpy(name, field);
                if(name != NULL){
                    if(!checkAndAddIfInList(&tweeters, name)){
                        addTweeter(&tweeters, name, 1);
                        numTweeters++;
                    }
                }

            }
            // Sort the Linked List by highest tweeter count.
            sortList(&tweeters);
            // Print the top 10 tweeters out.
            if(numTweeters >= 10){
                int getTweeters = 0;
                int printInt = 0;
                struct Tweeter *tempTweet = tweeters;
                while(getTweeters < 10 && tempTweet != NULL){
                    printInt = tempTweet->numberOfTweets;
                    printf(tempTweet->Name);
                    printf(": ");
                    printf("%d",printInt);
                    printf("\n");
                    tempTweet = tempTweet->nextTweeter;
                    getTweeters++;
                }
            }
        }
    }
    // Close the file.
    fclose (fd);
    return 0;

}
